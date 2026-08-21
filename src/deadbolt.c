#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT 1024

int main(void)
{
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

        pid_t pid = fork();

        if (pid < 0)
        {
            perror("fork");
            continue;
        }

        if (pid == 0)
        {
            char *args[MAX_INPUT];
            int i = 0;

            char *token = strtok(input, " ");

            while (token != NULL && i < MAX_INPUT - 1)
            {
                args[i++] = token;
                token = strtok(NULL, " ");
            }

            args[i] = NULL;

            execvp(args[0], args);

            perror("execvp");
            exit(EXIT_FAILURE);
        }
        else
        {
            int status;

            waitpid(pid, &status, 0);

            if (WIFEXITED(status))
            {
                printf("Process %d exited with status %d\n",
                       pid,
                       WEXITSTATUS(status));
            }
        }
    }

    return 0;
}
