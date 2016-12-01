#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "Queue.h"

#define NUMTHREADS 4
#define LIMIT 20

Queue* q;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t production_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t consume_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t product = PTHREAD_COND_INITIALIZER;
pthread_cond_t consume = PTHREAD_COND_INITIALIZER;

void* produtor(void* threadid) {
	for (int i = 0; i < LIMIT;)
	{
		sleep(1);
		if(queueNotFull(q)) {
			pthread_mutex_lock(&mutex);
			int r = rand()%40;
			enqueue(q, r);
			printf("Produtor adicionou: %d, thread: %d\n", r, threadid);
			pthread_mutex_unlock(&mutex);
			pthread_cond_signal(&consume);
			i++;
		}
		else {
			printf("Fila cheia, trava thread: %d!\n", threadid);
			pthread_mutex_lock(&production_mutex);
			pthread_cond_wait(&product, &production_mutex);
			pthread_mutex_unlock(&production_mutex);
			printf("Desbloqueado produtor de thread: %d\n", threadid);
		}
	}
	pthread_exit(NULL);
}

void* consumidor(void* threadid) {
	for (int i = 0; i < LIMIT;)
	{
		sleep(2);
		if(queueNotEmpty(q)) {
			pthread_mutex_lock(&mutex);
			int r = dequeue(q);
			printf("Consumidor consumiu: %d, thread: %d\n", r, threadid);
			pthread_mutex_unlock(&mutex);
			pthread_cond_signal(&product);
			i++;
		}
		else {
			printf("Fila vazia, trava thread: %d!\n", threadid);
			pthread_mutex_lock(&consume_mutex);
			pthread_cond_wait(&consume, &consume_mutex);
			pthread_mutex_unlock(&consume_mutex);
			printf("Desbloqueado consumidor de thread: %d\n", threadid);
		}
	}
	pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
	q = newQueue();

	pthread_t threads[NUMTHREADS];

	pthread_create(&threads[0], NULL, produtor, (void *)0);
	pthread_create(&threads[1], NULL, produtor, (void *)1);
	pthread_create(&threads[2], NULL, consumidor, (void *)2);
	pthread_create(&threads[3], NULL, consumidor, (void *)3);

	for (int i = 0; i < NUMTHREADS; i++)
	{
		pthread_join(threads[i], NULL);
	}
	
	return 0;
}
