#include "log_consumer.h"
#include <string.h>
#include <stdio.h>



#if !defined(EOF)
    #define EOF     -1
#endif



#define ASSERT_SELF__(self)             \
    if(self == NULL)                    \
        nullptr = true;



#define ASSERT_VTABLE__(self)           \
    if(self->parent.vtable == NULL)     \
        nullptr = true;



#define ASSERT_SELF(self)               \
    ASSERT_SELF__(self)                 \
                                        \
    if(nullptr == false)                \
    {                                   \
        ASSERT_VTABLE__(self)           \
    }



// foreward declaration
static bool    _Log_consumer_init(Log_consumer_t *self);
static uint8_t _create_id(Log_consumer_t *self);
static bool    _create_id_string(Log_consumer_t *self, const char *name);



static bool
_Log_consumer_init(Log_consumer_t *self)
{
    if(self == NULL)
        return false;

    Log_databuffer_clear_databuffer(self->buf);
    Log_databuffer_set_log_level_server(self->buf, self->log_level);
    Log_filter_ctor(&self->log_filter);

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

    sprintf(self->log_info.log_id_name, "%03u", _create_id(self));

    if(name != NULL)
        sprintf(self->log_info.log_id_name + 3, " %s", name);

    self->log_info.log_id_name[strlen(self->log_info.log_id_name)] = '\0';

    return true;
}



bool
Log_consumer_ctor(Log_consumer_t *self,
                  void *buffer,
                  uint8_t log_level,
                  Log_consumer_callbackHandlerT reg_callback,
                  Log_consumer_emitT emit,
                  Log_consumer_get_timestampT get_timestamp,
                  const char *name)
{
    bool nullptr = false;
    bool retval = false;

    ASSERT_SELF__(self);

    if(nullptr){
        // Debug_printf
        return false;
    }

    // "emit" can be NULL, if Log_consumer_t will be declared as "local"
    // "get_timestamp" can be NULL, if timestamp is not necessary
    if(buffer == NULL || /*emit == NULL || get_timestamp == NULL ||*/ reg_callback == NULL){
        // Debug_printf
        return retval;
    }

    if(sizeof (buffer) > DATABUFFER_SIZE){
        // Debug_printf
        return retval;
    }

    memset(self, 0, sizeof (Log_consumer_t));

    self->buf = buffer;
    self->log_level = log_level;
    self->vtable.reg_callback = reg_callback;
    self->vtable.emit = emit;
    self->vtable.get_timestamp = get_timestamp;

    retval = _create_id_string(self, name);
    retval = _Log_consumer_init(self);

    return retval;
}



void
Log_consumer_dtor(Log_consumer_t *self)
{
    memset(self, 0, sizeof (Log_consumer_t));
}



int
Log_consumer_callback_handler(Log_consumer_t *self, Log_consumer_callbackT callback)
{
    bool nullptr = false;

    ASSERT_SELF__(self);

    if(nullptr){
        // Debug_printf
        return false;
    }

    if(callback == NULL){
        // Debug_printf
        return false;
    }

    if(self->vtable.reg_callback != NULL)
        return self->vtable.reg_callback(callback, self);

    return EOF;
}



void
Log_consumer_emit(Log_consumer_t *self)
{
    bool nullptr = false;

    ASSERT_SELF__(self);

    if(nullptr){
        // Debug_printf
        return;
    }

    if(self->vtable.emit != NULL)
        self->vtable.emit();
}



uint64_t
Log_consumer_get_timestamp(Log_consumer_t *self)
{
    bool nullptr = false;

    ASSERT_SELF__(self);

    if(nullptr){
        // Debug_printf
        return 0;
    }

    if(self->vtable.get_timestamp == NULL)
        return 0;

    return self->vtable.get_timestamp();
}


/**/
#include "log_format.h"
/**/
void
Log_consumer_callback(void *data)
{
    if(data == NULL){
        // Debug_printf
        return;
    }

    Log_consumer_t *consumer = (Log_consumer_t *)data;
    Log_databuffer_t log_databuffer;

    if(consumer->log_filter.vtable->filtering(consumer->buf, consumer->log_level) == false){
        // Debug_printf -> Log filter!!!
        Log_databuffer_clear_databuffer(consumer->buf);
        return;
    }

    Log_databuffer_get_log_level_server(consumer->buf, &log_databuffer);
    Log_databuffer_get_log_level_client(consumer->buf, &log_databuffer);
    Log_databuffer_get_log_message(consumer->buf, &log_databuffer);

    memcpy(&consumer->log_info.log_databuffer, &log_databuffer, sizeof (Log_databuffer_t));

    Log_databuffer_clear_databuffer(consumer->buf);

    consumer->log_info.timestamp.timestamp = Log_consumer_get_timestamp(consumer);

    char buf[FORMAT_BUFFER_SIZE];
    push_log_format(&buf, &consumer->log_info);
    print_log_format(buf);
}
