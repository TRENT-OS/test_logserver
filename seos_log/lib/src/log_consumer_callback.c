#include "log_consumer_callback.h"
#include "log_symbol.h"
#include <string.h>



#if !defined(EOF)
    #define EOF     -1
#endif



bool
Log_consumer_callback_ctor(Log_consumer_callback_t *self,
                           Log_consumer_callback_emitT server_emit,
                           Log_consumer_callback_get_sender_id get_sender_id,
                           Log_consumer_callback_get_timestampT get_timestamp)
{
    bool nullptr = false;

    ASSERT_SELF__(self);

    if(nullptr){
        // Debug_printf
        return false;
    }

    // "server_emit" can be NULL, if Log_consumer_t will be declared as "local" log server
    // "get_timestamp" can be NULL, if timestamp is not necessary
    if(get_sender_id == NULL /*|| emit == NULL || get_timestamp == NULL*/){
        // Debug_printf
        return false;
    }

    self->get_sender_id = get_sender_id;
    self->server_emit = server_emit;
    self->get_timestamp = get_timestamp;

    return true;
}



void
Log_consumer_callback_dtor(Log_consumer_callback_t *self)
{
    bool nullptr = false;

    ASSERT_SELF__(self);

    if(nullptr){
        // Debug_printf
        return;
    }

    memset(self, 0, sizeof (Log_consumer_callback_t));
}
