// Basics of MPI communication - send and recieve
// A STAR Process

#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv) {
    int size, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Send and Recieve some data
    if (rank == 0)
    {
        int val = 1;
        printf("Process 0 initially had %d\n", val);
        // for (int i = 1; i < size; i++)
        // {
        //     MPI_Send(&val, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        //     MPI_Recv(&val, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        // }
        // printf("Process 0 Final value is %d\n", val); 

        for (int i = 1; i < size; i++)
            MPI_Send(&val, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        sleep(1);
        for (int i = 1; i < size; i++) {
            MPI_Recv(&val, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Process 0 received %d from process %d\n", val, i); 
        }

    }
    else
    {
        int val;
        MPI_Recv(&val, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process %d recieved %d\n", rank, val);
        val *= 2;
        MPI_Send(&val, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    
    return 0;
}
