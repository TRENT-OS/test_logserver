#
# Test Log server
#
# Copyright (C) 2020-2022, HENSOLDT Cyber GmbH
#

cmake_minimum_required(VERSION 3.7.2)

#-------------------------------------------------------------------------------
# use the SDK
find_package("os-sdk" REQUIRED)
os_sdk_set_defaults()
os_sdk_setup(CONFIG_FILE "system_config.h" CONFIG_PROJECT "system_config")


#-------------------------------------------------------------------------------
project(test_logserver C)

DeclareCAmkESComponent(
    LogServer
    SOURCES
        "components/LogServer/src/custom_format.c"
        "components/LogServer/src/LogServer.c"
    C_FLAGS
        -Wall
        -Werror
        # We need to remove the Debug_Logs from the FS
        -DOS_FILESYSTEM_REMOVE_DEBUG_LOGGING
    LIBS
        os_core_api
        os_log_server_backend_filesystem
        lib_debug
        os_logger
        os_filesystem
        TimeServer_client
)

DeclareCAmkESComponent(
    LogsAllLevels
    SOURCES
        "components/log_clients/LogsAllLevels/LogsAllLevels.c"
    C_FLAGS
        -Wall
        -Werror
    LIBS
        os_core_api
        os_logger
        lib_debug
        lib_compiler
)

DeclareCAmkESComponent(
    LogFileReader
    SOURCES
        "components/log_clients/LogFileReader/LogFileReader.c"
    C_FLAGS
        -Wall
        -Werror
    LIBS
        os_core_api
        os_logger
        lib_debug
)

DeclareCAmkESComponent(
    FileReaderWriter
    SOURCES
        "components/log_clients/FileReaderWriter/FileReaderWriter.c"
   C_FLAGS
        -Wall
        -Werror
        # We need to remove the Debug_Logs from the FS
        -DOS_FILESYSTEM_REMOVE_DEBUG_LOGGING
    LIBS
        os_core_api
        os_logger
        lib_debug
        os_filesystem
)

TimeServer_DeclareCAmkESComponent(
    TimeServer
)

RamDisk_DeclareCAmkESComponent(
    RamDisk
)

os_sdk_create_CAmkES_system("test_logserver.camkes")
