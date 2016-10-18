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

static ProcessRR current;

int runningFlag = 1;
int count = 0;
int count2 = 0;
int count3 = 0;



void alarmHandler(int sinal) 
{
	// stop current.
	kill(current.pid, SIGSTOP);
	enqueue(current);

	// Swap current.
	current = front();
	dequeue();

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
		current = front(); dequeue();
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


int main(int argc, char const *argv[])
{
	if (argc < 2) {
		printf("Faltam parametros para executar!\n");
	}

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
		enqueue(p);
	}

	current = front();
	dequeue();

	kill(current.pid, SIGCONT);

	signal(SIGCHLD, childHandler);
	signal(SIGALRM, alarmHandler);
	alarm(timeSlice);


	while(runningFlag)
	{
		sleep(1);
		//printf("%d, %d\n", count, count2);
		printf("Current pid: %d\n", current.pid);
	}
	
	printf("Done!\n");
}
