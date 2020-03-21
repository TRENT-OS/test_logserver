#include "LibDebug/Debug.h"

#include "seos_log_server_backend_filesystem.h"
#include "custom_format.h"

#include "seos_fs.h"
#include "seos_pm.h"

#include <stdio.h>
#include <camkes.h>

// log server id
#define LOG_SERVER_ID  0u

#define NO_FILTER_ID_FIRST 0xCAFE
#define NO_FILTER_ID_LAST  (NO_FILTER_ID_FIRST + Debug_LOG_LEVEL_CUSTOM)

static const uint8_t  PARTITION_ID  = 1u;

#define LOG_FILENAME_01 "log_01.txt"
#define LOG_FILENAME_02 "log_02.txt"

static Log_file_t log_file_01, log_file_02;

static char buf_log_server[DATABUFFER_SIZE];

typedef struct ClientConfig
{
    Log_consumer_t consumer;
    void*          buffer;
    Log_filter_t   log_filter;
    uint8_t        log_level;
    Log_file_t*    log_file;
    uint32_t       id;
    const char*    name;
} ClientConfig_t;

static ClientConfig_t clientConfigs[] =
{
    { .name = "LOG-SERVER",       .log_level = Debug_LOG_LEVEL_DEBUG,   .log_file = NULL,         .id = LOG_SERVER_ID },

    { .name = "LVL_NONE",         .log_level = Debug_LOG_LEVEL_NONE,    .log_file = NULL,         .id = 10u },
    { .name = NULL,               .log_level = Debug_LOG_LEVEL_ASSERT,  .log_file = NULL,         .id = 200u },
    { .name = "LVL_FATAL",        .log_level = Debug_LOG_LEVEL_FATAL,   .log_file = &log_file_01, .id = 3000u },
    { .name = "LVL_ERROR",        .log_level = Debug_LOG_LEVEL_ERROR,   .log_file = &log_file_02, .id = 40000u },
    { .name = "LVL_WARNING",      .log_level = Debug_LOG_LEVEL_WARNING, .log_file = NULL,         .id = 500000u },
    { .name = "LVL_INFO",         .log_level = Debug_LOG_LEVEL_INFO,    .log_file = NULL,         .id = 6000000u },
    { .name = "LVL_DEBUG",        .log_level = Debug_LOG_LEVEL_DEBUG,   .log_file = NULL,         .id = 0xFFFFu - 1 },
    { .name = "LVL_TRACE",        .log_level = Debug_LOG_LEVEL_TRACE,   .log_file = NULL,         .id = 0xFFFFu },
    { .name = "LVL_CUSTOM",       .log_level = Debug_LOG_LEVEL_CUSTOM,  .log_file = NULL,         .id = 0xFFFFFFFu - 1 },
    { .name = "LVL_MAX",          .log_level = Debug_LOG_LEVEL_MAX,     .log_file = NULL,         .id = 0xAAAAu },

    { .name = "FILTER_NULL",                                            .log_file = NULL,         .id = NO_FILTER_ID_FIRST },

    { .name = "CL_FILTER_NONE",                                         .log_file = NULL,         .id = NO_FILTER_ID_FIRST + 1 },
    { .name = "CL_FILTER_ASSERT",                                       .log_file = NULL,         .id = NO_FILTER_ID_FIRST + 2 },
    { .name = "CL_FILTER_FATAL",                                        .log_file = NULL,         .id = NO_FILTER_ID_FIRST + 3 },
    { .name = "CL_FILTER_ERROR",                                        .log_file = NULL,         .id = NO_FILTER_ID_FIRST + 4 },
    { .name = "CL_FILTER_WARNING",                                      .log_file = NULL,         .id = NO_FILTER_ID_FIRST + 5 },
    { .name = "CL_FILTER_INFO",                                         .log_file = NULL,         .id = NO_FILTER_ID_FIRST + 6 },
    { .name = "CL_FILTER_DEBUG",                                        .log_file = NULL,         .id = NO_FILTER_ID_FIRST + 7 },
    { .name = "CL_FILTER_TRACE",                                        .log_file = NULL,         .id = NO_FILTER_ID_FIRST + 8 },
    { .name = "CL_FILTER_CUSTOM",                                       .log_file = NULL,         .id = NO_FILTER_ID_LAST },

    { .name = "ILL_FORMATTED",    .log_level = Debug_LOG_LEVEL_DEBUG,   .log_file = NULL,         .id = 50000u },

    { .name = "LogFileReader",    .log_level = Debug_LOG_LEVEL_DEBUG,   .log_file = NULL,         .id = 50001u },
    { .name = "FileReaderWriter", .log_level = Debug_LOG_LEVEL_DEBUG,   .log_file = &log_file_02, .id = 50002u }
};

static const size_t CLIENT_CONFIGS_COUNT = sizeof(clientConfigs)
                                         / sizeof(*clientConfigs);

uint32_t API_LOG_SERVER_GET_SENDER_ID(void);

static Log_consumer_callback_t  log_consumer_callback;
static Log_format_t             format;
static Log_subject_t            subject;
static Log_output_t             filesystem, console;
static Log_emitter_callback_t   emitter_callback;

// Emitter configuration
static Log_subject_t  subject_log_server;
static Log_output_t   console_log_server;

static void mapClientConfigsDataPorts();
static void initLogFiles();
static void initLogTargetsAndSubjects();
static void initClients();
static bool filesystem_init(void);

#define LOG_SUCCESS() Debug_LOG_DEBUG("%s => SUCCESS!", __func__)

void log_server_interface__init()
{
    // set up consumer chain
    get_instance_Consumer_chain();

    initLogTargetsAndSubjects();
    initClients();

    // create filesystem
    if(!filesystem_init())
    {
        printf("Fail to init filesystem!\n");
        return;
    }

    initLogFiles();

    // start polling
    Consumer_chain_poll();

    LOG_SUCCESS();
}

int
run()
{
    const int demo_time_sec = 75;
    const int one_sec       = 1000;

    for(
        int time_passed_sec = 0;
        time_passed_sec < demo_time_sec;
        ++time_passed_sec)
    {
        api_time_server_sleep(one_sec);
    }

    printf("logServer finish\n");

    logServer_finish_emit();

    return 0;
}

static bool
filesystem_init(void)
{
    hPartition_t phandle;
    pm_disk_data_t pm_disk_data;
    pm_partition_data_t pm_partition_data;

    seos_err_t ret =  partition_manager_get_info_disk(&pm_disk_data);

    if(SEOS_SUCCESS != ret)
    {
        printf("Fail to get disk info! Error code: %d\n", ret);

        return false;
    }

    ret = partition_manager_get_info_partition(PARTITION_ID, &pm_partition_data);
    if(SEOS_SUCCESS != ret)
    {
        printf(
            "Fail to get partition info: %d! Error code: %d\n",
            pm_partition_data.partition_id,
            ret);

        return false;
    }

    ret = partition_init(pm_partition_data.partition_id, 0);
    if(SEOS_SUCCESS != ret)
    {
        printf(
            "Fail to init partition: %d! Error code: %d\n",
            pm_partition_data.partition_id,
            ret);

        return false;
    }

    phandle = partition_open(pm_partition_data.partition_id);
    if(!is_valid_partition_handle(phandle))
    {
        printf("Fail to open partition: %d!\n", pm_partition_data.partition_id);
        return false;
    }

    ret = partition_fs_create(
            phandle,
            FS_TYPE_FAT16,
            pm_partition_data.partition_size,
            0,  // default value: size of sector:   512
            0,  // default value: size of cluster:  512
            0,  // default value: reserved sectors count: FAT12/FAT16 = 1; FAT32 = 3
            0,  // default value: count file/dir entries: FAT12/FAT16 = 16; FAT32 = 0
            0,  // default value: count header sectors: 512
            FS_PARTITION_OVERWRITE_CREATE);

    if(SEOS_SUCCESS != ret)
    {
        printf(
            "Fail to create filesystem on partition: %d! Error code: %d\n",
            pm_partition_data.partition_id,
            ret);

        return false;
    }

    ret = partition_close(phandle);
    if(SEOS_SUCCESS != ret)
    {
        printf(
            "Fail to close partition: %d! Error code: %d\n",
            pm_partition_data.partition_id,
            ret);

        return false;
    }

    LOG_SUCCESS();

    return true;
}

/** Unfortunately needs to be evaluated at runtime i.e. they are not defined at
 *  compile time. */
void mapClientConfigsDataPorts()
{
    ClientConfig_t* clientConfig = &clientConfigs[0];

    (clientConfig)++->buffer = buf_log_server;

    (clientConfig)++->buffer = dataport_buf_lvl_none;
    (clientConfig)++->buffer = dataport_buf_lvl_assert;
    (clientConfig)++->buffer = dataport_buf_lvl_fatal;
    (clientConfig)++->buffer = dataport_buf_lvl_error;
    (clientConfig)++->buffer = dataport_buf_lvl_warning;
    (clientConfig)++->buffer = dataport_buf_lvl_info;
    (clientConfig)++->buffer = dataport_buf_lvl_debug;
    (clientConfig)++->buffer = dataport_buf_lvl_trace;
    (clientConfig)++->buffer = dataport_buf_lvl_custom;
    (clientConfig)++->buffer = dataport_buf_lvl_max;

    (clientConfig)++->buffer = dataport_buf_no_filters;

    (clientConfig)++->buffer = dataport_buf_cl_filter_none;
    (clientConfig)++->buffer = dataport_buf_cl_filter_assert;
    (clientConfig)++->buffer = dataport_buf_cl_filter_fatal;
    (clientConfig)++->buffer = dataport_buf_cl_filter_error;
    (clientConfig)++->buffer = dataport_buf_cl_filter_warning;
    (clientConfig)++->buffer = dataport_buf_cl_filter_info;
    (clientConfig)++->buffer = dataport_buf_cl_filter_debug;
    (clientConfig)++->buffer = dataport_buf_cl_filter_trace;
    (clientConfig)++->buffer = dataport_buf_cl_filter_custom;

    (clientConfig)++->buffer = dataport_buf_sendsIllFormattedString;

    (clientConfig)++->buffer = dataport_buf_logFileReader;
    (clientConfig)++->buffer = dataport_buf_fileReaderWriter;

    // If the manipulated pointer does not point to the end of clientConfigs
    // array, then data ports got misconfigured.
    assert(clientConfig == &clientConfigs[CLIENT_CONFIGS_COUNT]);
}

void initLogFiles()
{
    Log_file_ctor(&log_file_01, PARTITION_ID, LOG_FILENAME_01);
    Log_file_ctor(&log_file_02, PARTITION_ID, LOG_FILENAME_02);
    Log_file_create_log_file(&log_file_01);
    Log_file_create_log_file(&log_file_02);
}

void initLogTargetsAndSubjects()
{
    Log_format_ctor(&format);

    // register objects to observe
    Log_subject_ctor(&subject);

    // Emitter configuration
    Log_subject_ctor(&subject_log_server);

    // set up backend
    Log_output_filesystem_ctor(&filesystem, &format);
    Log_output_console_ctor(&console, &format);

    // Emitter configuration
    Log_output_console_ctor(&console_log_server, &custom_format);

    // attach observed object to subject
    Log_subject_attach((Subject_t *)&subject, (Observer_t *)&filesystem);
    Log_subject_attach((Subject_t *)&subject, (Observer_t *)&console);
    // Emitter configuration
    Log_subject_attach(
        (Subject_t *)&subject_log_server,
        (Observer_t *)&console_log_server);

    // Emitter configuration: set up registered functions layer
    Log_emitter_callback_ctor(&emitter_callback, NULL, API_LOG_SERVER_EMIT);
}

void initClients()
{
    // set up registered functions layer
    Log_consumer_callback_ctor(
        &log_consumer_callback,
        logServer_ready_emit,
        API_LOG_SERVER_GET_SENDER_ID,
        api_time_server_get_timestamp);

    mapClientConfigsDataPorts();

    // Emitter configuration: set up log emitter layer
    get_instance_Log_emitter(
        clientConfigs[LOG_SERVER_ID].buffer,
        &clientConfigs[LOG_SERVER_ID].log_filter,
        &emitter_callback);

    for(size_t i = 0; i < CLIENT_CONFIGS_COUNT; ++i)
    {
        Log_filter_ctor(
            &clientConfigs[i].log_filter,
             clientConfigs[i].log_level);

        bool isFilterNull = (NO_FILTER_ID_FIRST <= clientConfigs[i].id)
                         && (NO_FILTER_ID_LAST  >= clientConfigs[i].id);

        Log_filter_t* const pFilter = isFilterNull ?
                                        NULL : &clientConfigs[i].log_filter;

        const bool isLogServer  = (LOG_SERVER_ID == clientConfigs[i].id);
        Log_subject_t* pSubject = isLogServer ? &subject_log_server : &subject;

        Log_consumer_ctor(
            &clientConfigs[i].consumer,
            clientConfigs[i].buffer,
            pFilter,
            &log_consumer_callback,
            pSubject,
            clientConfigs[i].log_file,
            clientConfigs[i].id,
            clientConfigs[i].name
        );

        Consumer_chain_append(&clientConfigs[i].consumer);
    }
}
