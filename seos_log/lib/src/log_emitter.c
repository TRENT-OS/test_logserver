#include "log_emitter.h"
#include "log_databuffer.h"
#include "symbol.h"
#include <string.h>
#include <stdio.h>



#define ASSERT_SELF(self)               \
    if(self == NULL)                    \
        nullptr = true;



// Singleton
static Log_emitter_t _log_emitter;
static Log_emitter_t *this = NULL;



Log_emitter_t *
get_instance_Log_emitter(
        void *buffer,
        Log_emitter_waitT server_wait,
        Log_emitter_emitT client_emit)
{
    if(this == NULL){
        if(sizeof (buffer) > DATABUFFER_SIZE){
            // Debug_printf
            return false;
        }

        memset(&_log_emitter, 0, sizeof (Log_emitter_t));
        this = &_log_emitter;

        this->buf = buffer;

        this->vtable.server_wait = server_wait;
        this->vtable.client_emit = client_emit;

        Log_filter_ctor(&this->log_filter);
    }

    return this;
}



void
Log_emitter_dtor(void)
{
    memset(this, 0, sizeof (Log_emitter_t));
    this = NULL;
}



void *
Log_emitter_get_buffer(void)
{
    bool nullptr = false;

    ASSERT_SELF(this);

    if(nullptr){
        // Debug_printf
        return NULL;
    }

    return this->buf;
}


bool
Log_emitter_wait(void)
{
    bool nullptr = false;

    ASSERT_SELF(this);

    if(nullptr){
        // Debug_printf
        return false;
    }

    if(this->vtable.server_wait != NULL)
        this->vtable.server_wait();

    return true;
}



bool
Log_emitter_emit(void)
{
    bool nullptr = false;

    ASSERT_SELF(this);

    if(nullptr){
        // Debug_printf
        return false;
    }

    if(this->vtable.client_emit != NULL)
        this->vtable.client_emit();

    return true;
}



bool
Log_emitter_emit_log(uint8_t log_level, const char *format, ...)
{
    bool nullptr = false;
    int retval = false;
    char buf[LOG_MESSAGE_LENGTH];

    ASSERT_SELF(this);

    if(nullptr){
        // Debug_printf
        return false;
    }

    if(format == NULL){
        // Debug_printf
        return false;
    }

    if(this->log_filter.vtable->filtering(Log_emitter_get_buffer(), log_level) == false){
        // Debug_printf -> Log filter!!!
        return false;
    }

    Log_emitter_wait();

    va_list args;
    va_start (args, format);

    if(strlen(format) > LOG_MESSAGE_LENGTH){
        // Debug_printf
        return false;
    }

    retval = vsnprintf(buf, LOG_MESSAGE_LENGTH, format, args);
    if(retval < 0 || retval > LOG_MESSAGE_LENGTH)
        return false;

    Log_databuffer_set_log_level_client(Log_emitter_get_buffer(), log_level);
    Log_databuffer_set_log_message(Log_emitter_get_buffer(), buf);

    va_end (args);

    Log_emitter_emit();

    return true;
}
