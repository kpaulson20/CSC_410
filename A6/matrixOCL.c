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
    // Define local memory for a block of the matrices
    __local float Asub[16][16];
    __local float Bsub[16][16];
    
    // Get row and column indices for the work item
    int row = get_global_id(0);
    int col = get_global_id(1);
    
    float sum = 0.0;
    
    // Iterate over sub-blocks of A and B
    for (int k = 0; k < N; k += 16) {
        // Load a sub-matrix from global to local memory
        Asub[get_local_id(0)][get_local_id(1)] = A[row * N + (k + get_local_id(1))];
        Bsub[get_local_id(0)][get_local_id(1)] = B[(k + get_local_id(0)) * N + col];
        
        // Wait for all threads to load the sub-matrix
        barrier(CLK_LOCAL_MEM_FENCE);
        
        // Multiply the sub-matrices
        for (int n = 0; n < 16; n++) {
            sum += Asub[get_local_id(0)][n] * Bsub[n][get_local_id(1)];
        }
        
        // Wait for all threads to finish the computation for this block
        barrier(CLK_LOCAL_MEM_FENCE);
    }
    
    // Write the result back to global memory
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
    clGetPlatformIDs(1, &platform, NULL);

    // Get device
    cl_device_id device;
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);

    // Create OpenCL context
    cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);

    // Create command queue
    cl_command_queue queue = clCreateCommandQueue(context, device, 0, &err);

    // Create program from source
    cl_program program = clCreateProgramWithSource(context, 1, &kernelSource, NULL, &err);

    // Build the program
    clBuildProgram(program, 1, &device, NULL, NULL, NULL);

    // Create kernel
    cl_kernel kernel = clCreateKernel(program, "matrixMultiply", &err);

    // Create buffers
    cl_mem bufferA = clCreateBuffer(context, CL_MEM_READ_ONLY, bytes, NULL, &err);
    cl_mem bufferB = clCreateBuffer(context, CL_MEM_READ_ONLY, bytes, NULL, &err);
    cl_mem bufferC = clCreateBuffer(context, CL_MEM_WRITE_ONLY, bytes, NULL, &err);

    // Write data to buffers
    clEnqueueWriteBuffer(queue, bufferA, CL_TRUE, 0, bytes, A, 0, NULL, NULL);
    clEnqueueWriteBuffer(queue, bufferB, CL_TRUE, 0, bytes, B, 0, NULL, NULL);

    // Set kernel arguments
    clSetKernelArg(kernel, 0, sizeof(cl_mem), &bufferA);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &bufferB);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), &bufferC);
    clSetKernelArg(kernel, 3, sizeof(int), &N);

    // Set local work size (work-group size)
    size_t localWorkSize[2] = {16, 16};  // 16x16 work-items per work-group
    size_t globalWorkSize[2] = {N, N};  // N x N total work-items

    // Execute kernel
    clEnqueueNDRangeKernel(queue, kernel, 2, NULL, globalWorkSize, localWorkSize, 0, NULL, NULL);

    // Wait for the kernel to finish
    clFinish(queue);

    // Read the result back into C
    clEnqueueReadBuffer(queue, bufferC, CL_TRUE, 0, bytes, C, 0, NULL, NULL);

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
