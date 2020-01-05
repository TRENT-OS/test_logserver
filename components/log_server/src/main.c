#include "LibDebug/Debug.h"

#include "seos_log_server.h"

#include "seos_fs.h"
#include "seos_pm.h"

#include <stdio.h>

#include <camkes.h>



#define DATABUFFER_SERVER_01    (void *)dataport_buf_app01
#define DATABUFFER_SERVER_02    (void *)dataport_buf_app02
#define DATABUFFER_SERVER_03    (void *)dataport_buf_app03
#define DATABUFFER_SERVER_0x    (void *)dataport_buf_app0x

// log server id
#define LOG_SERVER_ID           0

// client id's
#define CLIENT_APP01_ID         10
#define CLIENT_APP02_ID         200
#define CLIENT_APP03_ID         3000
#define CLIENT_APP0x_ID         40000

#define PARTITION_ID            0
#define LOG_FILENAME            "log.txt"



uint32_t API_LOG_SERVER_GET_SENDER_ID(void);



static Log_filter_t filter_01, filter_02, filter_03, filter_0x;
static Log_consumer_t log_consumer_01, log_consumer_02, log_consumer_03, log_consumer_0x;
static Log_consumer_callback_t log_consumer_callback;
static Log_format_t format;
static Log_subject_t subject;
static Log_output_t filesystem, console;
static Log_file_t log_file;
static Log_emitter_callback_t emitter_callback;
// Emitter configuration
static Log_filter_t filter_log_server;
static Log_consumer_t log_consumer_log_server;
static Log_subject_t subject_log_server;
static Log_output_t console_log_server;
static char buf_log_server[DATABUFFER_SIZE];



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
    // set up consumer chain
    get_instance_Consumer_chain();

    // set up log format layer
    Log_format_ctor(&format);

    // register objects to observe
    Log_subject_ctor(&subject);
    // Emitter configuration
    Log_subject_ctor(&subject_log_server);

    // set up log file
    Log_file_ctor(&log_file, PARTITION_ID, LOG_FILENAME);

    // set up backend
    Log_output_filesystem_ctor(&filesystem, &format, &log_file.log_file_info);
    Log_output_console_ctor(&console, &format);
    // Emitter configuration
    Log_output_console_ctor(&console_log_server, &format);

    // attach observed object to subject
    Log_subject_attach((Subject_t *)&subject, (Observer_t *)&filesystem);
    Log_subject_attach((Subject_t *)&subject, (Observer_t *)&console);
    // Emitter configuration
    Log_subject_attach((Subject_t *)&subject_log_server, (Observer_t *)&console_log_server);

    // Emitter configuration: set up registered functions layer
    Log_emitter_callback_ctor(&emitter_callback, NULL, API_LOG_SERVER_EMIT);

    // set up log filter layer
    Log_filter_ctor(&filter_01, Debug_LOG_LEVEL_DEBUG);
    Log_filter_ctor(&filter_02, Debug_LOG_LEVEL_DEBUG);
    Log_filter_ctor(&filter_03, Debug_LOG_LEVEL_DEBUG);
    Log_filter_ctor(&filter_0x, Debug_LOG_LEVEL_DEBUG);
    // Emitter configuration
    Log_filter_ctor(&filter_log_server, Debug_LOG_LEVEL_DEBUG);

    // set up registered functions layer
    Log_consumer_callback_ctor(&log_consumer_callback, logServer_ready_emit, API_LOG_SERVER_GET_SENDER_ID, api_time_server_get_timestamp);

    // set up log consumer layer
    Log_consumer_ctor(&log_consumer_01, DATABUFFER_SERVER_01, &filter_01, &log_consumer_callback, &subject, &log_file, CLIENT_APP01_ID, "APP01");
    Log_consumer_ctor(&log_consumer_02, DATABUFFER_SERVER_02, &filter_02, &log_consumer_callback, &subject, &log_file, CLIENT_APP02_ID, NULL);
    Log_consumer_ctor(&log_consumer_03, DATABUFFER_SERVER_03, &filter_03, &log_consumer_callback, &subject, &log_file, CLIENT_APP03_ID, "APP03");
    Log_consumer_ctor(&log_consumer_0x, DATABUFFER_SERVER_0x, &filter_0x, &log_consumer_callback, &subject, &log_file, CLIENT_APP0x_ID, "APP0x");
    // Emitter configuration
    Log_consumer_ctor(&log_consumer_log_server, buf_log_server, &filter_log_server, &log_consumer_callback, &subject_log_server, &log_file, LOG_SERVER_ID, "LOG-SERVER");

    // Emitter configuration: set up log emitter layer
    get_instance_Log_emitter(buf_log_server, &filter_log_server, &emitter_callback);

    // set up consumer chain layer
    Consumer_chain_append(&log_consumer_01);
    Consumer_chain_append(&log_consumer_02);
    Consumer_chain_append(&log_consumer_03);
    Consumer_chain_append(&log_consumer_0x);
    // Emitter configuration
    Consumer_chain_append(&log_consumer_log_server);

    // create filesystem
    if(filesystem_init() == false){
        printf("Fail to init filesystem!\n");
        return;
    }

    // create log file
    Log_file_create_log_file(&log_file);

    // start polling
    Consumer_chain_poll();
}



int
run()
{
    int finish = 1;

    while (1){
        api_time_server_sleep(1000);

        if(finish == 20)
            break;

        finish++;
    }

    printf("demo finish\n");

    logServer_finish_emit();

    return 0;
}
