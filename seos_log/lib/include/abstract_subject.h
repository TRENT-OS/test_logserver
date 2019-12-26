#pragma once



#include <stdbool.h>
#include <string.h>
#include "abstract_observer.h"
#include "log_symbol.h"


typedef struct Subject_t Subject_t;


typedef void
(*Subject_dtorT)(Subject_t *self);


typedef bool
(*Subject_attachT)(Subject_t *self, Observer_t *observer);


typedef bool
(*Subject_detachT)(Subject_t *self, Observer_t *observer);


typedef void
(*Subject_notifyT)(Subject_t *self, void *data);


typedef struct
{
    Subject_dtorT   dtor;
    Subject_attachT attach;
    Subject_detachT detach;
    Subject_notifyT notify;
}
Subject_Vtable;


struct Subject_t
{
    const Subject_Vtable *vtable;
};


inline void
Subject_dtor(Subject_t *self)
{
    bool nullptr = false;

    ASSERT_SELF__(self);

    if(nullptr){
        // Debug_printf
        return;
    }

    memset(self, 0, sizeof (Subject_t));
}


inline bool
Subject_attach(Subject_t *self, Observer_t *observer)
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


inline bool
Subject_detach(Subject_t *self, Observer_t *observer)
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


inline void
Subject_notify(Subject_t *self, void *data)
{
    bool nullptr = false;

    ASSERT_SELF__(self);

    if(nullptr){
        // Debug_printf
        return;
    }

    self->vtable->notify(self, data);
}
