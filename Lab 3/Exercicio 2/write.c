#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char* getMessage();

int main (void) {

    int shmid;
    key_t key = 8752;

    char *message = getMessage();
    printf("%s\n", message);

    shmid = shmget(key, 1000*sizeof(char), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);

    if (shmid == -1) {
        printf("Error in shmget.\n\n");
        exit(1);
    }

    char *mem = (char*)shmat(shmid, 0, 0);
    strcpy(mem, message);
    printf("Frase salva com sucesso!\n");
    shmdt(mem);

    return 0;
}

char* getMessage() {
    char buffer[1000];
    scanf("%1000[^\n]", buffer);
    char *message = (char*)malloc(sizeof(char)*(strlen(buffer)));
    return strcpy(message, buffer);
}
