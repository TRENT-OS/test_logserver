#include "abstract_subject.h"
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
static bool Subject_attach(Subject_t *self, Observer_t *observer);
static bool Subject_detach(Subject_t *self, Observer_t *observer);
static void Subject_notify(Subject_t *self);



static const Subject_Vtable Subject_vtable =
{
    .dtor   = Subject_dtor,
    .attach = Subject_attach,
    .detach = Subject_detach,
    .notify = Subject_notify
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

    memset(self, 0, sizeof (Subject_t));

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
Subject_attach(Subject_t *self, Observer_t *observer)
{
    bool nullptr = false;

    ASSERT_SELF(self);

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
Subject_detach(Subject_t *self, Observer_t *observer)
{
    bool nullptr = false;

    ASSERT_SELF(self);

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
Subject_notify(Subject_t *self)
{
    bool nullptr = false;

    ASSERT_SELF(self);

    if(nullptr){
        // Debug_printf
        return;
    }

    self->vtable->notify(self);
}
