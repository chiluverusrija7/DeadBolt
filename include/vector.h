#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

typedef struct
{
    char **data;
    size_t size;
    size_t capacity;
} Vector;

void vector_init(Vector *vector);
int vector_push(Vector *vector, const char *value);
void vector_free(Vector *vector);

#endif
