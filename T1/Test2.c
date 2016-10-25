#include<stdio.h>

int main()
{
    // CPU-BOUND
    long i = 0;
    while(i < 550000000)
    {
        i++;
        if ( (i % 500000000 == 0)) {
            //printf("Teste forever2 rodando.\n");
        }
    }

    return 0;
}
