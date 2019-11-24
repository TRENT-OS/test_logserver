#pragma once


#include "log_consumer.h"

#if !defined (Debug_Config_PRINT_2_LOG_SERVER)
    #include "log_emitter.h"
#endif


bool
Local_log_server_init(void *buffer,
                           uint8_t log_level,
                           const char *name);


void
Local_log_server_delete(void);
