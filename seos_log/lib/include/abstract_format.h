#pragma once


#include "log_databuffer.h"
#include "log_symbol.h"
#include <stdbool.h>
#include <string.h>


typedef struct Format_t Format_t;


typedef void
(*FormatT_dtorT)(Format_t *self);


typedef bool
(*FormatT_convertT)(Format_t *self, Log_info_t *log_info);


typedef void
(*FormatT_printT)(Format_t *self);


typedef struct
{
    FormatT_dtorT    dtor;
    FormatT_convertT convert;
    FormatT_printT   print;
} FormatT_Vtable;


struct Format_t
{
    const FormatT_Vtable *vtable;
};


inline void
FormatT_dtor(Format_t *self)
{
    bool nullptr = false;

    ASSERT_SELF__(self);

    if(nullptr){
        // Debug_printf
        return;
    }

    memset(self, 0, sizeof (Format_t));
}


inline bool
FormatT_convert(Format_t *self, Log_info_t *log_info)
{
    bool nullptr = false;

    ASSERT_SELF__(self);

    if(nullptr){
        // Debug_printf
        return false;
    }

    if(log_info == NULL){
        // Debug_printf
        return false;
    }

    return self->vtable->convert(self, log_info);
}


inline void
FormatT_print(Format_t *self)
{
    bool nullptr = false;

    ASSERT_SELF__(self);

    if(nullptr){
        // Debug_printf
        return;
    }

    self->vtable->print(self);
}
