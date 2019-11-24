#include "log_filter.h"
#include "log_databuffer.h"
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
static bool _filtering(void *buf, uint8_t log_level_client);



static const Log_filter_Vtable Log_filter_vtable =
{
    .dtor      = Log_filter_dtor,
    .filtering = _filtering
};



static bool
_filtering(void *buf, uint8_t log_level_client)
{
    if(buf == NULL)
        return false;

    Log_databuffer_t log_databuffer;
    Log_databuffer_get_log_level_server(buf, &log_databuffer);

    if(log_level_client > log_databuffer.log_level_srv)
        return false;

    return true;
}



bool
Log_filter_ctor(Log_filter_t *self)
{
    bool nullptr = false;

    ASSERT_SELF__(self);

    if(nullptr){
        // Debug_printf
        return false;
    }

    memset(self, 0, sizeof (Log_filter_t));

    self->vtable = &Log_filter_vtable;

    return true;
}



void
Log_filter_dtor(Log_filter_t *self)
{
    memset(self, 0, sizeof (Log_filter_t));
}
