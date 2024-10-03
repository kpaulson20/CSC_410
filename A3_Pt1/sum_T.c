#include <stdio.h>
#include <pthread.h>

#define SIZE 1000000
#define NUM_THREADS 15

int arr[SIZE];
long long totalSum;
pthread_mutex_t lock;

// Entry function for each thread
void* sumPart(void* arg) {
    int thread_id = *(int*)arg;
    int chunk = SIZE / NUM_THREADS;
    int index_start = thread_id * chunk;
    int index_end = (thread_id == NUM_THREADS - 1) ? SIZE : (thread_id +1) * chunk;

    //Calculate local sum for thread
    long long local = 0;
    for (int i = index_start; i < index_end; i++){
        local += arr[i];
    }

    //Lock mutex
    pthread_mutex_lock(&lock);
    totalSum += local;
    pthread_mutex_unlock(&lock);

    pthread_exit(NULL);
}

int main() {
    // Initialize the array
    for (int i = 0; i < SIZE; i++) {
        arr[i] = i + 1; 
    }

    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    // Initialize mutex
    pthread_mutex_init(&lock, NULL);

    // Create threads to compute partial sums
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, sumPart, (void *)&thread_ids[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++){
        pthread_join(threads[i], NULL);
    }

    printf("Total Sum: %lld\n", totalSum);

    // Destroy mutex
    pthread_mutex_destroy(&lock);

    return 0;
}
