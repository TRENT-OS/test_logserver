#pragma once


#include "log_consumer.h"
#include "log_symbol.h"
#include <stdbool.h>


#if !defined (API_LOG_SERVER_EMIT)
    #define API_LOG_SERVER_EMIT     log_server_interface_emit
#endif


typedef struct Consumer_chain_t Consumer_chain_t;


typedef void
(*Consumer_chain_dtorT)(void);


typedef bool
(*Consumer_chain_appendT)(Log_consumer_t *consumer);


typedef bool
(*Consumer_chain_removeT)(Log_consumer_t *consumer);


typedef void
(*Consumer_chain_pollT)(void);


typedef struct
{
    Consumer_chain_dtorT   dtor;
    Consumer_chain_appendT append;
    Consumer_chain_removeT remove;
    Consumer_chain_pollT   poll;
}
Consumer_chain_Vtable;


typedef struct
{
    void *first;
} Consumer_chain_node_t;


struct Consumer_chain_t
{
    ListT_t                     listT;
    Consumer_chain_node_t       node;
    const Consumer_chain_Vtable *vtable;
};


Consumer_chain_t *
get_instance_Consumer_chain(void);


void
Consumer_chain_dtor(void);


bool
Consumer_chain_append(Log_consumer_t *consumer);


bool
Consumer_chain_remove(Log_consumer_t *consumer);


void
Consumer_chain_poll(void);


void
API_LOG_SERVER_EMIT(void);
