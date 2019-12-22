#include "log_emitter_callback.h"
#include "log_symbol.h"
#include <string.h>



bool
Log_emitter_callback_ctor(Log_emitter_callback_t *self,
                          Log_emitter_callback_waitT client_wait,
                          Log_emitter_callback_emitT client_emit)
{
    bool nullptr = false;

    ASSERT_SELF__(self);

    if(nullptr){
        // Debug_printf
        return false;
    }

    // "server_wait" can be NULL, if Log_emitter_t will be declared as "local" log server
    if(/*server_wait == NULL ||*/ client_emit == NULL){
        // Debug_printf
        return false;
    }

    self->client_emit = client_emit;
    self->client_wait = client_wait;

    return true;
}



void
Log_emitter_callback_dtor(Log_emitter_callback_t *self)
{
    memset(self, 0, sizeof (Log_emitter_callback_t));
}
