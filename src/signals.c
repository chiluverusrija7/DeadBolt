#define _POSIX_C_SOURCE 200809L

#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

#include "signals.h"

/*
 * Reap children that have already terminated.
 *
 * WNOHANG makes sure the handler never blocks.
 */
static void sigchld_handler(int signo)
{
    (void)signo;

    while (waitpid(-1, NULL, WNOHANG) > 0)
    {
        /* Reap all available terminated children. */
    }
}

/*
 * Install signal behavior for the shell.
 *
 * The shell itself ignores Ctrl-C and Ctrl-Z.
 * Child processes restore the default behavior.
 */
void install_signal_handlers(void)
{
    struct sigaction sa;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = SIG_IGN;

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTSTP, &sa, NULL);

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    sa.sa_handler = sigchld_handler;

    sigaction(SIGCHLD, &sa, NULL);
}

/*
 * Restore normal signal behavior in a child process.
 */
void restore_child_signals(void)
{
    struct sigaction sa;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = SIG_DFL;

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTSTP, &sa, NULL);
    sigaction(SIGCHLD, &sa, NULL);
}
