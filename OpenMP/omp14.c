// Reduction - do some operation across all iterations and store the combined result in one variable

#include <omp.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    int n = 100, sum = 0, tID;
    int arr[n];

    omp_set_num_threads (5);
    
    // Initializing array
    for (int i = 0; i < n; i++) {
        arr[i] = i + 1;
    }

    // Parallel sum using reduction
    #pragma omp parallel for private (tID) //reduction(+:sum)
    for (int i = 0; i < n; i++) {
        tID = omp_get_thread_num(); 

        //sleep(2);
        sum += arr[i];

        //printf("Hello from thread %d sum %d\n", tID, sum);
    }

    printf("Sum of array elements: %d\n", sum);  // sum should be 5050 
    return 0;
}
