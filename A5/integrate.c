#include <stdio.h>
#include <mpi.h>

#define N 1000000000 // Total number of intervals

// Function to integrate
double f(double x) {
    return 4.0 / (1.0 + x * x); // Function to integrate (approximation of pi)
}

double trapezoidalRule(int start, int end, double h) {
    double sum = 0.0;
    for (int i = start; i < end; ++i) {
        sum += f(i * h);
    }
    return sum;
}

int main(int argc, char** argv) {
    int rank, size;
    double pi, local_sum = 0.0, global_sum = 0.0;
    double h = 1.0 / N; // Step size

    // Initialize MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double start_time = MPI_Wtime();

    // Divide the work: each process handles a portion of the total intervals
    int local_n = N / size;  
    int start = rank * local_n;  
    int end = (rank == size - 1) ? N : (rank + 1) * local_n;

    // Compute the local sum
    local_sum = trapezoidalRule(start, end, h);

    // Reduce all local sums to the global sum
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        global_sum += 0.5 * (f(0) + f(1));  // Account for the f(0) and f(1) terms
        global_sum *= h; 
        printf("Estimated value of pi: %f\n", global_sum);
    }

    double end_time = MPI_Wtime();
    if (rank == 0) {
        double elapsed_time = end_time - start_time;
        printf("Time taken: %f seconds\n", elapsed_time);
    }

    MPI_Finalize();
    
    return 0;
}
