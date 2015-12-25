#define _POSIX_SOURCE 1

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void
chld_hdler()
{

}

unsigned int
mysleep(int sec)
{

    // sauvegarder vieux masque, metter nouveau
    sigset_t old_sig_set, new_sig_set;
    sigfillset(&new_sig_set);
    sigdelset(&new_sig_set, SIGALRM);

    sigprocmask(SIG_SETMASK, &new_sig_set, &old_sig_set);

    struct sigaction sigact;
    sigact.sa_mask = new_sig_set;
    sigact.sa_handler = chld_hdler;
    sigact.sa_flags = 0;

    sigaction(SIGALRM, &sigact, 0);
    unsigned int remaining_sec = alarm(sec);
    sigsuspend(&new_sig_set);

    sigact.sa_handler = SIG_DFL;
    sigaction(SIGALRM, &sigact, 0);
    // ! : remettre l'ancien masque
    //

    sigprocmask(SIG_SETMASK, &old_sig_set, NULL);
    return remaining_sec;
}

int
main(void)
{

    sigset_t sig_proc;
    sigfillset(&sig_proc);             // si c'était emptyset, il n'y aurait pas de signaux pendants

    sigprocmask(SIG_SETMASK, &sig_proc, NULL);

    printf("5 secondes de dodo\n");
    mysleep(5);
    printf("ah j\'ai bien dormi\n");
    alarm(1);

    return 0;
}

