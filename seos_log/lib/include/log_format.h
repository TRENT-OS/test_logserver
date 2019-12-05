#pragma once


#include "formatT.h"


typedef struct
{
    FormatT_t parent;
    char      buffer[FORMAT_BUFFER_SIZE];
}  Log_format_t;


bool
Log_format_ctor(Log_format_t *self);


void
Log_format_dtor(FormatT_t *self);


bool
Log_format_convert(FormatT_t *self, Log_info_t *log_info);


void
Log_format_print(FormatT_t *self);
