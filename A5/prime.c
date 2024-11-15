#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <mpi.h>

void sieveOfEratosthenes(int n) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Time measurement
    double start_time = MPI_Wtime();

    bool *is_prime = NULL;
    bool *local_is_prime = NULL;
    int range_size;

    // Step 1: Root process initializes the sieve
    if (rank == 0) {
        is_prime = (bool *)malloc((n + 1) * sizeof(bool));
        for (int i = 0; i <= n; i++) {
            is_prime[i] = true;
        }
        is_prime[0] = is_prime[1] = false;
    }

    // Step 2: Broadcast the size of the sieve to all processes
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Step 3: Calculate local range for each process
    range_size = (n + size - 1) / size;  // How much work each process should do
    int start = rank * range_size;
    int end = (rank + 1) * range_size - 1;
    if (end > n) {
        end = n;
    }

    // Allocate local array for each process
    local_is_prime = (bool *)malloc((end - start + 1) * sizeof(bool));
    for (int i = 0; i <= (end - start); i++) {
        local_is_prime[i] = true;
    }

    // Step 4: Find primes up to sqrt(n) in the root process
    if (rank == 0) {
        for (int p = 2; p <= sqrt(n); p++) {
            if (is_prime[p]) {
                for (int i = p * p; i <= n; i += p) {
                    is_prime[i] = false;
                }
            }
        }
    }

    // Step 5: Broadcast the primes up to sqrt(n) to all processes
    MPI_Bcast(is_prime, n + 1, MPI_BYTE, 0, MPI_COMM_WORLD);

    // Step 6: Mark non-prime numbers in the local range
    for (int p = 2; p <= sqrt(n); p++) {
        if (is_prime[p]) {
            // Find first multiple of p within range
            int local_start = start + (p - start % p) % p;
            if (local_start == p) local_start = p * p;
            for (int i = local_start; i <= end; i += p) {
                if (i != p) {
                    local_is_prime[i - start] = false;
                }
            }
        }
    }

    // Step 7: Gather all results at root process
    MPI_Gather(local_is_prime, range_size, MPI_BYTE, is_prime, range_size, MPI_BYTE, 0, MPI_COMM_WORLD);

    // Step 8: Root process prints the results
    if (rank == 0) {
        printf("Prime numbers up to %d are:\n", n);
        for (int i = 2; i <= n; i++) {
            if (is_prime[i]) {
                printf("%d ", i);
            }
        }
        printf("\n");
        free(is_prime);
    }

    free(local_is_prime);

    // Time measurement
    double end_time = MPI_Wtime();
    if (rank == 0) {
        printf("Time taken: %f seconds\n", end_time - start_time);
    }
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int n = 10000000;
    sieveOfEratosthenes(n);

    MPI_Finalize();
    return 0;
}
