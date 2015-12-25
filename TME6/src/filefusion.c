#define _POSIX_SOURCE 1

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int
main(int argc, char *argv[])
{
    if (argc < 4) {
        perror("nope");
        exit(1);
    }

    struct stat file_stat;
    stat(argv[1], &file_stat);

    if (!S_ISREG(file_stat.st_mode)) {
        exit(1);
    }

    stat(argv[2], &file_stat);

    if (!S_ISREG(file_stat.st_mode)) {
        exit(1);
    }

    if (access(argv[3], F_OK) == -1) {
        creat(argv[3], S_IRUSR | S_IWUSR);
    }

    FILE *f1 = fopen(argv[1], "r");
    FILE *f2 = fopen(argv[2], "r");
    FILE *f3 = fopen(argv[3], "w");

    char word1[20], word2[20];

    while (fscanf(f1, "%s", word1) != EOF && fscanf(f2, "%s", word2) != EOF) {
        fprintf(f3, "%s ", word1);
        fprintf(f3, "%s ", word2);
    }

    fclose(f1);
    fclose(f2);
    fclose(f3);

    return 0;
}

