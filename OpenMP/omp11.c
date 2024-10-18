// Critical - protect a block of code
// Like a mutex from pthreads
// more flexible, more overhead

#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main() 
{
    int tID;
    srand(time(NULL));
    omp_set_num_threads (4);

    #pragma omp parallel private (tID)
    {
        tID = omp_get_thread_num();
        printf("Hello from thread %d\n", tID);

        #pragma omp barrier

        //#pragma omp critical
        //{
            printf("Just me here from thread %d\n", tID);
            sleep(rand()%5);
            printf("Thread %d pretending to do work\n", tID);
        //}

    }


    return 0;
}