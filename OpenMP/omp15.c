// Static scheduing - what OpenMP does by default
// When the iterations have similar workload - divide it equally among the threads

#include <omp.h>
#include <stdio.h>

int main() {
    int n = 12, tID;
    int arr[n];

    omp_set_num_threads (3);

    // Static scheduling
    #pragma omp parallel for private (tID) schedule(static) // (static, 2)
    for (int i = 0; i < n; i++) {
        arr[i] = i * i;
        tID = omp_get_thread_num();
        printf("Thread %d computed arr[%d] = %d\n", tID, i, arr[i]);
    }

    return 0;
}
