#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <pthread.h>

typedef struct {
    int *board;
    int col;
    int n;
    pthread_mutex_t *mutex;
} ThreadData;

bool isSafe(int board[], int row, int col, int n) {
    for (int i = 0; i < col; i++) {
        if (board[i] == row || abs(board[i] - row) == abs(i - col)) {
            return false;
        }
    }
    return true;
}
void *solveNQueensUtil(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int *board = data->board;
    int col = data->col;
    int n = data->n;
    pthread_mutex_t *mutex = data->mutex;

    for (int i = 0; i < n; i++) {
        if (isSafe(board, i, col, n)) {
            board[col] = i;

            pthread_mutex_lock(mutex);

            if (col + 1 >= n){
                for (int j = 0; j < n; j++){
                    printf("%d", board[j]);
                }
                printf("\n");
                pthread_mutex_unlock(mutex);
            } else {
                ThreadData new_data = { .board = board, .col = col + 1, .n = n, .mutex = mutex };
                pthread_t thread;
                pthread_create(&thread, NULL, solveNQueensUtil, &new_data);
                pthread_join(thread, NULL);
            }
        }
    }
    return NULL;
    // Recursive case
    // write your code here
    //for (int i = 0; i < n; i++){
    //    if (isSafe(board, i, col, n)){
    //        board[col] = i;
    //        solveNQueensUtil(board, col + 1, n);
    //    }
    //}
}