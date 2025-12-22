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

static const char* SLOG_LEVEL_NAME_LIST[] = {
    "INFO", "DEBUG", "SUCCS", "WARN", "ERROR", "PANIC"
};


void slog(LLevel level, const char *format, ...);
void slogRaw(const char *format, ...);

#endif
