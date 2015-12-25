/**** spy.c ****/

#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#define _POSIX_SOURCE 1

#define BUFSZ 1024
#define WDSZ 24
#define NBUF 10

int usr_interrupt = 0;

void
parent_to_child()
{
    usr_interrupt = 1;
}

int cut_down(char *buf, char **cmd) {
	char* s = strtok(buf, " \n");
	int i = 0;
	while (s != NULL) {
		cmd[i] = (char*) malloc(sizeof(char) * WDSZ);
		strcpy(cmd[i], s);
		s = strtok(NULL, " \n");
		i++;
	}
	cmd[i] = NULL;
	return i;
}


int
main(void)
{

    int or = 1, pid, command_size, i;
    int pid_tab[NBUF];
	char buf[BUFSZ];
	char *command[WDSZ];

    int command_counter = 0;

    sigset_t mask, oldmask;

    struct sigaction action;
    /* Set up the mask of signals to temporarily block. */
    sigemptyset (&mask);
    sigaddset(&mask, SIGUSR1);


    action.sa_mask = mask;
    action.sa_flags = 0;
    action.sa_handler = parent_to_child;

    sigaction(SIGUSR1, &action, 0);



    while (or > 0)
    {
		for (i = 0; i < BUFSZ ; i++)
			buf[i] = '\0';
        if ((or = read(STDIN_FILENO, buf, BUFSZ)) == -1)
			perror("read");
		else if (or > 0) {
			command_size = cut_down(buf, (char**)command);

            pid = fork();

            if (pid > 0) {
                pid_tab[command_counter] = pid;
                command_counter++;
                printf("read %d commands\n", command_counter);
            }

            if (pid == 0) {
                /* Wait for a signal to arrive. */
                sigprocmask(SIG_BLOCK, &mask, &oldmask);
                while (!usr_interrupt)
                    sigsuspend (&oldmask);
                sigprocmask (SIG_UNBLOCK, &mask, NULL);
                usr_interrupt = 0;

                if (command_size == 2) {
                    execlp(command[0], command[0], command[1], NULL);
                } else {
                    execvp(command[0], command);
                }
            }

            if (pid > 0 && command_counter != NBUF) {
                continue;
            } else {
                for (i = 0; i < NBUF; i++) {
                    kill(pid_tab[i], SIGUSR1);
                    wait(0);
                }

                command_counter = 0;
            }
		}
    }

	return 0;
}
