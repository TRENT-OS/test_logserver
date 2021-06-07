#include "lib_debug/Debug.h"
#include "Logger/Common/OS_LoggerEntry.h"

#include <camkes.h>

#include <string.h>

static OS_LoggerFilter_Handle_t filter;

// Max possible entry is (OS_Logger_ENTRY_MESSAGE_LENGTH + 1 - LOG_HEADER_SZ).
//
// Currently the log header that the client generates consists of:
// 1. log lvl (9 characters) e.g. ` ASSERT: `
// 2. file name (126 characters) e.g.
//      `/home/jenkins/workspace/generic_jobs/generic_pipeline_sandbox/src/
//          system/components/log_clients/LogsAllLevels/LogsAllLevels.c:`
// 3. file line (4 characters including trailing whitespace) e.g. `43: `
#define LOG_HEADER_SZ (9 + sizeof(__FILE__) + 4)

static char maxPossibleLogEntry[(OS_Logger_ENTRY_MESSAGE_LENGTH + 1)
                                - LOG_HEADER_SZ];

// The following entry tries to log a full dataport size without regarding the
// necessary overhead of the metadata and the header. This message should get
// truncated to the max allowed size as outlined above.
static char tooLargeLogEntry[OS_DATAPORT_DEFAULT_SIZE];

static const char format_string[] =
    "%d %u %o %x %X %f %F %e %E %g %G %a %A %c %s %p %n %%";

#define NO_FILTER -1

void post_init()
{
    if (NO_FILTER == log_lvl)
    {
        OS_LoggerEmitter_getInstance(logServer_port, NULL, API_LOG_SERVER_EMIT);
    }
    else
    {
        OS_LoggerFilter_ctor(&filter, log_lvl);
        OS_LoggerEmitter_getInstance(logServer_port, &filter, API_LOG_SERVER_EMIT);
    }

    memset(maxPossibleLogEntry, 'H', sizeof(maxPossibleLogEntry));
    maxPossibleLogEntry[sizeof(maxPossibleLogEntry) - 1] = '\0';

    memset(tooLargeLogEntry, 'L', sizeof(tooLargeLogEntry));
    tooLargeLogEntry[sizeof(tooLargeLogEntry) - 1] = '\0';

    // Copy a marker to the end of the max possible entry length. This marker
    // will be used by the test parser to verify that the too large entry got
    // truncated correctly.
    memcpy(
        &tooLargeLogEntry[sizeof(maxPossibleLogEntry) - sizeof("END")],
        "END",
        strlen("END")); // Do not copy null terminator.
}

int run()
{
    Debug_LOG(Debug_LOG_LEVEL_NONE, "   NONE", "Debug_LOG_NONE");
    Debug_LOG(Debug_LOG_LEVEL_MAX,  "    MAX", "Debug_LOG_MAX");

    Debug_LOG_ASSERT("");
    Debug_LOG_ASSERT("%s", maxPossibleLogEntry);
    Debug_LOG_ASSERT("%s", tooLargeLogEntry);
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
