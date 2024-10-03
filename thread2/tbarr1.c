// Example that needs barriers

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define NUM_THREADS 5

void* func(void* arg) {
    int id = *((int*)arg);
    printf("Thread %d: Working on phase 1.\n", id);
    sleep(rand() % 3);
    printf("Thread %d: Done working on phase 1.\n", id);    

    // // Manually synchronize (inefficient)
    // sleep(2);  // Wait until all threads finish phase 1

    printf("Thread %d: Working on phase 2.\n", id);
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS] = {1, 2, 3, 4, 5};

    srand(time(NULL));

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, func, &thread_ids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
