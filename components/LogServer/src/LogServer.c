#include "LibDebug/Debug.h"

#include "Logger/Server/OS_LoggerFile.h"

#include "Logger/Server/OS_LoggerConsumerChain.h"
#include "Logger/Server/OS_LoggerConsumer.h"

#include "Logger/Server/OS_LoggerOutputConsole.h"
#include "Logger/Server/OS_LoggerOutputFileSystem.h"

#include "Logger/Client/OS_LoggerEmitter.h"

#include "custom_format.h"

#include "OS_Filesystem.h"
#include "OS_PartitionManager.h"

#include <stdio.h>
#include <camkes.h>

// log server id
#define LOG_SERVER_ID  0u

#define NO_FILTER_ID_FIRST 0xCAFE
#define NO_FILTER_ID_LAST  (NO_FILTER_ID_FIRST + Debug_LOG_LEVEL_CUSTOM)

static const uint8_t  PARTITION_ID  = 1u;

#define LOG_FILENAME_01 "log_01.txt"
#define LOG_FILENAME_02 "log_02.txt"

static OS_LoggerFile_Handle_t log_file_01, log_file_02;

static char buf_log_server[DATABUFFER_SIZE];

typedef struct ClientConfig
{
    OS_LoggerConsumer_Handle_t consumer;
    void*                      buffer;
    OS_LoggerFilter_Handle_t   log_filter;
    uint8_t                    log_level;
    OS_LoggerFile_Handle_t*    log_file;
    uint32_t                   id;
    const char*                name;
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
    { .name = "FileReaderWriter", .log_level = Debug_LOG_LEVEL_DEBUG,   .log_file = &log_file_02, .id = 50002u },

    { .name = "ChanMux",          .log_level = Debug_LOG_LEVEL_ASSERT,  .log_file = NULL,         .id = 3001u }
};

static const size_t CLIENT_CONFIGS_COUNT = sizeof(clientConfigs)
                                         / sizeof(*clientConfigs);

uint32_t API_LOG_SERVER_GET_SENDER_ID(void);

static OS_LoggerConsumerCallback_t       log_consumer_callback;
static OS_LoggerFormat_Handle_t          format;
static OS_LoggerSubject_Handle_t         subject;
static OS_LoggerOutput_Handle_t          filesystem, console;

// Emitter configuration
static OS_LoggerSubject_Handle_t  subject_log_server;
static OS_LoggerOutput_Handle_t   console_log_server;

static void mapClientConfigsDataPorts();
static void initLogFiles();
static void initLogTargetsAndSubjects();
static void initClients();
static bool filesystem_init(void);

#define LOG_SUCCESS() Debug_LOG_DEBUG("%s => SUCCESS!", __func__)

void pre_init()
{
    // set up consumer chain
    OS_LoggerConsumerChain_getInstance();

    initLogTargetsAndSubjects();
    initClients();

    // create filesystem
    if(!filesystem_init())
    {
        printf("Fail to init filesystem!\n");
        return;
    }

    initLogFiles();

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
    OS_PartitionManagerDataTypes_DiskData_t pm_disk_data;
    OS_PartitionManagerDataTypes_PartitionData_t pm_partition_data;

    OS_Error_t ret =  OS_PartitionManager_getInfoDisk(&pm_disk_data);

    if(OS_SUCCESS != ret)
    {
        printf("Fail to get disk info! Error code: %d\n", ret);

        return false;
    }

    ret = OS_PartitionManager_getInfoPartition(PARTITION_ID, &pm_partition_data);
    if(OS_SUCCESS != ret)
    {
        printf(
            "Fail to get partition info: %d! Error code: %d\n",
            pm_partition_data.partition_id,
            ret);

        return false;
    }

    ret = OS_Filesystem_init(pm_partition_data.partition_id, 0);
    if(OS_SUCCESS != ret)
    {
        printf(
            "Fail to init partition: %d! Error code: %d\n",
            pm_partition_data.partition_id,
            ret);

        return false;
    }

    phandle = OS_Filesystem_open(pm_partition_data.partition_id);
    if(!OS_Filesystem_validatePartitionHandle(phandle))
    {
        printf("Fail to open partition: %d!\n", pm_partition_data.partition_id);
        return false;
    }

    ret = OS_Filesystem_create(
            phandle,
            FS_TYPE_FAT16,
            pm_partition_data.partition_size,
            0,  // default value: size of sector:   512
            0,  // default value: size of cluster:  512
            0,  // default value: reserved sectors count: FAT12/FAT16 = 1; FAT32 = 3
            0,  // default value: count file/dir entries: FAT12/FAT16 = 16; FAT32 = 0
            0,  // default value: count header sectors: 512
            FS_PARTITION_OVERWRITE_CREATE);

    if(OS_SUCCESS != ret)
    {
        printf(
            "Fail to create filesystem on partition: %d! Error code: %d\n",
            pm_partition_data.partition_id,
            ret);

        return false;
    }

    ret = OS_Filesystem_close(phandle);
    if(OS_SUCCESS != ret)
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

    (clientConfig)++->buffer = dataport_buf_chanMux;

    // If the manipulated pointer does not point to the end of clientConfigs
    // array, then data ports got misconfigured.
    assert(clientConfig == &clientConfigs[CLIENT_CONFIGS_COUNT]);
}

void initLogFiles()
{
    OS_LoggerFile_ctor(&log_file_01, PARTITION_ID, LOG_FILENAME_01);
    OS_LoggerFile_ctor(&log_file_02, PARTITION_ID, LOG_FILENAME_02);
    OS_LoggerFile_create(&log_file_01);
    OS_LoggerFile_create(&log_file_02);
}

void initLogTargetsAndSubjects()
{
    OS_LoggerFormat_ctor(&format);

    // register objects to observe
    OS_LoggerSubject_ctor(&subject);

    // Emitter configuration
    OS_LoggerSubject_ctor(&subject_log_server);

    // set up backend
    OS_LoggerOutputFileSystem_ctor(&filesystem, &format);
    OS_LoggerOutputConsole_ctor(&console, &format);

    // Emitter configuration
    OS_LoggerOutputConsole_ctor(&console_log_server, &custom_format);

    // attach observed object to subject
    OS_LoggerSubject_attach(
        (OS_LoggerAbstractSubject_Handle_t *)&subject,
        &filesystem);

    OS_LoggerSubject_attach(
        (OS_LoggerAbstractSubject_Handle_t *)&subject,
        &console);

    // Emitter configuration
    OS_LoggerSubject_attach(
        (OS_LoggerAbstractSubject_Handle_t *)&subject_log_server,
        &console_log_server);
}

void initClients()
{
    // set up registered functions layer
    OS_LoggerConsumerCallback_ctor(
        &log_consumer_callback,
        API_LOG_SERVER_GET_SENDER_ID,
        api_time_server_get_timestamp);

    mapClientConfigsDataPorts();

    // Emitter configuration: set up log emitter layer
    OS_LoggerEmitter_getInstance(
        clientConfigs[LOG_SERVER_ID].buffer,
        &clientConfigs[LOG_SERVER_ID].log_filter,
        API_LOG_SERVER_EMIT);

    for(size_t i = 0; i < CLIENT_CONFIGS_COUNT; ++i)
    {
        OS_LoggerFilter_ctor(
            &clientConfigs[i].log_filter,
             clientConfigs[i].log_level);

        bool isFilterNull = (NO_FILTER_ID_FIRST <= clientConfigs[i].id)
                         && (NO_FILTER_ID_LAST  >= clientConfigs[i].id);

        OS_LoggerFilter_Handle_t* const pFilter = isFilterNull ?
                                        NULL : &clientConfigs[i].log_filter;

        const bool isLogServer  = (LOG_SERVER_ID == clientConfigs[i].id);
        OS_LoggerSubject_Handle_t* pSubject = isLogServer ? &subject_log_server : &subject;

        OS_LoggerConsumer_ctor(
            &clientConfigs[i].consumer,
            clientConfigs[i].buffer,
            pFilter,
            &log_consumer_callback,
            pSubject,
            clientConfigs[i].log_file,
            clientConfigs[i].id,
            clientConfigs[i].name
        );

        OS_LoggerConsumerChain_append(&clientConfigs[i].consumer);
    }
}
