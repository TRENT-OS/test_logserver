#pragma once


#include <stdbool.h>
#include <stdint.h>


typedef struct Log_filter_t Log_filter_t;


typedef void
(*Log_filter_dtorT)(Log_filter_t *self);


typedef bool
(*Log_filter_filteringT)(void *buf, uint8_t log_level_client);


typedef struct
{
    Log_filter_dtorT      dtor;
    Log_filter_filteringT filtering;
} Log_filter_Vtable;


struct Log_filter_t
{
    const Log_filter_Vtable *vtable;
} ;


bool
Log_filter_ctor(Log_filter_t *self);


void
Log_filter_dtor(Log_filter_t *self);
