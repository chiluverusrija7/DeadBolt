#include <stdio.h>
#include "vector.h"

int main(void)
{
    Vector vector;

    vector_init(&vector);

    vector_push(&vector, "ls");
    vector_push(&vector, "-l");
    vector_push(&vector, "/home");

    printf("Vector size: %zu\n", vector.size);

    for (size_t i = 0; i < vector.size; i++)
    {
        printf("Element %zu: %s\n", i, vector.data[i]);
    }

    vector_free(&vector);

    return 0;
}
