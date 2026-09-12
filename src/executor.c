#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

#include "executor.h"

int execute_command(const Command *command)
{
    if (command == NULL || command->arguments.count == 0)
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
        _exit(127);
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

int execute_pipeline(const Pipeline *pipeline)
{
    if (pipeline == NULL || pipeline->count == 0)
    {
        return -1;
    }

    /*
     * For N commands, we need N-1 pipes.
     */
    size_t pipe_count = pipeline->count - 1;

    int (*pipes)[2] = NULL;

    if (pipe_count > 0)
    {
        pipes = malloc(pipe_count * sizeof(int[2]));

        if (pipes == NULL)
        {
            perror("malloc");
            return -1;
        }

        for (size_t i = 0; i < pipe_count; i++)
        {
            if (pipe(pipes[i]) < 0)
            {
                perror("pipe");

                for (size_t j = 0; j < i; j++)
                {
                    close(pipes[j][0]);
                    close(pipes[j][1]);
                }

                free(pipes);
                return -1;
            }
        }
    }

    pid_t *pids = malloc(pipeline->count * sizeof(pid_t));

    if (pids == NULL)
    {
        perror("malloc");

        for (size_t i = 0; i < pipe_count; i++)
        {
            close(pipes[i][0]);
            close(pipes[i][1]);
        }

        free(pipes);
        return -1;
    }

    /*
     * Create one child for each command.
     */
    for (size_t i = 0; i < pipeline->count; i++)
    {
        pid_t pid = fork();

        if (pid < 0)
        {
            perror("fork");

            for (size_t j = 0; j < pipe_count; j++)
            {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            for (size_t j = 0; j < i; j++)
            {
                waitpid(pids[j], NULL, 0);
            }

            free(pids);
            free(pipes);
            return -1;
        }

        if (pid == 0)
        {
            /*
             * If this is not the first command,
             * connect stdin to previous pipe.
             */
            if (i > 0)
            {
                if (dup2(pipes[i - 1][0], STDIN_FILENO) < 0)
                {
                    perror("dup2 stdin");
                    _exit(127);
                }
            }

            /*
             * If this is not the last command,
             * connect stdout to next pipe.
             */
            if (i < pipeline->count - 1)
            {
                if (dup2(pipes[i][1], STDOUT_FILENO) < 0)
                {
                    perror("dup2 stdout");
                    _exit(127);
                }
            }

            /*
             * Close all pipe descriptors in child.
             */
            for (size_t j = 0; j < pipe_count; j++)
            {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            Command *command = &pipeline->commands[i];

            if (command->arguments.count == 0)
            {
                _exit(127);
            }

            char **argv = malloc(
                (command->arguments.count + 1) * sizeof(char *)
            );

            if (argv == NULL)
            {
                perror("malloc");
                _exit(127);
            }

            for (size_t j = 0;
                 j < command->arguments.count;
                 j++)
            {
                argv[j] = command->arguments.tokens[j];
            }

            argv[command->arguments.count] = NULL;

            execvp(argv[0], argv);

            fprintf(
                stderr,
                "deadbolt: %s: %s\n",
                argv[0],
                strerror(errno)
            );

            free(argv);
            _exit(127);
        }

        pids[i] = pid;
    }

    /*
     * Parent closes all pipe descriptors.
     */
    for (size_t i = 0; i < pipe_count; i++)
    {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    int final_status = 0;

    /*
     * Wait for every command.
     */
    for (size_t i = 0; i < pipeline->count; i++)
    {
        int status;

        if (waitpid(pids[i], &status, 0) < 0)
        {
            perror("waitpid");
            final_status = -1;
            continue;
        }

        /*
         * Shell returns the status of the last command.
         */
        if (i == pipeline->count - 1)
        {
            if (WIFEXITED(status))
            {
                final_status = WEXITSTATUS(status);
            }
            else if (WIFSIGNALED(status))
            {
                final_status = 128 + WTERMSIG(status);
            }
        }
    }

    free(pids);
    free(pipes);

    return final_status;
}
