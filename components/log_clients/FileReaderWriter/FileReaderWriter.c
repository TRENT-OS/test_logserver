#include "LibDebug/Debug.h"

#include "OS_Filesystem.h"
#include "OS_PartitionManager.h"
#include <camkes.h>



#if !defined (DATABUFFER_CLIENT)
    #define DATABUFFER_CLIENT       (void *)dataport_buf
#endif



#define PARTITION_ID                0

#define FILE_NAME_P1_F1             "p1_f1.txt"
#define FILE_NAME_P1_F2             "p1_f2.txt"

#define DATA_LENGTH                 1000

static OS_LoggerFilter_Handle_t filter;

static
OS_Error_t
_create_file(
    hPartition_t phandle,
    const char *name,
    int length,
    const char c);

static
OS_Error_t
_read_from_file(
    hPartition_t phandle,
    const char *name,
    int length,
    const char c);

static OS_Error_t
filesystem_init(void)
{
    hPartition_t phandle;
    OS_PartitionManagerDataTypes_DiskData_t pm_disk_data;
    OS_PartitionManagerDataTypes_PartitionData_t pm_partition_data;

    OS_Error_t ret = OS_PartitionManager_getInfoDisk(&pm_disk_data);
    if(OS_SUCCESS != ret)
    {
        Debug_LOG_ERROR("Fail to get disk info! Error code: %d", ret);
        return ret;
    }

    ret = OS_PartitionManager_getInfoPartition(
            PARTITION_ID,
            &pm_partition_data);

    if(OS_SUCCESS != ret)
    {
        Debug_LOG_ERROR(
            "Fail to get partition info: %d! Error code: %d",
            pm_partition_data.partition_id,
            ret);

        return ret;
    }

    ret = OS_Filesystem_init(pm_partition_data.partition_id, 0);
    if(OS_SUCCESS != ret)
    {
        Debug_LOG_ERROR("Fail to init partition: %d! Error code: %d",
        pm_partition_data.partition_id,
        ret);

        return ret;
    }

    phandle = OS_Filesystem_open(pm_partition_data.partition_id);
    if(!OS_Filesystem_validatePartitionHandle(phandle))
    {
        ret = OS_ERROR_INVALID_HANDLE;
        Debug_LOG_ERROR(
            "Fail to open partition: %d! Error code: %d",
            pm_partition_data.partition_id,
            ret);

        return ret;
    }

    ret = OS_Filesystem_create(
                phandle,
                FS_TYPE_FAT32,
                pm_partition_data.partition_size,
                0,  // default value: size of sector:   512
                0,  // default value: size of cluster:  512
                0,  // default value: reserved sectors count: FAT12/FAT16 = 1; FAT32 = 3
                0,  // default value: count file/dir entries: FAT12/FAT16 = 16; FAT32 = 0
                0,  // default value: count header sectors: 512
                FS_PARTITION_OVERWRITE_CREATE);

    if(OS_SUCCESS != ret)
    {
        Debug_LOG_ERROR(
            "Fail to create filesystem on partition: %d! Error code: %d",
            pm_partition_data.partition_id,
            ret);

        return ret;
    }

    ret = OS_Filesystem_close(phandle);
    if(OS_SUCCESS != ret)
    {
        Debug_LOG_ERROR(
            "Fail to close partition: %d!",
            pm_partition_data.partition_id);

        return ret;
    }

    return ret;
}

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
    hPartition_t phandle;
    int64_t length;

    /*******************/
    /* Filesystem init */
    /*******************/
    OS_Error_t ret = filesystem_init();
    if(OS_SUCCESS != ret)
    {
        Debug_LOG_ERROR(
            "Fail to init demo! Error code: %d",
            ret);

        return ret;
    }

    /*******************/
    /* Open partitions */
    /*******************/
    phandle = OS_Filesystem_open(PARTITION_ID);
    if(!OS_Filesystem_validatePartitionHandle(phandle))
    {
        ret = OS_ERROR_INVALID_HANDLE;
        Debug_LOG_ERROR(
            "Fail to open partition: %d! Error code: %d",
            PARTITION_ID,
            ret);

        return ret;
    }

    /*******************/
    /* Partition mount */
    /***************** */
    OS_Filesystem_mount(phandle);

    /****************/
    /* Create files */
    /****************/
    ret = _create_file(phandle, FILE_NAME_P1_F1, DATA_LENGTH, 'a');
    if(OS_SUCCESS != ret)
    {
        Debug_LOG_ERROR("_create_file error %d", ret);
        return ret;
    }

    ret = _create_file(phandle, FILE_NAME_P1_F2, DATA_LENGTH * 2, 'b');
    if(OS_SUCCESS != ret)
    {
        Debug_LOG_ERROR("_create_file error %d", ret);
        return ret;
    }

    /*****************/
    /* Get file size */
    /*****************/
    Debug_LOG_DEBUG("### Get file size from file %s ###", FILE_NAME_P1_F2);

    length = OS_Filesystem_getSizeOfFile(phandle, FILE_NAME_P1_F2);
    Debug_LOG_DEBUG("file_getSize:      %lld", length);
    if(length < 0){
        Debug_LOG_ERROR("file_getSize error from file: %s", FILE_NAME_P1_F2);
        return OS_ERROR_GENERIC;
    }

    /**************/
    /* Read files */
    /**************/
    ret = _read_from_file(phandle, FILE_NAME_P1_F1, DATA_LENGTH, 'a');
    if(OS_SUCCESS != ret)
    {
        Debug_LOG_ERROR("Read from file %s failed. Error code: %d",
        FILE_NAME_P1_F1,
        ret);

        return ret;
    }

    ret = _read_from_file(phandle, FILE_NAME_P1_F2, DATA_LENGTH * 2, 'b');
    if(OS_SUCCESS != ret)
    {
        Debug_LOG_ERROR("Read from file %s failed. Error code: %d",
        FILE_NAME_P1_F2,
        ret);

        return ret;
    }

    /**********************/
    /* Unmount partitions */
    /**********************/
    ret = OS_Filesystem_unmount(phandle);
    Debug_LOG_DEBUG("partition_unmount 1: %d", ret);

    if(OS_SUCCESS != ret)
    {
        return ret;
    }

    /********************/
    /* Close partitions */
    /********************/
    ret = OS_Filesystem_close(phandle);
    Debug_LOG_DEBUG("partition_close 1: %d", ret);

    if(OS_SUCCESS != ret)
    {
        return ret;
    }

    // destruction
    OS_LoggerEmitter_dtor();
    OS_LoggerFilter_dtor(&filter);

    return OS_SUCCESS;
}

//static
OS_Error_t
_create_file(
    hPartition_t phandle,
    const char *name,
    int length,
    const char c)
{
    hFile_t fhandle;
    char buf_write_file[length];

    if(length > DATABUFFER_SIZE){
        Debug_LOG_ERROR("Length for data buffer to big!");

        return OS_ERROR_INVALID_PARAMETER;
    }

    Debug_LOG_DEBUG("### Create file %s ###", name);
    // Open file
    fhandle = OS_Filesystem_openFile(phandle, name, FA_CREATE_ALWAYS | FA_WRITE);
    if(!OS_Filesystem_validateFileHandle(fhandle))
    {
        Debug_LOG_ERROR(
            "Failed to open the file: %s",
            name);

        return OS_ERROR_INVALID_HANDLE;
    }

    // fill buffer with test data
    memset(buf_write_file, c, length);

    // Call filesystem api function to write into a file
    OS_Error_t err = OS_Filesystem_writeFile(fhandle, 0, length, buf_write_file);
    Debug_LOG_DEBUG("file_write:        %d", err);

    if(OS_SUCCESS != err)
    {
        return err;
    }

    // Close this file
    err = OS_Filesystem_closeFile(fhandle);
    Debug_LOG_DEBUG("file_close:        %d", err);

    return err;
}

//static
OS_Error_t
_read_from_file(
    hPartition_t phandle,
    const char *name,
    int length,
    const char c)
{
    hFile_t fhandle;
    char buf_read_file[length];
    char buf_write_file[length];

    if(length > DATABUFFER_SIZE){
        Debug_LOG_ERROR("Length for data buffer to big!");

        return OS_ERROR_INVALID_PARAMETER;
    }

    // fill buffer with test data
    memset(buf_write_file, c, length);

    Debug_LOG_DEBUG("### Read from file %s ###", name);
    // Open file
    fhandle = OS_Filesystem_openFile(phandle, name, FA_READ);
    if(!OS_Filesystem_validateFileHandle(fhandle))
    {
        Debug_LOG_ERROR(
            "Failed to open the file: %s",
            name);

        return OS_ERROR_INVALID_HANDLE;
    }

    // clear the receive buffer
    memset(buf_read_file, 0, length);

    // Call filesystem api function to read from a file
    OS_Error_t err = OS_Filesystem_readFile(fhandle, 0, length, buf_read_file);
    Debug_LOG_DEBUG("file_read:         %d", err);
    if(OS_SUCCESS != err)
    {
        return err;
    }

    int readErr = 0;
    for(int i = 0; i < length; i++)
    {
        if(buf_read_file[i] != buf_write_file[i])
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

    if(readErr > 0)
    {
        return OS_ERROR_GENERIC;
    }

    // Close file
    err = OS_Filesystem_closeFile(fhandle);
    Debug_LOG_DEBUG("file_close:        %d", err);

    return err;
}
