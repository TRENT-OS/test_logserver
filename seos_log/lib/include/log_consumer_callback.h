#pragma once


#include <stdint.h>
#include <stdbool.h>


typedef void
(*Log_consumer_callbackT)(void *data);


typedef int
(*Log_consumer_callback_callbackHandlerT)(Log_consumer_callbackT callback, void *data);


typedef void
(*Log_consumer_callback_emitT)(void);


typedef uint64_t
(*Log_consumer_callback_get_timestampT)(void);


typedef struct
{
    Log_consumer_callback_emitT            emit;
    Log_consumer_callback_callbackHandlerT reg_callback;
    Log_consumer_callback_get_timestampT   get_timestamp;
} Log_consumer_callback_t;


bool
Log_consumer_callback_ctor(Log_consumer_callback_t *self,
                           Log_consumer_callback_callbackHandlerT reg_callback,
                           Log_consumer_callback_emitT emit,
                           Log_consumer_callback_get_timestampT get_timestamp);


void
Log_consumer_callback_dtor(Log_consumer_callback_t *self);
