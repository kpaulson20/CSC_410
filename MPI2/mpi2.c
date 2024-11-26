// MPI Barrier

#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    int rank, size;

    // Initialize MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    printf("Process %d: Before the barrier\n", rank);
    fflush(stdout);  

    // Simulate varying work times
    if (rank == 0) {
        printf("Process %d doing some work...\n", rank);
        sleep(5); 
    }

    MPI_Barrier(MPI_COMM_WORLD);

    printf("Process %d: After the barrier\n", rank);
    fflush(stdout);

    MPI_Finalize();

    return 0;
}
