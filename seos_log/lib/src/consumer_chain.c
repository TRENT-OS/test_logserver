#include "consumer_chain.h"
#include <string.h>



static const Consumer_chain_Vtable Consumer_chain_vtable =
{
    .dtor   = Consumer_chain_dtor,
    .append = Consumer_chain_append,
    .remove = Consumer_chain_remove,
    .poll   = Consumer_chain_poll
};



// Singleton
static Consumer_chain_t _consumer_chain;
static Consumer_chain_t *this = NULL;



Consumer_chain_t *
get_instance_Consumer_chain(void)
{
    if(this == NULL){
        bool retval = false;

        this = &_consumer_chain;
        this->vtable = &Consumer_chain_vtable;

        retval = ListT_ctor(&this->listT);
        if(retval == false)
            return NULL;
    }

    return this;
}



void
Consumer_chain_dtor(void)
{
    memset(this, 0, sizeof (Consumer_chain_t));
    this = NULL;
}



bool
Consumer_chain_append(Log_consumer_t *consumer)
{
    bool nullptr = false;
    bool retval = false;

    ASSERT_SELF__(this);

    if(nullptr){
        // Debug_printf
        return retval;
    }

    if(consumer == NULL){
        // Debug_printf
        return retval;
    }

    if(this->node.first == NULL){
        this->node.first = consumer;
        return true;
    }

    retval = this->listT.vtable->insert(this->listT.vtable->get_last((NodeT_t *)&(((Log_consumer_t *)(this->node.first))->node)), &consumer->node);

    return retval;
}



bool
Consumer_chain_remove(Log_consumer_t *consumer)
{
    bool nullptr = false;
    bool retval = false;

    ASSERT_SELF__(this);

    if(nullptr){
        // Debug_printf
        return false;
    }

    if(consumer == NULL){
        // Debug_printf
        return false;
    }

    if(this->node.first == consumer){
        this->node.first = this->listT.vtable->get_next(&consumer->node);

        if(this->node.first == consumer)
            this->node.first = NULL;
    }

    retval = this->listT.vtable->delete(&consumer->node);

    return retval;
}



bool
Consumer_chain_poll(void)
{
    bool nullptr = false;
    Log_consumer_t *next;

    ASSERT_SELF__(this);

    if(nullptr){
        // Debug_printf
        return false;
    }

    if(this->node.first == NULL)
        return false;

    next = this->node.first;

    while (1) {
        if(next->vtable->callback_handler(next, next->vtable->callback) >= 0){
            next->vtable->emit(next);
        }

        while (this->listT.vtable->has_next(&next->node)) {
            next = this->listT.vtable->get_next(&next->node);

            if(next->vtable->callback_handler(next, next->vtable->callback) >= 0){
                next->vtable->emit(next);
            }
        }

        next = this->node.first;
    }

    return true;
}
