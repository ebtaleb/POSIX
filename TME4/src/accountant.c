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
    counter++;
    signal_counter_array[sig]++;
    printf("quack\n");
}

int
main(void)
{

    sigset_t sig_proc;
    struct sigaction action;

    sigfillset(&sig_proc);

    action.sa_mask = sig_proc;
    action.sa_flags = 0;
    action.sa_handler = signal_handler;

    sigaction(SIGINT, &action, 0);

    sigaction(SIGHUP, &action, 0);
    sigaction(SIGALRM, &action, 0);
    sigaction(SIGQUIT, &action, 0);

    sigaction(SIGTERM, &action, 0);

    int i = 0;
    while (signal_counter_array[SIGINT] < MAX_INTR) {
        pause();
    }

    sigpending(&sig_proc);

    for (i = 0; i < 20; i++) {
        switch(i) {
            case SIGHUP: printf("SIGHUP recu : %d\n", signal_counter_array[i]);
                         break;

            case SIGINT: printf("SIGINT recu : %d\n", signal_counter_array[i]);
                         break;
            case SIGQUIT: printf("SIGQUIT recu : %d\n", signal_counter_array[i]);
                         break;
            case SIGILL: printf("SIGILL recu : %d\n", signal_counter_array[i]);
                         break;
            case SIGABRT: printf("SIGABRT recu : %d\n", signal_counter_array[i]);
                         break;
            case SIGTRAP: printf("SIGTRAP recu : %d\n", signal_counter_array[i]);
                         break;
            case SIGKILL: printf("SIGKILL recu : %d\n", signal_counter_array[i]);
                         break;
            case SIGSEGV: printf("SIGSEGV recu : %d\n", signal_counter_array[i]);
                         break;
            case SIGUSR1: printf("SIGUSR1 recu : %d\n", signal_counter_array[i]);
                         break;
            case SIGUSR2: printf("SIGUSR2 recu : %d\n", signal_counter_array[i]);
                         break;
            case SIGPIPE: printf("SIGPIPE recu : %d\n", signal_counter_array[i]);
                         break;
            case SIGALRM: printf("SIGALRM recu : %d\n", signal_counter_array[i]);
                         break;
            case SIGTERM: printf("SIGTERM recu : %d\n", signal_counter_array[i]);
                         break;
            case SIGCHLD: printf("SIGCHLD recu : %d\n", signal_counter_array[i]);
                         break;
            case SIGCONT: printf("SIGCONT recu : %d\n", signal_counter_array[i]);
                         break;
            case SIGSTOP: printf("SIGSTOP recu : %d\n", signal_counter_array[i]);
                         break;
            default:    printf("signal pas gere\n");
                        break;
        }
    }

    printf("nombre total de signaux recus : %d\n", counter);

    return EXIT_SUCCESS;
}

