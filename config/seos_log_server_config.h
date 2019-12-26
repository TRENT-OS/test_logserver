#pragma once


// include headers
#include <camkes.h>
#include <stdio.h>


// setup common
#define DATABUFFER_SIZE                 PAGE_SIZE


// setup dataports
// client part (all client dataports has the same name)
#if !defined (DATABUFFER_CLIENT)
    #define DATABUFFER_CLIENT           (void *)dataport_buf
#endif


// server part (each dataport have to name separately)
#define DATABUFFER_SERVER_01            (void *)dataport_buf_app01
#define DATABUFFER_SERVER_02            (void *)dataport_buf_app02
#define DATABUFFER_SERVER_03            (void *)dataport_buf_app03
#define DATABUFFER_SERVER_0x            (void *)dataport_buf_app0x
//...


// api interface name
#define LOG_SERVER_EMIT                 logServer_ready_emit
#define CLIENT_WAIT                     logServer_ready_wait

#define API_LOG_SERVER_EMIT             log_server_interface_emit
#define API_LOG_SERVER_GET_SENDER_ID    log_server_interface_get_sender_id
#define API_LOG_SERVER_READ_LOG_FILE    log_server_interface_read_log_file

#define API_TIME_SERVER_GET_TIMESTAMP   api_time_server_get_timestamp


// client id's
#define CLIENT_APP01_ID                 10
#define CLIENT_APP02_ID                 200
#define CLIENT_APP03_ID                 3000
#define CLIENT_APP0x_ID                 40000
