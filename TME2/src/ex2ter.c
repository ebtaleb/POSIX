#define _POSIX_SOURCE 1

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#ifndef N_FILS
#define N_FILS 9
#endif

int
main(void)
{
    pid_t pid_fils;

    int i = N_FILS;
    int status;

    printf("PERE : pid %d\n", getpid());

    while (i > 0) {
        pid_fils = fork();
        i--;

        if (pid_fils == -1) {
            perror("fork");
            exit(1);
        } else if (pid_fils == 0) {
            printf("FILS : pid %d, pid pere : %d\n", getpid(), getppid());
            return EXIT_SUCCESS;
        } else {
        }
    }

    int lastchild = getpid() + N_FILS;
    waitpid(lastchild, &status, 0);

    return EXIT_SUCCESS;
}

