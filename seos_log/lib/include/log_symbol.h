#pragma once


// common macro hacks
#define x_2_cat(a,b)                    a##b

#define VAR_EXPAND_2_VAR(x,y)           x_2_cat(x,y)


#define STR_(x)                         #x
#define STR(x)                          STR_(x)
#define STR_2_VAR(x,y)                  (STR_(x) STR_(y))


#define OVERFLOW( a, b, max)            (((b) > 0) && ((a) > (max) - (b)))


// include parameter for configuration file
#if !defined(SEOS_LOG_SERVER_CONFIG_H_FILE)
    #error "a configuration file must be provided! See seos_log_server_config.h.example"
#else
    #include STR(SEOS_LOG_SERVER_CONFIG_H_FILE)
#endif


// configuration of log server
#if !defined (DATABUFFER_SIZE)
    #define DATABUFFER_SIZE             4096
#endif

#define LOG_LEVEL_SERVER_OFFSET         0
#define LOG_LEVEL_CLIENT_OFFSET         1
#define LOG_LEVEL_MESSAGE_OFFSET        2

#define LOG_LEVEL_SERVER_LENGTH         2
#define LOG_LEVEL_CLIENT_LENGTH         2
#define LOG_MESSAGE_LENGTH              (DATABUFFER_SIZE - (LOG_LEVEL_SERVER_LENGTH + LOG_LEVEL_CLIENT_LENGTH))

#define LOG_ID_NAME_LENGTH              32


// configure log format string
#define FORMAT_ID_LENGTH                15
#define FORMAT_TIMESTAMP_LENTGH         20
#define FORMAT_LOG_LEVEL_SERVER_LENGTH  2
#define FORMAT_LOG_LEVEL_CLIENT_LENGTH  2
#define FORMAT_LOG_MESSAGE_LENGTH       LOG_MESSAGE_LENGTH
#define FORMAT_BUFFER_SIZE              (FORMAT_ID_LENGTH +                 \
                                         FORMAT_TIMESTAMP_LENTGH +          \
                                         FORMAT_LOG_LEVEL_SERVER_LENGTH +   \
                                         FORMAT_LOG_LEVEL_CLIENT_LENGTH +   \
                                         FORMAT_LOG_MESSAGE_LENGTH)


// macros
#define ASSERT_SELF__(self)             \
    if(self == NULL)                    \
        nullptr = true

#define ASSERT_VTABLE_PARENT__(self)    \
    if(self->vtable == NULL)            \
        nullptr = true

#define ASSERT_VTABLE_CHILD__(self)     \
    if(self->parent.vtable == NULL)     \
        nullptr = true

#define ASSERT_SELF_PARENT(self)        \
    ASSERT_SELF__(self);                \
                                        \
    ASSERT_VTABLE_PARENT__(self)

#define ASSERT_SELF(self)               \
    ASSERT_SELF__(self);                \
                                        \
    if(nullptr == false)                \
        ASSERT_VTABLE_CHILD__(self)
