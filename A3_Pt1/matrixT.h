#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 10  // Size of the matrix
#define NUM_THREADS 7 // Number of threads

int **A, **B, **C;  // Global matrices

// Information holder for each thread
typedef struct {
    int thread_id;
    int num_rows; 
} thread_data_t;

// Function for each thread
void* matrixMultiplyThread(void* arg) {
    thread_data_t *data = (thread_data_t*)arg;
    int thread_id = data->thread_id;
    int num_rows = data->num_rows;

    //int rows_per_thread = N / NUM_THREADS;
    int start_row = thread_id * (N/NUM_THREADS);
    int end_row = (thread_id + 1) * (N/NUM_THREADS);
    if (thread_id == (NUM_THREADS - 1)) {
            end_row = N;
    }

    for (int i = start_row; i < end_row; i++){
        for (int j = 0; j < N; j++){
            C[i][j] = 0;
            for (int k = 0; k < N; k++){
                C[i][j] += A[i][k] * B[i][j];
            }
        }
    }

    pthread_exit(NULL);
}

void displayMatrix(int** matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}
