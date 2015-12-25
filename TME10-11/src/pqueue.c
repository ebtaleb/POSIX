#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 32

typedef char msg_data[32];

struct data {
	msg_data m;
	int prno;
	int ord;
	size_t length;
};

struct pque {
	struct data *d[MAX];
	int front;
	int rear;
};

void initpque (struct pque *);
void add (struct pque *, struct data);
void delete ( struct pque *pq, struct data *t );
void display_pqueue(struct pque *);

int
main(void)
{
	struct pque q;
	struct data dt, temp;
	int i, j = 0;

	initpque(&q);

	printf("Enter Job description (max 32 chars) and its priority\n");
	printf("Lower the priority number, higher the priority\n");
	printf("Job				Priority\n");

	for (i = 0; i < 5; i++)
	{
		scanf ("%s %d", &dt.m, &dt.prno);
		dt.ord = j++;
		dt.length = strlen(dt.m);
		add(&q, dt);
	}
	printf ( "\n" ) ;


	display_pqueue(&q);
	return 0;
}

/* initialises data members */
void
initpque (struct pque *pq)
{
	int i;

	pq->front = pq->rear = -1 ;
	for (i = 0; i < MAX; i++)
	{	pq->d[i] = malloc(sizeof(struct data));


		pq->d[i]->m[0] = '\0';
		pq->d[i]->prno = pq->d[i]->ord = 0 ;
		pq->d[i]->length = 0 ;
	}
}

/* adds item to the priority queue */
void
add(struct pque *pq, struct data dt)
{
	struct data *temp = NULL;
	int i, j;

	if (pq->rear == MAX - 1)
	{
		printf ( "\nQueue is full." ) ;
		return;
	}

	pq->rear++ ;

	strcpy(pq->d[pq->rear]->m, dt.m);
	pq->d[pq->rear]->prno = dt.prno;
	pq->d[pq->rear]->ord = dt.ord;
	pq->d[pq->rear]->ord = dt.length;

	if (pq->front == -1)
		pq->front = 0 ;

	for (i = pq->front ; i <= pq->rear; i++)
	{
		for (j = i+1; j <= pq->rear; j++)
		{
			if (pq->d[i]->prno < pq->d[j]->prno)
			{
				temp = pq->d[i] ;
				pq->d[i] = pq->d[j] ;
				pq->d[j] = temp ;
			}
			else
			{
				if (pq->d[i]->prno == pq->d[j]->prno)
				{
					if (pq->d[i]->ord < pq->d[j]->ord)
					{
						temp = pq->d[i] ;
						pq->d[i] = pq->d[j];
						pq->d[j] = temp;
					}
				}
			}
		}
	}
}

/* removes item from priority queue */
void
delete ( struct pque *pq, struct data *t )
{
	if ( pq -> front == -1 ) {
		printf ( "\nQueue is Empty.\n" ) ;
		return;
	}


	strcpy(t->m, pq->d[pq->front]->m);
	t->prno = pq->d[pq->front]->prno;
	t->length = pq->d[pq->front]->length;
	t->ord = pq->d[pq->front]->ord;

	strcpy(pq->d[pq->front]->m, '\0');
	pq->d[pq->front]->prno = 0;
	pq->d[pq->front]->length = 0;
	pq->d[pq->front]->ord = 0;

	if ( pq -> front == pq -> rear )
		pq -> front = pq -> rear = -1 ;
	else
		pq -> front++ ;

}

void
display_pqueue(struct pque *pq)
{
	int front = pq->front;
	int rear = pq->rear;

	if ((front == -1) && (rear == -1)) {
		printf("\nQueue is empty");
		return;
	}

	printf ( "Process jobs prioritywise\n" ) ;
	printf ( "Job\tPriority\n" ) ;

	for (; front <= rear; front++) {
		printf("%s\t%d\n", pq->d[front]->m, pq->d[front]->prno);
	}
}
