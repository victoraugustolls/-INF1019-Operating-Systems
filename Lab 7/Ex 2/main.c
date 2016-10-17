#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/shm.h>
#include <sys/stat.h>

int buffer;
char *msg;

// inicializa valor do semaforo
int setSemValue(int semId);
// remove o semaforo
void delSemValue(int semId);
// operacao P
int semaforoP(int semId);
// operacao V
int semaforoV(int semId);

// union semun {
//     int val;
//     struct semid_ds *buf;
//     ushort *array;
// };

int main (int argc, char * argv[]){
    int i;
    int semId;
    char c;

    if (argc > 1) {
        //Memoria compartilhada
        buffer = shmget(8822, 16*sizeof(char), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
        msg = (char *)shmat(buffer, 0, 0);
        //

        //Semaforo
        semId = semget(8823, 1, 0666 | IPC_CREAT);
        setSemValue(semId);
        //

        //Loop
        do {
            i = 0;
            while(i < 16) {
                if((c = getchar()) != -1) {
                    msg[i] = c;
                    i++;
                }
            }
            semaforoP(semId);
        } while(1);
        //
    }

    else {
        //Semaforo
        while ((semId = semget (8823, 1, 0666)) < 0) {
            putchar('.');
            fflush(stdout);
            sleep (1);
        }
        //

        //Memoria compartilhada
        buffer = shmget(8822, 16*sizeof(char), IPC_EXCL | S_IRUSR | S_IWUSR);
        msg = (char *)shmat(buffer, 0, 0);
        //

        //Loop
        do {
            printf("Processo Impressor: ");
            fflush(stdout);
            for(i = 0; i < 16; i++){
                putchar(msg[i]);
                fflush(stdout);
            }
            semaforoV(semId);
        } while(1);
        //
    }
    delSemValue(semId);
    return 0;
}

int setSemValue(int semId) {
    union semun semUnion;
    semUnion.val = 0;
    return semctl(semId, 0, SETVAL, semUnion);
}

void delSemValue(int semId) {
    union semun semUnion;
    semctl(semId, 0, IPC_RMID, semUnion);
}

int semaforoP(int semId) {
    struct sembuf semB;
    semB.sem_num = 0;
    semB.sem_op = 1;
    semB.sem_flg = SEM_UNDO;
    semop(semId, &semB, 1);
    return 0;
}

int semaforoV(int semId) {
    struct sembuf semB;
    semB.sem_num = 0;
    semB.sem_op = -1;
    semB.sem_flg = SEM_UNDO;
    semop(semId, &semB, 1);
    return 0;
}
