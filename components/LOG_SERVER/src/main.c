#include "Debug.h"

#include "seos_log_server_config.h"
#include "consumer_chain.h"

#include <camkes.h>



int run()
{
    Log_consumer_t log_consumer_01, log_consumer_02, log_consumer_03;

    Log_consumer_ctor(&log_consumer_01, DATABUFFER_SERVER_01, Debug_LOG_LEVEL_NONE, dataAvailable_app01_reg_callback, logServer_ready_app01_emit, api_time_server_get_timestamp, "APP01");
    Log_consumer_ctor(&log_consumer_02, DATABUFFER_SERVER_02, Debug_LOG_LEVEL_WARNING, dataAvailable_app02_reg_callback, logServer_ready_app02_emit, api_time_server_get_timestamp, NULL);
    Log_consumer_ctor(&log_consumer_03, DATABUFFER_SERVER_03, Debug_LOG_LEVEL_WARNING, dataAvailable_app03_reg_callback, logServer_ready_app03_emit, api_time_server_get_timestamp, "APP03");

    get_instance_Consumer_chain();

    Consumer_chain_append(&log_consumer_01);
    Consumer_chain_append(&log_consumer_02);
    Consumer_chain_append(&log_consumer_03);

    Consumer_chain_poll();

    Consumer_chain_dtor();

    Log_consumer_dtor(&log_consumer_01);
    Log_consumer_dtor(&log_consumer_02);
    Log_consumer_dtor(&log_consumer_03);

    return 0;
}
