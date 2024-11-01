// Guided scheduling - compromise betweem static and dynamic schedling
// Assign larger chunks first -> then gradually decrease chunk size
// Helps reduce scheduling overhead and still balance workload division among threads

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

    // Guided scheduling
    #pragma omp parallel for private(tID) schedule(guided, 2)
    for (int i = 0; i < n; i++) {
        // simulate varying workload
        sleep(rand() % 4);
        
        arr[i] = i * i;

        tID = omp_get_thread_num();
        printf("Thread %d computed arr[%d] = %d\n", tID, i, arr[i]);
    }

    return 0;
}
