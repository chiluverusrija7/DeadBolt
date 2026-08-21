#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "executor.h"

int execute_command(const Command *command)
{
    if (command == NULL ||
        command->arguments.count == 0)
    {
        return -1;
    }

    char **argv = malloc(
        (command->arguments.count + 1) * sizeof(char *)
    );

    if (argv == NULL)
    {
        perror("malloc");
        return -1;
    }

    for (size_t i = 0;
         i < command->arguments.count;
         i++)
    {
        argv[i] = command->arguments.tokens[i];
    }

    argv[command->arguments.count] = NULL;

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        free(argv);
        return -1;
    }

    if (pid == 0)
    {
        execvp(argv[0], argv);

        perror("execvp");
        free(argv);
        exit(EXIT_FAILURE);
    }

    int status;

    if (waitpid(pid, &status, 0) < 0)
    {
        perror("waitpid");
        free(argv);
        return -1;
    }

    free(argv);

    if (WIFEXITED(status))
    {
        return WEXITSTATUS(status);
    }

    if (WIFSIGNALED(status))
    {
        return 128 + WTERMSIG(status);
    }

    return -1;
}
