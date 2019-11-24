cmake_minimum_required(VERSION 3.7.2)

seos_set_config_file(config/config.h)

seos_use_libs(
    SEOS_LIBS
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
        "components/LOG_SERVER/src/main.c"

        "seos_log/lib/src/log_databuffer.c"
        "seos_log/lib/src/listT.c"
        "seos_log/lib/src/log_consumer.c"
        "seos_log/lib/src/consumer_chain.c"
        "seos_log/lib/src/log_filter.c"
        "seos_log/lib/src/log_format.c"

        "seos_timestamp/timestamp.c"
    C_FLAGS
        -Wall
        -Werror
    LIBS
        seos_libs
)

# time server
DeclareCAmkESComponent(TIME_SERVER
    INCLUDES
        components/TIME_SERVER/include
        seos_timestamp
        config
    SOURCES
        "components/TIME_SERVER/src/main.c"
        "components/TIME_SERVER/src/time_server.c"
        "seos_timestamp/timestamp.c"
    C_FLAGS
        -Wall
        -Werror
)

# local log server
DeclareCAmkESComponent(APP01
    INCLUDES
        components/APP01/include
        seos_log/include
        seos_log/lib/include
        seos_timestamp
        config
    SOURCES
        "components/APP01/src/local_log_server.c"
        "components/APP01/src/main.c"

        "seos_log/lib/src/log_databuffer.c"
        "seos_log/lib/src/log_emitter.c"
        "seos_log/lib/src/log_consumer.c"
        "seos_log/lib/src/log_filter.c"
        "seos_log/lib/src/log_format.c"

        "seos_timestamp/timestamp.c"
    C_FLAGS
        -Wall
        -Werror
    LIBS
        seos_libs
)

# log server
DeclareCAmkESComponent(APP02
    INCLUDES
        seos_log/include
        seos_log/lib/include
        seos_timestamp
        config
    SOURCES
        "components/APP02/src/main.c"

        "seos_log/lib/src/log_databuffer.c"
        "seos_log/lib/src/log_emitter.c"
        "seos_log/lib/src/log_filter.c"

        "seos_timestamp/timestamp.c"
    C_FLAGS
        -Wall
        -Werror
    LIBS
        seos_libs
)

# log server
DeclareCAmkESComponent(APP03
    INCLUDES
        seos_log/include
        seos_log/lib/include
        seos_timestamp
        config
    SOURCES
        "components/APP03/src/main.c"

        "seos_log/lib/src/log_databuffer.c"
        "seos_log/lib/src/log_emitter.c"
        "seos_log/lib/src/log_filter.c"

        "seos_timestamp/timestamp.c"
    C_FLAGS
        -Wall
        -Werror
    LIBS
        seos_libs
)

DeclareCAmkESRootserver(demo_logserver.camkes)
GenerateCAmkESRootServer()
GenerateSimulateScript()
