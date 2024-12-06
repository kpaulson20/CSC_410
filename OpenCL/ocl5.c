// write and build kernel - Square each element

#define CL_TARGET_OPENCL_VERSION 120  
#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h>

#define VECTOR_SIZE 10

const char *kernelSource = 
    "__kernel void square(__global float* input, __global float* output) {  \n"
    "    int id = get_global_id(0);                                        \n"
    "    output[id] = input[id] * input[id];                               \n"
    "}                                                                     \n";

int main() {
    float A[VECTOR_SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    float B[VECTOR_SIZE];  // Array to store the results
    cl_platform_id platform;
    cl_device_id device;
    cl_context context;
    cl_command_queue command_queue;
    cl_mem bufferA, bufferB;
    cl_program program;
    cl_kernel kernel;
    cl_int ret;

    clGetPlatformIDs(1, &platform, NULL);
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
    
    context = clCreateContext(NULL, 1, &device, NULL, NULL, &ret);
    command_queue = clCreateCommandQueue(context, device, 0, &ret);

    // Allocate device memory
    bufferA = clCreateBuffer(context, CL_MEM_READ_ONLY, VECTOR_SIZE * sizeof(float), NULL, &ret);
    bufferB = clCreateBuffer(context, CL_MEM_WRITE_ONLY, VECTOR_SIZE * sizeof(float), NULL, &ret);

    // Copy input data to device
    clEnqueueWriteBuffer(command_queue, bufferA, CL_TRUE, 0, VECTOR_SIZE * sizeof(float), A, 0, NULL, NULL);

    // Build the kernel program
    program = clCreateProgramWithSource(context, 1, &kernelSource, NULL, &ret);
    clBuildProgram(program, 1, &device, NULL, NULL, NULL);

    // Create the OpenCL kernel
    kernel = clCreateKernel(program, "square", &ret);

    // Set kernel arguments
    clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&bufferA);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&bufferB);

    // Execute the kernel
    size_t global_work_size = VECTOR_SIZE;
    clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_work_size, NULL, 0, NULL, NULL);

    // Copy results back to host
    clEnqueueReadBuffer(command_queue, bufferB, CL_TRUE, 0, VECTOR_SIZE * sizeof(float), B, 0, NULL, NULL);

    // Display results
    printf("Squared elements:\n");
    for (int i = 0; i < VECTOR_SIZE; i++) {
        printf("%f ", B[i]);
    }
    printf("\n");

    // Cleanup
    clReleaseMemObject(bufferA);
    clReleaseMemObject(bufferB);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(command_queue);
    clReleaseContext(context);

    return 0;
}
