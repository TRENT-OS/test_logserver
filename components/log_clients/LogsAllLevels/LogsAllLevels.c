#include "lib_debug/Debug.h"
#include <camkes.h>

#include <string.h>

static OS_LoggerFilter_Handle_t filter;

// Max possible entry is 3941 characters (PAGE_SIZE - log_header
// e.g. 4096 - 193).
//
// Currently log_header's size is 193, and it consists of:
//
// 1. client id (11 characters) e.g. `0000051966 `
// 2. client name (15 characters) e.g. `FILTER_NULL    `
// 3. timestamp (20 characters) e.g. `01.01.1969-00:00:00 `
// 4. client filter_lvl (4 characters) e.g. `  0 `
// 5. server filer_lvl (4 characters) e.g. `  0 `
// 6. log lvl (9 characters) e.g. ` ASSERT: `
// 7. file name (126 characters) e.g.
//      `/home/jenkins/workspace/generic_jobs/generic_pipeline_sandbox/src/
//          system/components/log_clients/LogsAllLevels/LogsAllLevels.c:`
// 8. file line (4 characters including trailing whitespace) e.g. `43: `
#define LOG_HEADER_SZ (11 + 15 + 20 + 4 + 4 + sizeof(__FILE__) + 4)
static char maxPossibleLogEntry[PAGE_SIZE - LOG_HEADER_SZ];

static const char format_string[] =
                    "%d %u %o %x %X %f %F %e %E %g %G %a %A %c %s %p %n %%";

#define NO_FILTER -1

void post_init()
{
    if(NO_FILTER == log_lvl)
    {
        OS_LoggerEmitter_getInstance(logServer_buf, NULL, API_LOG_SERVER_EMIT);
    }
    else
    {
        OS_LoggerFilter_ctor(&filter, log_lvl);
        OS_LoggerEmitter_getInstance(logServer_buf, &filter, API_LOG_SERVER_EMIT);
    }

    memset(maxPossibleLogEntry, 'H', sizeof(maxPossibleLogEntry));
}

int run()
{
    Debug_LOG(Debug_LOG_LEVEL_NONE, "   NONE", "Debug_LOG_NONE");
    Debug_LOG(Debug_LOG_LEVEL_MAX,  "    MAX", "Debug_LOG_MAX");

    Debug_LOG_ASSERT("");
    Debug_LOG_ASSERT("%s", maxPossibleLogEntry);
    Debug_LOG_ASSERT("%s", format_string);

    Debug_LOG_ASSERT ("Debug_LOG_ASSERT");
    Debug_LOG_FATAL  ("Debug_LOG_FATAL");
    Debug_LOG_ERROR  ("Debug_LOG_ERROR");
    Debug_LOG_WARNING("Debug_LOG_WARNING");
    Debug_LOG_INFO   ("Debug_LOG_INFO");
    Debug_LOG_DEBUG  ("Debug_LOG_DEBUG");
    Debug_LOG_TRACE  ("Debug_LOG_TRACE");

    Debug_LOG(Debug_LOG_LEVEL_CUSTOM, " CUSTOM", "Debug_LOG_CUSTOM");

    return 0;
}
