#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 1000  // Size of the matrix
#define NUM_THREADS 4  // Number of threads

int **A, **B, **C;  // Global matrices

// Information holder for each thread
typedef struct {
    int thread_id;
    int num_rows;  
} thread_data_t;

// Entry function for each thread
void* matrixMultiplyThread(void* arg) {
    // Extract thread info from the passes argument
        // ------> write your code here

    // Calculate the start and ending row chunck for each thread to handle
        // -------> write your code here

    // Loop through the start and end row assigned to the thread and compute matrix multiplication
        // --------> write your code here

    pthread_exit(NULL);
}
