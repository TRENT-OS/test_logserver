#include "log_format.h"
#include <stdio.h>
#include <string.h>



bool
push_log_format(void *buf, Log_info_t *log_info)
{
    if(buf == NULL || log_info == NULL)
        return false;

    Time_t tm;
    Timestamp_get_time(&log_info->timestamp, 0, &tm);

    unsigned long msg_len = strlen(log_info->log_databuffer.log_message);

    if(msg_len > LOG_MESSAGE_LENGTH)
        msg_len = LOG_MESSAGE_LENGTH;

    sprintf((char *)buf, "%*s %02d.%02d.%04d-%02d:%02d:%02d%*d%*d %-*s",
                FORMAT_ID_LENGTH, log_info->log_id_name,
                tm.day, tm.month, tm.year, tm.hour, tm.min, tm.sec,
                FORMAT_LOG_LEVEL_SERVER_LENGTH, log_info->log_databuffer.log_level_srv,
                FORMAT_LOG_LEVEL_CLIENT_LENGTH, log_info->log_databuffer.log_level_client,
                (int)msg_len, log_info->log_databuffer.log_message);

    return true;
}



void
print_log_format(void *buf)
{
    if(buf == NULL)
        return;

    printf("%s\n", (char *)buf);
}
