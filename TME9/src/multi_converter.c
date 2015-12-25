/**** multi_converter.c ****/

#include "converters.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/mman.h>
#include <ctype.h>
#include <semaphore.h>

#define _POSIX_SOURCE 1

/* Converts from any to EUR */
double convertfrom(char* input_currency, double input_amount) {
	if (strcmp(input_currency, "GBP") == 0)
		return (input_amount / GBP);
	else if (strcmp(input_currency, "USD") == 0)
		return (input_amount / USD);
	else if (strcmp(input_currency, "JPY") == 0)
		return (input_amount / JPY);
	else if (strcmp(input_currency, "CNY") == 0)
		return (input_amount / CNY);
	else if (strcmp(input_currency, "EUR") == 0)
		return input_amount;
	else {
		perror("Unknown currency");
		exit(1);
	}
}


/* Converts from EUR to any */
double convertto(char* target_currency, double input_amount) {
	if (strcmp(target_currency, "GBP") == 0)
		return (input_amount * GBP);
	else if (strcmp(target_currency, "USD") == 0)
		return (input_amount * USD);
	else if (strcmp(target_currency, "JPY") == 0)
		return (input_amount * JPY);
	else if (strcmp(target_currency, "CNY") == 0)
		return (input_amount * CNY);
	else if (strcmp(target_currency, "EUR") == 0)
		return input_amount;
	else {
		perror("Unknown currency");
		exit(1);
	}
}


/* Converts from any to any */
double convert(char* input_currency, char* target_currency, double input_amount) {
	return convertto(target_currency, convertfrom(input_currency, input_amount));
}


/* Determines a currency string identifier, given its integer identifier */
char* determine_currency(int curr_nb) {
	switch(curr_nb) {
		case EUR_CONV: return "EUR";
		case GBP_CONV: return "GBP";
		case USD_CONV: return "USD";
		case JPY_CONV: return "JPY";
		case CNY_CONV: return "CNY";
		default: perror("Unknown currency"); exit(1);
	}
}

/* Processes a conversion request */
void handle_conversion_request(conversion_message request, conversion_message *result, int curr_nb) {
	result->pid_sender = getpid();
	strcpy(result->currency, determine_currency(curr_nb));
	result->amount = convert(request.currency, result->currency, request.amount);
}

char **g_argv;

sem_t *r_mutex;
sem_t *w_mutex;

void *comm = NULL;

void
int_handler()
{
    exit(0);
}


int main(int argc, char **argv)
{
    if (argc < 2) {
        printf("nope\n");
        exit(EXIT_FAILURE);
    }

    int shm_fd = -1;

    if ((shm_fd = shm_open(argv[1], O_RDWR | O_CREAT, 0600)) == -1) {
            perror("shm_open");
            exit(1);
    }

    if (ftruncate(shm_fd, sizeof(results_array)+sizeof(conversion_message)) == -1) {
            perror("ftruncate");
            exit(1);
    }

    if ((comm = mmap(NULL, sizeof(results_array)+sizeof(conversion_message), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0))== MAP_FAILED) {
            perror("mmap");
            exit(1);
    }

    r_mutex = sem_open("r_mut", O_CREAT, 0644, 1);
    w_mutex = sem_open("w_mut", O_CREAT, 0644, 0);

    sigset_t sig_server;
    struct sigaction action_on_int;

    sigfillset(&sig_server);
    sigdelset(&sig_server, SIGINT);
    sigprocmask(SIG_BLOCK, &sig_server, 0);

    action_on_int.sa_mask = sig_server;
    action_on_int.sa_flags = 0;
    action_on_int.sa_handler = int_handler;
    sigaction(SIGINT, &action_on_int, 0);

    atexit(int_handler);

    int i;
    pid_t pid = -1;
    for (i = 0; i < NB_CONVERTERS && ( (pid = fork()) > 0 ); i++) {
        if (pid == 0) {
            return EXIT_SUCCESS;
        } else {
        }
    }

	return 0;
}
