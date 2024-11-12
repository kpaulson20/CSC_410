// MPI collective communication
// Reduce

#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int rank, size;
    int send_data;
    int result;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Each process sets its send_data to its rank + 1
    send_data = rank + 1;
    printf("Process %d has data %d\n", rank, send_data);

    // Perform the reduction operation (sum) across all processes
    MPI_Reduce(&send_data, &result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Root process prints the result of the reduction
    if (rank == 0) {
        printf("Sum of all processes data is: %d\n", result);
    }

    MPI_Finalize();
    return 0;
}
