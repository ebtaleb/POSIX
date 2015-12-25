#define _POSIX_SOURCE 1

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>

int
main(int argc, char *argv[])
{
    if (argc < 2) {
        perror("nope");
        exit(-1);
    }

    struct stat stat_file;
    stat(argv[2], &stat_file);

    if (S_ISDIR(stat_file.st_mode)) {
        perror("Error : argv[2] must be a file");
        exit(1);
    }

    switch (argv[1][0]) {

        case 'E':
        case 'e':
            unlink(argv[2]);
            break;

        case 'R':
        case 'r':
            assert(argc == 4);
            rename(argv[2], argv[3]);
            break;

        case 'C':
        case 'c':
            assert(argc == 4);
            char mode = argv[3][0];

            if (mode == 'R' || mode == 'r') {
                chmod(argv[2], S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
                break;
            }

            if (mode == 'W' || mode == 'w') {
                chmod(argv[2], S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
                break;
            }
    }

    return 0;
}

