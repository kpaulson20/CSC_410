#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 1000

void displayMatrix(int** matrix, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void matrixMultiply(int** A, int** B, int** C, int n, int startRow, int endRow) {
    for (int i = startRow; i < endRow; ++i) {
        for (int j = 0; j < n; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < n; ++k) {
                C[i][j] += A[i][k] * B[k][j];
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
    
    int **A, **B, **C;
    
    // Dynamically allocate memory for the matrices (only on rank 0)
    if (rank == 0) {
        A = (int**)malloc(N * sizeof(int*));
        B = (int**)malloc(N * sizeof(int*));
        C = (int**)malloc(N * sizeof(int*));

        for (int i = 0; i < N; ++i) {
            A[i] = (int*)malloc(N * sizeof(int));
            B[i] = (int*)malloc(N * sizeof(int));
            C[i] = (int*)malloc(N * sizeof(int));
        }

        if (A == NULL || B == NULL || C == NULL) {
            printf("Memory allocation failed!\n");
            MPI_Abort(MPI_COMM_WORLD, -1);
        }

        // Initialize matrices A and B
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                A[i][j] = 1;
                B[i][j] = 1;
                C[i][j] = 0;
            }
        }

        printf("Matrices initialized successfully.\n");
    }

    // Broadcast matrix B to all processes
    if (rank != 0) {
        B = (int**)malloc(N * sizeof(int*));
        for (int i = 0; i < N; ++i) {
            B[i] = (int*)malloc(N * sizeof(int));
        }
    }
    MPI_Bcast(*B, N * N, MPI_INT, 0, MPI_COMM_WORLD);

    // Each process computes a part of matrix C
    int rowsPerProcess = N / size;
    int startRow = rank * rowsPerProcess;
    int endRow = (rank == size - 1) ? N : (rank + 1) * rowsPerProcess;

    // Allocate a local copy of matrix C for each process
    int** localC = (int**)malloc((endRow - startRow) * sizeof(int*));
    for (int i = 0; i < (endRow - startRow); ++i) {
        localC[i] = (int*)malloc(N * sizeof(int));
    }

    // Perform matrix multiplication for the assigned rows
    if (rank == 0) {
        matrixMultiply(A, B, localC, N, startRow, endRow);
    }

    // Gather the results from all processes
    MPI_Gather(*localC, rowsPerProcess * N, MPI_INT, *C, rowsPerProcess * N, MPI_INT, 0, MPI_COMM_WORLD);

    // Optionally display the resulting matrix C on rank 0
    if (rank == 0) {
        printf("Matrix multiplication complete!\n");
        // displayMatrix(C, N); // Uncomment if you want to display the result
    }

    // Free dynamically allocated memory
    for (int i = 0; i < N; ++i) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }
    free(A);
    free(B);
    free(C);

    for (int i = 0; i < (endRow - startRow); ++i) {
        free(localC[i]);
    }
    free(localC);

    double end_time = MPI_Wtime();

    double elapsed_time = start_time - end_time;
    printf("Elapsed time: %f seconds\n", elapsed_time);

    MPI_Finalize();
    return 0;
}
