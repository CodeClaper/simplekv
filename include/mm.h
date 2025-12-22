#include <stdlib.h>

#define instance(_type_) ((_type_ *)MemAlloc(sizeof(_type_)))

void OOM();
void *MemAlloc(size_t size);
void MemFree(void *ptr);
