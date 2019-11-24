#include "abstract_observer.h"
#include <string.h>



#define ASSERT_SELF__(self)             \
    if(self == NULL)                    \
        nullptr = true;



#define ASSERT_VTABLE__(self)           \
    if(self->vtable == NULL)            \
        nullptr = true;



#define ASSERT_SELF(self)               \
    ASSERT_SELF__(self)                 \
                                        \
    if(nullptr == false)                \
    {                                   \
        ASSERT_VTABLE__(self)           \
    }



// foreward declaration
static bool Observer_update(Observer_t *self);



static const Observer_Vtable Observer_vtable =
{
    .dtor   = Observer_dtor,
    .update = Observer_update
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

    memset(self, 0, sizeof (Observer_t));

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
Observer_update(Observer_t *self)
{
    bool nullptr = false;

    ASSERT_SELF(self);

    if(nullptr){
        // Debug_printf
        return false;
    }

    self->vtable->update(self);

    return true;
}
