// Basics of MPI communication - send and recieve
// MPI_Get_count, MPI_Probe

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char** argv) {
    int rank, size;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        // Send variable-length data to each process
        for (int i = 1; i <= size - 1; i++) {
            int data_size = i * 2;
            int* data = (int*)malloc(data_size * sizeof(int));
            
            for (int j = 0; j < data_size; j++) {
                data[j] = i * 10 + j; 
            }

            MPI_Send(data, data_size, MPI_INT, i, 0, MPI_COMM_WORLD);
            printf("Process 0 sent %d elements to process %d\n", data_size, i);
            free(data);
            sleep(1);
        }

    } else {
        // Probe for an incoming message from the master to get its size
        MPI_Probe(0, 0, MPI_COMM_WORLD, &status);

        // Determine the number of elements in the incoming message
        int count;
        MPI_Get_count(&status, MPI_INT, &count);
        printf("Process %d detected incoming data with %d elements\n", rank, count);

        // Allocate buffer to receive the message
        int* buffer = (int*)malloc(count * sizeof(int));

        // Receive the message
        MPI_Recv(buffer, count, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        printf("Process %d received data:", rank);
        for (int i = 0; i < count; i++) {
            printf(" %d", buffer[i]);
        }
        printf("\n");

        free(buffer);
    }

    MPI_Finalize();
    return 0;
}
