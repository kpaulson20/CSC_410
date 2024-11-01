// Dynamic scheduling 
// When some iterations take more time - helps reduce time
// Keep all threads busy - no idle thread while other threads are working 

#include <omp.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int n = 12, tID;
    int arr[n];
    srand(time(NULL));

    omp_set_num_threads (3);

    // Dynamic scheduling
    #pragma omp parallel for private (tID) schedule(dynamic, 2)
    for (int i = 0; i < n; i++) {
        // simulate varying workload
        sleep(rand() % 4);
        
        arr[i] = i * i;
        tID = omp_get_thread_num();
        printf("Thread %d computed arr[%d] = %d\n", tID, i, arr[i]);
    }

    return 0;
}
