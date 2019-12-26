#pragma once


#include "log_symbol.h"
#include <stdint.h>
#include <stdbool.h>


typedef int64_t
(*Log_file_client_callback_read_log_fileT)(uint64_t offset, uint64_t len, int64_t *log_file_size);


typedef struct
{
    Log_file_client_callback_read_log_fileT read_log_file;
} Log_file_client_callback_t;


bool
Log_file_client_callback_ctor(Log_file_client_callback_t *self,
                     Log_file_client_callback_read_log_fileT read_log_file);


void
Log_file_client_callback_dtor(Log_file_client_callback_t *self);
