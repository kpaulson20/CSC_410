// Intro to OpenMP basics

#include <stdio.h>
#include <omp.h>

int main() 
{

    printf("Before omp\n");

    #pragma omp parallel
    {
        printf("Hello\n");
    }

    printf("After omp\n");
    return 0;
}