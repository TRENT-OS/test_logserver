#include "Debug.h"

#include "log_symbol.h"
#include "consumer_chain.h"

#include <camkes.h>



int run()
{
    Log_filter_t filter_01, filter_02, filter_03;
    Log_consumer_t log_consumer_01, log_consumer_02, log_consumer_03;
    Log_consumer_callback_t reg_01, reg_02, reg_03;
    get_instance_Consumer_chain();

    // set up log filter layer
    Log_filter_ctor(&filter_01, Debug_LOG_LEVEL_DEBUG);
    Log_filter_ctor(&filter_02, Debug_LOG_LEVEL_DEBUG);
    Log_filter_ctor(&filter_03, Debug_LOG_LEVEL_DEBUG);

    // set up registered functions layer
    Log_consumer_callback_ctor(&reg_01, dataAvailable_app01_reg_callback, logServer_ready_app01_emit, api_time_server_get_timestamp);
    Log_consumer_callback_ctor(&reg_02, dataAvailable_app02_reg_callback, logServer_ready_app02_emit, api_time_server_get_timestamp);
    Log_consumer_callback_ctor(&reg_03, dataAvailable_app03_reg_callback, logServer_ready_app03_emit, api_time_server_get_timestamp);

    // set up log consumer layer
    Log_consumer_ctor(&log_consumer_01, DATABUFFER_SERVER_01, &filter_01, &reg_01, "APP01");
    Log_consumer_ctor(&log_consumer_02, DATABUFFER_SERVER_02, &filter_02, &reg_02, NULL);
    Log_consumer_ctor(&log_consumer_03, DATABUFFER_SERVER_03, &filter_03, &reg_03, "APP03");

    // set up consumer chain layer
    Consumer_chain_append(&log_consumer_01);
    Consumer_chain_append(&log_consumer_02);
    Consumer_chain_append(&log_consumer_03);

    Consumer_chain_poll();



    // destruction
    Consumer_chain_dtor();

    Log_consumer_callback_dtor(&reg_01);
    Log_consumer_callback_dtor(&reg_02);
    Log_consumer_callback_dtor(&reg_03);

    Log_filter_dtor(&filter_01);
    Log_filter_dtor(&filter_02);
    Log_filter_dtor(&filter_03);

    Log_consumer_dtor(&log_consumer_01);
    Log_consumer_dtor(&log_consumer_02);
    Log_consumer_dtor(&log_consumer_03);

    return 0;
}
