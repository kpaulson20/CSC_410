// Sections - parallelize different parts of your code

#include <stdio.h>
#include <omp.h>

int main() {
    int tID;

    omp_set_num_threads (3);

    #pragma omp parallel
    {
        tID = omp_get_thread_num();
        #pragma omp sections
        {
            #pragma omp section
            {
                printf("Task 1 done by thread %d\n", omp_get_thread_num());
            }

            #pragma omp section
            {
                printf("Task 2 done by thread %d\n", omp_get_thread_num());
            }

            #pragma omp section
            {
                printf("Task 3 by thread %d\n", omp_get_thread_num());
            }
        }
    }
    return 0;
}
