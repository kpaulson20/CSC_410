// Using barriers

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define NUM_THREADS 5

pthread_barrier_t barrier;

void* func(void* arg) {
    int id = *((int*)arg);
    printf("Thread %d: Working on phase 1.\n", id);
    sleep(rand() % 3);
    printf("Thread %d: Done working on phase 1.\n", id);
    
    pthread_barrier_wait(&barrier);

    printf("Thread %d: Working on phase 2.\n", id);
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS] = {1, 2, 3, 4, 5};

    srand(time(NULL));

    pthread_barrier_init(&barrier, NULL, NUM_THREADS);

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, func, &thread_ids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_barrier_destroy(&barrier);

    return 0;
}
