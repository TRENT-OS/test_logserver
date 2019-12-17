#pragma once


#include "log_filter.h"
#include "log_emitter_callback.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>


typedef struct Log_emitter_t Log_emitter_t;


typedef void
(*Log_emitter_dtorT)(void);


typedef void *
(*Log_emitter_get_bufferT)(void);


typedef bool
(*Log_emitter_waitT)(void);


typedef bool
(*Log_emitter_emitT)(void);


typedef bool
(*Log_emitter_emit_logT)(uint8_t log_level, const char *format, ...);


typedef struct
{
    Log_emitter_dtorT       dtor;
    Log_emitter_get_bufferT get_buffer;
    Log_emitter_waitT       wait;
    Log_emitter_emitT       emit;
    Log_emitter_emit_logT   emit_log;
} Log_emitter_Vtable;


struct Log_emitter_t
{
    void                     *buf;
    Log_filter_t             *log_filter;
    Log_emitter_callback_t   *callback_vtable;
    const Log_emitter_Vtable *vtable;
};


Log_emitter_t *
get_instance_Log_emitter(void *buffer,
                         Log_filter_t *log_filter,
                         Log_emitter_callback_t *callback_vtable);


void
Log_emitter_dtor(void);


bool
Log_emitter_emit_log(uint8_t log_level, const char *format, ...);
