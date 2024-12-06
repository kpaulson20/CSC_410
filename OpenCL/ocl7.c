// write and build kernel - element-wise addition 
// with timing

#define CL_TARGET_OPENCL_VERSION 120  
#include <stdio.h>
#include <CL/cl.h>
#include <stdlib.h>

#define VECTOR_SIZE 1024

const char* kernelSource = 
"__kernel void vector_add(__global float* A, __global float* B, __global float* C) {\n"
"   int id = get_global_id(0);\n"
"   C[id] = A[id] + B[id];\n"
"}\n";

int main() {
    float A[VECTOR_SIZE], B[VECTOR_SIZE], C[VECTOR_SIZE];

    // Initialize input vectors
    for (int i = 0; i < VECTOR_SIZE; i++) {
        A[i] = i * 1.0f;
        B[i] = i * 2.0f;
    }

    cl_platform_id platform_id = NULL;
    cl_device_id device_id = NULL;
    cl_context context = NULL;
    cl_command_queue command_queue = NULL;
    cl_program program = NULL;
    cl_kernel kernel = NULL;
    cl_mem bufferA = NULL, bufferB = NULL, bufferC = NULL;

    size_t global_work_size[1] = {VECTOR_SIZE};
    cl_int ret;

    // Get platform and device information
    ret = clGetPlatformIDs(1, &platform_id, NULL);
    ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, NULL);

    // Create OpenCL context and command queue
    context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
    command_queue = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &ret);

    // Create memory buffers on the device
    bufferA = clCreateBuffer(context, CL_MEM_READ_ONLY, VECTOR_SIZE * sizeof(float), NULL, &ret);
    bufferB = clCreateBuffer(context, CL_MEM_READ_ONLY, VECTOR_SIZE * sizeof(float), NULL, &ret);
    bufferC = clCreateBuffer(context, CL_MEM_WRITE_ONLY, VECTOR_SIZE * sizeof(float), NULL, &ret);

    // Copy input data to device buffers
    ret = clEnqueueWriteBuffer(command_queue, bufferA, CL_TRUE, 0, VECTOR_SIZE * sizeof(float), A, 0, NULL, NULL);
    ret = clEnqueueWriteBuffer(command_queue, bufferB, CL_TRUE, 0, VECTOR_SIZE * sizeof(float), B, 0, NULL, NULL);

    // Create kernel program from source
    program = clCreateProgramWithSource(context, 1, &kernelSource, NULL, &ret);

    // Build the program
    ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

    // Create the OpenCL kernel
    kernel = clCreateKernel(program, "vector_add", &ret);

    // Set kernel arguments
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), &bufferA);
    ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), &bufferB);
    ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), &bufferC);

    // Create event to measure kernel execution time
    cl_event kernel_event;

    // Execute the kernel
    ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, global_work_size, NULL, 0, NULL, &kernel_event);

    // Wait for the event to finish
    clWaitForEvents(1, &kernel_event);

    // Retrieve timing information
    cl_ulong start_time, end_time;
    ret = clGetEventProfilingInfo(kernel_event, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &start_time, NULL);
    ret = clGetEventProfilingInfo(kernel_event, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &end_time, NULL);

    double execution_time = (end_time - start_time) * 1e-6; // Convert from nanoseconds to milliseconds
    printf("Kernel execution time: %f ms\n", execution_time);

    // Read the output data back to the host
    ret = clEnqueueReadBuffer(command_queue, bufferC, CL_TRUE, 0, VECTOR_SIZE * sizeof(float), C, 0, NULL, NULL);

    // Display the result
    for (int i = 0; i < 10; i++) {
        printf("%f + %f = %f\n", A[i], B[i], C[i]);
    }

    // Step 12: Cleanup
    clReleaseEvent(kernel_event);
    clReleaseMemObject(bufferA);
    clReleaseMemObject(bufferB);
    clReleaseMemObject(bufferC);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(command_queue);
    clReleaseContext(context);

    return 0;
}
