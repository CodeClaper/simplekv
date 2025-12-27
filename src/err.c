
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Throw an error. */
void ThrowErr(char *msg) {
    fprintf(stderr, "%s:%s", msg, strerror(errno));
    exit(EXIT_FAILURE);
}
