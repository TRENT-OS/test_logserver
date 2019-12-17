#pragma once


#include "log_databuffer.h"
#include "log_symbol.h"
#include <stdbool.h>
#include <string.h>


typedef struct FormatT_t FormatT_t;


typedef void
(*FormatT_dtorT)(FormatT_t *self);


typedef bool
(*FormatT_convertT)(FormatT_t *self, Log_info_t *log_info);


typedef void
(*FormatT_printT)(FormatT_t *self);


typedef struct
{
    FormatT_dtorT    dtor;
    FormatT_convertT convert;
    FormatT_printT   print;
} FormatT_Vtable;


struct FormatT_t
{
    const FormatT_Vtable *vtable;
};


inline void
FormatT_dtor(FormatT_t *self)
{
    memset(self, 0, sizeof (FormatT_t));
}


inline bool
FormatT_convert(FormatT_t *self, Log_info_t *log_info)
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
FormatT_print(FormatT_t *self)
{
    bool nullptr = false;

    ASSERT_SELF__(self);

    if(nullptr){
        // Debug_printf
        return;
    }

    self->vtable->print(self);
}
