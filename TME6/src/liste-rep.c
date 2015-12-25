#define _POSIX_SOURCE 1

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

int
main(int argc, char *argv[])
{
    char *rep = argc == 1 ? "." : argv[1];

    if (access(rep, F_OK) == 0) {

        DIR *rep_list = opendir(rep);
        struct dirent *curr_rep = NULL;

        while ((curr_rep = readdir(rep_list)) != NULL) {
            printf("%s\n", curr_rep->d_name);
        }

        closedir(rep_list);

    } else {
        mkdir(rep, S_IRUSR | S_IWUSR);
    }

    return 0;
}

