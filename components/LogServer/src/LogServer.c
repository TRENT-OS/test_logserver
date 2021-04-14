#include "lib_debug/Debug.h"
#include "TimeServer.h"

#include "Logger/Server/OS_LoggerFile.h"

#include "Logger/Server/OS_LoggerConsumerChain.h"
#include "Logger/Server/OS_LoggerConsumer.h"

#include "Logger/Server/OS_LoggerOutputConsole.h"
#include "Logger/Server/OS_LoggerOutputFileSystem.h"

#include "Logger/Client/OS_LoggerEmitter.h"

#include "custom_format.h"

#include "OS_FileSystem.h"

#include <stdio.h>
#include <camkes.h>

// log server id
#define LOG_SERVER_ID  0u

#define NO_FILTER_ID_FIRST 0xCAFE
#define NO_FILTER_ID_LAST  (NO_FILTER_ID_FIRST + Debug_LOG_LEVEL_CUSTOM)

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

    { .name = "LogFileReader",    .log_level = Debug_LOG_LEVEL_DEBUG,   .log_file = NULL,         .id = 50001u },
    { .name = "FileReaderWriter", .log_level = Debug_LOG_LEVEL_DEBUG,   .log_file = &log_file_02, .id = 50002u },
};

static OS_FileSystem_Config_t cfgFs =
{
    .type = OS_FileSystem_Type_FATFS,
    .size = OS_FileSystem_USE_STORAGE_MAX,
    .storage = IF_OS_STORAGE_ASSIGN(
        storage_rpc,
        storage_port),
};

static const if_OS_Timer_t timer = IF_OS_TIMER_ASSIGN(
                                       timeServer_rpc,
                                       timeServer_notify);

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

static OS_FileSystem_Handle_t hFs;

void pre_init()
{
    // set up consumer chain
    OS_LoggerConsumerChain_getInstance();

    initLogTargetsAndSubjects();
    initClients();

    // create filesystem
    if (!filesystem_init())
    {
        printf("Fail to init filesystem!\n");
        return;
    }

    initLogFiles();

    LOG_SUCCESS();
}

static uint64_t
get_time_sec(
    void)
{
    OS_Error_t err;
    uint64_t sec;

    if ((err = TimeServer_getTime(&timer, TimeServer_PRECISION_SEC,
                                  &sec)) != OS_SUCCESS)
    {
        Debug_LOG_ERROR("TimeServer_getTime() failed with %d", err);
        sec = 0;
    }

    return sec;
}

int
run()
{
    OS_Error_t err;
    const int demo_time_sec = 75;

    for (
        int time_passed_sec = 0;
        time_passed_sec < demo_time_sec;
        ++time_passed_sec)
    {
        if ((err = TimeServer_sleep(&timer, TimeServer_PRECISION_SEC, 1)) != OS_SUCCESS)
        {
            Debug_LOG_ERROR("TimeServer_sleep() failed with %d", err);
        }
    }

    printf("logServer finish\n");

    logServer_finish_emit();

    return 0;
}

static bool
filesystem_init(void)
{
    OS_Error_t err = OS_FileSystem_init(&hFs, &cfgFs);
    if (OS_SUCCESS != err)
    {
        printf("OS_FileSystem_init failed with error code %d!", err);
        return false;
    }
    err = OS_FileSystem_format(hFs);
    if (OS_SUCCESS != err)
    {
        printf("OS_FileSystem_format failed with error code %d!", err);
        return false;
    }
    err = OS_FileSystem_mount(hFs);
    if (OS_SUCCESS != err)
    {
        printf("OS_FileSystem_mount failed with error code %d!", err);
        return false;
    }

    return true;
}

/** Unfortunately needs to be evaluated at runtime i.e. they are not defined at
 *  compile time. */
void mapClientConfigsDataPorts()
{
    ClientConfig_t* clientConfig = &clientConfigs[0];

    (clientConfig)++->buffer = buf_log_server;

    (clientConfig)++->buffer = buf_lvl_none_port;
    (clientConfig)++->buffer = buf_lvl_assert_port;
    (clientConfig)++->buffer = buf_lvl_fatal_port;
    (clientConfig)++->buffer = buf_lvl_error_port;
    (clientConfig)++->buffer = buf_lvl_warning_port;
    (clientConfig)++->buffer = buf_lvl_info_port;
    (clientConfig)++->buffer = buf_lvl_debug_port;
    (clientConfig)++->buffer = buf_lvl_trace_port;
    (clientConfig)++->buffer = buf_lvl_custom_port;
    (clientConfig)++->buffer = buf_lvl_max_port;

    (clientConfig)++->buffer = buf_no_filters_port;

    (clientConfig)++->buffer = buf_cl_filter_none_port;
    (clientConfig)++->buffer = buf_cl_filter_assert_port;
    (clientConfig)++->buffer = buf_cl_filter_fatal_port;
    (clientConfig)++->buffer = buf_cl_filter_error_port;
    (clientConfig)++->buffer = buf_cl_filter_warning_port;
    (clientConfig)++->buffer = buf_cl_filter_info_port;
    (clientConfig)++->buffer = buf_cl_filter_debug_port;
    (clientConfig)++->buffer = buf_cl_filter_trace_port;
    (clientConfig)++->buffer = buf_cl_filter_custom_port;

    (clientConfig)++->buffer = buf_logFileReader_port;
    (clientConfig)++->buffer = buf_fileReaderWriter_port;

    // If the manipulated pointer does not point to the end of clientConfigs
    // array, then data ports got misconfigured.
    assert(clientConfig == &clientConfigs[CLIENT_CONFIGS_COUNT]);
}

void initLogFiles()
{
    OS_LoggerFile_ctor(&log_file_01, hFs, LOG_FILENAME_01);
    OS_LoggerFile_ctor(&log_file_02, hFs, LOG_FILENAME_02);
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
        (OS_LoggerAbstractSubject_Handle_t*)&subject,
        &filesystem);

    OS_LoggerSubject_attach(
        (OS_LoggerAbstractSubject_Handle_t*)&subject,
        &console);

    // Emitter configuration
    OS_LoggerSubject_attach(
        (OS_LoggerAbstractSubject_Handle_t*)&subject_log_server,
        &console_log_server);
}

void initClients()
{
    // set up registered functions layer
    OS_LoggerConsumerCallback_ctor(
        &log_consumer_callback,
        API_LOG_SERVER_GET_SENDER_ID,
        get_time_sec);

    mapClientConfigsDataPorts();

    // Emitter configuration: set up log emitter layer
    OS_LoggerEmitter_getInstance(
        clientConfigs[LOG_SERVER_ID].buffer,
        &clientConfigs[LOG_SERVER_ID].log_filter,
        API_LOG_SERVER_EMIT);

    for (size_t i = 0; i < CLIENT_CONFIGS_COUNT; ++i)
    {
        OS_LoggerFilter_ctor(
            &clientConfigs[i].log_filter,
            clientConfigs[i].log_level);

        bool isFilterNull = (NO_FILTER_ID_FIRST <= clientConfigs[i].id)
                            && (NO_FILTER_ID_LAST  >= clientConfigs[i].id);

        OS_LoggerFilter_Handle_t* const pFilter = isFilterNull ?
                                                  NULL : &clientConfigs[i].log_filter;

        const bool isLogServer  = (LOG_SERVER_ID == clientConfigs[i].id);
        OS_LoggerSubject_Handle_t* pSubject = isLogServer
                                              ? &subject_log_server
                                              : &subject;

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
