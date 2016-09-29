#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM_FILHOS 3

int main (void) {

	pid_t pid;
	int status;
	int linha, coluna, valor = 0;
	FILE *fileMatriz1;
	FILE *fileMatriz2;
	FILE *fileMatrizResultado;
	FILE *fileMatrizResultadoRead;
	int matriz1[3][3];
	int matriz2[3][3];
	int matrizResultado[3][3];

	fileMatriz1 = fopen("fileMatriz1.txt", "r");
	if (fileMatriz1 == NULL) {
		printf("Error opening file!\n");
		exit(1);
	}

	fileMatriz2 = fopen("fileMatriz2.txt", "r");
	if (fileMatriz1 == NULL) {
		printf("Error opening file!\n");
		exit(1);
	}

	fileMatrizResultado = fopen("fileMatrizResultado.txt", "w");
	if (fileMatrizResultado == NULL) {
		printf("Error opening file!\n");
		exit(1);
	}

	fileMatrizResultadoRead = fopen("fileMatrizResultado.txt", "r");
	if (fileMatrizResultado == NULL) {
		printf("Error opening file!\n");
		exit(1);
	}

	printf("Matriz 1:\n");
	while(fscanf(fileMatriz1, "%d %d %d", &linha, &coluna, &valor) == 3) {
		matriz1[linha][coluna] = valor;
		printf("%d %d %d\n",linha, coluna, valor);
	}
	fclose(fileMatriz1);

	printf("\nMatriz 2:\n");
	while(fscanf(fileMatriz2, "%d %d %d", &linha, &coluna, &valor) == 3) {
		matriz2[linha][coluna] = valor;
		printf("%d %d %d\n",linha, coluna, valor);
	}
	fclose(fileMatriz2);

	printf("\nMatriz 1:\n");
	for(int x = 0; x<3; x++) {
		printf("%d %d %d\n", matriz1[x][0], matriz1[x][1], matriz1[x][2]);
	}

	printf("\nMatriz 2:\n");
	for(int x = 0; x<3; x++) {
		printf("%d %d %d\n", matriz2[x][0], matriz2[x][1], matriz2[x][2]);
	}

	pid = 1;

	printf("\nProcesso Pai Inicio\n");

	for(int j = 0; j<NUM_FILHOS; j++) {
		pid = fork();
		if (pid == 0) {
			printf("\nProcesso Filho %d Inicio\n\n", j+1);
			for (int i = 0; i < 3; i++) {
				int soma = matriz1[j][i] + matriz2[j][i];
				fprintf(fileMatrizResultado, "%d %d %d\n", j, i, soma);
				printf("Linha: %d / Coluna: %d / Valor: %d\n", j, i, soma);
			}
			printf("\nProcesso Filho %d Fim\n\n", j+1);
			exit(0);
		}
	}

	for (int i = 0; i < NUM_FILHOS; i++) {
		waitpid(-1, &status, 0);
	}

	printf("\nProcesso Pai Continua\n");

	printf("\nMatriz Resultado:\n");
	while(fscanf(fileMatrizResultadoRead, "%d %d %d", &linha, &coluna, &valor) == 3) {
		matrizResultado[linha][coluna] = valor;
		printf("%d %d %d\n",linha, coluna, valor);
	}
	fclose(fileMatrizResultadoRead);
	fclose(fileMatrizResultado);

	printf("\nMatriz Resultado:\n");
	for(int x = 0; x<3; x++) {
		printf("%d %d %d\n", matrizResultado[x][0], matrizResultado[x][1], matrizResultado[x][2]);
	}

	printf("\nProcesso Pai Fim\n\n");

	return 0;
}
