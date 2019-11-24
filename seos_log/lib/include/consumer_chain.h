#pragma once


#include "log_consumer.h"
#include <stdbool.h>


typedef struct Consumer_chain_t Consumer_chain_t;


typedef void
(*Consumer_chain_dtorT)(void);


typedef bool
(*Consumer_chain_appendT)(Log_consumer_t *consumer);


typedef bool
(*Consumer_chain_removeT)(Log_consumer_t *consumer);


typedef bool
(*Consumer_chain_pollT)(void);


typedef struct
{
    Consumer_chain_dtorT   dtor;
    Consumer_chain_appendT append;
    Consumer_chain_removeT remove;
    Consumer_chain_pollT   poll;
}
Consumer_chain_Vtable;


struct Consumer_chain_t
{
    ListT_t                     parent;
    Log_consumer_t              *first;
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


bool
Consumer_chain_poll(void);
