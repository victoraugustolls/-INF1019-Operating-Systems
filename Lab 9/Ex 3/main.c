#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "Queue.h"

#define NUMTHREADS 2
#define LIMIT 64

Queue* q;
int notBusy = 1;

void *produtor(void *threadid) {
	for (int i = 0; i < LIMIT;)
	{
		sleep(1);
		if(queueNotFull(q) && notBusy) {
			notBusy = 0;
			int r = rand()%40;
			enqueue(q, r);
			printf("Produtor adicionou: %d\n", r);
			notBusy = 1;
			i++;
		}
		else {
			printf("Fila cheia / ocupada!\n");
		}
	}
	pthread_exit(NULL);
}

void *consumidor(void *threadid) {
	for (int i = 0; i < LIMIT;)
	{
		sleep(2);
		if(queueNotEmpty(q) && notBusy) {
			notBusy = 0;
			int r = dequeue(q);
			printf("Consumidor consumiu: %d\n", r);
			notBusy = 1;
			i++;
		}
		else {
			printf("Fila vazia / ocupada!\n");
		}
	}
	pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
	q = newQueue();

	pthread_t threads[NUMTHREADS];

	pthread_create(&threads[0], NULL, produtor, (void *)0);
	pthread_create(&threads[1], NULL, consumidor, (void *)1);

	for (int i = 0; i < 2; i++)
	{
		pthread_join(threads[i], NULL);
	}
	
	return 0;
}
