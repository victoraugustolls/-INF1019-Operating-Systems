#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(void) {

    int fdIn;
    int fdOut;
    char text[100];

    if ((fdIn = open("input", O_RDONLY, 0666)) == -1) {
        perror("Error read open()");
        return -1;
    }

    if ((fdOut = open("output", O_RDWR|O_CREAT|O_TRUNC, 0666)) == -1) {
        perror("Error write open()");
        return -1;
    }

    if (dup2(fdIn, 0) == -1) {
        perror("Error dup2()");
        return -3;
    }

    if (dup2(fdOut, 1) == -1) {
        perror("Error dup2()");
        return -3;
    }

    scanf("%s\n", text);
    printf("%s\n", text);

    return 0;
}
