#define _POSIX_SOURCE 1

#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int
main(void)
{

    sigset_t sig_proc;
    struct sigaction action;

    sigemptyset(&sig_proc);
    sigaddset(&sig_proc, SIGINT);
    sigaddset(&sig_proc, SIGQUIT);
    sigprocmask(SIG_BLOCK, &sig_proc, NULL);

    action.sa_mask = sig_proc;
    action.sa_flags = 0;
    action.sa_handler = SIG_IGN;

    sigaction(SIGINT, &action, 0);

    sleep(5);

    sigprocmask(SIG_UNBLOCK, &sig_proc, NULL);

    sigpending(&sig_proc);

    if (sigismember(&sig_proc, SIGHUP)) {
        printf("SIGHUP pendant\n");
    }
    if (sigismember(&sig_proc, SIGINT)) {
        printf("SIGINT pendant\n");
    }
    if (sigismember(&sig_proc, SIGQUIT)) {
        printf("SIGQUIT pendant\n");
    }
    if (sigismember(&sig_proc, SIGILL)) {
        printf("SIGILL pendant\n");
    }
    if (sigismember(&sig_proc, SIGABRT)) {
        printf("SIGABRT pendant\n");
    }
    if (sigismember(&sig_proc, SIGKILL)) {
        printf("SIGKILL pendant\n");
    }
    if (sigismember(&sig_proc, SIGSEGV)) {
        printf("SIGSEGV pendant\n");
    }
    if (sigismember(&sig_proc, SIGPIPE)) {
        printf("SIGPIPE pendant\n");
    }
    if (sigismember(&sig_proc, SIGALRM)) {
        printf("SIGALRM pendant\n");
    }
    if (sigismember(&sig_proc, SIGTERM)) {
        printf("SIGTERM pendant\n");
    }
    if (sigismember(&sig_proc, SIGUSR1)) {
        printf("SIGUSR1 pendant\n");
    }
    if (sigismember(&sig_proc, SIGUSR2)) {
        printf("SIGUSR2 pendant\n");
    }
    if (sigismember(&sig_proc, SIGCHLD)) {
        printf("SIGCHLD pendant\n");
    }
    if (sigismember(&sig_proc, SIGCONT)) {
        printf("SIGCONT pendant\n");
    }
    if (sigismember(&sig_proc, SIGSTOP)) {
        printf("SIGSTOP pendant\n");
    }
    if (sigismember(&sig_proc, SIGTSTP)) {
        printf("SIGTSTP pendant\n");
    }


    printf("done\n");

    return EXIT_SUCCESS;
}

