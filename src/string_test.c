#include <stdio.h>
#include "dynamic_string.h"
int main(void)
{
    DynamicString str;

    string_init(&str);

    string_set(&str, "Dead");
    string_append(&str, "bolt");

    printf("String: %s\n", str.data);
    printf("Length: %zu\n", str.length);
    printf("Capacity: %zu\n", str.capacity);

    string_free(&str);

    return 0;
}
