#include <signal.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

int main()
{
    // IO-BOUND
    long i = 0;
    while(i < 400000000) { i++; }

    kill(getppid(), SIGUSR1);
    i = 0;

    while(i < 500000000)
    {
        i++;
    }

    return 0;
}
