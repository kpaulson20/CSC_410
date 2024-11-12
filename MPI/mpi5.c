// Basics of MPI communication - send and recieve
// Send and Receive a multiple data at once

#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

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
    if (rank == 0)
    {
        int data[10];
        for (int i = 0; i < 10; i++)
            data[i] = i;

        printf("Sending an array of 10 numbers\n");
        MPI_Send(&data, 10, MPI_INT, 1, 0, MPI_COMM_WORLD); 
        sleep(1);
    }
    else if (rank == 1) 
    {
        int recv_data[10];
        MPI_Recv(&recv_data, 10, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for (int i = 0; i < 10; i++) 
            printf("Recieved %d\n", recv_data[i]);
    }

    MPI_Finalize();
    
    return 0;
}
