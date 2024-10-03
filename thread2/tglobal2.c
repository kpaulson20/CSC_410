// Really see the problem by introducing delays

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 5

int global = 0;

void *func (void *arg)
{
    long tID;
    int temp;
    tID = (long) arg;
    printf("Hi from %ld\n", tID);
    printf("Thread %ld sees global as ---> %d\n", tID, global);
    sleep(rand()%5);

    // To really see the problem
    sleep(rand()%5);
    temp = global;           
    temp = temp + 10;
    sleep(rand()%5);
    global = temp;

    
    printf("Thread %ld sees global final as %d\n", tID, global);

    pthread_exit(NULL);
}

int main()
{
    pthread_t thread_ID[NUM_THREADS];
    void *status;
    global = 10;
    
    srand(time(NULL));

    for (long i = 0; i < NUM_THREADS; i++)
    {
        printf("Creating thread %ld\n", i);
        pthread_create(&thread_ID[i], NULL, func, (void *)i);
    }
    
    printf("All threads created!\n");

    for (long i = NUM_THREADS - 1; i >= 0; i--)
    {
        pthread_join(thread_ID[i], &status);
        printf("Thread %ld joined.\n", i);
    }

    printf("All threads joined.\n");
    printf("Finally in main(), global is ===> %d\n", global);

    return 0;
}