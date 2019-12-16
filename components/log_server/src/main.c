#include "Debug.h"

#include "log_symbol.h"
#include "consumer_chain.h"
#include "log_format.h"

#include "seos_fs.h"
#include "seos_pm.h"

#include <stdio.h>
#include <camkes.h>



#define PARTITION_ID    0



int8_t TEST_APP__init(bool create_fs);



static Log_filter_t filter_01, filter_02, filter_03;
static Log_consumer_t log_consumer_01, log_consumer_02, log_consumer_03;
static Log_consumer_callback_t reg_01, reg_02, reg_03;
static Log_format_t format_01, format_02, format_03;



int run()
{
    int8_t ret = EOF;
    ret = TEST_APP__init(true);
    if(ret < 0){
        printf("Fail to init demo!\n");
        return EOF;
    }

    get_instance_Consumer_chain();

    // set up log filter layer
    Log_filter_ctor(&filter_01, Debug_LOG_LEVEL_DEBUG);
    Log_filter_ctor(&filter_02, Debug_LOG_LEVEL_DEBUG);
    Log_filter_ctor(&filter_03, Debug_LOG_LEVEL_DEBUG);

    // set up log format layer
    Log_format_ctor(&format_01);
    Log_format_ctor(&format_02);
    Log_format_ctor(&format_03);

    // set up registered functions layer
    Log_consumer_callback_ctor(&reg_01, dataAvailable_app01_reg_callback, logServer_ready_app01_emit, api_time_server_get_timestamp);
    Log_consumer_callback_ctor(&reg_02, dataAvailable_app02_reg_callback, logServer_ready_app02_emit, api_time_server_get_timestamp);
    Log_consumer_callback_ctor(&reg_03, dataAvailable_app03_reg_callback, logServer_ready_app03_emit, api_time_server_get_timestamp);

    // set up log consumer layer
    Log_consumer_ctor(&log_consumer_01, DATABUFFER_SERVER_01, &filter_01, &reg_01, &format_01, "APP01");
    Log_consumer_ctor(&log_consumer_02, DATABUFFER_SERVER_02, &filter_02, &reg_02, &format_02, NULL);
    Log_consumer_ctor(&log_consumer_03, DATABUFFER_SERVER_03, &filter_03, &reg_03, &format_03, "APP03");

    // set up consumer chain layer
    Consumer_chain_append(&log_consumer_01);
    Consumer_chain_append(&log_consumer_02);
    Consumer_chain_append(&log_consumer_03);

    Consumer_chain_poll();



    // destruction
    Consumer_chain_dtor();

    Log_consumer_callback_dtor(&reg_01);
    Log_consumer_callback_dtor(&reg_02);
    Log_consumer_callback_dtor(&reg_03);

    Log_filter_dtor(&filter_01);
    Log_filter_dtor(&filter_02);
    Log_filter_dtor(&filter_03);

    Log_format_dtor(&format_01.parent);
    Log_format_dtor(&format_02.parent);
    Log_format_dtor(&format_03.parent);

    Log_consumer_dtor(&log_consumer_01);
    Log_consumer_dtor(&log_consumer_02);
    Log_consumer_dtor(&log_consumer_03);

    return 0;
}



int8_t TEST_APP__init(bool create_fs){
    hPartition_t phandle;
    pm_disk_data_t pm_disk_data;
    pm_partition_data_t pm_partition_data;

    partition_manager_get_info_disk(&pm_disk_data);
    partition_manager_get_info_partition(PARTITION_ID, &pm_partition_data);
    partition_init(pm_partition_data.partition_id, 0);

    phandle = partition_open(pm_partition_data.partition_id);

    if(create_fs == true)
        partition_fs_create(
                    phandle,
                    FS_TYPE_FAT32,
                    pm_partition_data.partition_size,
                    0,  // default value: size of sector:   512
                    0,  // default value: size of cluster:  512
                    0,  // default value: reserved sectors count: FAT12/FAT16 = 1; FAT32 = 3
                    0,  // default value: count file/dir entries: FAT12/FAT16 = 16; FAT32 = 0
                    0,  // default value: count header sectors: 512
                    FS_PARTITION_OVERWRITE_CREATE);
    else
        partition_fs_mount(phandle);

    partition_close(phandle);

    return 1;
}
