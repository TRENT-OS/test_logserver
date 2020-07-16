
/**
 * Copyright (C) 2019, Hensoldt Cyber GmbH
 *
 * OS libraries configurations
 */


#pragma once


//-----------------------------------------------------------------------------
// Debug
//-----------------------------------------------------------------------------
#if !defined(NDEBUG)
#   define Debug_Config_STANDARD_ASSERT
#   define Debug_Config_ASSERT_SELF_PTR
#else
#   define Debug_Config_DISABLE_ASSERT
#   define Debug_Config_NO_ASSERT_SELF_PTR
#endif

#define Debug_Config_LOG_LEVEL                  Debug_LOG_LEVEL_CUSTOM
#define Debug_Config_INCLUDE_LEVEL_IN_MSG
#define Debug_Config_LOG_WITH_FILE_LINE

//-----------------------------------------------------------------------------
// Memory
//-----------------------------------------------------------------------------
#define Memory_Config_USE_STDLIB_ALLOC


//-----------------------------------------------------------------------------
// Memory
//-----------------------------------------------------------------------------
#define Memory_Config_USE_STDLIB_ALLOC


//-----------------------------------------------------------------------------
// RamDisk
//-----------------------------------------------------------------------------
#define RAMDISK_SIZE_BYTES  (1 * 1024 * 1024)


//-----------------------------------------------------------------------------
// COMMON
//-----------------------------------------------------------------------------
#define DATABUFFER_SIZE                         4096


//-----------------------------------------------------------------------------
// LOGGER
//-----------------------------------------------------------------------------

#if !defined(CAMKES_TOOL_PROCESSING)

// If the belows header is included in the config file and Logger library is
// linked, the LibDebug will forward entries to the LogServer.
#include "Logger/Client/OS_Logger.h"

#endif //!defined(CAMKES_TOOL_PROCESSING)

// api interface name
#define API_LOG_SERVER_EMIT                     log_server_interface_emit
#define API_LOG_SERVER_GET_SENDER_ID            log_server_interface_get_sender_id
#define API_LOG_SERVER_READ_LOG_FILE            log_server_interface_read_log_file

#define Debug_LOG_LEVEL_CUSTOM                  9
#define Debug_LOG_LEVEL_MAX                     0xFE
