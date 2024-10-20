#ifndef NQUEENS_H
#define NQUEENS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <pthread.h>

typedef struct {
    int *board;
    int col;
    int n;
} ThreadData_t;

bool isSafe(int board[], int row, int col, int n);
void* threadSolveNQueens(void* arg);
void solveNQueensUtil(int board[], int col, int n);
void printBoard(int board[], int n);

bool isSafe(int board[], int row, int col, int n) {
    for (int i = 0; i < col; i++) {
        if (board[i] == row || abs(board[i] - row) == abs(i - col)) {
            return false;
        }
    }
    return true;
}

void* threadSolveNQueens(void* arg) {
    ThreadData_t *data = (ThreadData_t*)arg;
    int* board = data->board;
    int col = data->col;
    int n = data->n;

    // Call the solveNQueensUtil function
    solveNQueensUtil(board, col, n);
    
    return NULL;
}

void solveNQueensUtil(int board[], int col, int n) {
    // Base case
    if (col >= n) {
        printBoard(board, n);
        return;
    }

    // Recursive case
    for (int i = 0; i < n; i++) {
        if (isSafe(board, i, col, n)) {
            board[col] = i;

            // Create a new thread for the next column
            ThreadData_t *newData = malloc(sizeof(ThreadData_t));
            newData->board = board;
            newData->col = col + 1;
            newData->n = n;

            pthread_t thread;
            pthread_create(&thread, NULL, threadSolveNQueens, (void*)newData);
            pthread_detach(thread); // Detach the thread so we don't need to join

            // For synchronization, wait for the current thread to complete
            pthread_join(thread, NULL);
        }
    }
}

void printBoard(int board[], int n) {
    for (int i = 0; i < n; i++) {
            printf("%d ", board[i]);
        }
        printf("\n");
}

#endif // NQUEENS_H
