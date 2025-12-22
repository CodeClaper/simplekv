#ifndef __ERR_H__
#define __ERR_H__

#define UNEXPECTED_VALUE(EXPR) do {                    \
    fprintf(stderr,                                    \
      "%s:%d: %lld: unexpected value for " #EXPR "\n", \
      __FILE__, __LINE__, (long long)(EXPR)            \
    );                                                 \
    abort();                                           \
  } while (0)


void ThrowErr(char *msg);

#endif 
