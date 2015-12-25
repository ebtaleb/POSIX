#define _POSIX_SOURCE 1

#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_INTR 100

int counter = 0;
int signal_counter_array[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

void
signal_handler(int sig)
{
    sigset_t mask_set;
    sigset_t old_set;

    sigfillset(&mask_set);
    sigprocmask(SIG_SETMASK, &mask_set, &old_set);

    signal(SIGINT, signal_handler);
    counter++;
    signal_counter_array[sig]++;
    printf("quack\n");
    fflush(stdout);

    sigprocmask(SIG_SETMASK, &old_set, NULL);
}

int
main(void)
{

    struct sigaction action;


    signal(SIGINT, signal_handler);

    int i = 0;
    while (counter < MAX_INTR) {
        pause();
    }


    printf("nombre total de signaux recus : %d\n", counter);

//    for (i = 0; i < 20; i++) {
//        if (signal_counter_array[i] == 0) {
//            continue;
//        } else if (signal_counter_array[i]
//
//SIGHUP
//SIGINT
//SIGQUIT
//SIGILL
//SIGABRT
//SIGFPE
//SIGKILL
//SIGSEGV
//SIGPIPE
//SIGALRM
//SIGTERM
//

    return EXIT_SUCCESS;
}

