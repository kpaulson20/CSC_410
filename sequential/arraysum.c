#include <stdio.h>

#define SIZE 10000

int sumArray(int arr[], int size) {
    // Write your code here
    int sum = 0;
    int i = 0;
    for(i=0; i < size; i++)
    {
        sum += arr[i];
    }
}

int main() {
    int arr[SIZE];
    for (int i = 0; i < SIZE; i++) {
        arr[i] = i + 1; 
    }

    int totalSum = sumArray(arr, SIZE);
    printf("Total Sum: %d\n", totalSum);

    return 0;
}
