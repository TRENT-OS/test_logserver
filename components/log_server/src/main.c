#include "Debug.h"

#include "log_symbol.h"
#include "log_filter.h"
#include "log_consumer.h"
#include "consumer_chain.h"
#include "log_format.h"
#include "log_subject.h"
#include "log_output.h"

#include "seos_fs.h"
#include "seos_pm.h"

#include <stdio.h>

#include <camkes.h>



#define PARTITION_ID    0
#define LOG_FILENAME    "log.txt"



uint32_t API_LOG_SERVER_GET_SENDER_ID(void);



static Log_filter_t filter_01, filter_02, filter_03;
static Log_consumer_t log_consumer_01, log_consumer_02, log_consumer_03;
static Log_consumer_callback_t reg_01, reg_02, reg_03;
static Log_format_t format_01, format_02;
static Log_subject_t subject;
static Log_output_t filesystem, console;



static bool
filesystem_init(void)
{
    hPartition_t phandle;
    pm_disk_data_t pm_disk_data;
    pm_partition_data_t pm_partition_data;

    if(partition_manager_get_info_disk(&pm_disk_data) != SEOS_PM_SUCCESS)
    {
        printf("Fail to get disk info!\n");
        return false;
    }

    if(partition_manager_get_info_partition(PARTITION_ID, &pm_partition_data) != SEOS_PM_SUCCESS)
    {
        printf("Fail to get partition info: %d!\n", pm_partition_data.partition_id);
        return false;
    }

    if(partition_init(pm_partition_data.partition_id, 0) != SEOS_FS_SUCCESS)
    {
        printf("Fail to init partition: %d!\n", pm_partition_data.partition_id);
        return false;
    }

    if( (phandle = partition_open(pm_partition_data.partition_id)) < 0)
    {
        printf("Fail to open partition: %d!\n", pm_partition_data.partition_id);
        return false;
    }

    if(partition_fs_create(
                phandle,
                FS_TYPE_FAT32,
                pm_partition_data.partition_size,
                0,  // default value: size of sector:   512
                0,  // default value: size of cluster:  512
                0,  // default value: reserved sectors count: FAT12/FAT16 = 1; FAT32 = 3
                0,  // default value: count file/dir entries: FAT12/FAT16 = 16; FAT32 = 0
                0,  // default value: count header sectors: 512
                FS_PARTITION_OVERWRITE_CREATE)
        != SEOS_FS_SUCCESS)
    {
        printf("Fail to create filesystem on partition: %d!\n", pm_partition_data.partition_id);
        return false;
    }

    if(partition_close(phandle) != SEOS_FS_SUCCESS)
    {
        printf("Fail to close partition: %d!\n", pm_partition_data.partition_id);
        return false;
    }

    return true;
}



void log_server_interface__init(){
    bool ret;
    ret = filesystem_init();
    if(!ret){
        printf("Fail to init filesystem!\n");
        return;
    }

    // set up consumer chain
    get_instance_Consumer_chain();

    // set up log format layer
    Log_format_ctor(&format_01);
    Log_format_ctor(&format_02);

    // register objects to observe
    Log_subject_ctor(&subject);

    // set up backend
    Log_output_filesystem_ctor(&filesystem, &format_01, PARTITION_ID, LOG_FILENAME);
    Log_output_console_ctor(&console, &format_02);

    // attach observed object to subject
    Log_subject_attach((Subject_t *)&subject, (Observer_t *)&filesystem);
    Log_subject_attach((Subject_t *)&subject, (Observer_t *)&console);

    // set up log filter layer
    Log_filter_ctor(&filter_01, Debug_LOG_LEVEL_DEBUG);
    Log_filter_ctor(&filter_02, Debug_LOG_LEVEL_DEBUG);
    Log_filter_ctor(&filter_03, Debug_LOG_LEVEL_DEBUG);

    // set up registered functions layer
    Log_consumer_callback_ctor(&reg_01, logServer_ready_emit, log_server_interface_get_sender_id, api_time_server_get_timestamp);
    Log_consumer_callback_ctor(&reg_02, logServer_ready_emit, log_server_interface_get_sender_id, api_time_server_get_timestamp);
    Log_consumer_callback_ctor(&reg_03, logServer_ready_emit, log_server_interface_get_sender_id, api_time_server_get_timestamp);

    // set up log consumer layer
    Log_consumer_ctor(&log_consumer_01, DATABUFFER_SERVER_01, &filter_01, &reg_01, &subject, CLIENT_APP01_ID, "APP01");
    Log_consumer_ctor(&log_consumer_02, DATABUFFER_SERVER_02, &filter_02, &reg_02, &subject, CLIENT_APP02_ID, NULL);
    Log_consumer_ctor(&log_consumer_03, DATABUFFER_SERVER_03, &filter_03, &reg_03, &subject, CLIENT_APP03_ID, "APP03");

    // set up consumer chain layer
    Consumer_chain_append(&log_consumer_01);
    Consumer_chain_append(&log_consumer_02);
    Consumer_chain_append(&log_consumer_03);

    // start polling
    Consumer_chain_poll();
}
