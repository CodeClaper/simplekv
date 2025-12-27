#include <stdbool.h>

typedef enum ST_FLAG {
    ST_SUCCESS = 0,
    ST_INVALID,
    ST_OVERFLOW,
    ST_OUTRANGE
} ST_FLAG;

char *Trim(char *s);
bool StrIsEmpty(char *s);
bool StartWith(char *str, const char *prefix);
ST_FLAG StrToLong(char *val, long *ret);
ST_FLAG StrToFloat(char *val, float *ret);
