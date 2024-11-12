// Basics of MPI communication - send and recieve
// Send and Receive a single data

#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int size, rank;
    
    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Make sure to have only 2 processes
    if (size != 2) 
    {
        printf("This program requires only 2 processes\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // Send and Recieve some data
    int data;
    if (rank == 0)
    {
        data = 5;
        MPI_Send(&data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD); 
    }
    else if (rank == 1) {
        int recv_data;
        MPI_Recv(&recv_data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process rank 1, recieved %d from process rank 0\n", recv_data);
    }

    MPI_Finalize();
    
    return 0;
}
