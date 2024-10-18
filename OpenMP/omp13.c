// lastprivate

#include <stdio.h>
#include <omp.h>
#include <unistd.h>

int main()
{
    int N = 10, tID;
    int arr[10] = {12, 54, 23, 87, 34, 65, 91, 19, 46, 91};
    int max = 0, maxI = 0;

    //#pragma omp parallel for private(tID) lastprivate(max, maxI)
    #pragma omp parallel for private(tID)
    for (int i = 0; i < N; i++)
    {
        // tID = omp_get_thread_num();
        // printf("Iteration %d done by thread %d\n", i, tID);
        if(arr[i] > max) {
            sleep(1);
            max = arr[i];
            maxI = i;
        }
    }

    printf("Max value is %d at index %d\n", max, maxI);

    return 0;
}