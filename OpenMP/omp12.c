// private, firstprivate

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int main()
{
    int val = 5, x, tID;

    srand(time(NULL));

    printf("Before omp\n");

    omp_set_num_threads (5);

    // Assuming independent iterations
    //#pragma omp parallel private(tID) firstprivate(val)
    //#pragma omp parallel private(tID) private(val)
    #pragma omp parallel private(tID)
    {
        tID = omp_get_thread_num(); 
        printf("Hello from thread %d val %d\n", tID, val);

        #pragma omp barrier
            //#pragma omp critical 
            //{
                x = val;
                x = x + 1;
                val = x;
            //}

        #pragma omp barrier

        printf("See you from thread %d val %d\n", tID, val);
    }

    printf("After omp\n");

    printf("val is %d\n", val);

    return 0;
}