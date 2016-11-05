#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define NUMTHREADS 2

void *contCrescente(void *threadid) {
	for (int i = 0; i < 20; i++)
	{
		printf("T1 (crescente): %d\n", i+1);
		sleep(1);
	}
	pthread_exit(NULL);
}

void *contDecrescente(void *threadid) {
	for (int i = 30; i > 0; i--)
	{
		printf("T2 (decrescente): %d\n", i);
		sleep(2);
	}
	pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{

	pthread_t threads[NUMTHREADS];

	pthread_create(&threads[0], NULL, contCrescente, (void *)0);
	pthread_create(&threads[1], NULL, contDecrescente, (void *)1);

	for (int i = 0; i < 2; i++)
	{
		pthread_join(threads[i], NULL);
	}
	
	return 0;
}
