#include "LibDebug/Debug.h"
#include <camkes.h>

static void testLogging();

void post_init()
{
    OS_LoggerEmitter_getInstance(logServer_buf, NULL, API_LOG_SERVER_EMIT);

    if(0 != ready_reg_callback(&testLogging, NULL))
    {
        Debug_LOG_INFO("Failed to register a callback for the 'ready' event.");
    }
}

void testLogging()
{
    Debug_LOG_INFO("!!! Exception will be thrown below due to the ill formatted"
                      " string!!!");

    // Notice missing arguments for the format specifiers!
    Debug_LOG_INFO("%d %u %o %x %X %f %F %e %E %g %G %a %A %c %s %p %n %%");
}
