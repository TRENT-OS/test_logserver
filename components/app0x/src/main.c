#include "LibDebug/Debug.h"

#include "seos_logger.h"

#include <camkes.h>



#if !defined (DATABUFFER_CLIENT)
    #define DATABUFFER_CLIENT       (void *)dataport_buf
#endif



#define LOG_FILENAME_01             "log_01.txt"
#define LOG_FILENAME_02             "log_02.txt"



static Log_file_client_callback_t log_file_client_callback;
static Log_file_client_t log_file_client;



int run()
{
    // wait until log server is finished
    logServer_finish_wait();

    char buf[DATABUFFER_SIZE * 3];
    bool retval;

    // set up log file client callback
    Log_file_client_callback_ctor(&log_file_client_callback, API_LOG_SERVER_READ_LOG_FILE);

    // set up log file client
    Log_file_client_ctor(&log_file_client, DATABUFFER_CLIENT, buf, &log_file_client_callback);

    // read log file 01
    retval = Log_file_client_read_log_file(&log_file_client, LOG_FILENAME_01, 0, DATABUFFER_SIZE);
    if(retval == false){
        Debug_LOG_ERROR("Fail to read log file!");
        return 0;
    }

    printf("log file 01 content:\n%s", buf);

    // read log file 02
    retval = Log_file_client_read_log_file(&log_file_client, LOG_FILENAME_02, 0, DATABUFFER_SIZE);
    if(retval == false){
        Debug_LOG_ERROR("Fail to read log file!");
        return 0;
    }

    printf("log file 02 content:\n"
           "%s"
           "demo finished successfully\n",
                buf);

    // destruction
    Log_file_client_callback_dtor(&log_file_client_callback);

    Log_file_client_dtor(&log_file_client);

    return 0;
}
