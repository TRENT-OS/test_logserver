#pragma once


#include "abstract_log_output.h"
#include "log_format.h"
#include "log_file.h"
#include "listT.h"
#include <stdbool.h>


typedef struct
{
    NodeT_t             node;
    ListT_t             listT;
    Log_file_info_t     *log_file_info;
    Log_format_t        *log_format;
    const Output_Vtable *vtable;
} Log_output_t;


bool
Log_output_filesystem_ctor(Log_output_t *self,
                           Log_format_t *log_format,
                           Log_file_info_t *log_file_info);


void
Log_output_filesystem_dtor(Output_t *self);



bool
Log_output_console_ctor(Log_output_t *self,
                        Log_format_t *log_format);


void
Log_output_console_dtor(Output_t *self);
