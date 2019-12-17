#include "log_format.h"
#include <stdio.h>



// foreward declaration
static bool _Log_format_convert(FormatT_t *self, Log_info_t *log_info);
static void _Log_format_print(FormatT_t *self);



static const FormatT_Vtable Log_format_vtable = {
    .dtor    = Log_format_dtor,
    .convert = _Log_format_convert,
    .print   = _Log_format_print
};



bool
Log_format_ctor(Log_format_t *self)
{
    bool nullptr = false;

    ASSERT_SELF__(self);

    if(nullptr){
        // Debug_printf
        return false;
    }

    self->vtable = &Log_format_vtable;

    return true;
}



void
Log_format_dtor(FormatT_t *self)
{
    memset(self, 0, sizeof (Log_format_t));
}



static bool
_Log_format_convert(FormatT_t *self, Log_info_t *log_info)
{
    bool nullptr = false;

    ASSERT_SELF__(self);

    if(nullptr){
        // Debug_printf
        return false;
    }

    if(log_info == NULL)
        return false;

    Log_format_t *log_format = (Log_format_t *)self;
    char *buf = log_format->buffer;
    Time_t tm;

    Timestamp_get_time(&log_info->timestamp, 0, &tm);

    unsigned long msg_len = strlen(log_info->log_databuffer.log_message);

    if(msg_len > LOG_MESSAGE_LENGTH)
        msg_len = LOG_MESSAGE_LENGTH;

    sprintf(buf, "%*s %02d.%02d.%04d-%02d:%02d:%02d%*d%*d %-*s",
                FORMAT_ID_LENGTH, log_info->log_id_name,
                tm.day, tm.month, tm.year, tm.hour, tm.min, tm.sec,
                FORMAT_LOG_LEVEL_SERVER_LENGTH, log_info->log_databuffer.log_level_srv,
                FORMAT_LOG_LEVEL_CLIENT_LENGTH, log_info->log_databuffer.log_level_client,
                (int)msg_len, log_info->log_databuffer.log_message);

    return true;
}



static void
_Log_format_print(FormatT_t *self)
{
    bool nullptr = false;

    ASSERT_SELF__(self);

    if(nullptr){
        // Debug_printf
        return;
    }

    Log_format_t *log_format = (Log_format_t *)self;
    char *buf = log_format->buffer;

    printf("%s\n", buf);
}
