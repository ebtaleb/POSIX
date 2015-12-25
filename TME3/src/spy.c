/**** spy.c ****/

#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>


#define _POSIX_SOURCE 1

#define BUFSZ 1024
#define WDSZ 24


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
	char buf[BUFSZ];
	char *command[WDSZ];

    while (or > 0)
    {
		for (i = 0; i < BUFSZ ; i++)
			buf[i] = '\0';
        if ((or = read(STDIN_FILENO, buf, BUFSZ)) == -1)
			perror("read");
		else if (or > 0) {
			command_size = cut_down(buf, (char**)command);

            pid = fork();
            if (pid == 0) {
                execvp(command[0], command);
            } else {
                wait(0);
                i = 0;
                for (i = 0; i < command_size; i++)
                    free(command[i]);
            }
		}
    }

	return 0;
}
