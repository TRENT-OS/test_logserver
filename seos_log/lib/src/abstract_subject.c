#include "abstract_subject.h"
#include "log_symbol.h"
#include <string.h>



// foreward declaration
static bool _Subject_attach(Subject_t *self, Observer_t *observer);
static bool _Subject_detach(Subject_t *self, Observer_t *observer);
static void _Subject_notify(Subject_t *self);



static const Subject_Vtable Subject_vtable =
{
    .dtor   = Subject_dtor,
    .attach = _Subject_attach,
    .detach = _Subject_detach,
    .notify = _Subject_notify
};



bool
Subject_ctor(Subject_t *self, void *data)
{
    bool nullptr = false;

    ASSERT_SELF__(self);

    if(nullptr){
        // Debug_printf
        return false;
    }

    self->vtable = &Subject_vtable;
    self->data = data;

    return true;
}



void
Subject_dtor(Subject_t *self)
{
    memset(self, 0, sizeof (Subject_t));
}



static bool
_Subject_attach(Subject_t *self, Observer_t *observer)
{
    bool nullptr = false;

    ASSERT_SELF__(self);

    if(nullptr){
        // Debug_printf
        return false;
    }

    if(observer == NULL){
        // Debug_printf
        return false;
    }

    return self->vtable->attach(self, observer);
}



static bool
_Subject_detach(Subject_t *self, Observer_t *observer)
{
    bool nullptr = false;

    ASSERT_SELF__(self);

    if(nullptr){
        // Debug_printf
        return false;
    }

    if(observer == NULL){
        // Debug_printf
        return false;
    }

    return self->vtable->detach(self, observer);
}



static void
_Subject_notify(Subject_t *self)
{
    bool nullptr = false;

    ASSERT_SELF__(self);

    if(nullptr){
        // Debug_printf
        return;
    }

    self->vtable->notify(self);
}
