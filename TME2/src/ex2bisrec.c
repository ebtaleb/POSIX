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

void
make_nchilds(int n)
{
    if (n == 0) {
        return;
    }

    pid_t pid_fils = fork();

    if (pid_fils == -1) {
        perror("fork");
        exit(1);
    } else if (pid_fils == 0) {
        printf("FILS : pid %d, pid pere : %d\n", getpid(), getppid());
    } else {
        make_nchilds(n-1);
        wait(0);
    }

}

int
main(void)
{

    printf("PERE : pid %d\n", getpid());

    make_nchilds(N_FILS);

    return EXIT_SUCCESS;
}
