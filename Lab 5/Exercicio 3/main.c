#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

#define EVER  ;;

void child_routine(int open, int closing, int closed, int multiplier);
void userHandler(int sinal);

int main(void) {

    int open = 30;
    int closing = 5;
    int closed = 30;

    pid_t first_pid;
    pid_t second_pid;
    pid_t third_pid;
    pid_t fourth_pid;

    first_pid = fork();

    if (first_pid == 0) {
        child_routine(open, closing, closed, 0);
    }

    second_pid = fork();

    if (second_pid == 0) {
        child_routine(open, closing, closed, 1);
    }

    third_pid = fork();

    if (third_pid == 0) {
        child_routine(open, closing, closed, 2);
    }

    fourth_pid = fork();

    if (fourth_pid == 0) {
        child_routine(open, closing, closed, 3);
    }

    return 0;
}

void userHandler(int sinal) {


}

void child_routine(int open, int closing, int closed, int multiplier) {
    int child_open = 0;
    int child_closing = 0;
    int child_closed = 0;

    for(EVER) {
        child_open = open + multiplier*10;
        child_closing = closing + multiplier*5;
        child_closed = closed + multiplier*10;
        while (child_open > 0) {
            printf("Sinal %d aberto, fechando em: %d\n", multiplier+1, child_open);
            child_open = child_open - 5;
            sleep(5);
        }
        while (child_closing > 0) {
            printf("Sinal %d fechando, fechado em: %d\n", multiplier+1, child_closing);
            child_closing = child_closing - 5;
            sleep(5);
        }
        while (child_closed > 0) {
            printf("Sinal %d fechado, abrindo em: %d\n", multiplier+1, child_closed);
            child_closed = child_closed - 5;
            sleep(5);
        }
    }
}
