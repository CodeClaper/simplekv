#ifndef __SLOG_H__
#define __SLOG_H__

typedef enum LLevel {
    INFO,
    DBUG,
    SUCCS,
    WARN,
    ERROR,
    PANIC
} LLevel;


void slog(LLevel level, const char *format, ...);
void slogRaw(const char *format, ...);

#endif
