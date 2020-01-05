/*
   *  Partition manager init file for use partition manager as a component.
   *
   *  Copyright (C) 2019, Hensoldt Cyber GmbH
*/


#include "api_pm.h"                 // include path to partition_manager must be set in cmakelists.txt
#include "LibDebug/Debug.h"         // needs seos_libs

#include "ProxyNVM.h"               // needs seos libs
#include "ChanMux/ChanMuxClient.h"  // needs seos libs

#include <camkes.h>



#ifndef GET_PROPERTY_CHANMUX_DATAPORT_BUFFER
    #define GET_PROPERTY_CHANMUX_DATAPORT_BUFFER    (void *)chanMuxDataPort
#endif

#ifndef GET_PROPERTY_CHANMUX_CHANNEL
    #define GET_PROPERTY_CHANMUX_CHANNEL            6
#endif



void api_pm_component__init(void){
    seos_pm_result_t pm_stat;
    static ChanMuxClient chanmux;
    static ProxyNVM proxy_NVM;
    static char proxy_NVM_message[PAGE_SIZE] = {0};

    if (!ChanMuxClient_ctor(&chanmux, GET_PROPERTY_CHANMUX_CHANNEL, GET_PROPERTY_CHANMUX_DATAPORT_BUFFER))
    {
        Debug_LOG_ERROR("Failed to construct testChanMuxClient!");
        return;
    }

    if( !ProxyNVM_ctor(&proxy_NVM, &chanmux, proxy_NVM_message, sizeof(proxy_NVM_message) ) ){
        Debug_LOG_ERROR("Failed to construct ProxyNVM!");
        return;
    }

    pm_stat = api_pm_partition_manager_init(&proxy_NVM);
    if(pm_stat != SEOS_PM_SUCCESS)
        Debug_LOG_DEBUG("Fail to init partition manager.");
}
