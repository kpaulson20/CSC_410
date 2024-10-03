// Fixing the problem with mutex lock

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 5

int global = 0;
pthread_mutex_t lock;   // Using mutex

void *func (void *arg)
{
    long tID;
    int temp;
    tID = (long) arg;
    printf("Hi from %ld\n", tID);
    printf("Thread %ld sees global as ---> %d\n", tID, global);
    sleep(rand()%5);

    pthread_mutex_lock(&lock); // LOCK
    sleep(rand()%5);
    temp = global;           
    temp = temp + 10;
    sleep(rand()%5);
    global = temp;
    pthread_mutex_unlock(&lock); // UNLOCK
    
    printf("Thread %ld sees global final as %d\n", tID, global);

    pthread_exit(NULL);
}

int main()
{
    pthread_t thread_ID[NUM_THREADS];
    void *status;
    global = 10;
    
    srand(time(NULL));
    pthread_mutex_init(&lock, NULL);  // Allocate space for lock with initial value of unlock status

    for (long i = 0; i < NUM_THREADS; i++)
    {
        printf("Creating thread %ld\n", i);
        pthread_create(&thread_ID[i], NULL, func, (void *)i);
    }
    
    printf("All threads created!\n");

    for (long i = NUM_THREADS-1; i >= 0; i--)
    {
        pthread_join(thread_ID[i], &status);
        printf("Thread %ld joined.\n", i);
    }

    printf("All threads joined.\n");
    printf("Finally in main(), global is ===> %d\n", global);

    pthread_mutex_destroy(&lock);

    return 0;
}