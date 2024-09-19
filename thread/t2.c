// Pass some arguments now

#include <stdio.h>
#include <pthread.h>

void *func(void *arg){
    int *num = (int *)arg;  // type cast to int pointer
    printf("Hello from thread %d!\n", *num);
    return NULL;
}

int main()
{
    pthread_t thread1;
    int value1 = 1;

    pthread_create(&thread1, NULL, func, (void *)&value1);

    pthread_join(thread1, NULL);

    return 0;
}