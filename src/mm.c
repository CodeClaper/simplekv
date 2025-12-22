#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mm.h"

/* Out of memory. */
void OOM() {
    perror("Out of memory.");
    exit(EXIT_FAILURE);
}

/* Allocate memory. */
void *MemAlloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr == NULL) OOM();
    memset(ptr, 0, size);
    return ptr;
}

/* Free memory. */
void MemFree(void *ptr) {
    if (ptr == NULL) return;
    free(ptr);
}
