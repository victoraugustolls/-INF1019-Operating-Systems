#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

#define EVER  ;;
#define CHILD 2

void suspendPid(int pid);
void continuePid(int pid);

int main(void) {

    pid_t first_pid;
    pid_t second_pid;

    first_pid = fork();

    if (first_pid == 0) {
        for(EVER) {
            printf("filho 1 executando\n");
            sleep(1);
        }
    }

    second_pid = fork();

    if (second_pid == 0) {
        for(EVER) {
            printf("filho 2 executando\n");
            sleep(1);
        }
    }

    kill(second_pid, SIGSTOP);
    for (int i = 0; i < 5; i++) {
        sleep(1);
        suspendPid(first_pid);
        continuePid(second_pid);
        sleep(1);
        suspendPid(second_pid);
        continuePid(first_pid);
	}

    kill(first_pid, SIGKILL);
    kill(second_pid, SIGKILL);

    return 0;
}

void suspendPid(int pid) {
    printf("Pausando filho de pid: %d\n", pid);
    kill(pid, SIGSTOP);
    fflush(stdout);
}

void continuePid(int pid) {
    printf("Continua filho de pid: %d\n", pid);
    kill(pid, SIGCONT);
    fflush(stdout);
}
