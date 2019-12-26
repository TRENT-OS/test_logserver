#pragma once


#include "listT.h"
#include "log_filter.h"
#include "log_consumer_callback.h"
#include "log_subject.h"
#include "log_file.h"
#include "log_databuffer.h"


typedef struct Log_consumer_t Log_consumer_t;


typedef void
(*Log_consumer_dtorT)(Log_consumer_t *self);


typedef void
(*Log_consumer_processT)(void *data);


typedef void
(*Log_consumer_emitT)(Log_consumer_t *self);


typedef uint64_t
(*Log_consumer_get_timestampT)(Log_consumer_t *self);


typedef struct
{
    Log_consumer_dtorT          dtor;
    Log_consumer_processT       process;
    Log_consumer_emitT          emit;
    Log_consumer_get_timestampT get_timestamp;
} Log_consumer_Vtable;


struct Log_consumer_t
{
    NodeT_t                   node;
    void                      *buf;
    uint32_t                  id;
    Log_info_t                log_info;
    Log_filter_t              *log_filter;
    Log_subject_t             *log_subject;
    Log_file_t                *log_file;
    Log_consumer_callback_t   *callback_vtable;
    const Log_consumer_Vtable *vtable;
};


bool
Log_consumer_ctor(Log_consumer_t *self,
                  void *buffer,
                  Log_filter_t *log_filter,
                  Log_consumer_callback_t *callback_vtable,
                  Log_subject_t *log_subject,
                  Log_file_t *log_file,
                  uint32_t id,
                  const char *name);


void
Log_consumer_dtor(Log_consumer_t *self);
