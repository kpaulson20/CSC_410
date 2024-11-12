// Basics of MPI communication - send and recieve
// A RING Process

#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv) {
    int size, rank;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Make sure to have only 2 processes
    if (size != 5) 
    {
        printf("This program requires only 5 processes\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // Send and Recieve some data
    if (rank == 0)
    {
        int val = 1;
        MPI_Send(&val, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        printf("Process 0 initially had %d\n", val);
        sleep(1);
        MPI_Recv(&val, 1, MPI_INT, 4, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 0 Final value is %d\n", val); 
    }
    else
    {
        int val;
        MPI_Recv(&val, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process %d recieved %d\n", rank, val);
        val *= 2;
        MPI_Send(&val, 1, MPI_INT, (rank + 1) % 5, 0, MPI_COMM_WORLD);
    }


    MPI_Finalize();
    
    return 0;
}
