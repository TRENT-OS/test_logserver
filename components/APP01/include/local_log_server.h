#pragma once


#include "log_consumer.h"
#include "log_format.h"

#if !defined (Debug_Config_PRINT_2_LOG_SERVER)
    #include "log_emitter.h"
#endif


bool
Local_log_server_init(void *buffer,
                      Log_filter_t *log_filter_server,
                      Log_filter_t *log_filter_client,
                      Log_format_t *log_format,
                      const char *name);


void
Local_log_server_delete(void);
