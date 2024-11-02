#include <stdio.h>
#include <omp.h>

#define N 1000000000 // intervals

double f(double x) {
    return 4.0 / (1.0 + x * x); // Function to integrate
}

double trapezoidalRule() {
    double h = 1.0 / N; 
    double sum = 0.5 * (f(0) + f(1)); 

    #pragma omp parallel // parallelize the summation
    { 
        double localSum = 0.0;
        
        #pragma omp for // compute sum in parallel
        for (int i = 1; i < N; ++i) 
        {
            localSum += f(i * h);
        }

        #pragma omp critical // prevent race condition during update
        {
            sum += localSum;
        }
    }
    sum *= h; 
    return sum;
}

int main() {
    for (int threads = 1; threads < 10; threads++) {
        double start_time = omp_get_wtime();
        double pi = trapezoidalRule();
        double end_time = omp_get_wtime();
        printf("Threads: %d Estimated value of π: %f Time taken: %f seconds\n", threads, pi, end_time - start_time);
    }
    
    return 0;
}
