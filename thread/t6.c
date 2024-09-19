// Create multiple threads with join

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define ThreadNum 4

void *func(void *arg){
    long tID;
    tID = (long) arg;
    printf("Hello from thread %ld!\n", tID);
    pthread_exit(NULL);
}

int main()
{
    long i;
    void *status;
    pthread_t thread_ID[ThreadNum];
    
    printf("Before threads\n");
    for (i = 0; i < ThreadNum; i++)
    {
        printf("Creating thread %ld \n", i);
        pthread_create(&thread_ID[i], NULL, func, (void *)i);
    }

    printf("After create\n");

    for (i = 0; i < ThreadNum; i++)
    {
        pthread_join(thread_ID[i], &status);
        printf("Joining thread %ld \n", i);
    }

    printf("After all threads joined.\n");

    return 0;
}