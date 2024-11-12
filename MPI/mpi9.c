// MPI collective communication
// Broadcast

#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int rank, size;
    const int data_size = 5;
    int data[data_size];

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        for (int i = 0; i < data_size; i++) {
            data[i] = i + 1;  
        }
        printf("Process 0 broadcasting data: ");
        for (int i = 0; i < data_size; i++) {
            printf("%d ", data[i]);
        }
        printf("\n");
    }

    // Broadcast the data from process 0 to all processes
    MPI_Bcast(data, data_size, MPI_INT, 0, MPI_COMM_WORLD);

    // Each process receives the broadcasted data
    printf("Process %d received data: ", rank);
    for (int i = 0; i < data_size; i++) {
        printf("%d ", data[i]);
    }
    printf("\n");

    MPI_Finalize();
    return 0;
}
