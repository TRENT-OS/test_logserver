#
# Test Log server
#
# Copyright (C) 2020, Hensoldt Cyber GmbH
#

cmake_minimum_required(VERSION 3.7.2)

os_set_config_file(system_config.h)


#-------------------------------------------------------------------------------
project(system_config C)
add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_LIST_DIR})


#-------------------------------------------------------------------------------
project(demo_logserver C)

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
        os_libs
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
        os_libs
)

DeclareCAmkESComponent(
    SendsIllFormattedString
    SOURCES
        "components/log_clients/SendsIllFormattedString/SendsIllFormattedString.c"
    C_FLAGS
        -Wall
        -Werror
    LIBS
        os_core_api
        os_logger
        os_libs
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
        os_libs
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
        os_libs
        os_filesystem
)

DeclareCAmkESComponent_TimeServer(
    TimeServer
)

DeclareCAmkESComponent_RamDisk(
    RamDisk
    system_config
)

DeclareAndCreateCamkESSystem(test_logserver.camkes)
GenerateSimulateScript()
