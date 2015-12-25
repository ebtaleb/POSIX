#define _POSIX_SOURCE 1

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int
main(int argc, char *argv[])
{
    if (argc < 3) {
        printf("nope\n");
        exit(1);
    }

    struct stat file_stat;
    stat(argv[1], &file_stat);

    if (!S_ISREG(file_stat.st_mode)) {
        exit(1);
    }

    FILE *f1 = fopen(argv[1], "r");
    char word1[20];
    int occur = 0;

    while (fscanf(f1, "%s", word1) != EOF) {

        if (strcmp(word1, argv[2]) == 0) {
            occur++;
            printf("Occurence de %s a l'offset %lu\n", argv[2], ftell(f1));
        }
    }

    fclose(f1);

    printf("Nombre d'occurences de %s : %d\n", argv[2], occur);

    return 0;
}

