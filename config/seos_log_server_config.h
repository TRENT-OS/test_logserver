#pragma once


// include headers
#include <camkes.h>
#include <stdio.h>


// setup common
#define DATABUFFER_SIZE                 400 // PAGE_SIZE


// setup dataports
// client part (all client dataports has the same name)
#if !defined (DATABUFFER_CLIENT)
    #define DATABUFFER_CLIENT           (void *)dataport_buf
#endif


// server part (each dataport have to name separately)
#define DATABUFFER_SERVER_01            (void *)dataport_buf_app01
#define DATABUFFER_SERVER_02            (void *)dataport_buf_app02
#define DATABUFFER_SERVER_03            (void *)dataport_buf_app03
//...
