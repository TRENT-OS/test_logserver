#pragma once


#include "listT.h"
#include "log_filter.h"
#include "log_consumer_callback.h"
#include "log_subject.h"


typedef struct Log_consumer_t Log_consumer_t;


typedef void
(*Log_consumer_dtorT)(Log_consumer_t *self);


typedef int
(*Log_consumer_callbackHandlerT)(Log_consumer_t *self, Log_consumer_callbackT callback);


typedef void
(*Log_consumer_emitT)(Log_consumer_t *self);


typedef uint64_t
(*Log_consumer_get_timestampT)(Log_consumer_t *self);


typedef struct
{
    Log_consumer_dtorT            dtor;
    Log_consumer_callbackT        callback;
    Log_consumer_callbackHandlerT callback_handler;
    Log_consumer_emitT            emit;
    Log_consumer_get_timestampT   get_timestamp;
} Log_consumer_Vtable;


struct Log_consumer_t
{
    NodeT_t                   node;
    void                      *buf;
    Log_filter_t              *log_filter;
    Log_subject_t             *log_subject;
    Log_consumer_callback_t   *callback_vtable;
    const Log_consumer_Vtable *vtable;
};


bool
Log_consumer_ctor(Log_consumer_t *self,
                  void *buffer,
                  Log_filter_t *log_filter,
                  Log_consumer_callback_t *callback_vtable,
                  Log_subject_t *log_subject,
                  const char *name);


void
Log_consumer_dtor(Log_consumer_t *self);
