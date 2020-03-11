#include "LibDebug/Debug.h"
#include "seos_logger.h"
#include <camkes.h>

static Log_filter_t filter;
static Log_emitter_callback_t reg;

static void setUpLogging();
static void testLogging();
static void tearDownLogging();

int run()
{
    setUpLogging();
    testLogging();
    tearDownLogging();

    return 0;
}

void setUpLogging()
{
    Log_emitter_callback_ctor(&reg, logServer_ready_wait, API_LOG_SERVER_EMIT);
    get_instance_Log_emitter(logServer_buf, NULL, &reg);
}

void testLogging()
{
    // Notice missing arguments for the format specifiers!
    Debug_LOG_INFO("%d %u %o %x %X %f %F %e %E %g %G %a %A %c %s %p %n %%");
}

void tearDownLogging()
{
    Log_emitter_dtor();
    Log_emitter_callback_dtor(&reg);
    Log_filter_dtor(&filter);
}
