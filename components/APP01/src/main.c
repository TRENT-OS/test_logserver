#include "Debug.h"

#include "seos_log_server_config.h"
#include "local_log_server.h"

#include <camkes.h>



int run()
{
    // local Log server
    char buf[DATABUFFER_SIZE];

    Local_log_server_init(buf, Debug_LOG_LEVEL_NONE, "Local");

    Debug_LOG_FATAL("APP01");
    Debug_LOG(Debug_LOG_LEVEL_FATAL, "FATAL", "APP01a");
    Debug_LOG_FATAL("APP01b");

    // ...

    Log_emitter_dtor();

    return 0;
}
