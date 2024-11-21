#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 1000  // Size of the matrix

// Matrix multiplication function for local computation
void matrixMultiply(int* local_A, int* B, int* local_C, int n, int startRow, int endRow) {
    for (int i = startRow; i < endRow; ++i) {
        for (int j = 0; j < n; ++j) {
            local_C[i * n + j] = 0;  // Initialize local C values to 0
            for (int k = 0; k < n; ++k) {
                local_C[i * n + j] += local_A[i * n + k] * B[k * n + j];  // Multiply and accumulate
            }
        }
    }
}

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    double start_time = MPI_Wtime();

    int *A = NULL, *B = NULL, *C = NULL;
    int rows_per_process = N / size;

    // Allocate matrices on rank 0
    if (rank == 0) {
        A = (int*)malloc(N * N * sizeof(int));  // Flattened matrix A
        B = (int*)malloc(N * N * sizeof(int));  // Flattened matrix B
        C = (int*)malloc(N * N * sizeof(int));  // Flattened matrix C

        if (A == NULL || B == NULL || C == NULL) {
            printf("Memory allocation failed!\n");
            MPI_Abort(MPI_COMM_WORLD, -1);
        }

        // Initialize matrices A and B (only on rank 0)
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                A[i * N + j] = 1;
                B[i * N + j] = 1;
            }
        }

        printf("Matrices initialized successfully.\n");
    }

    // Allocate local memory for each process
    int* local_A = (int*)malloc(rows_per_process * N * sizeof(int));  // Local portion of A
    int* B = (int*)malloc(N * N * sizeof(int));
    int* local_C = (int*)malloc(rows_per_process * N * sizeof(int));  // Local portion of C

    if (local_A == NULL || local_C == NULL) {
        printf("Memory allocation failed for local matrices!\n");
        MPI_Abort(MPI_COMM_WORLD, -1);
    }

    // Broadcast matrix B to all processes
    MPI_Bcast(B, N * N, MPI_INT, 0, MPI_COMM_WORLD);

    // Scatter matrix A to all processes (each process gets its chunk of rows)
    MPI_Scatter(A, rows_per_process * N, MPI_INT, local_A, rows_per_process * N, MPI_INT, 0, MPI_COMM_WORLD);

    // Calculate the range of rows each process is responsible for
    int startRow = rank * rows_per_process;
    int endRow = (rank == size - 1) ? N : (rank + 1) * rows_per_process;

    // Perform local matrix multiplication (local_A * B = local_C)
    matrixMultiply(local_A, B, local_C, N, startRow, endRow);

    // Gather the results from all processes to the full matrix C on rank 0
    MPI_Gather(local_C, rows_per_process * N, MPI_INT, C, rows_per_process * N, MPI_INT, 0, MPI_COMM_WORLD);

    // Optionally display the resulting matrix C on rank 0
    if (rank == 0) {
        printf("Matrix multiplication complete!\n");
        // displayMatrix(C, N); // Uncomment if you want to display the result
    }

    // Free dynamically allocated memory
    if (rank == 0) {
        free(A);
        free(B);
        free(C);
    }
    free(local_A);
    free(local_C);

    double end_time = MPI_Wtime();
    if (rank == 0) {
        double elapsed_time = end_time - start_time;
        printf("Elapsed time: %f seconds\n", elapsed_time);
    }
    
    MPI_Finalize();
    
    return 0;
}
