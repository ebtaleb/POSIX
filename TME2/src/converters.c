/**** converters.c ****/

#include <string.h>
#include <sys/wait.h>
#include <converters.h>

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

int main(int argc, char **argv)
{
	int i;
    pid_t pid_fils;
    conversion_message req, *res;

    if (argc < 2) {
        printf("nope\n");
        exit(EXIT_FAILURE);
    }

    req.pid_sender = getpid();
	strcpy(req.currency, argv[1]);
    req.amount = atof(argv[2]);

    printf("Conversion for: %s %.3f\n", req.currency, req.amount);

    for (i = 0; i < 5; i++) {
        pid_fils = fork();

        if (pid_fils == 0) {

            res = malloc(sizeof(conversion_message));
            res->pid_sender = -1;
            res->currency[0] = '\0';
            res->amount = 0.0;

            handle_conversion_request(req, res, i);
            display_results(res);
            return EXIT_SUCCESS;
        }

    }

    for (i = 0; i < 5; i++) {
        wait(0);
    }

    printf("End of conversion\n");

	return 0;
}


/* Traite une requete de conversion.
	request :	requete
	result :	resultat de la conversion
	conv_nb :	numero de conversion ; definit la devise cible ; cf. identifiants en debut de fichier. */
void handle_conversion_request(conversion_message request, conversion_message *result, int conv_nb)
{
    if (request.pid_sender > 0) {
        result->pid_sender = getpid();
        strcpy(result->currency, determine_currency(conv_nb));
        result->amount = convert(request.currency, result->currency, request.amount);
    }
}

/* Affiche le contenu d'une reponse a une requete de conversion. (exos 2 et 3)
	result :			resultat de conversion */
void display_results(conversion_message *result)
{
    printf("%s %.3f\n", result->currency, result->amount);
}
