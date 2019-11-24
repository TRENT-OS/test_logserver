#pragma once


#include "listT.h"
#include "log_databuffer.h"
#include "log_filter.h"


typedef void
(*Log_consumer_callbackT)(void *data);


typedef int
(*Log_consumer_callbackHandlerT)(Log_consumer_callbackT callback, void *data);


typedef void
(*Log_consumer_emitT)(void);


typedef uint64_t
(*Log_consumer_get_timestampT)(void);


typedef struct
{
    Log_consumer_emitT            emit;
    Log_consumer_callbackHandlerT reg_callback;
    Log_consumer_get_timestampT   get_timestamp;
} Log_consumer_Vtable;


typedef struct
{
    NodeT_t             node;
    void                *buf;
    uint8_t             log_level;
    Log_info_t          log_info;
    Log_filter_t        log_filter;
    Log_consumer_Vtable vtable;
} Log_consumer_t;


bool
Log_consumer_ctor(Log_consumer_t *self,
                  void *buffer,
                  uint8_t log_level,
                  Log_consumer_callbackHandlerT reg_callback,
                  Log_consumer_emitT emit,
                  Log_consumer_get_timestampT get_timestamp,
                  const char *name);


void
Log_consumer_dtor(Log_consumer_t *self);


void
Log_consumer_callback(void *data);


int
Log_consumer_callback_handler(Log_consumer_t *self, Log_consumer_callbackT callback);


void
Log_consumer_emit(Log_consumer_t *self);


uint64_t
Log_consumer_get_timestamp(Log_consumer_t *self);
