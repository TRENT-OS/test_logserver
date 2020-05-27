/*
   *  Partition Manager
   *
   *  Copyright (C) 2020, Hensoldt Cyber GmbH
*/


#include "LibDebug/Debug.h"
#include "ChanMuxNvmDriver.h"
#include "api_pm.h"

#include <camkes.h>


static const ChanMuxClientConfig_t chanMuxClientConfig = {
    .port  = CHANMUX_DATAPORT_DUPLEX_SHARED_ASSIGN(chanMux_port),
    .wait  = chanMux_event_hasData_wait,
    .write = chanMux_rpc_write,
    .read  = chanMux_rpc_read
};

static ChanMuxNvmDriver chanMuxNvmDriver;


void api_pm_component__init(void)
{
    if (!ChanMuxNvmDriver_ctor(
            &chanMuxNvmDriver,
            &chanMuxClientConfig))
    {
        Debug_LOG_ERROR("Failed to construct ChanMuxNvmDriver");
        return;
    }

    OS_Error_t ret = api_pm_partition_manager_init(
                            ChanMuxNvmDriver_get_nvm(&chanMuxNvmDriver));
    if(ret != OS_SUCCESS)
    {
        Debug_LOG_DEBUG("Fail to init partition manager, code %d", ret);
        return;
    }
}
