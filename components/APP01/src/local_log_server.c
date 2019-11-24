#include "local_log_server.h"
#include <stddef.h>



static Log_consumer_t consumer;



static void Local_emitter_emit(void);
static int  Local_consumer_callbackHandler(Log_consumer_callbackT callback, void *data);



bool
Local_log_server_init(void *buffer,
                           uint8_t log_level,
                           const char *name)
{
    if(buffer == NULL)
        return false;

    get_instance_Log_emitter(buffer, NULL, Local_emitter_emit);
    Log_consumer_ctor(&consumer, buffer, log_level, Local_consumer_callbackHandler, NULL, NULL, name);

    return true;
}



void
Local_log_server_delete(void)
{
    Log_consumer_dtor(&consumer);
}



static void
Local_emitter_emit(void)
{
    Local_consumer_callbackHandler(Log_consumer_callback, &consumer);
}



static int
Local_consumer_callbackHandler(Log_consumer_callbackT callback, void *data)
{
    callback(data);

    return 1;
}
