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
    thread_data_t *data = (thread_data_t*)arg;
    int thread_id = data->thread_id;
    int num_rows = data->num_rows;

    // Calculate the start and ending row chunck for each thread to handle
    // -------> write your code here
    int rows_per_thread = N / NUM_THREADS;
    int start_row = thread_id * rows_per_thread;
    int end_row = (thread_id == NUM_THREADS -1) ? N :start_row + rows_per_thread;

    // Loop through the start and end row assigned to the thread and compute matrix multiplication
    // --------> write your code here
    for (int i = start_row; i < end_row; i++){
        for (int j = 0; j < N; j++){
            C[i][j] = 0;
            for (int k = 0; k <N; k++){
                C[i][j] += A[i][j] * B[i][j];
            }
        }
    }
    pthread_exit(NULL);
}
