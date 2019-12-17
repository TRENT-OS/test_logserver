#pragma once


#include "formatT.h"


typedef struct
{
    char                 buffer[FORMAT_BUFFER_SIZE];
    const FormatT_Vtable *vtable;
} Log_format_t;


bool
Log_format_ctor(Log_format_t *self);


void
Log_format_dtor(FormatT_t *self);
