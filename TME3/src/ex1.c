#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>
#include <sys/wait.h>

#define _POSIX_SOURCE 1

#ifndef N_FILS
#define N_FILS 9
#endif

int
main(void)
{
    int i, j;
    pid_t pid;
    pid_t *pid_array = calloc(N_FILS+1, sizeof(pid_t));

    pid_array[0] = getpid();
    printf("pere : pid %d\n", pid_array[0]);

    for (i = 1; ((i < N_FILS+1) && (pid = fork()) == 0); i++){

        if (pid == -1) {
            perror("nope");
            return EXIT_FAILURE;
        }

        pid_array[i] = getpid();

        if (pid == 0 && i == N_FILS) {
            for (j = 1; j < N_FILS+1; j++) {
                printf("fils %d : pid %d\n", j, pid_array[j]);
            }
        }
    }

    wait(0);

    return EXIT_SUCCESS;
}
