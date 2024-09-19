// Create a simple thread

#include <stdio.h>
#include <pthread.h>

void *func(void *arg)
{
    printf("Hello!\n");
    return NULL;
}

int main()
{
    pthread_t thread1;

    printf("Before thread\n");

    pthread_create(&thread1, NULL, func, NULL);
    // if (pthread_create(&thread1, NULL , func, NULL) != 0){
    //     perror("Thread creation failed.\n");
    //     return 1;
    // }

    printf("After create\n");

    pthread_join(thread1, NULL);

    printf("After thread\n");

    return 0;
}