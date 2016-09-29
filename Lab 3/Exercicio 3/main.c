#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_FILHOS 10
#define NUMBER 1

void generateRandomVector(int* randomVector);

int main (void) {

    int shmid_vector, shmid_count;
    pid_t pid;

    shmid_vector = shmget(IPC_PRIVATE, 100*sizeof(int), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    shmid_count = shmget(IPC_PRIVATE, 100*sizeof(int), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);

    if (shmid_vector == -1) {
        printf("Error in shmget.\n\n");
        exit(1);
    }
    if (shmid_count == -1) {
        printf("Error in shmget.\n\n");
        exit(1);
    }

    int* randomVector = (int*)shmat(shmid_vector, 0, 0);
    int* results = (int*)shmat(shmid_count, 0, 0);
    generateRandomVector(randomVector);

    for(int j = 0; j < NUM_FILHOS; j++) {
		pid = fork();
		if (pid == 0) {
			printf("\nProcesso Filho %d Inicio\n\n", j+1);
			for (int i = 0; i < 10; i++) {
                printf("Casa: %d / Valor:%d\n",j*10+i, randomVector[j*10+i]);
                if (randomVector[j*10+i] == NUMBER) {
                    printf("Achou %d\n", NUMBER);
                    results[j*10+i] = 1;
                }
			}
			printf("\nProcesso Filho %d Fim\n", j+1);
			exit(0);
		}
	}

    for (int i = 0; i < NUM_FILHOS; i++) {
		waitpid(-1, NULL, 0);
	}

    printf("\nPosições:\n");
    for (int i = 0; i < 100; i++) {
        if (results[i] == 1) {
            printf("%d\n", i);
        }
    }

    shmdt(randomVector);
    shmdt(results);

    shmctl(shmid_vector, IPC_RMID, 0);
    shmctl(shmid_count, IPC_RMID, 0);

    return 0;
}

void generateRandomVector(int* randomVector) {
    for (int i = 0; i < 100; i++) {
        randomVector[i] = i%5;
    }
}
