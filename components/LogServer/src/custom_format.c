#include "custom_format.h"
#include <stdio.h>

static bool _Log_format_convert(
    OS_LoggerAbstractFormat_Handle_t *self,
    OS_LoggerDataBuffer_info *log_info);

static const OS_LoggerAbstractFormat_vtable_t _custom_format_vtable = {
    .dtor    = OS_LoggerFormat_dtor,
    .convert = _Log_format_convert,
    .print   = OS_LoggerFormat_print
};

OS_LoggerFormat_Handle_t custom_format =
{
    .vtable = &_custom_format_vtable
};

static
bool
_Log_format_convert(
    OS_LoggerAbstractFormat_Handle_t *self,
    OS_LoggerDataBuffer_info *log_info)
{
    OS_Logger_CHECK_SELF(self);

    if(NULL == log_info)
    {
        return false;
    }

    size_t msg_len = strlen(log_info->log_databuffer.log_message);

    if(msg_len > OS_Logger_MESSAGE_LENGTH)
    {
        msg_len = OS_Logger_MESSAGE_LENGTH;
    }

    sprintf(
        (((OS_LoggerFormat_Handle_t *)self))->buffer,
        "%-*s       CUSTOM_FORMAT         %.*s\n",
            OS_Logger_ID_AND_NAME_LENGTH, log_info->log_id_and_name,
            (int)msg_len, log_info->log_databuffer.log_message);

    return true;
}
