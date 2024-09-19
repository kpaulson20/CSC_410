// Create multiple threads with return value

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define ThreadNum 7

void *func(void *arg){
    long tID;
    tID = (long) arg;

    long thread_ret = 15 * tID + 3;

    printf("Hello from thread %ld!\n", tID);
    pthread_exit( (void *) thread_ret);
}

int main()
{
    long i, retNum;
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
        retNum = (long) status;
        printf("Joining thread %ld with return code %ld\n", i, retNum);
    }

    printf("After all threads joined.\n");

    return 0;
}