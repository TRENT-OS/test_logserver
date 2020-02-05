cmake_minimum_required(VERSION 3.7.2)

seos_set_config_file(config/config.h)

seos_use_libs(
    SEOS_LIBS
    SEOS_PARTITION_MANAGER
    SEOS_FILESYSTEM_CORE
    SEOS_FILESYSTEM_FAT
    SEOS_FILESYSTEM_SPIFFS
    SEOS_LOGGER
)

project(demo_logserver C)

# log server
DeclareCAmkESComponent(LOG_SERVER
    SOURCES
        "components/log_server/src/custom_format.c"
        "components/log_server/src/main.c"
    C_FLAGS
        -Wall
        -Werror
        -DSEOS_FS_BUILD_AS_LIB_BASIC_HANDLE
        -DSEOS_PARTITION_MANAGER_BUILD_AS_COMPONENT
    LIBS
        seos_core_api
        seos_log_server_backend_filesystem
        seos_libs
        seos_partition_manager_api
        seos_filesystem_core
        seos_filesystem_fat
        seos_filesystem_spiffs
)

# time server
DeclareCAmkESComponent(TIME_SERVER
    INCLUDES
        components/time_server/include
        components/time_server/timestamp
    SOURCES
        "components/time_server/src/main.c"
        "components/time_server/src/time_server.c"
        "components/time_server/timestamp/timestamp.c"
    C_FLAGS
        -Wall
        -Werror
)

# log_client
DeclareCAmkESComponent(LOG_CLIENT
    SOURCES
        "components/log_client/src/main.c"
    C_FLAGS
        -Wall
        -Werror
    LIBS
        seos_core_api
        seos_logger
        seos_libs
)

# app0x
DeclareCAmkESComponent(APP0x
    SOURCES
        "components/app0x/src/main.c"
    C_FLAGS
        -Wall
        -Werror
    LIBS
        seos_core_api
        seos_logger
        seos_libs
)

# app_filesystem
DeclareCAmkESComponent(APP_FILESYSTEM
    SOURCES
        "components/app_filesystem/src/main.c"
    C_FLAGS
        -DSEOS_FS_BUILD_AS_LIB_BASIC_HANDLE
        -DSEOS_PARTITION_MANAGER_BUILD_AS_COMPONENT
        -Wall
        -Werror
    LIBS
        seos_core_api
        seos_logger
        seos_libs
        seos_partition_manager_api
        seos_filesystem_core
        seos_filesystem_fat
        seos_filesystem_spiffs
)

# PARTITION_MANAGER
DeclareCAmkESComponent(PARTITION_MANAGER
    SOURCES
        "components/partition_manager/src/main.c"
    C_FLAGS
        -Wall
        -Werror
        -DSEOS_PARTITION_MANAGER_BUILD_AS_COMPONENT
    LIBS
        seos_core_api
        seos_logger
        seos_libs
        seos_partition_manager
)

# UartDrv
DeclareCAmkESComponent(UartDrv
    SOURCES
        "components/Uart/src/UartDrv.c"
        "components/Uart/src/qemu_uart.c"
    C_FLAGS
        -Wall
        -Werror
    LIBS
        seos_core_api
        seos_logger
        seos_libs
)

# ChanMux
DeclareCAmkESComponent(ChanMux
    SOURCES
        "components/ChanMux/src/instance/ChanMux_Instance.c"
    C_FLAGS
        -Wall
        -Werror
    LIBS
        seos_core_api
        seos_logger
        seos_libs
)

DeclareCAmkESRootserver(demo_logserver.camkes)
GenerateCAmkESRootServer()
GenerateSimulateScript()
