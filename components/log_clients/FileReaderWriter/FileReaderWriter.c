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



bool _create_file(hPartition_t phandle, const char *name, int length, const char c);
bool _read_from_file(hPartition_t phandle, const char *name, int length, const char c);



static bool
filesystem_init(void)
{
    hPartition_t phandle;
    pm_disk_data_t pm_disk_data;
    pm_partition_data_t pm_partition_data;

    if(partition_manager_get_info_disk(&pm_disk_data) != SEOS_PM_SUCCESS)
    {
        printf("Fail to get disk info!\n");
        return false;
    }

    if(partition_manager_get_info_partition(PARTITION_ID, &pm_partition_data) != SEOS_PM_SUCCESS)
    {
        printf("Fail to get partition info: %d!\n", pm_partition_data.partition_id);
        return false;
    }

    if(partition_init(pm_partition_data.partition_id, 0) != SEOS_FS_SUCCESS)
    {
        printf("Fail to init partition: %d!\n", pm_partition_data.partition_id);
        return false;
    }

    if( (phandle = partition_open(pm_partition_data.partition_id)) < 0)
    {
        printf("Fail to open partition: %d!\n", pm_partition_data.partition_id);
        return false;
    }

    if(partition_fs_create(
                phandle,
                FS_TYPE_FAT32,
                pm_partition_data.partition_size,
                0,  // default value: size of sector:   512
                0,  // default value: size of cluster:  512
                0,  // default value: reserved sectors count: FAT12/FAT16 = 1; FAT32 = 3
                0,  // default value: count file/dir entries: FAT12/FAT16 = 16; FAT32 = 0
                0,  // default value: count header sectors: 512
                FS_PARTITION_OVERWRITE_CREATE)
        != SEOS_FS_SUCCESS)
    {
        printf("Fail to create filesystem on partition: %d!\n", pm_partition_data.partition_id);
        return false;
    }

    if(partition_close(phandle) != SEOS_FS_SUCCESS)
    {
        printf("Fail to close partition: %d!\n", pm_partition_data.partition_id);
        return false;
    }

    return true;
}



int run(void) {
    seos_fs_result_t partition_stat;
    uint8_t file_stat;
    hPartition_t phandle;
    int64_t length;
    int8_t ret;

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
    ret = filesystem_init();
    if(ret < 0){
        Debug_LOG_ERROR("Fail to init demo!");
        return EOF;
    }

    /*******************/
    /* Open partitions */
    /*******************/
    phandle = partition_open(PARTITION_ID);
    if(phandle < 0)
        return EOF;

    /*******************/
    /* Partition mount */
    /***************** */
    partition_fs_mount(phandle);

    /****************/
    /* Create files */
    /****************/
    file_stat = _create_file(phandle, FILE_NAME_P1_F1, DATA_LENGTH, 'a');
    if(file_stat == false){
        Debug_LOG_ERROR("_create_file error %d", file_stat);
        return EOF;
    }

    file_stat = _create_file(phandle, FILE_NAME_P1_F2, DATA_LENGTH * 2, 'b');
    if(file_stat == false){
        Debug_LOG_ERROR("_create_file error %d", file_stat);
        return EOF;
    }

    /*****************/
    /* Get file size */
    /*****************/
    Debug_LOG_DEBUG("### Get file size from file %s ###", FILE_NAME_P1_F2);

    length = file_getSize(phandle, FILE_NAME_P1_F2);
    Debug_LOG_DEBUG("file_getSize:      %lld", length);
    if(length < 0){
        Debug_LOG_ERROR("file_getSize error from file: %s", FILE_NAME_P1_F2);
        return EOF;
    }

    /**************/
    /* Read files */
    /**************/
    file_stat = _read_from_file(phandle, FILE_NAME_P1_F1, DATA_LENGTH, 'a');
    if(file_stat == false){
        Debug_LOG_ERROR("_read_from_file error %d", file_stat);
        return EOF;
    }

    file_stat = _read_from_file(phandle, FILE_NAME_P1_F2, DATA_LENGTH * 2, 'b');
    if(file_stat == false){
        Debug_LOG_ERROR("_read_from_file error %d", file_stat);
        return EOF;
    }

    /**********************/
    /* Unmount partitions */
    /**********************/
    partition_stat = partition_fs_unmount(phandle);
    Debug_LOG_DEBUG("partition_unmount 1: %d", (uint8_t)partition_stat);
    if(partition_stat > 0)
        return EOF;

    /********************/
    /* Close partitions */
    /********************/
    partition_stat = partition_close(phandle);
    Debug_LOG_DEBUG("partition_close 1: %d", (uint8_t)partition_stat);
    if(partition_stat > 0)
        return EOF;

    // destruction
    Log_emitter_dtor();

    Log_emitter_callback_dtor(&reg);

    Log_filter_dtor(&filter);

    return 0;
}



bool _create_file(hPartition_t phandle, const char *name, int length, const char c){
    hFile_t fhandle;
    seos_fs_result_t file_stat = SEOS_FS_SUCCESS;
    char buf_write_file[length];

    if(length > DATABUFFER_SIZE){
        Debug_LOG_ERROR("Length for data buffer to big!");
        return false;
    }

    Debug_LOG_DEBUG("### Create file %s ###", name);
    // Open file
    fhandle = file_open(phandle, name, FA_CREATE_ALWAYS | FA_WRITE);
    if(fhandle < 0)
        return EOF;

    // fill buffer with test data
    memset(buf_write_file, c, length);

    // Call filesystem api function to write into a file
    file_stat = file_write(fhandle, 0, length, buf_write_file);
    Debug_LOG_DEBUG("file_write:        %d", (uint8_t)file_stat);
    if(file_stat > 0)
        return false;

    // Close this file
    file_stat = file_close(fhandle);
    Debug_LOG_DEBUG("file_close:        %d", (uint8_t)file_stat);
    if(file_stat > 0)
        return false;

    return true;
}



bool _read_from_file(hPartition_t phandle, const char *name, int length, const char c){
    hFile_t fhandle;
    seos_fs_result_t file_stat = SEOS_FS_SUCCESS;
    char buf_read_file[length];
    char buf_write_file[length];
    int8_t read_err = EOF;

    if(length > DATABUFFER_SIZE){
        Debug_LOG_ERROR("Length for data buffer to big!");
        return false;
    }

    // fill buffer with test data
    memset(buf_write_file, c, length);

    Debug_LOG_DEBUG("### Read from file %s ###", name);
    // Open file
    fhandle = file_open(phandle, name, FA_READ);
    if(fhandle < 0)
        return EOF;

    // clear the receive buffer
    memset(buf_read_file, 0, length);

    // Call filesystem api function to read from a file
    file_stat = file_read(fhandle, 0, length, buf_read_file);
    Debug_LOG_DEBUG("file_read:         %d", (uint8_t)file_stat);
    if(file_stat > 0)
        return false;

    for(int i = 0; i < length; i++){
        if(buf_read_file[i] != buf_write_file[i]){
            Debug_LOG_ERROR("Read values corrupted!");
            read_err = 1;
        }
    }

    if(read_err > 0)
        return false;

    // Close file
    file_stat = file_close(fhandle);
    Debug_LOG_DEBUG("file_close:        %d", (uint8_t)file_stat);
    if(file_stat > 0)
        return false;

    return true;
}
