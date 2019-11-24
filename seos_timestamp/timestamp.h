#pragma once


#include <stdint.h>
#include <stdbool.h>


typedef struct
{
    uint8_t  sec;
    uint8_t  min;
    uint8_t  hour;
    uint8_t  day;
    uint8_t  month;
    uint16_t year;
} Time_t;


typedef struct
{
    uint64_t timestamp;
} Timestamp_t;


Timestamp_t *
get_instance_Timestamp(void);


void
Timestamp_dtor(void);


bool
Timestamp_create_timestamp(const char *date, const char *time);


bool
Timestamp_get_time(Timestamp_t *t_stamp, uint8_t hours, Time_t *tm);


bool
Timestamp_get_timestamp(Time_t *tm, Timestamp_t *t_stamp);
