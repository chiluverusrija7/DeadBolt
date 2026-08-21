#ifndef STRING_H
#define STRING_H

#include <stddef.h>

typedef struct
{
    char *data;
    size_t length;
    size_t capacity;
} DynamicString;

void string_init(DynamicString *str);
int string_set(DynamicString *str, const char *value);
int string_append(DynamicString *str, const char *value);
void string_free(DynamicString *str);

#endif
