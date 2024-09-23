#include <stdio.h>
#include <pthread.h>

#define SIZE 1000000
#define NUM_THREADS 3

int arr[SIZE];
int partialSums[NUM_THREADS] = {0}; // Array to store partial sums for each thread

// Entry function for each thread
void* sumPart(void* arg) {
    // Divide the work for each thread based on their id and let them compute partial sums
    // -----> Write your code here
    
}

int main() {
    // Initialize the array
    for (int i = 0; i < SIZE; i++) {
        arr[i] = i + 1; 
    }

    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    // Create threads to compute partial sums
    // ------> Write your code here
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, sumPart, (void *)&thread_ids[i]);
    }

    // Wait for all threads to finish
    // -------> Write your code here
    for (int i = 0; i < NUM_THREADS; i++){
        pthread_join(threads[i], NULL);
    }
        
    // Combine the partial sums from all threads
    int totalSum = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        totalSum += partialSums[i];
    }

    printf("Total Sum: %d\n", totalSum);

    return 0;
}
