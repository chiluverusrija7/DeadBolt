#include <stdio.h>
#include "parser.h"

int main(void)
{
    Pipeline pipeline;

    pipeline_init(&pipeline);

    if (!parse_pipeline("ls -l /home | grep srija", &pipeline))
    {
        printf("Parsing failed.\n");
        return 1;
    }

    printf("Number of commands: %zu\n", pipeline.count);

    for (size_t i = 0; i < pipeline.count; i++)
    {
        printf("\nCommand %zu:\n", i + 1);

        for (size_t j = 0;
             j < pipeline.commands[i].arguments.count;
             j++)
        {
            printf("  %s\n",
                   pipeline.commands[i].arguments.tokens[j]);
        }
    }

    pipeline_free(&pipeline);

    return 0;
}
