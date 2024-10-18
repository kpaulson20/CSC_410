// Shared vs Private variable

#include <stdio.h>
#include <omp.h>
#include <unistd.h>

int main() 
{
    int val = 10; // shared variable

    printf("Before omp\n");

    omp_set_num_threads (4);

    #pragma omp parallel
    {
        int val2 = 10; // private variable
        val++;
        val2++;
        //sleep(1);
        printf("Hello from thread %d with value 1: %d and value 2: %d\n", omp_get_thread_num(), val, val2);
    }

    printf("After omp\n");
    return 0;
}