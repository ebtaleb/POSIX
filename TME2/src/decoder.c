/**** decoder.c ****/

#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>


#define _POSIX_SOURCE 1


int magicsq[3][3] = {{4, -1, 8}, {-1, -1, -1}, {2, -1, 6}};


int check() {
	int i, j, sums[8];
	for (i = 0; i < 8; i++)
		sums[i] = 0;
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			sums[i] += magicsq[i][j];               // sums of rows
			sums[i+3] += magicsq[j][i];             // sums of columns
			if (i == j) sums[6] += magicsq[i][j];       // sum of first diag
			if ((i+j) == 2) sums[7] += magicsq[i][j];   // sum of second diag
		}
	}
	int result = 1;
	i = 1;
	while ((i < 8) && (result == 1)) {
		if (sums[0] != sums[i])
			result = 0;
		i++;
	}
	return result;
}


void display() {
	int i, j;
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++)
			printf("  %d", magicsq[i][j]);
		printf("\n");
	}
	printf("\n");
}


int solve(int x) {
	/* CODE A EXECUTER PAR CHAQUE PROCESSUS FILS */
    int counter = 0;
    int is_a_magic_sq;
    int b, c, d, e;

    magicsq[0][1] = x;

    for (b = 0; b < 10; b++) {
        for (c = 0; c < 10; c++) {
            for (d = 0; d < 10; d++) {
                for (e = 0; e < 10; e++) {
                    magicsq[1][0] = b;
                    magicsq[1][1] = c;
                    magicsq[1][2] = d;
                    magicsq[2][1] = e;

                    if ( (is_a_magic_sq = check()) == 1) {
                        display();
                        counter++;
                        continue;
                    }
                }
            }
        }
    }

    return counter;

}

int main(void)
{
	/* LANCEMENT ET ATTENTE DES PROCESSUS FILS */
    int i, status = 0;
    int total = 0;
    pid_t pid_fils;

    for (i = 0; i < 10; i++) {
        pid_fils = fork();

        if (pid_fils == -1) {
            perror("fork");
            exit(1);
        } else if (pid_fils == 0) {
            printf("FILS : pid %d, pid pere : %d\n", getpid(), getppid());
            printf("i = %d\n", i);
            solve(i);
            return solve(i);
        } else {
        }
    }

    for (i = 0; i < 10; i++) {
        wait(&status);
        total += status;
        status = 0;
    }

    printf("nombre total de solutions : %d\n", total);

    return EXIT_SUCCESS;


}
