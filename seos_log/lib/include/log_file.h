#pragma once


#include "log_symbol.h"
#include "seos_fs.h"
#include <stdint.h>
#include <stdbool.h>


typedef struct Log_file_t Log_file_t;


typedef void
(*Log_file_dtorT)(Log_file_t *self);


typedef int64_t
(*Log_file_read_log_fileT)(Log_file_t *self, uint64_t offset, uint64_t len, int64_t log_file_size);


typedef struct
{
    Log_file_dtorT          dtor;
    Log_file_read_log_fileT read_log_file;
} Log_file_Vtable;


typedef struct
{
    uint8_t      drv_id;
    char         filename[LOG_ID_NAME_LENGTH];
    hPartition_t phandle;
    uint64_t     offset;
    uint64_t     lenght;
} Log_file_info_t;


struct Log_file_t
{
    Log_file_info_t       log_file_info;
    const Log_file_Vtable *vtable;
};


bool
Log_file_ctor(Log_file_t *self,
              uint8_t drv_id,
              const char *filename);


void
Log_file_dtor(Log_file_t *self);
