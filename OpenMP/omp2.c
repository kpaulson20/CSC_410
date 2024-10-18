// Set number of threads and get thread ID

#include <stdio.h>
#include <omp.h>

int main() 
{

    printf("Before omp\n");

    omp_set_num_threads (4);

    //#pragma omp parallel num_threads(4)
    #pragma omp parallel
    {
        printf("Hello\n");
        //printf("Hello from thread %d\n", omp_get_thread_num());
    }

    printf("After omp\n");
    return 0;
}