#define _POSIX_SOURCE 1

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

void
chld_hdler()
{

}

void
mywait()
{
    printf("gotcha\n");
    sigset_t sig;
    sigfillset(&sig);
    sigdelset(&sig, SIGCHLD);

    struct sigaction sigact;
    sigact.sa_mask = sig;
    sigact.sa_handler = chld_hdler;
    sigact.sa_flags = 0;

    sigaction(SIGCHLD, &sigact, 0);
    sigsuspend(&sig);
}

int
main(void)
{

    sigset_t sig_proc;
    sigfillset(&sig_proc);             // si c'était emptyset, il n'y aurait pas de signaux pendants

    sigprocmask(SIG_SETMASK, &sig_proc, NULL);

    //struct sigaction action;
    //action.sa_mask = sig_proc;
    //action.sa_flags = 0;
    //action.sa_handler = mywait;

    //sigaction(SIGCHLD, &action, 0);


    pid_t pid = fork();

    if (pid == 0) {
        printf("je suis le fils\n");
        sleep(5);
        return EXIT_SUCCESS;
    } else {
        mywait();
        printf("fils a été attendu\n");
    }

    return 0;
}
