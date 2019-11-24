#include "Debug.h"

#include "seos_log_server_config.h"

#if !defined (Debug_Config_PRINT_2_LOG_SERVER)
    #include "log_emitter.h"
#endif

#include <camkes.h>



int run()
{
    get_instance_Log_emitter(DATABUFFER_CLIENT, logServer_ready_wait, dataAvailable_emit);

    Debug_LOG_WARNING("APP02");
    Debug_LOG_ERROR("APP02a");

    // ...

    Log_emitter_dtor();

    return 0;
}
