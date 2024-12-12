// OpenCL for N-body simulation

#define CL_TARGET_OPENCL_VERSION 120
#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define G 6.67430e-11f  
#define NUM_BODIES 1000    
#define DT (60 * 60 * 24)     
#define MIN_DISTANCE 1e3f // Minimum allowed distance (to avoid singularity)
#define MAX_FORCE 1e20f   // Maximum allowable force (to prevent overflow)

typedef struct {
    float x, y;
    float vx, vy;
    float mass;
} Body;

// OpenCL kernel to compute the gravitational forces between bodies
const char *kernel_source = R"(
typedef struct {
    float x, y;
    float vx, vy;
    float mass;
} Body;

__kernel void compute_forces(__global const Body *bodies, __global float *fx, __global float *fy, const int num_bodies) {
    int i = get_global_id(0);
    if (i >= num_bodies) return;

    float fx_i = 0.0f;
    float fy_i = 0.0f;

    for (int j = 0; j < num_bodies; j++) {
        if (i != j) {
            float dx = bodies[j].x - bodies[i].x;
            float dy = bodies[j].y - bodies[i].y;
            float distance_sq = dx * dx + dy * dy;

            // Clamp the minimum distance to avoid singularities
            if (distance_sq < 1e6f) distance_sq = 1e6f;

            float distance = sqrt(distance_sq);

            // Compute force magnitude with clamping
            float force_magnitude = (6.67430e-11f * bodies[i].mass * bodies[j].mass) / distance_sq;
            if (force_magnitude > 1e20f) force_magnitude = 1e20f; // Cap the force magnitude

            fx_i += force_magnitude * dx / distance;
            fy_i += force_magnitude * dy / distance;
        }
    }

    fx[i] = fx_i;
    fy[i] = fy_i;
}
)";

// OpenCL kernel to update the positions and velocities of bodies
const char *update_kernel_source = R"(
typedef struct {
    float x, y;
    float vx, vy;
    float mass;
} Body;

__kernel void update_bodies(__global Body *bodies, __global const float *fx, __global const float *fy, const float dt, const int num_bodies) {
    int i = get_global_id(0);
    if (i >= num_bodies) return;

    // Update velocities
    bodies[i].vx += fx[i] / bodies[i].mass * dt;
    bodies[i].vy += fy[i] / bodies[i].mass * dt;

    // Update positions
    bodies[i].x += bodies[i].vx * dt;
    bodies[i].y += bodies[i].vy * dt;
}
)";

int main() {
    
    Body bodies[NUM_BODIES];

    // Initializing position, velocity, and mass for each body
    for (int i = 0; i < NUM_BODIES; i++) {
        bodies[i].x = rand() % 1000000000; 
        bodies[i].y = rand() % 1000000000;  
        bodies[i].vx = (rand() % 100 - 50) * 1e3;  
        bodies[i].vy = (rand() % 100 - 50) * 1e3;  
        bodies[i].mass = (rand() % 100 + 1) * 1e24;  
    }

    cl_int err;

    // OpenCL setup
    cl_platform_id platform;
    clGetPlatformIDs(1, &platform, NULL);

    cl_device_id device;
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);

    cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
    cl_command_queue queue = clCreateCommandQueue(context, device, 0, &err);


    // Create OpenCL buffers
    // complete this
    size_t bytes = NUM_BODIES * sizeof(Body);
    cl_mem buffer_bodies = clCreateBuffer(context, CL_MEM_READ_WRITE, bytes, NULL, &err);
    cl_mem buffer_fx = clCreateBuffer(context, CL_MEM_WRITE_ONLY, NUM_BODIES * sizeof(float), NULL, &err);
    cl_mem buffer_fy = clCreateBuffer(context, CL_MEM_WRITE_ONLY, NUM_BODIES * sizeof(float), NULL, &err);

    // Create and build OpenCL program for the kernels
    // complete this
    const char *sources[2] = {kernel_source, update_kernel_source};
    cl_program program = clCreateProgramWithSource(context, 2, sources, NULL, &err);
    clBuildProgram(program, 1, &device, NULL, NULL, NULL);

    cl_kernel compute_kernel = clCreateKernel(program, "compute_forces", &err);
    cl_kernel update_kernel = clCreateKernel(program, "update_bodies", &err);

    // Set kernel arguments for compute forces kernel
    clSetKernelArg(compute_kernel, 0, sizeof(cl_mem), &buffer_bodies);
    clSetKernelArg(compute_kernel, 1, sizeof(cl_mem), &buffer_fx);
    clSetKernelArg(compute_kernel, 2, sizeof(cl_mem), &buffer_fy);
    clSetKernelArg(compute_kernel, 3, sizeof(int), &NUM_BODIES);

    // Set kernel arguments for update positions kernel
    clSetKernelArg(update_kernel, 0, sizeof(cl_mem), &buffer_bodies);
    clSetKernelArg(update_kernel, 1, sizeof(cl_mem), &buffer_fx);
    clSetKernelArg(update_kernel, 2, sizeof(cl_mem), &buffer_fy);
    clSetKernelArg(update_kernel, 3, sizeof(float), &DT);
    clSetKernelArg(update_kernel, 4, sizeof(int), &NUM_BODIES);

    // Main simulation loop
    for (int step = 0; step < 1000; step++) {
        // complete this
        // Write bodies data to the buffer
        clEnqueueWriteBuffer(queue, buffer_bodies, CL_TRUE, 0, bytes, bodies, 0, NULL, NULL);

        // Execute the compute forces kernel
        size_t global_work_size = NUM_BODIES;
        clEnqueueNDRangeKernel(queue, compute_kernel, 1, NULL, &global_work_size, NULL, 0, NULL, NULL);
        clFinish(queue);

        // Execute the update positions kernel
        clEnqueueNDRangeKernel(queue, update_kernel, 1, NULL, &global_work_size, NULL, 0, NULL, NULL);
        clFinish(queue);
    }

    // Get result
    // complete this
    clEnqueueReadBuffer(queue, buffer_bodies, CL_TRUE, 0, bytes, bodies, 0, NULL, NULL);

    // Print final positions and velocities
    for (int i = 0; i < NUM_BODIES; i++) {
        printf("Body %d: Position = (%.2f, %.2f), Velocity = (%.2f, %.2f)\n", 
               i, bodies[i].x, bodies[i].y, bodies[i].vx, bodies[i].vy);
    }

    // Clean up OpenCL resources
    clReleaseMemObject(buffer_bodies);
    clReleaseMemObject(buffer_fx);
    clReleaseMemObject(buffer_fy);
    clReleaseKernel(kernel);
    clReleaseKernel(update_kernel);
    clReleaseProgram(program);
    clReleaseProgram(update_program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    return 0;
}
