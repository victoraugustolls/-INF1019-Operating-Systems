#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM_FILHOS 5

int searchWord(char* word);

int main(void) {

    pid_t pid;

    FILE* words;

    char* search[5] = {"Irene", "ceu", "humor", "Pedro", "boa"};

    words = fopen("words.txt", "w");
    if (words == NULL) {
        printf("Error opening file!\n");
		exit(1);
    }

    printf("Começo do Processo Pai\n");

    pid = 1;

	for(int j = 0; j<NUM_FILHOS; j++) {
		pid = fork();
		if (pid == 0) {
			printf("\nComeço do Processo Filho %d\n\n", j+1);
            printf("Procurando: %s\n", search[j]);
            int count = searchWord(search[j]);
            fprintf(words, "%s %d\n",search[j], count);
			printf("\nFim do Processo Filho %d\n\n", j+1);
			exit(0);
		}
	}

    int i = 0;
    while (i < NUM_FILHOS) {
        waitpid(-1, NULL, 0);
        i++;
    }

    printf("\nContinua do Processo Pai\n\n");

    fclose(words);

    words = fopen("words.txt", "r");
    if (words == NULL) {
        printf("Error opening file!\n");
		exit(1);
    }

    char word[10];
    int count;

    while(fscanf(words, "%s %d", word, &count) != EOF) {
        printf("%s %d\n", word, count);
    }

    fclose(words);
    printf("\nFim do Processo Pai\n\n");

    return 0;
}

int searchWord(char* word) {
    FILE* poema;
    char find[100];
    int value = 0;
    poema = fopen("poema.txt", "r");
    while (fscanf(poema, "%s ", find) != EOF) {
        if (strstr(find, word) != NULL) {
            value++;
        }
    }
    fclose(poema);
    return value;
}
