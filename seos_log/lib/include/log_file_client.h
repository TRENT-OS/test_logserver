#pragma once


#include "log_symbol.h"
#include "log_file_client_callback.h"
#include <stdint.h>
#include <stdbool.h>


typedef struct Log_file_client_t Log_file_client_t;


typedef void
(*Log_file_client_dtorT)(Log_file_client_t *self);


typedef bool
(*Log_file_client_read_log_fileT)(Log_file_client_t *self, uint64_t offset, uint64_t len);


typedef struct
{
    Log_file_client_dtorT          dtor;
    Log_file_client_read_log_fileT read_log_file;
} Log_file_client_Vtable;


struct Log_file_client_t
{
    void *src_buf;
    void *dest_buf;
    Log_file_client_callback_t *callback_vtable;
    const Log_file_client_Vtable *vtable;
};


bool
Log_file_client_ctor(Log_file_client_t *self,
                     void *src_buf,
                     void *dest_buf,
                     Log_file_client_callback_t *log_file_client_callback);


void
Log_file_client_dtor(Log_file_client_t *self);


bool
Log_file_client_read_log_file(Log_file_client_t *self, uint64_t offset, uint64_t len);
