#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>

#define EVER  ;;

time_t tempo;

void user1Handler(int sinal);
void user2Handler(int sinal);
int price(long int seconds);

int main(void) {

    signal(SIGUSR1, user1Handler);
    signal(SIGUSR2, user2Handler);

    for(EVER);

    return 0;
}

void user1Handler(int sinal) {
    printf("SIGUSR1\n");
    tempo = time(NULL);
}

void user2Handler(int sinal) {
    printf("SIGUSR2\n");
    tempo = time(NULL) - tempo;
    printf("Chamada durou %ld segundos e custou %d centavos.\n", tempo, price((long int)tempo));
}

int price(long int seconds) {

    if (seconds <= 60) {
        return seconds*2;
    }
    else {
        return 120 + (seconds-60)*1;
    }
}
