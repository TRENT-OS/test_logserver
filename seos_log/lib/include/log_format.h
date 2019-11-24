#pragma once


#include "log_databuffer.h"
#include <stdbool.h>


bool
push_log_format(void *buf, Log_info_t *log_info);


void
print_log_format(void *buf);
