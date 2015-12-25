#define _POSIX_SOURCE 1

#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int
main(int argc, char **argv)
{

    if (argc < 2) {
        printf("nope\n");
        exit(1);
    }

    int arg = atoi(argv[1]);
    char num[5];

    if (arg >= 0) {
        printf("%d iteration de pid %d\n", arg, getpid());
        snprintf(num, 5, "%d", arg-1);
        execlp(argv[0], argv[0], num, NULL);
    }

    return 0;

}

