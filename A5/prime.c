#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <mpi.h>

void sieveOfEratosthenesMPI(int start, int end, bool *is_prime, int n) {
    // Initially assume all numbers in the range are prime (true)
    for (int i = start; i <= end; i++) {
        is_prime[i - start] = true;  // 1 means prime
    }

    if (start <= 1) is_prime[0] = false;  // Mark 0 as non-prime
    if (start <= 2) is_prime[1] = false;  // Mark 1 as non-prime

    // We only need to mark multiples of primes that are <= sqrt(n)
    int limit = (int)sqrt(n) + 1;

    // For each prime number p <= sqrt(n), mark multiples in each local range
    for (int p = 2; p <= limit; p++) {
        if (p * p > end) break; // No need to process primes above the local range
        if (p >= start && p <= end && is_prime[p - start]) {  // If p is still marked as prime in the range
            // Mark multiples of p in the local range
            for (int i = start + (p - start % p) % p; i <= end; i += p) {
                if (i >= start && i <= end) {
                    is_prime[i - start] = false;  // 0 means not prime
                }
            }
        }
    }
}

int main(int argc, char *argv[]) {
    int n = 10000000;  // The upper bound for the sieve
    int rank, size;

    // Initialize MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Get the start time
    double start_time = MPI_Wtime();

    // Calculate the range of numbers each process will handle
    int local_n = n / size;
    int remainder = n % size;

    int start = rank * local_n + 2;  // We start at 2, the first prime
    int end = (rank + 1) * local_n + 1;

    if (rank == size - 1) {
        // The last process takes the remainder
        end = n;
        local_n = n - rank * (n / size); // Adjust for the remaining numbers
    }

    // Allocate local prime array
    bool *is_prime_local = (bool *)malloc(local_n * sizeof(bool));

    // Call sieve function with the parameters (start, end, is_prime_local, n)
    sieveOfEratosthenesMPI(start, end, is_prime_local, n);

    // Gather results from all processes
    bool *is_prime_global = NULL;
    if (rank == 0) {
        // Allocate enough space for all the elements (n + 1)
        is_prime_global = (bool *)malloc((n + 1) * sizeof(bool));
    }

    // Gather the local results to process 0
    MPI_Gather(is_prime_local, local_n, MPI_CHAR, is_prime_global, local_n, MPI_CHAR, 0, MPI_COMM_WORLD);

    // Print primes in process 0
    if (rank == 0) {
        printf("Prime numbers up to %d are:\n", n);
        for (int i = 2; i <= n; i++) {
            if (is_prime_global[i]) {
                printf("%d ", i);
            }
        }
        printf("\n");

        // Free the memory used by global_is_prime
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

    // Finalize MPI
    MPI_Finalize();
    return 0;
}
