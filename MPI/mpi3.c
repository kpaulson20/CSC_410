// Basics of MPI communication - send and recieve
// MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_Status

#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv) {
    int rank, size;
    MPI_Status status;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        int data = 10;    

        // Send tasks to all worker processes
        for (int i = 1; i <= size - 1; i++) {
            MPI_Send(&data, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            printf("Process 0 sent task to process %d\n", i);
            sleep(1);
        }

        // Receive any message (using MPI_ANY_TAG) from any process (using MPI_ANY_SOURCE)
        for (int i = 1; i <= size - 1; i++) {
            int result;
            MPI_Recv(&result, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            printf("Process 0 received result %d from process %d with tag %d\n", result, status.MPI_SOURCE, status.MPI_TAG);
        }

    } else {
        int recv_data;

        MPI_Recv(&recv_data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        printf("Process %d received task with data %d\n", rank, recv_data);

        // Do some work and send back the result to process 0
        recv_data += rank;  
        MPI_Send(&recv_data, 1, MPI_INT, 0, rank, MPI_COMM_WORLD);  // Tag is set to rank
    }

    MPI_Finalize();
    return 0;
}
