#define _POSIX_SOURCE 1

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

int
main(void)
{
    int i, j, k, p;

    printf("PERE : pid %d\n", getpid());

    for (i = 0; i < 3; i++) {

        if ( (p = fork()) == 0 ) {
            printf("pid %d, i = %d\n", getpid(), i);
            j = 0;

            while ( j < i && ( (p = fork()) == 0 ) ) {
                j++;
            }

            if (p == 0) {
                printf("pid %d, j = %d\n", getpid(), j);
            } else {
                wait(0);
            }

            exit(j);
        } else {
            for (k = 0; k < 3; k++) wait(0);
        }
    }

    printf("PERE : pid %d\n", getpid());

    return 0;
}

