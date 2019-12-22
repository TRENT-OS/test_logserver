/**
 * @addtogroup LibDebug
 * @{
 *
 * @file Debug.h
 *
 * @brief Debugging facility macros and functions
 *
 * @author Carmelo Pintaudi
 *
 * Copyright (C) 2019, Hensoldt Cyber GmbH
 */
#if !defined(DEBUG_H)
#define DEBUG_H

/* Includes ------------------------------------------------------------------*/

#if !defined(DEBUG_CONFIG_H_FILE)
#   error a configuration file must be provided! See Debug_Config.h.example
#else
#   define Debug_XSTR(d)    Debug_STR(d)
#   define Debug_STR(d)     #d
#   include Debug_XSTR(DEBUG_CONFIG_H_FILE)
#endif

#include <stdbool.h>
#include <stddef.h>

/* Exported types ------------------------------------------------------------*/

#if !defined (Debug_Config_STANDARD_ASSERT) && \
    !defined (Debug_Config_DISABLE_ASSERT)  && \
    !defined (Debug_Config_ENABLE_ASSERT)
#error make a choice!
#endif

#if !defined (Debug_Config_ASSERT_SELF_PTR) && \
    !defined (Debug_Config_NO_ASSERT_SELF_PTR)
#error make a choice!
#endif

/* Macro hacks ---------------------------------------------------------------*/

#define Debug_STRINGIZE(x) Debug_STRINGIZE2(x)
#define Debug_STRINGIZE2(x) #x

/* Exported constants --------------------------------------------------------*/

#define Debug_LOG_LEVEL_NONE    0
#define Debug_LOG_LEVEL_ASSERT  1
#define Debug_LOG_LEVEL_FATAL   2
#define Debug_LOG_LEVEL_ERROR   3
#define Debug_LOG_LEVEL_WARNING 4
#define Debug_LOG_LEVEL_INFO    5
#define Debug_LOG_LEVEL_DEBUG   6
#define Debug_LOG_LEVEL_TRACE   7

/* Exported macro ------------------------------------------------------------*/
/* Print macro ---------------------------------------------------------------*/

#define Debug_PRINT(LEVEL, ...)  \
    Debug_PRINT__(LEVEL, __VA_ARGS__)

#if !defined (Debug_Config_PRINT_2_LOG_SERVER)
    #include <stdio.h>
    #define Debug_PRINT__(LEVEL, ...)   \
        do                              \
        {                               \
            printf(__VA_ARGS__);        \
            printf("\n");               \
        } while (0)
#else
    #include "log_emitter.h"

    #define Debug_PRINT__(LEVEL, ...)               \
    do                                              \
    {                                               \
        Log_emitter_emit_log(LEVEL, __VA_ARGS__);   \
    } while (0)
#endif

/* Build message -------------------------------------------------------------*/

#define Debug_LOG_MSG(...) \
    Debug_LOG_MSG_WITH_FILE_LINE(__VA_ARGS__)

#if defined(Debug_Config_LOG_WITH_FILE_LINE)
#   define Debug_LOG_MSG_WITH_FILE_LINE(...)    \
         __FILE__ ":" Debug_STRINGIZE(__LINE__) \
         ": " __VA_ARGS__
#else
#   define Debug_LOG_MSG_WITH_FILE_LINE(...) \
        __VA_ARGS__
#endif

/* Check constants------------------------------------------------------------*/

#if !defined(Debug_Config_LOG_LEVEL)  \
    || Debug_Config_LOG_LEVEL == Debug_LOG_LEVEL_NONE

#    if !defined(Debug_LOG)
#        define Debug_LOG(LEVEL, LEVEL_STR, ...)
#    endif
#    if !defined(Debug_LOG_ASSERT)
#        define Debug_LOG_ASSERT(...)
#    endif
#    if !defined(Debug_LOG_FATAL)
#        define Debug_LOG_FATAL(...)
#    endif
#    if !defined(Debug_LOG_ERROR)
#        define Debug_LOG_ERROR(...)
#    endif
#    if !defined(Debug_LOG_WARNING)
#        define Debug_LOG_WARNING(...)
#    endif
#    if !defined(Debug_LOG_INFO)
#        define Debug_LOG_INFO(...)
#    endif
#    if !defined(Debug_LOG_DEBUG)
#        define Debug_LOG_DEBUG(...)
#    endif
#    if !defined(Debug_LOG_TRACE)
#        define Debug_LOG_TRACE(...)
#    endif

#else

#    if defined(Debug_Config_INCLUDE_LEVEL_IN_MSG)
#    define Debug_LOG(LEVEL, LEVEL_STR, ...) \
            Debug_PRINT(LEVEL, LEVEL_STR ": " Debug_LOG_MSG(__VA_ARGS__))
#    else
#    define Debug_LOG(LEVEL, LEVEL_STR, ...) \
            Debug_PRINT(LEVEL, Debug_LOG_MSG(__VA_ARGS__))
#    endif

#    if Debug_Config_LOG_LEVEL < Debug_LOG_LEVEL_TRACE
#        define Debug_LOG_TRACE(...)
#    else
#        define Debug_LOG_TRACE(...)\
             Debug_LOG(Debug_LOG_LEVEL_TRACE, "TRACE", __VA_ARGS__)
#    endif
#    if Debug_Config_LOG_LEVEL < Debug_LOG_LEVEL_DEBUG
#        define Debug_LOG_DEBUG(...)
#    else
#        define Debug_LOG_DEBUG(...)\
             Debug_LOG(Debug_LOG_LEVEL_DEBUG, "DEBUG", __VA_ARGS__)
#    endif
#    if Debug_Config_LOG_LEVEL < Debug_LOG_LEVEL_INFO
#        define Debug_LOG_INFO(...)
#    else
#        define Debug_LOG_INFO(...)\
             Debug_LOG(Debug_LOG_LEVEL_INFO, "INFO", __VA_ARGS__)
#    endif
#    if Debug_Config_LOG_LEVEL < Debug_LOG_LEVEL_WARNING
#        define Debug_LOG_WARNING(...)
#    else
#        define Debug_LOG_WARNING(...)\
             Debug_LOG(Debug_LOG_LEVEL_WARNING, "WARNING", __VA_ARGS__)
#    endif
#    if Debug_Config_LOG_LEVEL < Debug_LOG_LEVEL_ERROR
#        define Debug_LOG_ERROR(...)
#    else
#        define Debug_LOG_ERROR(...)\
             Debug_LOG(Debug_LOG_LEVEL_ERROR, "ERROR", __VA_ARGS__)
#    endif
#    if Debug_Config_LOG_LEVEL < Debug_LOG_LEVEL_FATAL
#        define Debug_LOG_FATAL(...)
#    else
#        define Debug_LOG_FATAL(...)\
             Debug_LOG(Debug_LOG_LEVEL_FATAL, "FATAL", __VA_ARGS__)
#    endif
#    if Debug_Config_LOG_LEVEL < Debug_LOG_LEVEL_ASSERT
#        define Debug_LOG_ASSERT(...)
#    else
#        define Debug_LOG_ASSERT(...)\
             Debug_LOG(Debug_LOG_LEVEL_ASSERT, "ASSERT", __VA_ARGS__)
#    endif

#endif

/* Assert checks -------------------------------------------------------------*/

#define Debug_ASSERT_PRINTFLN(C__) \
do\
{\
    if (!(C__)){\
        Debug_LOG_ASSERT("Function: %s: %s", __func__, Debug_STRINGIZE(C__));\
    } \
    Debug_ASSERT(C__);\
}\
while(0)

#if defined(Debug_Config_ENABLE_ASSERT)

#   if defined (Debug_Config_ASSERT_PRINT_LINE)
#       define Debug_ASSERT(C__) \
            Debug_ASSERT_PRINTFLN(C__)
#   else
#       define Debug_ASSERT(C__) Debug_assert(C__)
#   endif

#   define Debug_STATIC_ASSERT(C__) enum { assert_static__ = 1/(C__) }

#elif defined(Debug_Config_STANDARD_ASSERT)

#   include <assert.h>
#   define Debug_ASSERT(C__)        assert(C__)
#   define Debug_STATIC_ASSERT(C__) static_assert(C__, "Debug_STATIC_ASSERT")

#else

#define Debug_ASSERT(C__) { bool ok__ = (C__); ok__ = ok__; }
#define Debug_STATIC_ASSERT(C__)

#endif

#if defined (Debug_Config_ASSERT_SELF_PTR)
#   define Debug_ASSERT_SELF(self)  Debug_ASSERT(self != NULL);
#else
#   define Debug_ASSERT_SELF(self)
#endif

/* Exported functions ------------------------------------------------------- */

#ifdef Debug_Config_ENABLE_ASSERT

#include "compiler.h"

void
Debug_failedAssert();

INLINE void
Debug_assert(bool cond)
{
    if (!cond)
    {
        Debug_failedAssert();
    }
}

#endif

#endif /* <HEADER_UNIQUE_SYMBOL_H> */
///@}
