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



static ChanMuxClient chanmux;
static ProxyNVM proxy_NVM;
static char proxy_NVM_message[PAGE_SIZE] = {0};



void api_pm_component__init(void)
{
    if (!ChanMuxClient_ctor(
            &chanmux,
            GET_PROPERTY_CHANMUX_CHANNEL,
            GET_PROPERTY_CHANMUX_DATAPORT_BUFFER,
            GET_PROPERTY_CHANMUX_DATAPORT_BUFFER))
    {
        Debug_LOG_ERROR("Failed to construct testChanMuxClient!");
        return;
    }

    if( !ProxyNVM_ctor(&proxy_NVM, &chanmux, proxy_NVM_message, sizeof(proxy_NVM_message) ) ){
        Debug_LOG_ERROR("Failed to construct ProxyNVM!");
        return;
    }

    seos_err_t err = api_pm_partition_manager_init(&proxy_NVM);

    if(SEOS_SUCCESS != err)
    {
        Debug_LOG_ERROR("Fail to init partition manager. Error code: %d", err);
    }
}
