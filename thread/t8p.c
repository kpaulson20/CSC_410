// Do some work with threads - parallel version
// Count prime numbers

#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <time.h>

#define NUM_THREADS 4 

int range = 10000000;
int prime_counts[NUM_THREADS];  // Array to store prime counts for each thread

int is_prime(int num) {
    if (num < 2) return 0;
    for (int i = 2; i <= sqrt(num); i++) {
        if (num % i == 0) return 0;
    }
    return 1;
}

void* count_primes_thread(void* arg) {
    int thread_id = *(int*)arg;
    int start = thread_id * (range / NUM_THREADS) + 2;  // Define range start (start from 2)
    int end = (thread_id + 1) * (range / NUM_THREADS);  // Define range end
    int count = 0;

    for (int i = start; i <= end; i++) {
        if (is_prime(i)) {
            count++;
        }
    }

    prime_counts[thread_id] = count;  // Store the prime count for this thread

    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_args[NUM_THREADS];
    int primes_total = 0;

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_args[i] = i;
        pthread_create(&threads[i], NULL, count_primes_thread, &thread_args[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Combine results from all threads
    for (int i = 0; i < NUM_THREADS; i++) {
        primes_total += prime_counts[i];
    }

    printf("Total primes: %d\n", primes_total);

    return 0;
}
