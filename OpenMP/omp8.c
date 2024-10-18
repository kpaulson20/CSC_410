// Barriers -> improvement on omp3.c

#include <stdio.h>
#include <omp.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

int main() 
{
    int tID;
    srand(time(NULL));

    printf("Before omp\n");

    omp_set_num_threads (4);

    //#pragma omp parallel num_threads(4)
    #pragma omp parallel private (tID)
    {
        tID = omp_get_thread_num();
        printf("First Hello from thread %d\n", tID);
        //sleep(rand()%3);

        //#pragma omp barrier

        printf("Second Hello from thread %d\n", tID);
    }

    printf("After omp\n");

    return 0;
}