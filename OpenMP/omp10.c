// Atomic - trying to protect single memory operation
// more efficient, less over head

#include <stdio.h>
#include <omp.h>

int main() {
    int sum = 0;

    #pragma omp parallel for
    for (int i = 1; i <= 100; i++) {
        //#pragma omp atomic
        sum += i;  
    }

    printf("Final sum = %d\n", sum);

    return 0;
}
