#include<stdio.h>

int main()
{
    // CPU-BOUND
    long i = 0;
    while(i < 250000000)
    {
        i++;
        if ( (i % 50000000 == 0)) {
            // printf("Teste Test1 rodando.\n");
            // i = 0;
        }
    }
    
    return 0;
}
