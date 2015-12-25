#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{

    int i, pid;
    int length;
    char *arg_vect[7];

    arg_vect[0] = "gcc";
    arg_vect[1] = "-o";
    arg_vect[3] = "-c";
    arg_vect[5] = NULL;

    for (i = 1; (i < argc && ((pid = fork()) == 0)); i++) {
        arg_vect[2] = strdup(argv[i]);
        arg_vect[4] = strdup(argv[i]);   // nom du fichier en entree
        length = strlen(arg_vect[4]);
        arg_vect[2][length-1] = 'o';

        //pid = fork();

        if (pid == 0) {
            execvp(arg_vect[0], arg_vect);
        } else {
            wait(0);
            free(arg_vect[2]);
            free(arg_vect[4]);
        }
    }


    arg_vect[2] = "affiche";
    for (i = 3; i < 3+argc-1; i++) {
        arg_vect[i] = strdup(argv[i-2]);
    }
    arg_vect[3+argc-1] = NULL;
    execvp(arg_vect[0], arg_vect);

    return 0;
}

