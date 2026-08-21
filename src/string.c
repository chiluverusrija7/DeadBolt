#include <stdlib.h>
#include <string.h>
#include "dynamic_string.h"

void string_init(DynamicString *str)
{
    str->data = NULL;
    str->length = 0;
    str->capacity = 0;
}

int string_set(DynamicString *str, const char *value)
{
    size_t length = strlen(value);

    if (length + 1 > str->capacity)
    {
        size_t new_capacity = length + 1;

        char *new_data = realloc(str->data, new_capacity);

        if (new_data == NULL)
        {
            return 0;
        }

        str->data = new_data;
        str->capacity = new_capacity;
    }

    strcpy(str->data, value);
    str->length = length;

    return 1;
}

int string_append(DynamicString *str, const char *value)
{
    size_t append_length = strlen(value);
    size_t required = str->length + append_length + 1;

    if (required > str->capacity)
    {
        size_t new_capacity =
            (str->capacity == 0) ? required : str->capacity * 2;

        while (new_capacity < required)
        {
            new_capacity *= 2;
        }

        char *new_data = realloc(str->data, new_capacity);

        if (new_data == NULL)
        {
            return 0;
        }

        str->data = new_data;
        str->capacity = new_capacity;
    }

    memcpy(str->data + str->length, value, append_length + 1);
    str->length += append_length;

    return 1;
}

void string_free(DynamicString *str)
{
    free(str->data);

    str->data = NULL;
    str->length = 0;
    str->capacity = 0;
}
