#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main (void) {

    int shmid;
    key_t key = 8752;

    shmid = shmget(key, sizeof(char)*1000, IPC_EXCL | S_IRUSR | S_IWUSR);

    if (shmid == -1) {
        printf("Error in shmget.\n\n");
        exit(1);
    }

    char *mem = (char*)shmat(shmid, 0, SHM_RDONLY);
    printf("Frase recuperada:\n");
    printf("%s\n", mem);
    shmdt(mem);
    shmctl(shmid, IPC_RMID, 0);

    return 0;
}
