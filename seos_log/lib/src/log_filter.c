#include "log_filter.h"
#include "log_symbol.h"
#include <string.h>



// foreward declaration
static bool _filtering(uint8_t log_level_self, uint8_t log_level);



static const Log_filter_Vtable Log_filter_vtable =
{
    .dtor      = Log_filter_dtor,
    .filtering = _filtering
};



static bool
_filtering(uint8_t log_level_self, uint8_t log_level)
{
    if(log_level_self < log_level)
        return false;

    return true;
}



bool
Log_filter_ctor(Log_filter_t *self, uint8_t log_level)
{
    bool nullptr = false;

    ASSERT_SELF__(self);

    if(nullptr){
        // Debug_printf
        return false;
    }

    self->log_level = log_level;
    self->vtable = &Log_filter_vtable;

    return true;
}



void
Log_filter_dtor(Log_filter_t *self)
{
    memset(self, 0, sizeof (Log_filter_t));
}



bool
Log_filter_filtering(Log_filter_t *self, uint8_t log_level)
{
    bool nullptr = false;

    ASSERT_SELF__(self);

    // if self == NULL, return value is true...
    // ...because no log filter is installed
    if(nullptr){
        // Debug_printf
        return true;
    }

    return self->vtable->filtering(self->log_level, log_level);
}
