#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

bool isSafe(int board[], int row, int col, int n) {
    for (int i = 0; i < col; i++) {
        if (board[i] == row || abs(board[i] - row) == abs(i - col)) {
            return false;
        }
    }
    return true;
}

void solveNQueensUtil(int board[], int col, int n) {
    // Base case
        // write your code here
        if (isSafe == true)
        {
            printf(board[n]);
        }
        return;
    // Recursive case
        // write your code here
}