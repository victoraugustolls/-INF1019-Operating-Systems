#include <stdio.h>
#include <stdlib.h>

#include <signal.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

#include <string.h>

#include "Queue.h"

#define timeSlice 1
#define limit 7

// To run:
// ./a.out ./Test1 ./Test2

static Queue* waitQueue;

static ProcessRR mainList[20];
static ProcessRR current;
static int size = 0;


int runningFlag = 1;
int count = 0;
int count2 = 0;
int count3 = 0;

int cmpfunc (const void * a, const void * b)
{
	ProcessRR* p1 = (ProcessRR*)a;
	ProcessRR* p2 = (ProcessRR*)b;

	return ( p2->priority - p1->priority );
}

void sortList()
{
	qsort(mainList, size, sizeof(ProcessRR), cmpfunc);
}

void printa()
{

	printf("Lista de prontos:\n");
	printf("Inicio -> (");
	for (int i = 0; i < size; i ++)
	{
		printf("## %s | %d ##", mainList[i].name, mainList[i].priority);
	}
	printf(") <- Fim");
	printf("\n");
	printf("Lista em espera:\n");
	print(waitQueue);
	printf("\n");
}

void alarmHandler(int sinal)
{
	// stop current.
	if(current.pid != -1) {
		kill(current.pid, SIGSTOP);
	}

	if (current.priority < 7)
	{
		current.priority ++;
	}
	mainList[size] = current; size++;

	sortList();

	// Swap current.
	current = mainList[size-1]; size--;

	// Resume current.
	if(current.pid != -1) {
		kill(current.pid, SIGCONT);
		printf("Corrente -- Processo: %s | Prioridade: %d\n", current.name, current.priority);
		printa();
	}
	count++;
	alarm(timeSlice);

	if (!isQueueEmpty(waitQueue))
	{
		printf("Tirando do wait processo: %s\n", front(waitQueue).name);
		mainList[size] = front(waitQueue); size++;
		sortList();
		// enqueue(mainQueue, front(waitQueue));
		dequeue(waitQueue);
		//printf("WaitQueue esta vazia apos tirar front? %d\n",isQueueEmpty(waitQueue));
	}
}


void childHandler(int sinal)
{
	int status;
	pid_t pid = waitpid(-1, &status, WUNTRACED | WCONTINUED | WNOHANG);

	if (WIFEXITED(status) == 1) // exited normally
	{
		count2++;
		printf("Processo %s terminou de rodar!\n", current.name);
		if(size == 0)
		{
			runningFlag = 0;
			alarm(0);
		}
		else {
			current = mainList[size-1]; size--;
			kill(current.pid, SIGCONT);
			printf("Corrente -- Processo: %s | Prioridade: %d\n", current.name, current.priority);
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
	}
	enqueue(waitQueue, current);

	// Swap current.
	current = mainList[size-1]; size--;

	// Resume current.
	if(current.pid != -1) {
		kill(current.pid, SIGCONT);
		printf("Corrente -- Processo: %s | Prioridade: %d\n", current.name, current.priority);
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

	waitQueue = newQueue();

	for(int i = 1; i < argc; i+=2) {
		printf("%s, %d\n", argv[i], atoi(argv[i+1]));

		// ProcessRR p = {fork(), atoi(argv[i+1]), argv[i]};
		ProcessRR p;// = {fork(), 0, argv[i]};
		strcpy(p.name, argv[i]);
		p.priority = atoi(argv[i+1]);
		p.pid = fork();
		if(!p.pid) // child process
	 	{
			if(execv(argv[i], NULL) < 0) {
				printf("Erro ao executar o programa %s.\n", argv[i]);
				exit(0);
			}
		}
		kill(p.pid, SIGSTOP);
		mainList[size] = p; size++;
	}
	sortList();

	current = mainList[size-1]; size--;

	kill(current.pid, SIGCONT);
	printf("Corrente -- Processo: %s | Prioridade: %d\n", current.name, current.priority);
	printa();
	alarm(timeSlice);


	while(runningFlag)
	{
		// sleep(1);
		// printf("%d, %d\n", count, count2);
		// printf("Current pid: %d / priority: %d\n", current.pid, current.priority);
	}

	printf("Done!\n");
}
