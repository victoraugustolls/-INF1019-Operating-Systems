#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {

    int fd[2]; // descritor dos pipes

    if (pipe(fd) < 0) {
        puts ("Erro ao abrir os pipes");
        exit (-1);
    }

    if (fork() == 0) {
        close(fd[0]);

        if (dup2(fd[1], 1) == -1) {
            perror("Error write open()");
            return -1;
        }

        char *argv[10];
        argv[0] = "ls";
        argv[1] = "-l";
        argv[2] = NULL;
        execvp("ls", argv);

        close(fd[1]);
        exit(0);
    }

    if(fork() == 0) {
        close(fd[1]);

        if (dup2(fd[0], 0) == -1) {
            perror("Error write open()");
            return -1;
        }

        char *argv[10];
        argv[0] = "grep";
        argv[1] = "main";
        argv[2] = NULL;
        execvp("grep", argv);

        close(fd[0]);
        exit(1);
    }

    close(fd[1]);
    close(fd[0]);

    return 0;
}
