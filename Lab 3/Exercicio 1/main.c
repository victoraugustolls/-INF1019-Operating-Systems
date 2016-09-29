#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_FILHOS 3

int main (void) {

    int matriz1[9] = {1,2,3,1,2,3,1,2,3};
    int matriz2[9] = {1,2,3,1,2,3,1,2,3};

    pid_t pid;

    int shmid;

    shmid = shmget(IPC_PRIVATE, 9*sizeof(int), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);

    if (shmid == -1) {
        printf("Error in shmget.\n\n");
        exit(1);
    }

    int *matrizResultado = (int*)shmat(shmid, 0, 0);

    for(int j = 0; j < NUM_FILHOS; j++) {
		pid = fork();
		if (pid == 0) {
			printf("\nProcesso Filho %d Inicio\n\n", j+1);
			for (int i = 0; i < 3; i++) {
                int soma = matriz1[i+3*j] + matriz2[i+3*j];
                matrizResultado[i+3*j] = soma;
    			printf("Linha: %d / Coluna: %d / Valor: %d\n", j, i, matrizResultado[i+3*j]);
			}
			printf("\nProcesso Filho %d Fim\n\n", j+1);
			exit(0);
		}
	}

    for (int i = 0; i < NUM_FILHOS; i++) {
		waitpid(-1, NULL, 0);
	}

    printf("Matriz Resultante:\n\n");
    for (int i = 0; i < 3; i++) {
        printf("%d %d %d\n", matrizResultado[i*3], matrizResultado[i*3+1], matrizResultado[i*3+2]);
    }

    shmdt(matrizResultado);
    shmctl(shmid, IPC_RMID, 0);

    return 0;
}
