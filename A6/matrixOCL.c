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
    //Setup
    cl_int err;
    cl_platform_id platform;
    clGetPlatformIDs(1, &platform, NULL);
    
    cl_device_id device;
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);

    cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
    cl_command_queue queue = clCreateCommandQueue(context, device, 0, &err);

    // Compile
    cl_program program = clCreateProgramWithSource(context, 1, &kernelSource, NULL, &err);
    err = clBuildProgram(program, 1, &device, NULL, NULL, NULL);
    if (err != CL_SUCCESS) {
        char buildLog[1024];
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, sizeof(buildLog), buildLog, NULL);
        printf("Error in kernel compilation: %s\n", buildLog);
        return -1;
    }

    cl_kernel kernel = clCreateKernel(program, "matrixMultiply", &err);

    // Buffers
    cl_mem bufferA = clCreateBuffer(context, CL_MEM_READ_ONLY, bytes, NULL, &err);
    cl_mem bufferB = clCreateBuffer(context, CL_MEM_READ_ONLY, bytes, NULL, &err);
    cl_mem bufferC = clCreateBuffer(context, CL_MEM_WRITE_ONLY, bytes, NULL, &err);

    // Write data to buffers
    clEnqueueWriteBuffer(queue, bufferA, CL_TRUE, 0, bytes, A, 0, NULL, NULL);
    clEnqueueWriteBuffer(queue, bufferB, CL_TRUE, 0, bytes, B, 0, NULL, NULL);

    // Kernel Args
    clSetKernelArg(kernel, 0, sizeof(cl_mem), &bufferA);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &bufferB);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), &bufferC);
    clSetKernelArg(kernel, 3, sizeof(int), &N);

    // Execute kernel
    size_t globalWorkSize[2] = {N, N};  // Matrix size N x N
    clEnqueueNDRangeKernel(queue, kernel, 2, NULL, globalWorkSize, NULL, 0, NULL, NULL);
    clFinish(queue);  // Wait for the kernel to finish

    // Read the result into C
    clEnqueueReadBuffer(queue, bufferC, CL_TRUE, 0, bytes, C, 0, NULL, NULL);

    // Print result
    //printf("Result Matrix:\n");
    //for (int i = 0; i < N; i++) {
    //     for (int j = 0; j < N; j++) {
    //         printf("%f ", C[i * N + j]);
    //     }
    //     printf("\n");
    //}

    // Cleanup
    clReleaseMemObject(bufferA);
    clReleaseMemObject(bufferB);
    clReleaseMemObject(bufferC);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    // Free memory
    free(A);
    free(B);
    free(C);

    return 0;
}
