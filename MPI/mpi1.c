// Intro to MPI

#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    
    // Initializing MPI environment
    MPI_Init(&argc, &argv);

    // Get the number of processes
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Get the rank of the process
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // // Get processor name
    // char processor_name[MPI_MAX_PROCESSOR_NAME];
    // int name_len;
    // MPI_Get_processor_name(processor_name, &name_len);

    // Print a hello message from each process
    printf("Hello from processor rank %d out of %d processors\n", rank, size);

    // Finalizing MPI environment
    MPI_Finalize();
    
    return 0;
}
