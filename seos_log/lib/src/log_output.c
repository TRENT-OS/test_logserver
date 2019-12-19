#include "log_output.h"
#include <string.h>



// foreward declaration
static void _Log_observer_dtor(Observer_t *self);
static bool _Log_output_filesystem_update(Observer_t *self, void *data);
static bool _Log_output_console_update(Observer_t *self, void *data);
static bool _Log_output_filesystem_print(Output_t *self, void *data);
static bool _Log_output_console_print(Output_t *self, void *data);



static void
_Log_observer_dtor(Observer_t *self)
{
    memset(self, 0, sizeof (Observer_t));
}



static const Output_Vtable Log_output_filesystem_vtable =
{
    .parent.dtor   = _Log_observer_dtor,
    .parent.update = _Log_output_filesystem_update,
    .dtor          = Log_output_filesystem_dtor,
    .print         = _Log_output_filesystem_print
};



bool
Log_output_filesystem_ctor(Log_output_t *self,
                           Log_format_t *log_format,
                           uint8_t drv_id,
                           const char *filename)
{
    bool nullptr = false;
    bool retval = false;

    ASSERT_SELF__(self);

    if(nullptr){
        // Debug_printf
        return false;
    }

    if(log_format == NULL || filename == NULL){
        // Debug_printf
        return retval;
    }

    retval = ListT_ctor(&self->listT);

    self->node.prev = NULL;
    self->node.next = NULL;

    strcpy(self->fs.filename, filename);

    self->log_format = log_format;

    self->vtable = &Log_output_filesystem_vtable;
    return retval;
}



void
Log_output_filesystem_dtor(Output_t *self)
{
    bool nullptr = false;

    ASSERT_SELF__(self);

    if(nullptr){
        // Debug_printf
        return;
    }

    Log_output_t *log_output = (Log_output_t *)self;

    file_close(log_output->fs.fhandle);
    partition_fs_unmount(log_output->fs.phandle);
    partition_close(log_output->fs.phandle);

    memset(self, 0, sizeof (Log_output_t));
}



static bool
_Log_output_filesystem_update(Observer_t *self, void *data)
{
    bool nullptr = false;

    ASSERT_SELF__(self);

    if(nullptr){
        // Debug_printf
        return false;
    }

    if(data == NULL){
        // Debug_printf
        return false;
    }

    Log_output_t *log_output = (Log_output_t *)self;
    log_output->vtable->print((Output_t *)log_output, data);

    return true;
}




static bool
_Log_output_filesystem_print(Output_t *self, void *data)
{
    bool nullptr = false;

    ASSERT_SELF__(self);

    if(nullptr){
        // Debug_printf
        return false;
    }

    Log_output_t *log_output = (Log_output_t *)self;

    // log format layer
    log_output->log_format->vtable->convert((FormatT_t *)log_output->log_format, (Log_info_t *)data);
    log_output->log_format->vtable->print((FormatT_t *)log_output->log_format);

    return true;
}



static const Output_Vtable Log_output_console_vtable =
{
    .parent.dtor   = _Log_observer_dtor,
    .parent.update = _Log_output_console_update,
    .dtor          = Log_output_console_dtor,
    .print         = _Log_output_console_print
};



bool
Log_output_console_ctor(Log_output_t *self,
                        Log_format_t *log_format)
{
    bool nullptr = false;
    bool retval = false;

    ASSERT_SELF__(self);

    if(nullptr){
        // Debug_printf
        return false;
    }
    if(log_format == NULL){
        // Debug_printf
        return retval;
    }

    retval = ListT_ctor(&self->listT);

    self->node.prev = NULL;
    self->node.next = NULL;

    self->log_format = log_format;

    self->vtable = &Log_output_console_vtable;

    return retval;
}



void
Log_output_console_dtor(Output_t *self)
{
    memset(self, 0, sizeof (Log_output_t));
}



static bool
_Log_output_console_update(Observer_t *self, void *data)
{
    bool nullptr = false;

    ASSERT_SELF__(self);

    if(nullptr){
        // Debug_printf
        return false;
    }

    if(data == NULL){
        // Debug_printf
        return false;
    }

    Log_output_t *log_output = (Log_output_t *)self;
    log_output->vtable->print((Output_t *)log_output, data);

    return true;
}



static bool
_Log_output_console_print(Output_t *self, void *data)
{
    bool nullptr = false;

    ASSERT_SELF__(self);

    if(nullptr){
        // Debug_printf
        return false;
    }

    Log_output_t *log_output = (Log_output_t *)self;

    // log format layer
    log_output->log_format->vtable->convert((FormatT_t *)log_output->log_format, (Log_info_t *)data);
    log_output->log_format->vtable->print((FormatT_t *)log_output->log_format);

    return true;
}
