// MPI collective communication
// Scatter

#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int rank, size;
    const int elements_per_process = 3;
    int send_data[elements_per_process * 4];  // Assuming we have 4 processes
    int recv_data[elements_per_process];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        // Initialize the data to be scattered in the root process
        for (int i = 0; i < elements_per_process * size; i++) {
            send_data[i] = i + 1; 
        }
        printf("Root process prepared data for scattering: ");
        for (int i = 0; i < elements_per_process * size; i++) {
            printf("%d ", send_data[i]);
        }
        printf("\n");
    }

    // Scatter the data to all processes
    MPI_Scatter(send_data, elements_per_process, MPI_INT, recv_data, elements_per_process, MPI_INT, 0, MPI_COMM_WORLD);

    // Each process prints the received chunk of data
    printf("Process %d received data: ", rank);
    for (int i = 0; i < elements_per_process; i++) {
        printf("%d ", recv_data[i]);
    }
    printf("\n");

    MPI_Finalize();
    return 0;
}
