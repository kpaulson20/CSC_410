// Allocating memory and transferring data

#define CL_TARGET_OPENCL_VERSION 120  
#include <CL/cl.h>
#include <stdio.h>

#define VECTOR_SIZE 10

int main() {
    float A[VECTOR_SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    cl_platform_id platform;
    cl_device_id device;
    cl_context context;
    cl_command_queue command_queue;
    cl_mem bufferA;
    cl_int ret;

    clGetPlatformIDs(1, &platform, NULL);
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
    context = clCreateContext(NULL, 1, &device, NULL, NULL, &ret);
    command_queue = clCreateCommandQueue(context, device, 0, &ret);

    // Allocate buffer
    bufferA = clCreateBuffer(context, CL_MEM_READ_ONLY, VECTOR_SIZE * sizeof(float), NULL, &ret);

    // Copy data to buffer
    ret = clEnqueueWriteBuffer(command_queue, bufferA, CL_TRUE, 0, VECTOR_SIZE * sizeof(float), A, 0, NULL, NULL);
    if (ret == CL_SUCCESS) {
        printf("Data transferred to device memory successfully\n");
    }

    clReleaseMemObject(bufferA);
    clReleaseCommandQueue(command_queue);
    clReleaseContext(context);
    return 0;
}
