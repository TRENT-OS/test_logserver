#include "LibDebug/Debug.h"

#include "seos_fs.h"
#include "seos_pm.h"
#include "seos_logger.h"

#include <camkes.h>



#if !defined (DATABUFFER_CLIENT)
    #define DATABUFFER_CLIENT       (void *)dataport_buf
#endif



#define PARTITION_ID                0

#define FILE_NAME_P1_F1             "p1_f1.txt"
#define FILE_NAME_P1_F2             "p1_f2.txt"

#define DATA_LENGTH                 1000

static Log_filter_t filter;
static Log_emitter_callback_t reg;

static
seos_err_t
_create_file(
    hPartition_t phandle,
    const char *name,
    int length,
    const char c);

static
seos_err_t
_read_from_file(
    hPartition_t phandle,
    const char *name,
    int length,
    const char c);

static seos_err_t
filesystem_init(void)
{
    hPartition_t phandle;
    pm_disk_data_t pm_disk_data;
    pm_partition_data_t pm_partition_data;

    seos_err_t ret = partition_manager_get_info_disk(&pm_disk_data);
    if(SEOS_SUCCESS != ret)
    {
        Debug_LOG_ERROR("Fail to get disk info! Error code: %d", ret);
        return ret;
    }

    ret = partition_manager_get_info_partition(
            PARTITION_ID,
            &pm_partition_data);

    if(SEOS_SUCCESS != ret)
    {
        Debug_LOG_ERROR(
            "Fail to get partition info: %d! Error code: %d",
            pm_partition_data.partition_id,
            ret);

        return ret;
    }

    ret = partition_init(pm_partition_data.partition_id, 0);
    if(SEOS_SUCCESS != ret)
    {
        Debug_LOG_ERROR("Fail to init partition: %d! Error code: %d",
        pm_partition_data.partition_id,
        ret);

        return ret;
    }

    phandle = partition_open(pm_partition_data.partition_id);
    if(!is_valid_partition_handle(phandle))
    {
        ret = SEOS_ERROR_INVALID_HANDLE;
        Debug_LOG_ERROR(
            "Fail to open partition: %d! Error code: %d",
            pm_partition_data.partition_id,
            ret);

        return ret;
    }

    ret = partition_fs_create(
                phandle,
                FS_TYPE_FAT32,
                pm_partition_data.partition_size,
                0,  // default value: size of sector:   512
                0,  // default value: size of cluster:  512
                0,  // default value: reserved sectors count: FAT12/FAT16 = 1; FAT32 = 3
                0,  // default value: count file/dir entries: FAT12/FAT16 = 16; FAT32 = 0
                0,  // default value: count header sectors: 512
                FS_PARTITION_OVERWRITE_CREATE);

    if(SEOS_SUCCESS != ret)
    {
        Debug_LOG_ERROR(
            "Fail to create filesystem on partition: %d! Error code: %d",
            pm_partition_data.partition_id,
            ret);

        return ret;
    }

    ret = partition_close(phandle);
    if(SEOS_SUCCESS != ret)
    {
        Debug_LOG_ERROR(
            "Fail to close partition: %d!",
            pm_partition_data.partition_id);

        return ret;
    }

    return ret;
}



int run(void)
{
    hPartition_t phandle;
    int64_t length;

    /**********/
    /* Logger */
    /**********/
    // set up registered functions layer
    Log_emitter_callback_ctor(&reg, logServer_ready_wait, API_LOG_SERVER_EMIT);

    // set up log filter layer
    Log_filter_ctor(&filter, Debug_LOG_LEVEL_DEBUG);

    get_instance_Log_emitter(DATABUFFER_CLIENT, &filter, &reg);

    /*******************/
    /* Filesystem init */
    /*******************/
    seos_err_t ret = filesystem_init();
    if(SEOS_SUCCESS != ret)
    {
        Debug_LOG_ERROR(
            "Fail to init demo! Error code: %d",
            ret);

        return ret;
    }

    /*******************/
    /* Open partitions */
    /*******************/
    phandle = partition_open(PARTITION_ID);
    if(!is_valid_partition_handle(phandle))
    {
        ret = SEOS_ERROR_INVALID_HANDLE;
        Debug_LOG_ERROR(
            "Fail to open partition: %d! Error code: %d",
            PARTITION_ID,
            ret);

        return ret;
    }

    /*******************/
    /* Partition mount */
    /***************** */
    partition_fs_mount(phandle);

    /****************/
    /* Create files */
    /****************/
    ret = _create_file(phandle, FILE_NAME_P1_F1, DATA_LENGTH, 'a');
    if(SEOS_SUCCESS != ret)
    {
        Debug_LOG_ERROR("_create_file error %d", ret);
        return ret;
    }

    ret = _create_file(phandle, FILE_NAME_P1_F2, DATA_LENGTH * 2, 'b');
    if(SEOS_SUCCESS != ret)
    {
        Debug_LOG_ERROR("_create_file error %d", ret);
        return ret;
    }

    /*****************/
    /* Get file size */
    /*****************/
    Debug_LOG_DEBUG("### Get file size from file %s ###", FILE_NAME_P1_F2);

    length = file_getSize(phandle, FILE_NAME_P1_F2);
    Debug_LOG_DEBUG("file_getSize:      %lld", length);
    if(length < 0){
        Debug_LOG_ERROR("file_getSize error from file: %s", FILE_NAME_P1_F2);
        return SEOS_ERROR_GENERIC;
    }

    /**************/
    /* Read files */
    /**************/
    ret = _read_from_file(phandle, FILE_NAME_P1_F1, DATA_LENGTH, 'a');
    if(SEOS_SUCCESS != ret)
    {
        Debug_LOG_ERROR("Read from file %s failed. Error code: %d",
        FILE_NAME_P1_F1,
        ret);

        return ret;
    }

    ret = _read_from_file(phandle, FILE_NAME_P1_F2, DATA_LENGTH * 2, 'b');
    if(SEOS_SUCCESS != ret)
    {
        Debug_LOG_ERROR("Read from file %s failed. Error code: %d",
        FILE_NAME_P1_F2,
        ret);

        return ret;
    }

    /**********************/
    /* Unmount partitions */
    /**********************/
    ret = partition_fs_unmount(phandle);
    Debug_LOG_DEBUG("partition_unmount 1: %d", ret);

    if(SEOS_SUCCESS != ret)
    {
        return ret;
    }

    /********************/
    /* Close partitions */
    /********************/
    ret = partition_close(phandle);
    Debug_LOG_DEBUG("partition_close 1: %d", ret);

    if(SEOS_SUCCESS != ret)
    {
        return ret;
    }

    // destruction
    Log_emitter_dtor();
    Log_emitter_callback_dtor(&reg);
    Log_filter_dtor(&filter);

    return SEOS_SUCCESS;
}

//static
seos_err_t
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

        return SEOS_ERROR_INVALID_PARAMETER;
    }

    Debug_LOG_DEBUG("### Create file %s ###", name);
    // Open file
    fhandle = file_open(phandle, name, FA_CREATE_ALWAYS | FA_WRITE);
    if(!is_valid_file_handle(fhandle))
    {
        Debug_LOG_ERROR(
            "Failed to open the file: %s",
            name);

        return SEOS_ERROR_INVALID_HANDLE;
    }

    // fill buffer with test data
    memset(buf_write_file, c, length);

    // Call filesystem api function to write into a file
    seos_err_t err = file_write(fhandle, 0, length, buf_write_file);
    Debug_LOG_DEBUG("file_write:        %d", err);

    if(SEOS_SUCCESS != err)
    {
        return err;
    }

    // Close this file
    err = file_close(fhandle);
    Debug_LOG_DEBUG("file_close:        %d", err);

    return err;
}

//static
seos_err_t
_read_from_file(
    hPartition_t phandle,
    const char *name,
    int length,
    const char c)
{
    hFile_t fhandle;
    char buf_read_file[length];
    char buf_write_file[length];
    int8_t read_err = EOF;

    if(length > DATABUFFER_SIZE){
        Debug_LOG_ERROR("Length for data buffer to big!");

        return SEOS_ERROR_INVALID_PARAMETER;
    }

    // fill buffer with test data
    memset(buf_write_file, c, length);

    Debug_LOG_DEBUG("### Read from file %s ###", name);
    // Open file
    fhandle = file_open(phandle, name, FA_READ);
    if(!is_valid_file_handle(fhandle))
    {
        Debug_LOG_ERROR(
            "Failed to open the file: %s",
            name);

        return SEOS_ERROR_INVALID_HANDLE;
    }

    // clear the receive buffer
    memset(buf_read_file, 0, length);

    // Call filesystem api function to read from a file
    seos_err_t err = file_read(fhandle, 0, length, buf_read_file);
    Debug_LOG_DEBUG("file_read:         %d", err);
    if(SEOS_SUCCESS != err)
    {
        return err;
    }

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
            ++read_err;
        }
    }

    if(read_err > 0)
    {
        return SEOS_ERROR_GENERIC;
    }

    // Close file
    err = file_close(fhandle);
    Debug_LOG_DEBUG("file_close:        %d", err);

    return err;
}
