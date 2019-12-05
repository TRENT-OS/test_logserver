#pragma once


#include "listT.h"
#include "log_databuffer.h"
#include "log_filter.h"
#include "log_consumer_callback.h"
#include "log_format.h"


typedef struct
{
    NodeT_t                 node;
    void                    *buf;
    Log_info_t              log_info;
    Log_filter_t            *log_filter;
    Log_format_t            *log_format;
    Log_consumer_callback_t *callback_vtable;
} Log_consumer_t;


bool
Log_consumer_ctor(Log_consumer_t *self,
                  void *buffer,
                  Log_filter_t *log_filter,
                  Log_consumer_callback_t *callback_vtable,
                  Log_format_t *log_format,
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
