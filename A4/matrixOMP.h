#include <stdio.h>

#define N 10 // Size of the matrices


void displayMatrix(int matrix[N][N]) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void matrixMultiply(int A[N][N], int B[N][N], int C[N][N]) {
    // Write your code here
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            C[i][j] = 0;
            for (int k = 0; k < N; k++)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }  
    }
}