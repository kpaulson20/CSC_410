// write and build a simple kernel
// work-items annd work-groups

#define CL_TARGET_OPENCL_VERSION 120 
#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h>

const char *kernelSource = R"(
__kernel void work() {
    int global_id = get_global_id(0);
    int local_id = get_local_id(0);
    int group_id = get_group_id(0);
    printf("Global ID %d, Local ID %d, Group ID %d\n", global_id, local_id, group_id);
}
)";

int main() {
    cl_platform_id platform;
    cl_device_id device;
    cl_context context;
    cl_command_queue queue;
    cl_program program;
    cl_kernel kernel;

    clGetPlatformIDs(1, &platform, NULL);
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);

    context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);
    queue = clCreateCommandQueue(context, device, 0, NULL);

    // Create and compile the kernel program
    program = clCreateProgramWithSource(context, 1, &kernelSource, NULL, NULL);
    clBuildProgram(program, 1, &device, NULL, NULL, NULL);

    // Create the kernel
    kernel = clCreateKernel(program, "work", NULL);

    // Execute the kernel
        // Define global and local work sizes
    size_t global_work_size = 16; // Total number of work-items
    size_t local_work_size = 4;   // Work-items per work-group

    // Enqueue the kernel with specified work sizes
    clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_work_size, &local_work_size, 0, NULL, NULL);

    // Wait for completion
    clFinish(queue);

    // Cleanup
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    return 0;
}
