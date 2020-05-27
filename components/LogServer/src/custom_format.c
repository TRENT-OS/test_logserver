#include "custom_format.h"
#include <stdio.h>

static OS_Error_t _Log_format_convert(
    OS_LoggerAbstractFormat_Handle_t *self,
    OS_LoggerEntry_t const * const entry);

static const OS_LoggerAbstractFormat_vtable_t _custom_format_vtable = {
    .convert = _Log_format_convert,
    .print   = OS_LoggerFormat_print
};

OS_LoggerFormat_Handle_t custom_format =
{
    .vtable = &_custom_format_vtable
};

static
OS_Error_t
_Log_format_convert(
    OS_LoggerAbstractFormat_Handle_t *self,
    OS_LoggerEntry_t const * const entry)
{
    OS_Logger_CHECK_SELF(self);

    if(NULL == entry)
    {
        return OS_ERROR_INVALID_PARAMETER;
    }

    size_t msg_len = strlen(entry->msg);

    if(msg_len > OS_Logger_MESSAGE_LENGTH)
    {
        msg_len = OS_Logger_MESSAGE_LENGTH;
    }

    OS_LoggerFormat_Handle_t * const log_format =
                                        (OS_LoggerFormat_Handle_t*)self;

    sprintf(
        log_format->buffer,
        "%.*u %-*s       CUSTOM_FORMAT         %.*s\n",

            OS_Logger_ID_LENGTH, entry->consumerMetadata.id,
            OS_Logger_NAME_LENGTH, entry->consumerMetadata.name,

            (int)msg_len, entry->msg);

    return OS_SUCCESS;
}
