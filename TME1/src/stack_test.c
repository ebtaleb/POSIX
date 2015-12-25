#include <stack.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef STACK_SIZE
#define STACK_SIZE 13
#endif

int main(int argc, char** argv) {

	char* mywords[4] = {"The Force", "is", "strong", "within this Jedi"};
	int i;
	int nb;
	
	init_stack();
	
	for(i = 0; i < 4; i++)
		push(mywords[i]);
	
	nb = size();
	
	printf("La taille de la pile est %d\n", nb);
	
	for(i = 0; i < nb; i++)
		printf("%s \n", (char*)pop());

    printf("taille max de la pile: %d\n", STACK_SIZE);

	return EXIT_SUCCESS;
}
