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

        retval = ListT_ctor(&this->parent);
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

    ASSERT_SELF(this);

    if(nullptr){
        // Debug_printf
        return retval;
    }

    if(consumer == NULL){
        // Debug_printf
        return retval;
    }

    if(this->first == NULL){
        this->first = consumer;
        return true;
    }

    retval = this->parent.vtable->insert(this->parent.vtable->get_last(&this->first->node), &consumer->node);

    return retval;
}



bool
Consumer_chain_remove(Log_consumer_t *consumer)
{
    bool nullptr = false;
    bool retval = false;

    ASSERT_SELF(this);

    if(nullptr){
        // Debug_printf
        return false;
    }

    if(consumer == NULL){
        // Debug_printf
        return false;
    }

    if(this->first == consumer){
        this->first = this->parent.vtable->get_next(&consumer->node);

        if(this->first == consumer)
            this->first = NULL;
    }

    retval = this->parent.vtable->delete(&consumer->node);

    return retval;
}



bool
Consumer_chain_poll(void)
{
    bool nullptr = false;
    Log_consumer_t *next;

    ASSERT_SELF(this);

    if(nullptr){
        // Debug_printf
        return false;
    }

    if(this->first == NULL)
        return false;

    next = this->first;

    while (1) {
        if(Log_consumer_callback_handler(next, Log_consumer_callback) >= 0){
            Log_consumer_emit(next);
        }

        while (this->parent.vtable->has_next(&next->node)) {
            next = this->parent.vtable->get_next(&next->node);

            if(Log_consumer_callback_handler(next, Log_consumer_callback) >= 0){
                Log_consumer_emit(next);
            }
        }

        next = this->first;
    }

    return true;
}
