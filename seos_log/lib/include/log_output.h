#pragma once


#include "abstract_log_output.h"
#include "log_format.h"
#include "seos_fs.h"
#include "listT.h"
#include <stdbool.h>


typedef struct
{
    uint8_t      drv_id;
    char         filename[LOG_ID_NAME_LENGTH];
    hPartition_t phandle;
    uint32_t     file_offset;
} Log_output_filesystem_t;


typedef struct
{
    NodeT_t                 node;
    ListT_t                 listT;
    Log_output_filesystem_t fs;
    Log_format_t            *log_format;
    const Output_Vtable     *vtable;
} Log_output_t;


bool
Log_output_filesystem_ctor(Log_output_t *self,
                           Log_format_t *log_format,
                           uint8_t drv_id,
                           const char *filename);


void
Log_output_filesystem_dtor(Output_t *self);



bool
Log_output_console_ctor(Log_output_t *self,
                        Log_format_t *log_format);


void
Log_output_console_dtor(Output_t *self);
