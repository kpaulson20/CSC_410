#include "matrixOMP.h"
#include <omp.h>

int main() {
    // Static allocation of matrices
    int A[N][N];
    int B[N][N];
    int C[N][N];

    printf("Matrices allocated successfully.\n");

    // Initialize matrices A and B
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            A[i][j] = 1;
            B[i][j] = 1;
            C[i][j] = 0;
        }
    }

    printf("Matrices initialized successfully.\n");

    for (int threads = 1; threads < 10; threads++){
       double start_time = omp_get_wtime();
       matrixMultiply(A, B, C); 
       double end_time = omp_get_wtime();

       printf("Threads: %d Time taken: %f seconds\n", threads, end_time - start_time);
    }
    
    printf("Matrix multiplication complete!\n");

    // Display the resulting matrix C
    printf("Resulting Matrix C:\n");
    displayMatrix(C);

    return 0;
}