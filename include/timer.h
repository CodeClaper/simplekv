#ifndef __TIMER_H__
#define __TIMER_H__

#include <bits/types/struct_timeval.h>
#include <stdint.h>
#include <time.h>

typedef enum TLevel {
    SECOND,
    MILLISECOND,
    MICROSECOND,
    NANOSECOND
} TLevel;


int64_t GetCurrentTimestamp(TLevel level);
char *GetCurrentDatetime(TLevel level);
char *FormatTime(char *format, time_t t);
char* GetCurrentTimestampFormat(char *format);
double TimeSpan(struct timeval end_time, struct timeval start_time);

#endif
