#include <stdio.h>
#include <string.h>

#include "parser.h"
#include "executor.h"
#include "signals.h"

#define MAX_INPUT 1024

int main(void)
{
    /*
     * Install shell signal handlers.
     *
     * The shell ignores Ctrl-C and Ctrl-Z,
     * while child processes restore default behavior.
     */
    install_signal_handlers();

    char input[MAX_INPUT];

    printf("DEADBOLT\n");
    printf("Type 'exit' to quit.\n");

    while (1)
    {
        printf("deadbolt> ");
        fflush(stdout);

        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "exit") == 0)
        {
            break;
        }

        if (strlen(input) == 0)
        {
            continue;
        }

        Pipeline pipeline;

        pipeline_init(&pipeline);

        if (!parse_pipeline(input, &pipeline))
        {
            printf("Parsing failed.\n");
            pipeline_free(&pipeline);
            continue;
        }

        int status;

        /*
         * Single command:
         *     ls
         *
         * Multiple commands:
         *     ls | grep c | wc -l
         */
        if (pipeline.count == 1)
        {
            status = execute_command(&pipeline.commands[0]);
        }
        else
        {
            status = execute_pipeline(&pipeline);
        }

        printf("Exit status: %d\n", status);

        pipeline_free(&pipeline);
    }

    return 0;
}
