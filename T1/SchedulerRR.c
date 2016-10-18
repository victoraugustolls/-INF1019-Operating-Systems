#include <stdio.h>
#include <stdlib.h>

#include <signal.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

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



void alarmHandler(int sinal) 
{
	// stop current.
	kill(current.pid, SIGSTOP);
	enqueue(mainQueue, current);

	// Swap current.
	current = front(mainQueue);
	dequeue(mainQueue);

	// Resume current.
	kill(current.pid, SIGCONT);

	count++;
	alarm(timeSlice);
}


void childHandler(int sinal) 
{
	int status;
	pid_t pid = waitpid(-1, &status, WUNTRACED | WCONTINUED | WNOHANG);

	if (WIFEXITED(status) == 1) // exited normally
	{
		count2++;
		printf("Process %d finished running!\n", pid);
		current = front(mainQueue); dequeue(mainQueue);
		if(current.pid == -1)
		{
			runningFlag = 0;
			alarm(0);
		}
		else {
			kill(current.pid, SIGCONT);
			alarm(3);
		}
	}
}

void ioStartedHandler(int sinal) 
{
	// stop current.
	kill(current.pid, SIGSTOP);
	enqueue(waitQueue, current);

	// Swap current.
	current = front(mainQueue);
	dequeue(mainQueue);

	// Resume current.
	kill(current.pid, SIGCONT);

	alarm(timeSlice);
}

void ioDoneHandler(int sinal) 
{
	enqueue(mainQueue, front(waitQueue));
	dequeue(waitQueue);
}

int main(int argc, char const *argv[])
{
	if (argc < 2) {
		printf("Faltam parametros para executar!\n");
		exit(1);
	}

	Queue q1 = {NULL, NULL};
	mainQueue = &q1;
	Queue q2 = {NULL, NULL};
	waitQueue = &q2;
	
	for(int i = 1; i < argc; i++) {
		printf("%s\n", argv[i]);

		ProcessRR p = {fork()};
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

	current = front(mainQueue);
	dequeue(mainQueue);

	kill(current.pid, SIGCONT);

	signal(SIGCHLD, childHandler);
	signal(SIGALRM, alarmHandler);
	signal(SIGUSR1, ioStartedHandler);
	signal(SIGUSR2, ioDoneHandler);
	
	alarm(timeSlice);


	while(runningFlag)
	{
		sleep(1);
		//printf("%d, %d\n", count, count2);
		printf("Current pid: %d\n", current.pid);
	}
	
	printf("Done!\n");
}
