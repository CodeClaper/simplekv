#ifndef __DICT_H__
#define __DICT_H__

#include <stdbool.h>
#include <stdint.h>

typedef enum NodeTag {
    NODE_INT,
    NODE_BOOL,
    NODE_FLOAT,
    NODE_DOUBLE,
    NODE_LONG,
    NODE_STRING
} NodeTag;

typedef struct Node {
    NodeTag type;
    union {
        int     intVal;
        bool    booVal;
        float   floatVal;
        double  doubleVal;
        int64_t longVal;
        char    *strVal;
    } value;
} Node;

#endif
