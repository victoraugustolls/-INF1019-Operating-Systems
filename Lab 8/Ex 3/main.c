#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define NUMTHREADS 2

int global;

void *contCrescente(void *threadid) {
	for (int i = 0; i < 20; i++)
	{
		global++;
		printf("T1 (crescente): %d / valor global: %d\n", i+1, global);
		sleep(1);
	}
	pthread_exit(NULL);
}

void *contDecrescente(void *threadid) {
	for (int i = 30; i > 0; i--)
	{
		global++;
		printf("T2 (decrescente): %d / valor global: %d\n", i, global);
		sleep(2);
	}
	pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
	global = 0;

	pthread_t threads[NUMTHREADS];

	printf("valor inicial global: %d\n", global);

	pthread_create(&threads[0], NULL, contCrescente, (void *)0);
	pthread_create(&threads[1], NULL, contDecrescente, (void *)1);

	for (int i = 0; i < 2; i++)
	{
		pthread_join(threads[i], NULL);
	}
	
	return 0;
}
