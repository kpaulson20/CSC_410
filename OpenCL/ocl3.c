// write and build a simple kernel - Hello world
// work-items

#define CL_TARGET_OPENCL_VERSION 120 
#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h>

// OpenCL kernel code as a string
const char *kernelSource = R"(
__kernel void helloWorld() {
    int global_id = get_global_id(0);
    printf("Global ID %d: Hello, OpenCL World!\n", global_id);
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
    kernel = clCreateKernel(program, "helloWorld", NULL);

    // Execute the kernel
    clEnqueueTask(queue, kernel, 0, NULL, NULL);

    // size_t globalWorkSize = 5; // 5 threads
    // clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &globalWorkSize, NULL, 0, NULL, NULL);
    clFinish(queue);


    // Cleanup
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    return 0;
}
