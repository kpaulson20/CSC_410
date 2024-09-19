// Do some work - sequential version
// Count prime numbers

#include <stdio.h>
#include <math.h>
#include <time.h>

int is_prime(int num) {
    if (num < 2) return 0;
    for (int i = 2; i <= sqrt(num); i++) {
        if (num % i == 0) return 0;
    }
    return 1;
}

int count_primes(int limit) {
    int count = 0;
    for (int i = 2; i <= limit; i++) {
        if (is_prime(i)) {
            count++;
        }
    }
    return count;
}

int main() {
    int range = 10000000;

    int primes_total = count_primes(range);
    printf("Total primes: %d", primes_total);

    return 0;
}
