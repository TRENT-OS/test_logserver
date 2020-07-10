#include "LibDebug/Debug.h"

#include "OS_FileSystem.h"

#include <stdlib.h>
#include <string.h>

#include <camkes.h>



#if !defined (DATABUFFER_CLIENT)
#define DATABUFFER_CLIENT       (void *)dataport_buf
#endif



#define FILE_NAME_P1_F1             "p1_f1.txt"
#define FILE_NAME_P1_F2             "p1_f2.txt"

#define DATA_LENGTH                 1000

static OS_LoggerFilter_Handle_t filter;

static OS_FileSystem_Config_t cfgFs =
{
    .type = OS_FileSystem_Type_LITTLEFS,
    .size = OS_FileSystem_STORAGE_MAX,
    .storage = OS_FILESYSTEM_ASSIGN_Storage(
        storage_rpc,
        storage_dp),
};

static
OS_Error_t
_create_file(
    OS_FileSystem_Handle_t hFs,
    const char* name,
    int length,
    const char c);

static
OS_Error_t
_read_from_file(
    OS_FileSystem_Handle_t hFs,
    const char* name,
    int length,
    const char c);

void post_init()
{
    OS_LoggerFilter_ctor(&filter, Debug_LOG_LEVEL_DEBUG);

    OS_LoggerEmitter_getInstance(
        DATABUFFER_CLIENT,
        &filter,
        API_LOG_SERVER_EMIT);
}

int run(void)
{
    OS_Error_t err;
    OS_FileSystem_Handle_t hFs;
    size_t length;

    /*************/
    /* Create fs */
    /*************/
    err = OS_FileSystem_init(&hFs, &cfgFs);
    if (OS_SUCCESS != err)
    {
        Debug_LOG_ERROR("OS_FileSystem_init failed with error code %d!", err);
        return err;
    }
    err = OS_FileSystem_format(hFs);
    if (OS_SUCCESS != err)
    {
        Debug_LOG_ERROR("OS_FileSystem_format failed with error code %d!", err);
        return err;
    }
    err = OS_FileSystem_mount(hFs);
    if (OS_SUCCESS != err)
    {
        Debug_LOG_ERROR("OS_FileSystem_mount failed with error code %d!", err);
        return err;
    }

    /****************/
    /* Create files */
    /****************/
    err = _create_file(hFs, FILE_NAME_P1_F1, DATA_LENGTH, 'a');
    if (OS_SUCCESS != err)
    {
        Debug_LOG_ERROR("_create_file error %d", err);
        return err;
    }

    err = _create_file(hFs, FILE_NAME_P1_F2, DATA_LENGTH * 2, 'b');
    if (OS_SUCCESS != err)
    {
        Debug_LOG_ERROR("_create_file error %d", err);
        return err;
    }

    /*****************/
    /* Get file size */
    /*****************/
    Debug_LOG_DEBUG("### Get file size from file %s ###", FILE_NAME_P1_F2);

    err = OS_FileSystemFile_getSize(hFs, FILE_NAME_P1_F2, &length);
    if (OS_SUCCESS != err)
    {
        Debug_LOG_ERROR("file_getSize error from file: %s", FILE_NAME_P1_F2);
        return OS_ERROR_GENERIC;
    }

    Debug_LOG_DEBUG("file_getSize:      %zu", length);

    /**************/
    /* Read files */
    /**************/
    err = _read_from_file(hFs, FILE_NAME_P1_F1, DATA_LENGTH, 'a');
    if (OS_SUCCESS != err)
    {
        Debug_LOG_ERROR("Read from file %s failed. Error code: %d",
                        FILE_NAME_P1_F1,
                        err);
        return err;
    }

    err = _read_from_file(hFs, FILE_NAME_P1_F2, DATA_LENGTH * 2, 'b');
    if (OS_SUCCESS != err)
    {
        Debug_LOG_ERROR("Read from file %s failed. Error code: %d",
                        FILE_NAME_P1_F2,
                        err);
        return err;
    }

    /**********************/
    /* Unmount partitions */
    /**********************/
    err = OS_FileSystem_unmount(hFs);
    Debug_LOG_DEBUG("partition_unmount: %d", err);

    if (OS_SUCCESS != err)
    {
        return err;
    }

    /********************/
    /* Close partitions */
    /********************/
    err = OS_FileSystem_free(hFs);
    Debug_LOG_DEBUG("partition_close: %d", err);

    if (OS_SUCCESS != err)
    {
        return err;
    }

    return OS_SUCCESS;
}

//static
OS_Error_t
_create_file(
    OS_FileSystem_Handle_t hFs,
    const char* name,
    int length,
    const char c)
{
    OS_Error_t err;
    OS_FileSystemFile_Handle_t hFile;
    char buf_write_file[length];

    if (length > DATABUFFER_SIZE)
    {
        Debug_LOG_ERROR("Length for data buffer to big!");

        return OS_ERROR_INVALID_PARAMETER;
    }

    Debug_LOG_DEBUG("### Create file %s ###", name);
    // Open file
    err = OS_FileSystemFile_open(hFs, &hFile, name,
                                 OS_FileSystem_OpenMode_RDWR,
                                 OS_FileSystem_OpenFlags_CREATE);
    if (OS_SUCCESS != err)
    {
        Debug_LOG_ERROR("Failed to open the file: %s",  name);
        return OS_ERROR_INVALID_HANDLE;
    }

    // fill buffer with test data
    memset(buf_write_file, c, length);

    // Call filesystem api function to write into a file
    err = OS_FileSystemFile_write(hFs, hFile, 0, length, buf_write_file);
    Debug_LOG_DEBUG("file_write:        %d", err);

    if (OS_SUCCESS != err)
    {
        return err;
    }

    // Close this file
    err = OS_FileSystemFile_close(hFs, hFile);
    Debug_LOG_DEBUG("file_close:        %d", err);

    return err;
}

//static
OS_Error_t
_read_from_file(
    OS_FileSystem_Handle_t hFs,
    const char* name,
    int length,
    const char c)
{
    OS_Error_t err;
    OS_FileSystemFile_Handle_t hFile;
    char buf_read_file[length];
    char buf_write_file[length];

    if (length > DATABUFFER_SIZE)
    {
        Debug_LOG_ERROR("Length for data buffer to big!");

        return OS_ERROR_INVALID_PARAMETER;
    }

    // fill buffer with test data
    memset(buf_write_file, c, length);

    Debug_LOG_DEBUG("### Read from file %s ###", name);
    err = OS_FileSystemFile_open(hFs, &hFile, name,
                                 OS_FileSystem_OpenMode_RDONLY,
                                 OS_FileSystem_OpenFlags_NONE);
    if (OS_SUCCESS != err)
    {
        Debug_LOG_ERROR("Failed to open the file: %s",  name);
        return OS_ERROR_INVALID_HANDLE;
    }

    // clear the receive buffer
    memset(buf_read_file, 0, length);

    // Call filesystem api function to read from a file
    err = OS_FileSystemFile_read(hFs, hFile, 0, length, buf_read_file);
    Debug_LOG_DEBUG("file_read:         %d", err);
    if (OS_SUCCESS != err)
    {
        return err;
    }

    int readErr = 0;
    for (int i = 0; i < length; i++)
    {
        if (buf_read_file[i] != buf_write_file[i])
        {
            Debug_LOG_ERROR(
                "Read values corrupted at index %d! " \
                "buf_read_file[i] = %d, buf_write_file[i] = %d",
                i,
                buf_read_file[i],
                buf_write_file[i]);

            ++readErr;
        }
    }

    if (readErr > 0)
    {
        return OS_ERROR_GENERIC;
    }

    // Close file
    err = OS_FileSystemFile_close(hFs, hFile);
    Debug_LOG_DEBUG("file_close:        %d", err);

    return err;
}
