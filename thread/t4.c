// Create multiple threads now

#include <stdio.h>
#include <pthread.h>

void *func(void *arg){
    printf("Hello there!\n");
    return NULL;
}

int main()
{
    pthread_t thread_ID[4];
    
    printf("Before threads\n");
    for (int i = 0; i < 4; i++)
        pthread_create(&thread_ID[i], NULL, func, NULL);

    printf("After create\n");

    for (int i = 0; i < 4; i++)    
        pthread_join(thread_ID[i], NULL);

    printf("After threads\n");

    return 0;
}