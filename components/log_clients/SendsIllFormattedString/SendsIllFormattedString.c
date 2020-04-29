#include "LibDebug/Debug.h"
#include <camkes.h>

static OS_LoggerFilter_Handle_t filter;

static void testLogging();
static void tearDownLogging();

void post_init()
{
    OS_LoggerEmitter_getInstance(logServer_buf, NULL, API_LOG_SERVER_EMIT);
}

int run()
{
    // Delaying till other components are done with their logging.
    //
    // This componente throws an exception, so we want it to be the last one to
    // log anything.
    ready_wait();

    testLogging();
    tearDownLogging();

    return 0;
}

void testLogging()
{
    Debug_LOG_INFO("!!! Exception will be thrown below due to the ill formatted"
                      " string!!!");

    // Notice missing arguments for the format specifiers!
    Debug_LOG_INFO("%d %u %o %x %X %f %F %e %E %g %G %a %A %c %s %p %n %%");
}

void tearDownLogging()
{
    OS_LoggerEmitter_dtor();
    OS_LoggerFilter_dtor(&filter);
}
