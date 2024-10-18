#include "nqueens_Pthread.h"

int main() {
    int n = 8;
    int *board = (int*)malloc(n * sizeof(int));
    pthread_mutex_t mutex;

    if (board == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < n; i++) {
        board[i] = -1;
    }

    for (int i = 0; i < n; i++) {
        board[0] = i;

        ThreadData data = { .board = board, .col = 0, .n = n, .mutex = &mutex };
        pthread_t thread;
        pthread_create(&thread, NULL, solveNQueensUtil, &data);
        pthread_detach(thread);
    }

    pthread_mutex_destroy(&mutex);
    free(board);
    return 0;
}
