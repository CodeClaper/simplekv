
#include <stdio.h>
#include <stdlib.h>

/* Throw an error. */
void ThrowErr(char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}
