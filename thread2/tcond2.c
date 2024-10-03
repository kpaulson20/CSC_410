// Using condition variables

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>


#define NUM_THREADS 5
int workers_ready = 0; // Number of workers that are done

pthread_mutex_t mutex;
pthread_cond_t cond;

void* work(void* arg) {
    int id = *((int*)arg);
    sleep(rand() % 3);  
    printf("Worker %d: Done.\n", id);

    pthread_mutex_lock(&mutex);
    workers_ready++;
    if (workers_ready == NUM_THREADS) {
        pthread_cond_signal(&cond);
    }
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void* tally_work(void* arg) {
    pthread_mutex_lock(&mutex);
    while(workers_ready < NUM_THREADS) {
        pthread_cond_wait(&cond, &mutex);
    }
    printf("All work is done. Proceeding with tallying.\n");
    printf("Total work done: %d\n", workers_ready);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main() {
    pthread_t workers[5], tally_thread;
    int worker_ids[NUM_THREADS] = {1, 2, 3, 4, 5};

    srand(time(NULL));

    // Create worker threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&workers[i], NULL, work, &worker_ids[i]);
    }

    // Create tally thread
    pthread_create(&tally_thread, NULL, tally_work, NULL);

    // Join all threads
    for (int i = 0; i < 5; i++) {
        pthread_join(workers[i], NULL);
    }
    pthread_join(tally_thread, NULL);

    // Clean up
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}
