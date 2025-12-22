#include <stdio.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "slog.h"
#include "server.h"
#include "mm.h"
#include "timer.h"

/* Flush log message to disk. */
static void flushlog(char* msg) {
    char log_path[BUFF_SIZE];
    FILE *file;
    char *sys_date;

    sys_date = GetCurrentTimestampFormat("%Y-%m-%d");
    sprintf(log_path, "%s%s.%s", server.logDir, sys_date, "log");
    file = fopen(log_path, "a");
    if (file == NULL) {
        fprintf(stderr, "Try to open log file '%s', error info: %s. \n", 
                log_path, strerror(errno));
        exit(1);
    }

    fputs(msg, file);
    fflush(file);
    fclose(file);
    MemFree(sys_date);
}

/* slog. */
void slog(LLevel level, const char *format, ...) {
    int len;
    va_list ap;

    /* Calculate the len. */
    va_start(ap, format);
    len = vsnprintf(NULL, 0, format, ap);
    if (len <= 0) {
        va_end(ap);
        return;
    }

    len = len + 1;
    char message[len];
    memset(message, 0, len);

    va_start(ap, format);
    vsnprintf(message, len, format, ap);
    va_end(ap);

    /* Only print higher level log. */
    if (level >= server.llevel) {
        char *sys_time = GetCurrentDatetime(MICROSECOND);
        char buff[len + 100];
        sprintf(buff, "[%s][%d][%s]:\t%s\n", 
                sys_time, getpid(), 
                SLOG_LEVEL_NAME_LIST[level], message);
        fprintf(stdout, "%s", buff);
        flushlog(buff);
        MemFree(sys_time);
    }
}

/* slog raw. */
void slogRaw(const char *format, ...) {
    int len;
    va_list ap;

    /* Calculate the len. */
    va_start(ap, format);
    len = vsnprintf(NULL, 0, format, ap);
    if (len <= 0) {
        va_end(ap);
        return;
    }

    len = len + 1;
    char message[len];
    memset(message, 0, len);

    va_start(ap, format);
    vsnprintf(message, len, format, ap);
    va_end(ap);

    /* Only print higher level log. */
    fprintf(stdout, "%s", message);
    flushlog(message);
}

