#pragma once


#include "abstract_observer.h"
#include <stdbool.h>


typedef struct Subject_t Subject_t;


typedef void
(*Subject_dtorT)(Subject_t *self);


typedef bool
(*Subject_attachT)(Subject_t *self, Observer_t *observer);


typedef bool
(*Subject_detachT)(Subject_t *self, Observer_t *observer);


typedef void
(*Subject_notifyT)(Subject_t *self);


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
    void                 *data;
    const Subject_Vtable *vtable;
};


bool
Subject_ctor(Subject_t *self, void *data);


void
Subject_dtor(Subject_t *self);
