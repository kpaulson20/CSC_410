#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <mpi.h>

void sieveOfEratosthenesMPI(int n) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Get the start time
    double start_time = MPI_Wtime();

    // Calculate the range of numbers each process will handle
    int local_n = n / size;
    int start = rank * local_n + 2;  // We start at 2, the first prime
    int end = (rank + 1) * local_n + 1;

    if (rank == size - 1) {
        // The last process takes the remainder
        end = n;
    }

    // Ensure the last process gets the correct segment
    if (rank == size - 1) {
        local_n = n - rank * (n / size); // The last process gets the remaining elements
    }

    // Allocate local prime array
    int *is_prime_local = (int *)malloc(local_n * sizeof(int));  // Use local_n to allocate memory

    // Initially assume all numbers are prime (represented as 1 for true)
    for (int i = 0; i < local_n; i++) {
        is_prime_local[i] = 1;  // 1 means prime
    }

    // We only need to mark multiples of primes that are <= sqrt(n)
    int limit = (int)sqrt(n) + 1;

    // For each prime number p <= sqrt(n), mark multiples in each local range
    for (int p = 2; p <= limit; p++) {
        // Broadcast the prime number p to all processes
        MPI_Bcast(&p, 1, MPI_INT, 0, MPI_COMM_WORLD);

        // Calculate the first multiple of p in the local range
        int first_multiple = start + (p - start % p) % p;
        if (first_multiple < p * p) {
            first_multiple = p * p; // Start marking from p^2 if smaller multiples are already marked
        }

        // Mark multiples of p in the local range
        for (int i = first_multiple; i <= end; i += p) {
            if (i >= start && i <= end) {
                is_prime_local[i - start] = 0;  // 0 means not prime
            }
        }
    }

    // Gather the results to process 0
    int *is_prime_global = NULL;
    if (rank == 0) {
        // Allocate enough space for all the elements (n + 1)
        is_prime_global = (int *)malloc((n + 1) * sizeof(int));
    }

    // Gather the local results to process 0
    MPI_Gather(is_prime_local, local_n, MPI_INT, is_prime_global, local_n, MPI_INT, 0, MPI_COMM_WORLD);

    // Print primes in process 0
    if (rank == 0) {
        printf("Prime numbers up to %d are:\n", n);
        for (int i = 2; i <= n; i++) {
            if (is_prime_global[i]) {
                printf("%d ", i);
            }
        }
        printf("\n");

        free(is_prime_global);
    }

    // Free the local memory
    free(is_prime_local);

    // Get the end time and calculate the elapsed time
    double end_time = MPI_Wtime();
    double elapsed_time = end_time - start_time;

    // Print the elapsed time in the root process (rank 0)
    if (rank == 0) {
        printf("Elapsed time: %.4f seconds\n", elapsed_time);
    }
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int n = 10000000; // Change the range as needed
    sieveOfEratosthenesMPI(n);

    MPI_Finalize();
    return 0;
}
