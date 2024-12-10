// Matrix Multiplication using OpenCL

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
    
    // Initialize sum for this particular element of C
    float sum = 0.0;
    
    // Perform the multiplication of the row of A and column of B
    for (int k = 0; k < N; k++) {
        sum += A[row * N + k] * B[k * N + col];
    }

    // Store the result in the corresponding position of C
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

    if (!A || !B || !C) {
        printf("Error allocating memory on the host\n");
        return -1;
    }

    // Initialize matrices A and B
    for (int i = 0; i < N * N; i++) {
        A[i] = 1.0f;
        B[i] = 1.0f;
        C[i] = 0.0f;
    }

    // OpenCL setup and matrix multiplication
    cl_int err;
    
    // Get platform
    cl_platform_id platform;
    err = clGetPlatformIDs(1, &platform, NULL);
    if (err != CL_SUCCESS) {
        printf("Error getting platform IDs\n");
        return -1;
    }

    // Get device
    cl_device_id device;
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
    if (err != CL_SUCCESS) {
        printf("Error getting device IDs\n");
        return -1;
    }

    // Create OpenCL context
    cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
    if (err != CL_SUCCESS || !context) {
        printf("Error creating OpenCL context\n");
        return -1;
    }

    // Create command queue
    cl_command_queue queue = clCreateCommandQueue(context, device, 0, &err);
    if (err != CL_SUCCESS || !queue) {
        printf("Error creating command queue\n");
        return -1;
    }

    // Create program from source
    cl_program program = clCreateProgramWithSource(context, 1, &kernelSource, NULL, &err);
    if (err != CL_SUCCESS || !program) {
        printf("Error creating program\n");
        return -1;
    }

    // Build the program
    err = clBuildProgram(program, 1, &device, NULL, NULL, NULL);
    if (err != CL_SUCCESS) {
        char buildLog[1024];
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, sizeof(buildLog), buildLog, NULL);
        printf("Error in kernel compilation: %s\n", buildLog);
        return -1;
    }

    // Create kernel
    cl_kernel kernel = clCreateKernel(program, "matrixMultiply", &err);
    if (err != CL_SUCCESS || !kernel) {
        printf("Error creating kernel\n");
        return -1;
    }

    // Create buffers
    cl_mem bufferA = clCreateBuffer(context, CL_MEM_READ_ONLY, bytes, NULL, &err);
    cl_mem bufferB = clCreateBuffer(context, CL_MEM_READ_ONLY, bytes, NULL, &err);
    cl_mem bufferC = clCreateBuffer(context, CL_MEM_WRITE_ONLY, bytes, NULL, &err);
    
    if (err != CL_SUCCESS || !bufferA || !bufferB || !bufferC) {
        printf("Error creating buffers\n");
        return -1;
    }

    // Write data to buffers
    err = clEnqueueWriteBuffer(queue, bufferA, CL_TRUE, 0, bytes, A, 0, NULL, NULL);
    if (err != CL_SUCCESS) {
        printf("Error writing to buffer A\n");
        return -1;
    }
    
    err = clEnqueueWriteBuffer(queue, bufferB, CL_TRUE, 0, bytes, B, 0, NULL, NULL);
    if (err != CL_SUCCESS) {
        printf("Error writing to buffer B\n");
        return -1;
    }

    // Set kernel arguments
    err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &bufferA);
    err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &bufferB);
    err |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &bufferC);
    err |= clSetKernelArg(kernel, 3, sizeof(int), &N);

    if (err != CL_SUCCESS) {
        printf("Error setting kernel arguments\n");
        return -1;
    }

    // Set global work size
    size_t globalWorkSize[2] = {N, N};  // N x N matrix

    // Execute kernel
    err = clEnqueueNDRangeKernel(queue, kernel, 2, NULL, globalWorkSize, NULL, 0, NULL, NULL);
    if (err != CL_SUCCESS) {
        printf("Error launching kernel\n");
        return -1;
    }

    // Wait for the kernel to finish
    clFinish(queue);

    // Read the result back into C
    err = clEnqueueReadBuffer(queue, bufferC, CL_TRUE, 0, bytes, C, 0, NULL, NULL);
    if (err != CL_SUCCESS) {
        printf("Error reading from buffer C\n");
        return -1;
    }

    // Print result (optional)
    // printf("Result Matrix:\n");
    // for (int i = 0; i < N; i++) {
    //     for (int j = 0; j < N; j++) {
    //         printf("%f ", C[i * N + j]);
    //     }
    //     printf("\n");
    // }

    // Cleanup
    clReleaseMemObject(bufferA);
    clReleaseMemObject(bufferB);
    clReleaseMemObject(bufferC);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    // Free host memory
    free(A);
    free(B);
    free(C);

    return 0;
}
