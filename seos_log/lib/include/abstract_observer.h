#pragma once


#include <stdbool.h>


typedef struct Observer_t Observer_t;


typedef void
(*Observer_dtorT)(Observer_t *self);


typedef bool
(*Observer_updateT)(Observer_t *self);


typedef struct
{
    Observer_dtorT   dtor;
    Observer_updateT update;
}
Observer_Vtable;


struct Observer_t
{
    void                  *data;
    const Observer_Vtable *vtable;
};


bool
Observer_ctor(Observer_t *self, void *data);


void
Observer_dtor(Observer_t *self);
