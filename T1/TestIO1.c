#include <signal.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>


int main()
{
    // IO-BOUND
    long i = 0;
    while(i < 50000000) { i++; }

    kill(getppid(), SIGUSR1);

    while(i < 50000000) { i++; }
    
    return 0;
}
