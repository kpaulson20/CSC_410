#include "nqueens_Pthread.h"

int main() {
    int n = 8; // Size of the board
    int* board = (int*)malloc(n * sizeof(int));
    if (board == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        board[i] = -1; // Initialize the board
    }

    ThreadData_t data = {board, 0, n};
    threadSolveNQueens((void*)&data);

    free(board);
    return 0;
}
