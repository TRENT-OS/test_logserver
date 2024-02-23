/*
 * Copyright (C) 2020-2024, HENSOLDT Cyber GmbH
 * 
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * For commercial licensing, contact: info.cyber@hensoldt.net
 */

#include "lib_debug/Debug.h"
#include "Logger/Client/OS_LoggerFileClient.h"
#include <camkes.h>

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
    OS_LoggerFileClientCallback_ctor(
        &log_file_client_callback,
        API_LOG_SERVER_READ_LOG_FILE);

    // set up log file client
    OS_LoggerFileClient_ctor(
        &log_file_client,
        logServer_port,
        buf,
        &log_file_client_callback);

    // read log file 01
    retval = OS_LoggerFileClient_read(
                 &log_file_client,
                 LOG_FILENAME_01,
                 0,
                 DATABUFFER_SIZE);
    if (retval != OS_SUCCESS)
    {
        Debug_LOG_ERROR("Fail to read log file! Error code: %d", retval);
        return 0;
    }

    printf("log file 01 content:\n%s", buf);

    // read log file 02
    retval = OS_LoggerFileClient_read(
                 &log_file_client,
                 LOG_FILENAME_02,
                 0,
                 DATABUFFER_SIZE);
    if (retval != OS_SUCCESS)
    {
        Debug_LOG_ERROR("Fail to read log file! Error code: %d", retval);
        return 0;
    }

    printf("\nlog file 02 content:\n"
           "%s"
           "\ntest finished successfully\n",
           buf);

    return 0;
}
