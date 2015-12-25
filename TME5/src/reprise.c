#define _POSIX_SOURCE 1

#define NFIN 10

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <setjmp.h>

int sig_number;

void
sig_handler(int sig)
{
    // nouveau masque ou on ignore tous les signaux, on sauve l'ancien

    sig_number = sig;

    // on restaure l'ancien masque
}

int
main(void)
{
    int counter = 0;
    jmp_buf a_buffer;
    sigset_t sig;
    sigfillset(&sig);             // si c'était emptyset, il n'y aurait pas de signaux pendants
    sigdelset(&sig, SIGALRM);
    sigdelset(&sig, SIGINT);

    sigprocmask(SIG_SETMASK, &sig, NULL);
    printf("temporisateur armé\n");
    sleep(5);

    while (counter < NFIN) {
        sigsuspend(&sig);
        if (sigsetjmp(a_buffer, 1) == 0) {

        } else {

        }

        sigpending(&sig);

        if (sigismember(&sig, SIGINT)) {
            counter++;
        }

        if (sigismember(&sig, SIGALRM)) {
            break;
        }

        siglongjmp(a_buffer, 1);
    }


    return 0;
}

