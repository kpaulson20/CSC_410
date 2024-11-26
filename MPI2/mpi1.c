// Local and External variables in MPI

#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

// External variable but each process has its own instance
int global_data = 42;

int main(int argc, char* argv[]) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Local variable specific to each process
    int local_data = rank + 1;


    printf("Initial value -> Process %d: local_data = %d, global_data = %d\n", rank, local_data, global_data);
    sleep(1);
    
    // Modify local and external variables differently in each process
    local_data *= 2;
    global_data += rank;

    printf("Updated value -> Process %d: local_data = %d, global_data = %d\n", rank, local_data, global_data);
    sleep(1);
    
    // Synchronize using MPI_Bcast
    if (rank == 0) {
        global_data = 100;  
    }
    MPI_Bcast(&global_data, 1, MPI_INT, 0, MPI_COMM_WORLD);

    printf("After communication -> Process %d: local_data = %d, global_data = %d\n", rank, local_data, global_data);

    MPI_Finalize();

    return 0;
}
