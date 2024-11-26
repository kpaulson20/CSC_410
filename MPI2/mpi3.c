// MPI_Wtime for timing

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Start time
    double start_time = MPI_Wtime(); 

    // Simulate some computation
    int num_elements = 1000000;
    int *data = (int*)malloc(num_elements * sizeof(int));

    // Each process will perform a computation on a portion of the data
    for (int i = 0; i < num_elements; ++i) {
        data[i] = i + rank;  // Simple computation
    }

    // Simulate some delay
    MPI_Barrier(MPI_COMM_WORLD);  
    
    // End time
    double end_time = MPI_Wtime();

    // Print the time taken by each process
    printf("Process %d: Time taken: %f seconds\n", rank, end_time - start_time);

    free(data);

    MPI_Finalize();

    return 0;
}
