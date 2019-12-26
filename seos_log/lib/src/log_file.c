#include "log_file.h"
#include "consumer_chain.h"
#include "log_consumer.h"
#include <string.h>



#if !defined (API_LOG_SERVER_READ_LOG_FILE)
    #define API_LOG_SERVER_READ_LOG_FILE    log_server_interface_read_log_file
#endif



// foreward declaration
static bool    _prepare_filesystem(Log_file_info_t *log_file_info);
static int64_t _Log_file_read_log_file(Log_file_t *self, uint64_t offset, uint64_t len, int64_t log_file_size);



static const Log_file_Vtable Log_file_vtable =
{
    .dtor          = Log_file_dtor,
    .read_log_file = _Log_file_read_log_file
};



int64_t
API_LOG_SERVER_READ_LOG_FILE(uint64_t offset,
                             uint64_t len,
                             int64_t *log_file_size)
{
    Log_consumer_t *log_consumer = Consumer_chain_get_sender();
    if(log_consumer == NULL)
        return -1;

    *log_file_size = file_getSize(log_consumer->log_file->log_file_info.phandle, log_consumer->log_file->log_file_info.filename);
    if(*log_file_size < 0)
        return -1;

    log_consumer->log_file->log_file_info.lenght = (uint64_t)*log_file_size;

    if(offset > (uint64_t)*log_file_size)
        return -1;

    if(*log_file_size <= (int64_t)(offset + len)){
        len = (uint64_t)((uint64_t)*log_file_size - offset);
    }

    hFile_t fhandle;
    fhandle = file_open(log_consumer->log_file->log_file_info.phandle, log_consumer->log_file->log_file_info.filename, FA_READ);
#if defined (SEOS_FS_BUILD_AS_LIB)
    if(fhandle == NULL){
#else
    if(fhandle < 0){
#endif
        printf("Fail to open file: %s!\n", log_consumer->log_file->log_file_info.filename);
            return -1;
    }

    if(file_read(fhandle, (long)offset, (long)len, log_consumer->buf) != SEOS_FS_SUCCESS)
    {
        printf("Fail to read file: %s!\n", log_consumer->log_file->log_file_info.filename);
            return -1;
    }

    if(file_close(fhandle) != SEOS_FS_SUCCESS)
    {
        printf("Fail to close file: %s!\n", log_consumer->log_file->log_file_info.filename);
            return -1;
    }

    return (int64_t)len;
}



static bool
_prepare_filesystem(Log_file_info_t *log_file_info)
{
    if(log_file_info == NULL){
        // Debug_printf
        return false;
    }

    log_file_info->phandle = partition_open(log_file_info->drv_id);
#if defined (SEOS_FS_BUILD_AS_LIB)
    if(log_file_info->phandle == NULL){
#else
    if(log_file_info->phandle < 0){
#endif
        return false;
    }

    if(partition_fs_mount(log_file_info->phandle) != SEOS_FS_SUCCESS)
        return false;

    // create empty file
    hFile_t fhandle;
    fhandle = file_open(log_file_info->phandle, log_file_info->filename, FA_CREATE_ALWAYS);
#if defined (SEOS_FS_BUILD_AS_LIB)
    if(fhandle == NULL){
#else
    if(fhandle < 0){
#endif
        printf("Fail to open file: %s!\n", log_file_info->filename);
        return false;
    }

    if(file_close(fhandle) != SEOS_FS_SUCCESS)
    {
        printf("Fail to close file: %s!\n", log_file_info->filename);
        return false;
    }

    log_file_info->offset = 0;

    return true;
}



bool
Log_file_ctor(Log_file_t *self,
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

    if(filename == NULL){
        // Debug_printf
        return false;
    }

    self->vtable = &Log_file_vtable;

    self->log_file_info.drv_id = drv_id;
    strcpy(self->log_file_info.filename, filename);

    retval = _prepare_filesystem(&self->log_file_info);
    return retval;
}



void
Log_file_dtor(Log_file_t *self)
{
    bool nullptr = false;

    ASSERT_SELF__(self);

    if(nullptr){
        // Debug_printf
        return;
    }

    if(partition_fs_unmount(self->log_file_info.phandle) != SEOS_FS_SUCCESS)
    {
        printf("Fail to unmount partition: %d!\n", self->log_file_info.drv_id);
        return;
    }

    if(partition_close(self->log_file_info.phandle) != SEOS_FS_SUCCESS)
    {
        printf("Fail to close partition: %d!\n", self->log_file_info.drv_id);
        return;
    }

    memset(self, 0, sizeof (Log_file_t));
}



static int64_t
_Log_file_read_log_file(Log_file_t *self,
                        uint64_t offset,
                        uint64_t len,
                        int64_t log_file_size)
{
    bool nullptr = false;

    ASSERT_SELF__(self);

    if(nullptr){
        // Debug_printf
        return -1;
    }

    return API_LOG_SERVER_READ_LOG_FILE(offset, len, &log_file_size);
}
