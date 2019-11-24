#pragma once


#include "log_filter.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>


typedef void
(*Log_emitter_waitT)(void);


typedef void
(*Log_emitter_emitT)(void);


typedef struct
{
    Log_emitter_waitT server_wait;
    Log_emitter_emitT client_emit;
} Log_emitter_Vtable;


typedef struct
{
    void               *buf;
    Log_filter_t       log_filter;
    Log_emitter_Vtable vtable;
} Log_emitter_t;


Log_emitter_t *
get_instance_Log_emitter(void *buffer,
                  Log_emitter_waitT server_wait,
                  Log_emitter_emitT client_emit);


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
