// MPI collective communication
// Gather

#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int rank, size;
    const int elements_per_process = 2;
    int send_data[elements_per_process];
    int recv_data[elements_per_process * 4];  // Assuming 4 processes

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Each process prepares some data to send
    for (int i = 0; i < elements_per_process; i++) {
        send_data[i] = rank * 10 + i;  // Unique values per process
    }

    // Gather the data from all processes into the root process
    MPI_Gather(send_data, elements_per_process, MPI_INT, recv_data, elements_per_process, MPI_INT, 0, MPI_COMM_WORLD);

    // Root process prints the gathered data
    if (rank == 0) {
        printf("Root process gathered data: ");
        for (int i = 0; i < elements_per_process * size; i++) {
            printf("%d ", recv_data[i]);
        }
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}
