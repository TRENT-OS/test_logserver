#pragma once


#include <stdbool.h>
#include <string.h>
#include "abstract_observer.h"
#include "log_symbol.h"


typedef struct Output_t Output_t;


typedef void
(*Output_dtorT)(Output_t *self);


typedef bool
(*Output_printT)(Output_t *self, void *data);


typedef struct
{
    Observer_Vtable parent;
    Output_dtorT    dtor;
    Output_printT   print;
}
Output_Vtable;


struct Output_t
{
    const Output_Vtable *vtable;
};


inline void
Output_dtor(Output_t *self)
{
    memset(self, 0, sizeof (Output_t));
}


inline bool
Output_print(Output_t *self, void *data)
{
    bool nullptr = false;

    ASSERT_SELF__(self);

    if(nullptr){
        // Debug_printf
        return false;
    }

    return self->vtable->print(self, data);
}
