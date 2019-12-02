#include "local_log_server.h"
#include <stddef.h>



static Log_consumer_t _consumer;
static Log_consumer_callback_t _consumer_vtable;
static Log_emitter_callback_t _emitter_vtable;



static void Local_emitter_emit(void);
int  Local_consumer_callbackHandler(Log_consumer_callbackT callback, void *data);



bool
Local_log_server_init(void *buffer,
                      Log_filter_t *log_filter_server,
                      Log_filter_t *log_filter_client,
                      const char *name)
{
    if(buffer == NULL || log_filter_server == NULL || log_filter_client == NULL)
        return false;

    Log_consumer_callback_ctor(&_consumer_vtable, Local_consumer_callbackHandler, NULL, NULL);
    Log_emitter_callback_ctor(&_emitter_vtable, NULL, Local_emitter_emit);

    Log_consumer_ctor(&_consumer, buffer, log_filter_server, &_consumer_vtable, name);
    get_instance_Log_emitter(buffer, log_filter_client, &_emitter_vtable);

    return true;
}



void
Local_log_server_delete(void)
{
    Log_consumer_dtor(&_consumer);
    Log_consumer_callback_dtor(&_consumer_vtable);
    Log_emitter_callback_dtor(&_emitter_vtable);
}



static void
Local_emitter_emit(void)
{
    Log_consumer_callback_handler(&_consumer, Log_consumer_callback);
}



int
Local_consumer_callbackHandler(Log_consumer_callbackT callback, void *data)
{
    callback(data);

    return 1;
}
