#include "log_output.h"
#include <string.h>



// foreward declaration
static void _Log_observer_dtor(Observer_t *self);
static bool _Log_output_filesystem_update(Observer_t *self, void *data);
static bool _Log_output_filesystem_print(Output_t *self, void *data);
static bool _Log_output_console_update(Observer_t *self, void *data);
static bool _Log_output_console_print(Output_t *self, void *data);
static bool _prepare_filesystem(uint8_t drv_id,
                                const char *filename,
                                Log_output_filesystem_t *fs);



static const Output_Vtable Log_output_filesystem_vtable =
{
    .parent.dtor   = _Log_observer_dtor,
    .parent.update = _Log_output_filesystem_update,
    .dtor          = Log_output_filesystem_dtor,
    .print         = _Log_output_filesystem_print
};



static void
_Log_observer_dtor(Observer_t *self)
{
    memset(self, 0, sizeof (Observer_t));
}



static bool
_prepare_filesystem(uint8_t drv_id,
                    const char *filename,
                    Log_output_filesystem_t *fs)
{
    if(filename == NULL || fs == NULL){
        // Debug_printf
        return false;
    }

    fs->phandle = partition_open(drv_id);
#if defined (SEOS_FS_BUILD_AS_LIB)
    if(fs->phandle == NULL){
#else
    if(fs->phandle < 0){
#endif
        return false;
    }

    if(partition_fs_mount(fs->phandle) != SEOS_FS_SUCCESS)
        return false;

    // create empty file
    hFile_t fhandle;
    fhandle = file_open(fs->phandle, filename, FA_CREATE_ALWAYS);
#if defined (SEOS_FS_BUILD_AS_LIB)
    if(fhandle == NULL){
#else
    if(fhandle < 0){
#endif
        printf("Fail to open file: %s!\n", filename);
        return false;
    }

    if(file_close(fhandle) != SEOS_FS_SUCCESS)
    {
        printf("Fail to close file: %s!\n", filename);
        return false;
    }

    strcpy(fs->filename, filename);
    fs->drv_id = drv_id;
    fs->file_offset = 0;

    return true;
}



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

    self->log_format = log_format;

    self->vtable = &Log_output_filesystem_vtable;

    retval = _prepare_filesystem(drv_id, filename, &self->fs);

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

    if(partition_fs_unmount(log_output->fs.phandle) != SEOS_FS_SUCCESS)
    {
        printf("Fail to unmount partition: %d!\n", log_output->fs.drv_id);
        return;
    }

    if(partition_close(log_output->fs.phandle) != SEOS_FS_SUCCESS)
    {
        printf("Fail to close partition: %d!\n", log_output->fs.drv_id);
        return;
    }

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



#include "log_consumer.h"
static bool
_Log_output_filesystem_print(Output_t *self, void *data)
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

    hFile_t fhandle;
    Log_output_t *log_output = (Log_output_t *)self;

    // log format layer
    log_output->log_format->vtable->convert((FormatT_t *)log_output->log_format, (Log_info_t *)&(((Log_consumer_t *)data)->log_info) );
    //log_output->log_format->vtable->print((FormatT_t *)log_output->log_format);

    fhandle = file_open(log_output->fs.phandle, log_output->fs.filename, FA_WRITE);
#if defined (SEOS_FS_BUILD_AS_LIB)
    if(fhandle == NULL){
#else
    if(fhandle < 0){
#endif
        printf("Fail to open file: %s!\n", log_output->fs.filename);
        return false;
    }

    if(file_write(fhandle, log_output->fs.file_offset, strlen(log_output->log_format->buffer), log_output->log_format->buffer) != SEOS_FS_SUCCESS)
    {
        printf("Fail to write file: %s!\n", log_output->fs.filename);
        return false;
    }

    if(file_close(fhandle) != SEOS_FS_SUCCESS)
    {
        printf("Fail to close file: %s!\n", log_output->fs.filename);
        return false;
    }

    log_output->fs.file_offset += strlen(log_output->log_format->buffer);

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
    log_output->log_format->vtable->convert((FormatT_t *)log_output->log_format, (Log_info_t *)&(((Log_consumer_t *)data)->log_info));
    log_output->log_format->vtable->print((FormatT_t *)log_output->log_format);

    return true;
}
