// A for loop in parallel with openmp
// private

#include <stdio.h>
#include <unistd.h>
#include <omp.h>

int main()
{
    int tID;

    printf("Before omp\n");

    omp_set_num_threads (3);

    // Assuming independent iterations
    #pragma omp parallel for // private(tID)
    for (int i = 0; i < 9; i++)
    {
        tID = omp_get_thread_num();
        printf("Iteration %d done by thread %d\n", i, tID);
    }

    printf("After omp\n");


    return 0;
}