// Matrix Multiplicaton using OpenCL

#define CL_TARGET_OPENCL_VERSION 120
#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h>

const char *kernelSource = R"(
__kernel void matrixMultiply(__global float* A, 
                             __global float* B, 
                             __global float* C, 
                             const int N) {
    int row = get_global_id(0);
    int col = get_global_id(1);
    
    float sum = 0.0;
    for (int k = 0; k < N; k++) {
        sum += A[row * N + k] * B[k * N + col];
    }
    C[row * N + col] = sum;
}
)";


int main() {
    const int N = 1000; // Matrix dimension
    size_t bytes = N * N * sizeof(float);

    // Host data
    float *A = (float*)malloc(bytes);
    float *B = (float*)malloc(bytes);
    float *C = (float*)malloc(bytes);

    // Initialize matrices
    for (int i = 0; i < N * N; i++) {
        A[i] = 1.0f; 
        B[i] = 1.0f; 
        C[i] = 0.0f;
    }

    // OpenCL setup and matrix multiplication


    // Print result
    //printf("Result Matrix:\n");
    //for (int i = 0; i < N; i++) {
    //     for (int j = 0; j < N; j++) {
    //         printf("%f ", C[i * N + j]);
    //     }
    //     printf("\n");
    //}

    // Cleanup


    return 0;
}
