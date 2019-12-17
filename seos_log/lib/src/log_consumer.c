#include "log_consumer.h"
#include <string.h>
#include <stdio.h>



#if !defined(EOF)
    #define EOF     -1
#endif



// foreward declaration
static void     _Log_consumer_callback(void *data);
static int      _Log_consumer_callback_handler(Log_consumer_t *self, Log_consumer_callbackT callback);
static void     _Log_consumer_emit(Log_consumer_t *self);
static uint64_t _Log_consumer_get_timestamp(Log_consumer_t *self);
static bool    _Log_consumer_init(void *buffer, Log_filter_t *log_filter);
static uint8_t _create_id(Log_consumer_t *self);
static bool    _create_id_string(Log_consumer_t *self, const char *name);



static const Log_consumer_Vtable Log_consumer_vtable = {
    .dtor             = Log_consumer_dtor,
    .callback         = _Log_consumer_callback,
    .callback_handler = _Log_consumer_callback_handler,
    .emit             = _Log_consumer_emit,
    .get_timestamp    = _Log_consumer_get_timestamp,
};



static bool
_Log_consumer_init(void *buffer, Log_filter_t *log_filter)
{
    if(buffer == NULL)
        return false;

    Log_databuffer_clear_databuffer(buffer);

    if(log_filter != NULL){
        // Debug_printf -> no log filter installed
        Log_databuffer_set_log_level_server(buffer, log_filter->log_level);
    }

    return true;
}



static uint8_t
_create_id(Log_consumer_t *self)
{
    if(self == NULL)
        return 0;

    static uint8_t i = 0;

    if( OVERFLOW( i, 1, UINT8_MAX ) ){
        // Debug_printf
        return 0;
    }

    i++;

    return i;
}



static bool
_create_id_string(Log_consumer_t *self, const char *name)
{
    if(self == NULL)
        return false;

    sprintf(self->log_subject->log_info.log_id_name, "%03u", _create_id(self));

    if(name != NULL)
        sprintf(self->log_subject->log_info.log_id_name + 3, " %s", name);

    self->log_subject->log_info.log_id_name[strlen(self->log_subject->log_info.log_id_name)] = '\0';

    return true;
}



bool
Log_consumer_ctor(Log_consumer_t *self,
                  void *buffer,
                  Log_filter_t *log_filter,
                  Log_consumer_callback_t *callback_vtable,
                  Log_subject_t *log_subject,
                  const char *name)
{
    bool nullptr = false;
    bool retval = false;

    ASSERT_SELF__(self);

    if(nullptr){
        // Debug_printf
        return false;
    }

    // "log_filter" can be NULL, if no log filter is installed
    if(buffer == NULL || callback_vtable == NULL || log_subject == NULL/*|| log_filter == NULL*/){
        // Debug_printf
        return retval;
    }

    if(sizeof (buffer) > DATABUFFER_SIZE){
        // Debug_printf
        return retval;
    }

    self->buf = buffer;
    self->log_filter = log_filter;
    self->log_subject = log_subject;

    self->vtable = &Log_consumer_vtable;
    self->callback_vtable = callback_vtable;

    retval = _create_id_string(self, name);
    if(retval == false){
         // Debug_printf
         return false;
     }

    retval = _Log_consumer_init(self->buf, self->log_filter);

    return retval;
}



void
Log_consumer_dtor(Log_consumer_t *self)
{
    memset(self, 0, sizeof (Log_consumer_t));
}



static int
_Log_consumer_callback_handler(Log_consumer_t *self, Log_consumer_callbackT callback)
{
    bool nullptr = false;

    ASSERT_SELF__(self);

    if(nullptr){
        // Debug_printf
        return -1;
    }

    if(callback == NULL){
        // Debug_printf
        return -2;
    }

    if(self->callback_vtable->reg_callback != NULL)
        return self->callback_vtable->reg_callback(callback, self);

    return -3;
}



static void
_Log_consumer_emit(Log_consumer_t *self)
{
    bool nullptr = false;

    ASSERT_SELF__(self);

    if(nullptr){
        // Debug_printf
        return;
    }

    if(self->callback_vtable->emit != NULL)
        self->callback_vtable->emit();
}



static uint64_t
_Log_consumer_get_timestamp(Log_consumer_t *self)
{
    bool nullptr = false;

    ASSERT_SELF__(self);

    if(nullptr){
        // Debug_printf
        return 0;
    }

    if(self->callback_vtable->get_timestamp != NULL)
        return self->callback_vtable->get_timestamp();

    return 0;
}



static void
_Log_consumer_callback(void *data)
{
    if(data == NULL){
        // Debug_printf
        return;
    }

    Log_consumer_t *consumer = (Log_consumer_t *)data;

    // get log level client
    Log_databuffer_get_log_level_client(consumer->buf, &consumer->log_subject->log_info.log_databuffer);

    if(consumer->log_filter->vtable->filtering(consumer->log_filter, consumer->log_subject->log_info.log_databuffer.log_level_client) == false){
        // Debug_printf -> Log filter!!!
        Log_databuffer_clear_databuffer(consumer->buf);
        return;
    }

    Log_databuffer_get_info(consumer->buf, &consumer->log_subject->log_info.log_databuffer);

    Log_databuffer_clear_databuffer(consumer->buf);

    consumer->log_subject->log_info.timestamp.timestamp = consumer->vtable->get_timestamp(consumer);

    // log subject
    consumer->log_subject->vtable->notify((Subject_t *)consumer->log_subject);
}
