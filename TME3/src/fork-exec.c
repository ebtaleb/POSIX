#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>

#define _POSIX_SOURCE 1

int
main(void) {

	struct timeval tv;

	printf("Resultat de la commande 'date'\n");

    pid_t pid_fils = fork();

    if (pid_fils == -1) {
        perror("nope");
        return EXIT_FAILURE;
    }

	if (pid_fils == 0) {
        execlp("date", "date", "+%s", NULL);
        return EXIT_SUCCESS;
    }

    wait(0);

	printf("Comparaison avec la fonction 'gettimeofday'\n");

	gettimeofday(&tv, NULL);
	printf("%d\n", (int)tv.tv_sec);

	return 0;
}
