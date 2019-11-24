#include "log_observer_listT.h"
#include <stddef.h>



#define ASSERT_SELF__(self)             \
    if(self == NULL)                    \
        nullptr = true;



#define ASSERT_VTABLE__(self)           \
    if(self->listT.vtable == NULL)      \
        nullptr = true;



#define ASSERT_SELF(self)               \
    ASSERT_SELF__(self)                 \
                                        \
    if(nullptr == false)                \
    {                                   \
        ASSERT_VTABLE__(self)           \
    }



bool
Node_has_prev(Log_observer_t *self)
{
    bool nullptr = false;

    ASSERT_SELF(self);

    if(nullptr){
        // Debug_printf
        return false;
    }

    return self->listT.vtable->has_prev(&self->node);
}



bool
Node_has_next(Log_observer_t *self)
{
    bool nullptr = false;

    ASSERT_SELF(self);

    if(nullptr){
        // Debug_printf
        return false;
    }

    return self->listT.vtable->has_next(&self->node);
}



Log_observer_t *
Node_get_prev(Log_observer_t *self)
{
    bool nullptr = false;

    ASSERT_SELF(self);

    if(nullptr){
        // Debug_printf
        return NULL;
    }

    return (Log_observer_t *)self->listT.vtable->get_prev(&self->node);
}



Log_observer_t *
Node_get_next(Log_observer_t *self)
{
    bool nullptr = false;

    ASSERT_SELF(self);

    if(nullptr){
        // Debug_printf
        return NULL;
    }

    return (Log_observer_t *)self->listT.vtable->get_next(&self->node);
}



bool
Node_insert(Log_observer_t *self, Log_observer_t *new_node)
{
    bool nullptr = false;

    ASSERT_SELF(self);

    if(nullptr){
        // Debug_printf
        return false;
    }

    NodeT_t *node = NULL;

    if(new_node != NULL)
        node = &new_node->node;

    return self->listT.vtable->insert(&self->node, node);
}



bool
Node_delete(Log_observer_t *self)
{
    bool nullptr = false;

    ASSERT_SELF(self);

    if(nullptr){
        // Debug_printf
        return false;
    }

    bool retval = false;

    retval = self->listT.vtable->delete(&self->node);

    self->node.next = NULL;
    self->node.prev = NULL;

    return retval;
}



Log_observer_t *
Node_get_first(Log_observer_t *self)
{
    bool nullptr = false;

    ASSERT_SELF(self);

    if(nullptr){
        // Debug_printf
        return NULL;
    }

    return (Log_observer_t *)self->listT.vtable->get_first(&self->node);
}



Log_observer_t *
Node_get_last(Log_observer_t *self)
{
    bool nullptr = false;

    ASSERT_SELF(self);

    if(nullptr){
        // Debug_printf
        return NULL;
    }

    return (Log_observer_t *)self->listT.vtable->get_last(&self->node);
}
