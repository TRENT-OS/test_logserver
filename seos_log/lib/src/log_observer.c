#include "log_observer.h"
#include "log_symbol.h"
#include <string.h>



// foreward declaration
static bool _update(Observer_t *self);



static const Observer_Vtable Log_observer_vtable =
{
    .dtor   = Observer_dtor,
    .update = _update
};



/********************/
/* Static functions */
/********************/
static bool
_update(Observer_t *self)
{
    bool nullptr = false;

    ASSERT_SELF_PARENT(self);

    if(nullptr){
        // Debug_printf
        return false;
    }

    Log_observer_t *log_obs = (Log_observer_t *)self->data;

    if(log_obs == NULL){
        // Debug_printf
        return false;
    }

    // ...

    return true;
}



bool
Log_observer_ctor(Log_observer_t *self)
{
    bool nullptr = false;
    bool retval = false;

    ASSERT_SELF__(self);

    if(nullptr){
        // Debug_printf
        return false;
    }

    self->node.prev = NULL;
    self->node.next = NULL;

    retval = ListT_ctor(&self->listT);

    retval = Observer_ctor(&self->parent, self);

    self->parent.vtable = &Log_observer_vtable;

    return retval;
}



void
Log_observer_dtor(Log_observer_t *self)
{
    memset(self, 0, sizeof (Log_observer_t));
}



bool
Log_observer_update(Log_observer_t *self)
{
    bool nullptr = false;

    ASSERT_SELF(self);

    if(nullptr){
        // Debug_printf
        return false;
    }

    self->parent.vtable->update(&self->parent);

    return true;
}
