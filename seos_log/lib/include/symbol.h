#pragma once


// common makro hacks
#define x_2_cat(a,b)                    a##b

#define VAR_EXPAND_2_VAR(x,y)           x_2_cat(x,y)


#define STR_(x)                         #x
#define STR(x)                          STR_(x)
#define STR_2_VAR(x,y)                  (STR_(x) STR_(y))


#define OVERFLOW( a, b, max)            (((b) > 0) && ((a) > (max) - (b)))


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

#define LOG_ID_NAME_LENGTH              512


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
