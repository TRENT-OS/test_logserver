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
    Log_emitter_callback_ctor(&reg, logServer_ready_wait, log_server_interface_emit);

    // set up log filter layer
    Log_filter_ctor(&filter, Debug_LOG_LEVEL_DEBUG);

    get_instance_Log_emitter(DATABUFFER_CLIENT, &filter, &reg);

    Debug_LOG_WARNING("APP02");
    Debug_LOG_ERROR("APP02a");

    // ...

    // destruction
    Log_emitter_dtor();

    Log_emitter_callback_dtor(&reg);

    Log_filter_dtor(&filter);

    return 0;
}
