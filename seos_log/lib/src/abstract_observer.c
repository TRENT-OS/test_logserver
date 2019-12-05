#include "abstract_observer.h"
#include "log_symbol.h"
#include <string.h>



// foreward declaration
static bool _Observer_update(Observer_t *self);



static const Observer_Vtable Observer_vtable =
{
    .dtor   = Observer_dtor,
    .update = _Observer_update
};



bool
Observer_ctor(Observer_t *self, void *data)
{
    bool nullptr = false;

    ASSERT_SELF__(self);

    if(nullptr){
        // Debug_printf
        return false;
    }

    self->data = data;
    self->vtable = &Observer_vtable;

    return true;
}



void
Observer_dtor(Observer_t *self)
{
    memset(self, 0, sizeof (Observer_t));
}



static bool
_Observer_update(Observer_t *self)
{
    bool nullptr = false;

    ASSERT_SELF__(self);

    if(nullptr){
        // Debug_printf
        return false;
    }

    self->vtable->update(self);

    return true;
}
