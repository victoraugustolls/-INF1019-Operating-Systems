#include <stdio.h>
#include <stdlib.h>

#include <signal.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

#include <string.h>

#include "Queue.h"

#define timeSlice 1

// To run:
// ./a.out ./Test1 ./Test2

static Queue* mainQueue;
static Queue* waitQueue;
static ProcessRR current;


int runningFlag = 1;
int count = 0;
int count2 = 0;
int count3 = 0;

void printa()
{
	printf("Fila de prontos:\n");
	print(mainQueue);
	printf("Fila em espera:\n");
	print(waitQueue);
	printf("\n");
}

void alarmHandler(int sinal)
{
	if (!isQueueEmpty(waitQueue))
	{
		printf("Tirando do wait processo %s\n", front(waitQueue).name);
		enqueue(mainQueue, front(waitQueue));
		dequeue(waitQueue);
		// printf("WaitQueue esta vazia apos tirar front? %d\n",isQueueEmpty(waitQueue));
	}

	// stop current.
	if(current.pid != -1) {
		kill(current.pid, SIGSTOP);
		enqueue(mainQueue, current);
	}

	// Swap current.
	current = front(mainQueue);
	dequeue(mainQueue);

	// Resume current.
	if(current.pid != -1) {
		kill(current.pid, SIGCONT);
		printf("Corrente -- Processo: %s\n", current.name);
		printa();
	}
	count++;
	alarm(timeSlice);
}


void childHandler(int sinal)
{
	int status;
	pid_t pid = waitpid(-1, &status, WUNTRACED | WCONTINUED | WNOHANG);

	if (WIFEXITED(status)) // exited normally
	{
		count2++;
		printf("Processo %s terminou de rodar!\n", current.name);
		current = front(mainQueue);
		dequeue(mainQueue);
		if(current.pid == -1 && isQueueEmpty(waitQueue))
		{
			runningFlag = 0;
			alarm(0);
		}
		else {
			if (current.pid != -1) {
				kill(current.pid, SIGCONT);
				printf("Corrente -- Processo: %s\n", current.name);
			}
			printa();
			alarm(timeSlice);
		}
	}
}

void ioStartedHandler(int sinal)
{
	printf("IO Started Handler\n");

	// stop current.
	if(current.pid != -1) {
		printf("Dando stop no processo %s\n", current.name);
		kill(current.pid, SIGSTOP);
		enqueue(waitQueue, current);
	}

	// Swap current.
	current = front(mainQueue);
	dequeue(mainQueue);

	// Resume current.
	if(current.pid != -1) {
		kill(current.pid, SIGCONT);
		printf("Corrente -- Processo: %s\n", current.name);
		printa();
	}

	alarm(timeSlice);
}

int main(int argc, char const *argv[])
{
	if (argc < 2) {
		printf("Faltam parametros para executar!\n");
		exit(1);
	}

	signal(SIGCHLD, childHandler);
	signal(SIGALRM, alarmHandler);
	signal(SIGUSR1, ioStartedHandler);

	mainQueue = newQueue();
	waitQueue = newQueue();

	for(int i = 1; i < argc; i++) {
		printf("%s\n", argv[i]);

		ProcessRR p;// = {fork(), 0, argv[i]};
		strcpy(p.name, argv[i]);
		p.priority = 0;
		p.pid = fork();
		if(!p.pid) // child process
	 	{
			if(execv(argv[i], NULL) < 0) {
				printf("Erro ao executar o programa %s.\n", argv[i]);
				exit(0);
			}
		}
		kill(p.pid, SIGSTOP);
		enqueue(mainQueue, p);
	}

	// printf("Primeiro processo que será executado: %d\n", front(mainQueue).pid);
	current = front(mainQueue);
	dequeue(mainQueue);

	kill(current.pid, SIGCONT);
	printf("Corrente -- Processo: %s\n", current.name);
	printa();

	alarm(timeSlice);

	while(runningFlag)
	{
		// printf("Current pid: %d\n", current.pid);
		// sleep(1);
		//printf("%d, %d\n", count, count2);
	}

	printf("Done!\n");
}
