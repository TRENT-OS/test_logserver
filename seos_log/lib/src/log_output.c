#include "log_output.h"
#include <string.h>



// foreward declaration
static bool _prepare_filesystem(uint8_t drv_id, Log_output_filesystem_t *fs);
static void _Log_observer_dtor(Observer_t *self);
static bool _Log_output_update(Observer_t *self, void *data);
static bool _Log_output_filesystem_print(Output_t *self, void *data);
static bool _Log_output_console_print(Output_t *self, void *data);



//static bool
//_prepare_filesystem(uint8_t drv_id, Log_output_filesystem_t *fs)
//{
//    if(fs == NULL){
//        // Debug_printf
//        return false;
//    }

//    hPartition_t phandle;
//    phandle = partition_open(drv_id);
//#if defined (SEOS_FS_BUILD_AS_LIB)
//    if(phandle == NULL)
//#else
//    if(phandle < 0)
//#endif
//        return false;

//    if(partition_fs_mount(phandle) != SEOS_FS_SUCCESS)
//        return false;

////    SeosFileStream_ctor(&fs->file_stream, phandle, fs->filename, FileStream_OpenMode_W);
////    SeosFileStream_close(SeosFileStream_TO_STREAM(&fs->file_stream));

//    fs->phandle = phandle;

//    return true;
//}



static void
_Log_observer_dtor(Observer_t *self)
{
    memset(self, 0, sizeof (Observer_t));
}



static bool
_Log_output_update(Observer_t *self, void *data)
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



static const Output_Vtable Log_output_filesystem_vtable =
{
    .parent.dtor   = _Log_observer_dtor,
    .parent.update = _Log_output_update,
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

    retval = _prepare_filesystem(drv_id, &self->fs);

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

    partition_fs_unmount(log_output->fs.phandle);
    partition_close(log_output->fs.phandle);

    memset(self, 0, sizeof (Log_output_t));
}



static bool
_prepare_filesystem(uint8_t drv_id, Log_output_filesystem_t *fs)
{
    if(fs == NULL){
        // Debug_printf
        return false;
    }

    hPartition_t phandle;
    phandle = partition_open(drv_id);
#if defined (SEOS_FS_BUILD_AS_LIB)
    if(phandle == NULL)
#else
    if(phandle < 0)
#endif
        return false;

    if(partition_fs_mount(phandle) != SEOS_FS_SUCCESS)
        return false;

//    SeosFileStream_ctor(&fs->file_stream, phandle, fs->filename, FileStream_OpenMode_W);
//    SeosFileStream_close(SeosFileStream_TO_STREAM(&fs->file_stream));

    fs->phandle = phandle;

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
//    hFile_t fhandle;



    // log format layer
    log_output->log_format->vtable->convert((FormatT_t *)log_output->log_format, (Log_info_t *)data);
    log_output->log_format->vtable->print((FormatT_t *)log_output->log_format);




//    FileStream* fileStream;
//    fileStream = SeosFileStream_reOpen(SeosFileStream_TO_FILE_STREAM(&log_output->fs.file_stream), FileStream_OpenMode_A);
//    //SeosFileStream_seek(SeosFileStream_TO_FILE_STREAM(&log_output->fs.file_stream), 0, FileStream_SeekMode_Curr);
//    SeosFileStream_write(FileStream_TO_STREAM(fileStream), log_output->log_format->buffer, strlen(log_output->log_format->buffer));
//    SeosFileStream_close(FileStream_TO_STREAM(fileStream));






//    fhandle = file_open(log_output->fs.phandle, log_output->fs.filename, FA_CREATE_NEW | FA_WRITE);
//#if defined (SEOS_FS_BUILD_AS_LIB)
//    if(fhandle == NULL){
//#else
//    if(fhandle < 0)
//#endif
//        return false;
//    }

//    if(file_write(fhandle, 0, sizeof (log_output->log_format->buffer), log_output->log_format->buffer) != SEOS_FS_SUCCESS)
//        return false;

//    if(file_close(fhandle) != SEOS_FS_SUCCESS)
//        return false;






















    return true;
}



static const Output_Vtable Log_output_console_vtable =
{
    .parent.dtor   = _Log_observer_dtor,
    .parent.update = _Log_output_update,
    .dtor          = Log_output_filesystem_dtor,
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
