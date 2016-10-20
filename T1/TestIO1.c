#include <signal.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

int main()
{
    // IO-BOUND
    long i = 0;
    while(i < 500000000) { i++; }

    kill(getppid(), SIGUSR1);

    while(i < 50000000)
    {
        printf("NAO DEI STOP MUAHAHA\n");
        if (i%100000)
        {
            printf("NAO DEI STOP MUAHAHA\n");
        }
        i++;
    }

    return 0;
}
