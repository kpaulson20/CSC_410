// Thread local storage - private, firstprivate

#include <stdio.h>
#include <omp.h>
#include <unistd.h>

int main() 
{
    int val = 10;
    int tID;

    printf("Before omp\n");

    omp_set_num_threads (4);

    #pragma omp parallel //private (tID, val) //firstprivate(val)
    {
        //val++;
        tID = omp_get_thread_num();
        //sleep(1);
        val++;
        printf("Hello from thread %d with value %d\n", tID, val);
    }

    printf("After omp\n");
    printf("val is %d\n", val);
    return 0;
}