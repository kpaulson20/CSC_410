// Non-blocking send and receive 
// More efficient for overlapping communication and reduces idle time.

#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int data = 5;
    MPI_Request request;
    MPI_Status status;

    // Non-blocking send from process 0 to process 1
    if (rank == 0) {
        MPI_Isend(&data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);
        printf("Process 0 sent data %d (non-blocking)\n", data);
        MPI_Wait(&request, &status);  // Ensure the send completes
    } else if (rank == 1) {
        MPI_Irecv(&data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);
        MPI_Wait(&request, &status);  // Wait for the receive to complete
        printf("Process 1 received data %d (non-blocking)\n", data);
    }

    MPI_Finalize();
    return 0;
}
