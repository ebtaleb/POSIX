#include <list.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

typedef struct afifo{
    struct list_type *dll_fifo;
	int number_of_elements;
} fifo;

fifo f;

int
init_queue()
{
    f.dll_fifo = malloc(sizeof(struct list_type));
    assert(f.dll_fifo != NULL);
    init_list(f.dll_fifo);
    f.number_of_elements = 0;
	return 0;
}

int
queue(void* element)
{
    insert_head(f.dll_fifo, element);
    f.number_of_elements++;
}

void*
dequeue()
{
	void* result = NULL;
	if (f.number_of_elements > 0) {
		result = extract_tail(f.dll_fifo);
        f.number_of_elements--;
	}
	return result;
}

int
size()
{
	return f.number_of_elements;
}

