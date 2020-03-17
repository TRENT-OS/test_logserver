#include "custom_format.h"
#include <stdio.h>

static bool _Log_format_convert(Format_t *self, Log_info_t *log_info);

static const FormatT_Vtable _custom_format_vtable = {
    .dtor    = Log_format_dtor,
    .convert = _Log_format_convert,
    .print   = Log_format_print
};

Log_format_t custom_format =
{
    .vtable = &_custom_format_vtable
};

static bool
_Log_format_convert(Format_t *self, Log_info_t *log_info)
{
    CHECK_SELF(self);

    if(NULL == log_info)
    {
        return false;
    }

    size_t msg_len = strlen(log_info->log_databuffer.log_message);

    if(msg_len > LOG_MESSAGE_LENGTH)
    {
        msg_len = LOG_MESSAGE_LENGTH;
    }

    sprintf(
        (((Log_format_t *)self))->buffer,
        "%-*s       CUSTOM_FORMAT      %.*s\n",
            LOG_ID_AND_NAME_LENGTH, log_info->log_id_and_name,
            (int)msg_len, log_info->log_databuffer.log_message);

    return true;
}
