#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

#define EVER  ;;
#define ALARM 10

void intHandler(int sinal);
void quitHandler(int sinal);
void handler(int sinal);
void alarmHandler(int sinal);
void killHandler(int sinal);
void floatHandler(int sinal);

int main(void) {

    // void (*p) (int); //ponteiro para função que recebe int como parâmentro
    signal(SIGKILL, killHandler);
    signal(SIGFPE, floatHandler);
    // float x = 100.0/0.0;
    int x = 0.0, y = 0.0;
    printf("Numero 1: ");
    scanf("%d", &x);
    printf("Numero 2: ");
    scanf("%d", &y);
    printf("Soma: %d\n", x+y);
    printf("Subtracao: %d\n", x-y);
    printf("Multiplicacao: %d\n", x*y);
    printf("Divisao: %d\n", x/y);
    // printf("%g\n", x);
    // p = signal(SIGINT, intHandler);
    // printf("Endereço do manipulador anterior %p\n", p);
    // p = signal(SIGQUIT, quitHandler);
    // printf("Endereço do manipulador anterior %p\n", p);
    // signal(SIGUSR1, handler);
    // signal(SIGALRM, alarmHandler);
    // printf("Crtl-C desabilitado. Use Ctrl-\\ para terminar\n");
    printf("Pid %d\n", getpid());
    // alarm(ALARM);
    // printf("Pausando o processo\n");
    // pause();
    // printf("Continuando\n");
    for(EVER);

    fflush(stdout);
    return 0;
}

void intHandler(int sinal) {
    printf("Você precionou Ctrl-C (%d)\n", sinal);
    fflush(stdout);
}

void quitHandler(int sinal) {
    printf("Terminando o processo...\n");
    exit(0);
}

void handler(int sinal) {
    printf("Recebido sinal %d\n", sinal);
}

void alarmHandler(int sinal) {
    printf("%d segundos\n", ALARM);
    alarm(ALARM);
}

void killHandler(int sinal) {
    printf("Terminando processo com sinal %d", sinal);
    fflush(stdout);
    exit(0);
}

void floatHandler(int sinal) {
    printf("Erro de ponto flutuante com sinal %d\n", sinal);
    fflush(stdout);
    exit(0);
}
