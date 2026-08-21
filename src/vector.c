#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"

void vector_init(Vector *vector)
{
    vector->data = NULL;
    vector->size = 0;
    vector->capacity = 0;
}

int vector_push(Vector *vector, const char *value)
{
    if (vector->size == vector->capacity)
    {
        size_t new_capacity =
            (vector->capacity == 0) ? 4 : vector->capacity * 2;

        char **new_data =
            realloc(vector->data, new_capacity * sizeof(char *));

        if (new_data == NULL)
        {
            return 0;
        }

        vector->data = new_data;
        vector->capacity = new_capacity;
    }

    vector->data[vector->size] = malloc(strlen(value) + 1);

    if (vector->data[vector->size] == NULL)
    {
        return 0;
    }

    strcpy(vector->data[vector->size], value);
    vector->size++;

    return 1;
}

void vector_free(Vector *vector)
{
    for (size_t i = 0; i < vector->size; i++)
    {
        free(vector->data[i]);
    }

    free(vector->data);

    vector->data = NULL;
    vector->size = 0;
    vector->capacity = 0;
}
