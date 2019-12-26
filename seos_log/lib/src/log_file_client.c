#include "log_file_client.h"
#include <string.h>



static Log_file_client_Vtable Log_file_client_vtable =
{
    .dtor          = Log_file_client_dtor,
    .read_log_file = Log_file_client_read_log_file
};



bool
Log_file_client_ctor(Log_file_client_t *self,
                     void *src_buf,
                     void *dest_buf,
                     Log_file_client_callback_t *log_file_client_callback)
{
    bool nullptr = false;

    ASSERT_SELF__(self);

    if(nullptr){
        // Debug_printf
        return false;
    }

    if(src_buf == NULL || dest_buf == NULL || log_file_client_callback == NULL){
        // Debug_printf
        return false;
    }

    self->src_buf = src_buf;
    self->dest_buf = dest_buf;
    self->vtable = &Log_file_client_vtable;
    self->callback_vtable = log_file_client_callback;

    return true;
}



void
Log_file_client_dtor(Log_file_client_t *self)
{
    bool nullptr = false;

    ASSERT_SELF__(self);

    if(nullptr){
        // Debug_printf
        return;
    }

    memset(self, 0, sizeof (Log_file_client_t));
}



bool
Log_file_client_read_log_file(Log_file_client_t *self,
                              uint64_t offset,
                              uint64_t len)
{
    bool nullptr = false;

    ASSERT_SELF__(self);

    if(nullptr){
        // Debug_printf
        return -1;
    }

    if(self->callback_vtable->read_log_file == NULL)
        return false;

    int64_t log_file_size;
    int64_t read_bytes;

    do{
        read_bytes = self->callback_vtable->read_log_file(offset, len, &log_file_size);
        if(read_bytes <= 0)
            break;

        memcpy((char *)self->dest_buf + offset, self->src_buf, (size_t)read_bytes);

        offset += (uint64_t)read_bytes;

        if((int64_t)offset >= log_file_size){
            break;
        }
    } while(1);

    return true;
}
