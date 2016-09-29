#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {

    int nDadosTx, nDadosRx;
    int fd[2];
    const char textoTX[] = "uma mensagem";
    char textoRX[sizeof textoTX];

    if (pipe(fd) < 0) {
        puts ("Erro ao abrir os pipes");
        exit (-1);
    }

    if (fork() == 0) {
        close(fd[0]);
        nDadosTx = write(fd[1], textoTX, strlen(textoTX)+1);
        printf("%d dados escritos\n", nDadosTx);
        close(fd[1]);
        exit(0);
    }
    else {
        close(fd[1]);
        nDadosRx = read(fd[0], textoRX, sizeof textoRX);
        printf("%d dados lidos: %s\n", nDadosRx, textoRX);
        close(fd[0]);
    }

    return 0;
}
