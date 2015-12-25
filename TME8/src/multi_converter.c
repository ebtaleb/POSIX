/**** multi_converter.c ****/

#include "converters.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

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

void
do_nothing()
{
}

char **g_argv;

FILE *request_fd;
FILE *response_fd;

int tubeDesc[2] = {-1, -1};

void
int_handler()
{
    printf("closing fds...\n");
    fclose(request_fd);
    fclose(response_fd);

    printf("removing pipes...\n");
    unlink(g_argv[1]);
    unlink(g_argv[2]);

    close(tubeDesc[0]);
    close(tubeDesc[1]);

    exit(0);
}


int main(int argc, char **argv)
{
    if (argc < 3) {
        printf("nope\n");
        exit(EXIT_FAILURE);
    }

    if (pipe(tubeDesc) == -1) {
        printf("nope\n");
        exit(EXIT_FAILURE);
    }

    if (mkfifo(argv[1], S_IRUSR | S_IWUSR) == -1) {
        printf("nope\n");
        exit(EXIT_FAILURE);
    }

    if (mkfifo(argv[2], S_IRUSR | S_IWUSR) == -1) {
        printf("nope\n");
        exit(EXIT_FAILURE);
    }

    // non blocking reads
    int attr = fcntl(tubeDesc[0], F_GETFL);
    attr |= O_NONBLOCK;
    fcntl(tubeDesc[0], F_SETFL, attr);

    sigset_t sig_server;
    struct sigaction action_on_usr1, action_on_int;

    sigemptyset(&sig_server);

    action_on_usr1.sa_mask = sig_server;
    action_on_usr1.sa_flags = 0;
    action_on_usr1.sa_handler = do_nothing;
    sigaction(SIGUSR1, &action_on_usr1, 0);

    action_on_int.sa_mask = sig_server;
    action_on_int.sa_flags = 0;
    action_on_int.sa_handler = int_handler;
    sigaction(SIGINT, &action_on_int, 0);

    atexit(int_handler);

	conversion_message req;
	results_array rep;

    request_fd = fopen(argv[1], "r");
    if (request_fd == NULL) {
        printf("nope\n");
        exit(EXIT_FAILURE);
    }

    response_fd = fopen(argv[2], "w");
    if (response_fd == NULL) {
        printf("nope2\n");
        exit(EXIT_FAILURE);
    }

    printf("avant read server\n");
    fread(&req, sizeof(conversion_message), 1, request_fd);
    printf("apres read server\n");

    printf("P%d> Converting %s %.3f\n", req.pid_sender, req.currency, req.amount);

    int i;
    pid_t pid = -1;
    for (i = 0; i < NB_CONVERTERS && ( (pid = fork()) > 0 ); i++) {
        if (pid == 0) {
            printf("Son of PID %d is %dth converter\n", getpid(), i);
            handle_conversion_request(req, &rep[i], i);
            write(tubeDesc[1], &rep, sizeof(results_array));
            kill(SIGUSR1, getppid());
            return EXIT_SUCCESS;
        } else {
            wait(0);
            //sigsuspend(&sig_server);
            read(tubeDesc[0], &rep, sizeof(results_array));
        }
    }

    fwrite(&rep, sizeof(results_array), 1, response_fd);
    fflush(response_fd);

	return 0;
}
