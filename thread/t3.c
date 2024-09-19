// Return a result from the thread now

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


void *square(void *arg) {
    int *num = (int *)arg;
    int *result = malloc(sizeof(int));
    *result = (*num) * (*num);
    pthread_exit(result); // returns a result when joining
}

int main() {
    pthread_t thread1;
    int value = 5;
    void *status;

    pthread_create(&thread1, NULL, square, (void *)&value);

    
    // Wait for the thread to finish and get the result
    pthread_join(thread1, &status);

    // Process the result returned by the thread
    int *result = (int *)status;
    printf("Square of %d is %d\n", value, *result);
    free(result);

    return 0;
}
