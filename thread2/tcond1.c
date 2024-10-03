// Example that needs condition variables

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>


#define NUM_THREADS 5
int workers_ready = 0; // Number of workers that are done


void* work(void* arg) {
    int id = *((int*)arg);
    sleep(rand() % 3);  
    printf("Worker %d: Done.\n", id);
    workers_ready++;
    return NULL;
}

void* tally_work(void* arg) {
    printf("All work is done. Proceeding with tallying.\n");
    printf("Total work done: %d\n", workers_ready);
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

    return 0;
}
