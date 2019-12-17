#include "Debug.h"

#include "log_symbol.h"
#include "local_log_server.h"

#include <camkes.h>



//static Log_filter_t filter_client, filter_server;
//static Log_format_t format;
//char buf[DATABUFFER_SIZE];



int run()
{
//    // set up log filter layer
//    Log_filter_ctor(&filter_client, Debug_LOG_LEVEL_DEBUG);
//    Log_filter_ctor(&filter_server, Debug_LOG_LEVEL_DEBUG);

//    // set up log format layer
//    Log_format_ctor(&format);

//    // local Log server
//    Local_log_server_init(buf, &filter_server, &filter_client, &format, "Local");

//    Debug_LOG_FATAL("APP01");
//    Debug_LOG(Debug_LOG_LEVEL_FATAL, "FATAL", "APP01a");
//    Debug_LOG_FATAL("APP01b");

//    // ...

//    // destruction
//    Log_emitter_dtor();

//    Log_format_dtor((FormatT_t *)&format);

//    Log_filter_dtor(&filter_server);
//    Log_filter_dtor(&filter_client);

    return 0;
}
