// Single, Master - I just need a single thread to do the work

#include <stdio.h>
#include <omp.h>

int main() 
{
    int tID;

    omp_set_num_threads (4);

    #pragma omp parallel private (tID)
    {
        tID = omp_get_thread_num();
        printf("First Hello from thread %d\n", tID);

        #pragma omp barrier

        #pragma omp single //nowait , master
        printf("Solo work - No teams - by thread %d\n", tID);

        //#pragma omp barrier

        printf("Second Hello from thread %d\n", tID);
    }


    return 0;
}