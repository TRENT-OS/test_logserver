#include "Debug.h"

#include "log_symbol.h"
#include "log_emitter_callback.h"
#include "log_filter.h"

#if !defined (Debug_Config_PRINT_2_LOG_SERVER)
    #include "log_emitter.h"
#endif

#include <camkes.h>



static Log_filter_t filter;
static Log_emitter_callback_t reg;



int run()
{
    // set up registered functions layer
    Log_emitter_callback_ctor(&reg, CLIENT_WAIT, API_LOG_SERVER_EMIT);

    // set up log filter layer
    Log_filter_ctor(&filter, Debug_LOG_LEVEL_DEBUG);

    get_instance_Log_emitter(DATABUFFER_CLIENT, &filter, &reg);

    Debug_LOG_FATAL("APP03");
    Debug_LOG_ERROR("APP03a");
    Debug_LOG_TRACE("APP03b");
    Debug_LOG_WARNING("APP03c");
    Debug_LOG_WARNING("APP03d");

    // ...

    // destruction
    Log_emitter_dtor();

    Log_emitter_callback_dtor(&reg);

    Log_filter_dtor(&filter);

    return 0;
}
