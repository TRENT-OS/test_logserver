cmake_minimum_required(VERSION 3.7.2)

set(LOGGER_CONFIG_H_FILE ${CMAKE_CURRENT_LIST_DIR}/config/seos_log_server_config.h)
set(SEOS_FS_CONFIG_H_FILE ${CMAKE_CURRENT_LIST_DIR}/config/seos_fs_config.h)
set(SEOS_PM_CONFIG_H_FILE ${CMAKE_CURRENT_LIST_DIR}/config/seos_pm_config.h)

seos_set_config_file(config/config.h)

seos_use_libs(
    SEOS_LIBS
    SEOS_PARTITION_MANAGER
    SEOS_FILESYSTEM_CORE
    SEOS_FILESYSTEM_FAT
    SEOS_FILESYSTEM_SPIFFS
)

project(demo_logserver C)

# log server
DeclareCAmkESComponent(LOG_SERVER
    INCLUDES
        seos_log/include
        seos_log/lib/include
        seos_timestamp
        config
    SOURCES
        "components/log_server/src/main.c"

        "seos_log/lib/src/consumer_chain.c"
        "seos_log/lib/src/listT.c"
        "seos_log/lib/src/log_consumer.c"
        "seos_log/lib/src/log_consumer_callback.c"
        "seos_log/lib/src/log_databuffer.c"
        "seos_log/lib/src/log_file.c"
        "seos_log/lib/src/log_filter.c"
        "seos_log/lib/src/log_format.c"
        "seos_log/lib/src/log_output.c"
        "seos_log/lib/src/log_subject.c"

        "seos_timestamp/timestamp.c"
    C_FLAGS
        -Wall
        -Werror
        # log server
        -DLOGGER_CONFIG_H_FILE=${LOGGER_CONFIG_H_FILE}
        # filesystem
        -DSEOS_FS_CONFIG_H_FILE=${SEOS_FS_CONFIG_H_FILE}
        #-DSEOS_FS_BUILD_AS_LIB
        -DSEOS_FS_BUILD_AS_LIB_BASIC_HANDLE
        # partition manager
        -DSEOS_PM_CONFIG_H_FILE=${SEOS_PM_CONFIG_H_FILE}
        -DSEOS_PARTITION_MANAGER_BUILD_AS_COMPONENT # partition manager build use as component
    LIBS
        seos_core_api
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
        seos_timestamp
        config
    SOURCES
        "components/time_server/src/main.c"
        "components/time_server/src/time_server.c"
        "seos_timestamp/timestamp.c"
    C_FLAGS
        -Wall
        -Werror
)

# local log server
DeclareCAmkESComponent(APP01
    INCLUDES
        seos_log/include
        seos_log/lib/include
        seos_timestamp
        config
    SOURCES
        "components/app01/src/main.c"

        "seos_log/lib/src/log_databuffer.c"
        "seos_log/lib/src/log_emitter_callback.c"
        "seos_log/lib/src/log_emitter.c"
        "seos_log/lib/src/log_filter.c"

        "seos_timestamp/timestamp.c"
    C_FLAGS
        -Wall
        -Werror
        -DLOGGER_CONFIG_H_FILE=${LOGGER_CONFIG_H_FILE}
    LIBS
        seos_libs
)

# app 02
DeclareCAmkESComponent(APP02
    INCLUDES
        seos_log/include
        seos_log/lib/include
        seos_timestamp
        config
    SOURCES
        "components/app02/src/main.c"

        "seos_log/lib/src/log_databuffer.c"
        "seos_log/lib/src/log_emitter_callback.c"
        "seos_log/lib/src/log_emitter.c"
        "seos_log/lib/src/log_filter.c"

        "seos_timestamp/timestamp.c"
    C_FLAGS
        -Wall
        -Werror
        -DLOGGER_CONFIG_H_FILE=${LOGGER_CONFIG_H_FILE}
    LIBS
        seos_libs
)

# app 03
DeclareCAmkESComponent(APP03
    INCLUDES
        seos_log/include
        seos_log/lib/include
        seos_timestamp
        config
    SOURCES
        "components/app03/src/main.c"

        "seos_log/lib/src/log_databuffer.c"
        "seos_log/lib/src/log_emitter_callback.c"
        "seos_log/lib/src/log_emitter.c"
        "seos_log/lib/src/log_filter.c"

        "seos_timestamp/timestamp.c"
    C_FLAGS
        -Wall
        -Werror
        -DLOGGER_CONFIG_H_FILE=${LOGGER_CONFIG_H_FILE}
    LIBS
        seos_libs
)

# app 0x
DeclareCAmkESComponent(APP0x
    INCLUDES
        seos_log/include
        seos_log/lib/include
        seos_timestamp
        config
    SOURCES
        "components/app0x/src/main.c"

        "seos_log/lib/src/log_databuffer.c"
        "seos_log/lib/src/log_emitter_callback.c"
        "seos_log/lib/src/log_emitter.c"
        "seos_log/lib/src/log_filter.c"

        "seos_timestamp/timestamp.c"

        "seos_log/lib/src/log_file_client.c"
        "seos_log/lib/src/log_file_client_callback.c"
    C_FLAGS
        -Wall
        -Werror
        -DLOGGER_CONFIG_H_FILE=${LOGGER_CONFIG_H_FILE}
    LIBS
        seos_libs
)

# PARTITION_MANAGER #
DeclareCAmkESComponent(PARTITION_MANAGER
    SOURCES
        "components/partition_manager/src/main.c"
    C_FLAGS
        -Wall
        -Werror
        -DSEOS_PM_CONFIG_H_FILE=${SEOS_PM_CONFIG_H_FILE}
        -DSEOS_PARTITION_MANAGER_BUILD_AS_COMPONENT # partition manager build use as component
    LIBS
        seos_libs
        seos_partition_manager # provides sources and headers
)

# Component UartDrv #
DeclareCAmkESComponent(UartDrv
    SOURCES
        "components/Uart/src/UartDrv.c"
        "components/Uart/src/qemu_uart.c"
    C_FLAGS
        -Wall
        -Werror
)

# Component ChanMux #
DeclareCAmkESComponent(ChanMux
    SOURCES
        "components/ChanMux/src/instance/ChanMux_Instance.c"
    C_FLAGS
        -Wall
        -Werror
    LIBS
        seos_core_api
        seos_libs
)

DeclareCAmkESRootserver(demo_logserver.camkes)
GenerateCAmkESRootServer()
GenerateSimulateScript()
