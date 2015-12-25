#define _POSIX_SOURCE 1

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

FILE *fd_file = NULL;
int stdout_copy;

void
rediriger_stdout(const char *file)
{
    fd_file = fopen(file, "w");
    stdout_copy = dup(1);
    dup2(fileno(fd_file), 1);
}

void
restaurer_stdout()
{
    dup2(stdout_copy, 1);
    fclose(fd_file);
}

int
main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("nope\n");
        exit(1);
    }

    printf("avant la redirection \n");
    rediriger_stdout(argv[1]);
    printf("après la redirection \n");
    restaurer_stdout();
    printf("après avoir restauré stdout \n");

    return EXIT_SUCCESS ;
}

