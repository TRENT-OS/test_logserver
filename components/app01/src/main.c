#include "LibDebug/Debug.h"

#include "seos_logger.h"

#include <camkes.h>



#if !defined (DATABUFFER_CLIENT)
    #define DATABUFFER_CLIENT       (void *)dataport_buf
#endif



static Log_filter_t filter;
static Log_emitter_callback_t reg;



int run()
{
    // set up registered functions layer
    Log_emitter_callback_ctor(&reg, logServer_ready_wait, API_LOG_SERVER_EMIT);

    // set up log filter layer
    Log_filter_ctor(&filter, Debug_LOG_LEVEL_DEBUG);

    get_instance_Log_emitter(DATABUFFER_CLIENT, &filter, &reg);

    Debug_LOG_FATAL("APP01");
    Debug_LOG(Debug_LOG_LEVEL_FATAL, "FATAL", "APP01a");
    //Debug_LOG_FATAL("APP01a");
    Debug_LOG_FATAL("APP01b");

    // ...

    // destruction
    Log_emitter_dtor();

    Log_emitter_callback_dtor(&reg);

    Log_filter_dtor(&filter);

    return 0;
}
