#pragma once


#include "log_symbol.h"
#include "timestamp.h"
#include <stdint.h>


typedef struct
{
    uint8_t log_level_srv;
    uint8_t log_level_client;
    char    log_message[LOG_MESSAGE_LENGTH];
} Log_databuffer_t;


typedef struct
{
    char             log_id_name[LOG_ID_NAME_LENGTH];
    Timestamp_t      timestamp;
    Log_databuffer_t log_databuffer;
} Log_info_t;


int
Log_databuffer_set_log_level_server(void *buf, uint8_t log_level_server);


int
Log_databuffer_set_log_level_client(void *buf, uint8_t log_level_client);


int
Log_databuffer_set_log_message(void *buf, const char *msg);


bool
Log_databuffer_get_log_level_server(void *buf, Log_databuffer_t *log_databuffer);


bool
Log_databuffer_get_log_level_client(void *buf, Log_databuffer_t *log_databuffer);


bool
Log_databuffer_get_log_message(void *buf, Log_databuffer_t *log_databuffer);


bool
Log_databuffer_get_info(void *buf, Log_databuffer_t *log_databuffer);


bool
Log_databuffer_clear_databuffer(void *buf);
