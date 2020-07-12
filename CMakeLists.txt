#
# Test Log Server
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
        -DOS_FS_BUILD_AS_LIB_BASIC_HANDLE
        -DOS_PARTITION_MANAGER_BUILD_AS_COMPONENT
    LIBS
        os_core_api
        os_log_server_backend_filesystem
        os_libs
        os_logger
        os_partition_manager_api
        os_filesystem_core
        os_filesystem_fat
        os_filesystem_spiffs
)

DeclareCAmkESComponent(
    TimeServer
    INCLUDES
        components/test_fixture/time_server/include
        components/test_fixture/time_server/timestamp
    SOURCES
        "components/test_fixture/time_server/src/main.c"
        "components/test_fixture/time_server/src/time_server.c"
        "components/test_fixture/time_server/timestamp/timestamp.c"
    C_FLAGS
        -Wall
        -Werror
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
        -DOS_FS_BUILD_AS_LIB_BASIC_HANDLE
        -DOS_PARTITION_MANAGER_BUILD_AS_COMPONENT
        -Wall
        -Werror
    LIBS
        os_core_api
        os_logger
        os_libs
        os_partition_manager_api
        os_filesystem_core
        os_filesystem_fat
        os_filesystem_spiffs
)

DeclareCAmkESComponent(
    PartitionManager
    SOURCES
        "components/test_fixture/partition_manager/partition_manager.c"
    C_FLAGS
        -Wall
        -Werror
        -DOS_PARTITION_MANAGER_BUILD_AS_COMPONENT
    LIBS
        os_core_api
        os_logger
        os_libs
        proxy_nvm
        os_partition_manager
)

DeclareCAmkESComponent_UART(
    UART
)

DeclareCAmkESComponent_ChanMux(
    ChanMux
    components/test_fixture/ChanMux/ChanMux_config.c
    system_config
)


DeclareAndCreateCamkESSystem(test_logserver.camkes)
GenerateSimulateScript()
