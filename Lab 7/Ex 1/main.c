#include <sys/sem.h>
#include <unistd.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

// union semun {
//     int val;
//     struct semid_ds *buf;
//     ushort *array;
// };

// inicializa valor do semaforo
int setSemValue(int semId);
// remove o semaforo
void delSemValue(int semId);
// operacao P
int semaforoP(int semId);
// operacao V
int semaforoV(int semId);

int main(int argc, char *argv[]) {

    int i;
    char letra = 'o';
    int semId;

    if (argc > 1) {
        printf("argc > 1\n");
        semId = semget(8752, 1, 0666 | IPC_CREAT);
        setSemValue(semId);
        letra = 'x';
        sleep(2);
    }
    else {
        while ((semId = semget(8752, 1, 0666)) < 0) {
            putchar('.');
            fflush(stdout);
            sleep(1);
        }
    }

    for (i = 0; i < 10; i++) {
        semaforoP(semId);
        putchar(toupper(letra));
        fflush(stdout);
        sleep(rand()%3);
        putchar(letra);
        fflush(stdout);
        semaforoV(semId);
        sleep(rand()%2);
    }

    printf("\nProcesso %d terminou\n", getpid());

    if (argc > 1) {
        sleep(10);
        delSemValue(semId);
    }

    return 0;
}

int setSemValue(int semId) {
    union semun semUnion;
    semUnion.val = 1;
    return semctl(semId, 0, SETVAL, semUnion);
}

void delSemValue(int semId) {
    union semun semUnion;
    semctl(semId, 0, IPC_RMID, semUnion);
}

int semaforoP(int semId) {
    struct sembuf semB;
    semB.sem_num = 0;
    semB.sem_op = -1;
    semB.sem_flg = SEM_UNDO;
    semop(semId, &semB, 1);
    return 0;
}

int semaforoV(int semId) {
    struct sembuf semB;
    semB.sem_num = 0;
    semB.sem_op = 1;
    semB.sem_flg = SEM_UNDO;
    semop(semId, &semB, 1);
    return 0;
}
