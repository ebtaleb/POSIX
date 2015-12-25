/**** conversion_client.c ****/

#include "converters.h"
#include <fcntl.h>

#define _POSIX_SOURCE 1

/* Displays the contents of a conversion results message */
void display_results(conversion_message request, results_array results) {
	int i;
	printf("Conversion for: %s %.3f\n", request.currency, request.amount);
	for(i = 0; i < NB_CONVERTERS; i++)
		printf("\t %s %.3f\n", results[i].currency, results[i].amount);
}

int main(int argc, char **argv)
{
    if (argc < 5) {
        printf("nope\n");
        exit(EXIT_FAILURE);
    }

    FILE *request_fd = NULL;
    FILE *response_fd = NULL;

    request_fd = fopen(argv[1], "w");
    if (request_fd == NULL) {
        printf("nope\n");
        exit(EXIT_FAILURE);
    }

    response_fd = fopen(argv[2], "r");
    if (response_fd == NULL) {
        printf("nope2\n");
        exit(EXIT_FAILURE);
    }

	conversion_message req;
    req.pid_sender = getpid();
    strcpy(req.currency, argv[3]);
    req.amount = atof(argv[4]);


    fwrite(&req, sizeof(conversion_message), 1, request_fd);
    fflush(request_fd);
    //close(request_fd);
    //printf("Message client to server sent\n");

	results_array *rep = calloc(1, sizeof(results_array));
    //printf("Avant read client\n");

    fread(rep, sizeof(results_array), 1, response_fd);
    //close(response_fd);
    //printf("Apres read client\n");
    display_results(req, *rep);
    fclose(request_fd);
    fclose(response_fd);

	return 0;
}

