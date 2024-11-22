***********************ABOUT***********************
Parallelizing three programs using MPI. matrixMul.c
calculates the multiplication of matricies A and
B and stores them in a blank matrix C. integrate.c
calculates the value of pi through the use of the
trapezoidal rule. prime.c calculates all the prime
numbers using Sieve Of Eratosthenes. I tried to
time the programs using MPI_Wtime(), but I could
only get so far in testing for two of the programs.
The matrixMul.c program failed at 2 processes and the 
prime.c program failed to calculate at 6 processes.

Date 11/22/2024