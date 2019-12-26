#include "Debug.h"

#include <camkes.h>

#include "log_symbol.h"
#include "log_file_client.h"
#include "log_file_client_callback.h"

#include <stdint.h>
#include <string.h>
#include <stdio.h>



static Log_file_client_callback_t log_file_client_callback;
static Log_file_client_t log_file_client;



int run()
{
    // wait until log server is finished
    logServer_finish_wait();

    char buf[DATABUFFER_SIZE];
    bool retval;

    // set up log file client callback
    Log_file_client_callback_ctor(&log_file_client_callback, API_LOG_SERVER_READ_LOG_FILE);

    // set up log file client
    Log_file_client_ctor(&log_file_client, DATABUFFER_CLIENT, buf, &log_file_client_callback);

    // read log file
    retval = Log_file_client_read_log_file(&log_file_client, 0, DATABUFFER_SIZE);
    if(retval == false){
        Debug_LOG_ERROR("Fail to read log file!");
        return 0;
    }

    printf("log file content:\n%s", buf);

    // destruction
    Log_file_client_callback_dtor(&log_file_client_callback);

    Log_file_client_dtor(&log_file_client);

    return 0;
}