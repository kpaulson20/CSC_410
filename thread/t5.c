// Create multiple threads part 2

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define ThreadNum 4

void *func(void *arg){
    long tID;
    tID = (long) arg;
    //sleep(tID * 10 % (tID+1));
    printf("Hello from thread %ld!\n", tID);
    pthread_exit(NULL);
}

int main()
{
    long i;
    pthread_t thread_ID[ThreadNum];
    
    printf("Before threads\n");
    for (i = 0; i < ThreadNum; i++)
    {
        printf("Creating thread %ld \n", i);
        pthread_create(&thread_ID[i], NULL, func, (void *)i);
    }

    printf("After create\n");

    pthread_exit(NULL);
    printf("Exiting...\n");

    return 0;
}