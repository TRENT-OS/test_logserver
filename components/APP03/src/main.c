#include "Debug.h"

#include "seos_log_server_config.h"

#if !defined (Debug_Config_PRINT_2_LOG_SERVER)
    #include "log_emitter.h"
#endif

#include <camkes.h>



int run()
{
    get_instance_Log_emitter(DATABUFFER_CLIENT, logServer_ready_wait, dataAvailable_emit);

    Debug_LOG_FATAL("APP03");
    Debug_LOG_ERROR("APP03a");
    Debug_LOG_TRACE("APP03b");
    Debug_LOG_WARNING("APP03c");
    Debug_LOG_WARNING("APP03d");

    // ...

    Log_emitter_dtor();

    return 0;
}
