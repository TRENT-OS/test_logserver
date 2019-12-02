#pragma once


#include "log_filter.h"
#include "log_emitter_callback.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>


typedef struct
{
    void                   *buf;
    Log_filter_t           *log_filter;
    Log_emitter_callback_t *callback_vtable;
} Log_emitter_t;


Log_emitter_t *
get_instance_Log_emitter(void *buffer,
                         Log_filter_t *log_filter,
                         Log_emitter_callback_t *callback_vtable);


void
Log_emitter_dtor(void);


void *
Log_emitter_get_buffer(void);


bool
Log_emitter_wait(void);


bool
Log_emitter_emit(void);


bool
Log_emitter_emit_log(uint8_t log_level, const char *format, ...);
