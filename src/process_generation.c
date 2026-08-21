#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(void)
{
    pid_t pid1, pid2;

    printf("Parent Process: PID = %d\n", getpid());

    pid1 = fork();

    if (pid1 < 0)
    {
        perror("fork");
        return 1;
    }

    if (pid1 == 0)
    {
        printf("Child 1: PID = %d, Parent PID = %d\n",
               getpid(), getppid());

        sleep(2);

        printf("Child 1 completed.\n");
        exit(0);
    }

    pid2 = fork();

    if (pid2 < 0)
    {
        perror("fork");
        return 1;
    }

    if (pid2 == 0)
    {
        printf("Child 2: PID = %d, Parent PID = %d\n",
               getpid(), getppid());

        sleep(1);

        printf("Child 2 completed.\n");
        exit(0);
    }

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    printf("Parent: Both child processes completed.\n");

    return 0;
}
