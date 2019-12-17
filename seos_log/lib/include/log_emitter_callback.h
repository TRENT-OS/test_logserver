#pragma once


#include "log_filter.h"
#include <stdarg.h>
#include <stdbool.h>


typedef void
(*Log_emitter_callback_waitT)(void);


typedef void
(*Log_emitter_callback_emitT)(void);


typedef struct
{
    Log_emitter_callback_waitT server_wait;
    Log_emitter_callback_emitT client_emit;
} Log_emitter_callback_t;


bool
Log_emitter_callback_ctor(Log_emitter_callback_t *self,
                          Log_emitter_callback_waitT server_wait,
                          Log_emitter_callback_emitT client_emit);


void
Log_emitter_callback_dtor(Log_emitter_callback_t *self);
