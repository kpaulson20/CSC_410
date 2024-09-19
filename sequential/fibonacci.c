#include <stdio.h>

int fibonacci(int n) {
    if (n <= 1) {
        return n;
    }
    return fibonacci(n - 1) + fibonacci(n - 2);
}

void displayFib(int n) {
    printf("Fibonacci sequence up to %d:\n", n);
    for (int i = 0; i <= n; i++) {
        printf("%d \n", fibonacci(i));
    }
    printf("\n");
}

int main() {
    int n = 30; 

    displayFib(n);
    
    return 0;
}
