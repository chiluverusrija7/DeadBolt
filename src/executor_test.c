#include <stdio.h>

#include "parser.h"
#include "executor.h"

int main(void)
{
    Pipeline pipeline;

    pipeline_init(&pipeline);

    if (!parse_pipeline("echo Hello", &pipeline))
    {
        printf("Parsing failed.\n");
        return 1;
    }

    int status = execute_command(&pipeline.commands[0]);

    printf("Exit status: %d\n", status);

    pipeline_free(&pipeline);

    return 0;
}
