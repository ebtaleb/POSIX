#include <list.h>
#include <stdlib.h>
#include <stdio.h>

/* Structure de la pile */
typedef struct astack{
    struct list_type *dll_stack;
	int number_of_elements;
} stack;

stack s;

/* Initialisation de la pile */
int init_stack() {
    
    s.dll_stack = malloc(sizeof(struct list_type));
    init_list(s.dll_stack);
    s.number_of_elements = 0;
	return 0;
}


/* Operation d'empilement */
void push(void* element) {
    insert_head(s.dll_stack, element);
    s.number_of_elements++;
}


/*Operation de depilement */
void* pop() {
	void* result = NULL;
	if (s.number_of_elements > 0) {
		result = extract_head(s.dll_stack);
        s.number_of_elements--;
	}
	return result;
}


/* Calcul du nombre d'elements dans la pile */
int size() {
	return s.number_of_elements;
}
