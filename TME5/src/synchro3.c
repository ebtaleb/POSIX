#define _POSIX_SOURCE 1

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

void calcul_1 ( ) {
	int i;
    printf("calcul_1 fait par pid %d\n", getpid());
	for (i = 0; i < 1E8; i++);
    printf("done\n");
    // envoie SIGUSR1 au ppid, SIGUSR2 a pid_fils[0] si pid_fils[1] est défini
}

void calcul_2 () {
	int i;
    printf("calcul_2 fait par pid %d\n", getpid());
	for (i = 0; i < 1E8; i++);
    printf("done\n");
}

int main (void) {
	int i=0;
	pid_t pid_fils[2] = {-1, -1};

    // arbre
	while ((i<2) && ((pid_fils[i] = fork())!=0))
		i++;

    // le 2eme fils crée connait l'identifiant du 1er
    //if (pid_fils[0] > 0 && pid_fils[1] == 0)
    //
    //
    // ou alors on laisse le pere coordonner
    // if (pid_fils[0] > 0 && pid_fils[1] > 0)

	calcul_1 ();
    // attend les 2 autres procs, doit recevoir deux signaux
    // sigsuspend(SIGUSR1) sigsuspend(SIGUSR2)
	calcul_2 ();
	printf ("fin processus %d \n",i);

	return EXIT_SUCCESS;
}
