#include "LibDebug/Debug.h"
#include "Logger/Client/OS_LoggerFileClient.h"
#include <camkes.h>



#if !defined (DATABUFFER_CLIENT)
    #define DATABUFFER_CLIENT       (void *)dataport_buf
#endif



#define LOG_FILENAME_01             "log_01.txt"
#define LOG_FILENAME_02             "log_02.txt"



static OS_LoggerFileClientCallback_Handle_t log_file_client_callback;
static OS_LoggerFileClient_Handle_t log_file_client;



int run()
{
    // wait until log server is finished
    logServer_finish_wait();

    char buf[DATABUFFER_SIZE * 3];
    bool retval;

    // set up log file client callback
    OS_LoggerFileClientCallback_ctor(&log_file_client_callback, API_LOG_SERVER_READ_LOG_FILE);

    // set up log file client
    OS_LoggerFileClient_ctor(&log_file_client, DATABUFFER_CLIENT, buf, &log_file_client_callback);

    // read log file 01
    retval = OS_LoggerFileClient_read(&log_file_client, LOG_FILENAME_01, 0, DATABUFFER_SIZE);
    if(retval == false){
        Debug_LOG_ERROR("Fail to read log file!");
        return 0;
    }

    printf("log file 01 content:\n%s", buf);

    // read log file 02
    retval = OS_LoggerFileClient_read(&log_file_client, LOG_FILENAME_02, 0, DATABUFFER_SIZE);
    if(retval == false){
        Debug_LOG_ERROR("Fail to read log file!");
        return 0;
    }

    printf("log file 02 content:\n"
           "%s"
           "demo finished successfully\n",
                buf);

    // destruction
    OS_LoggerFileClientCallback_dtor(&log_file_client_callback);

    OS_LoggerFileClient_dtor(&log_file_client);

    return 0;
}
