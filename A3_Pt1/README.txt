********************ABOUT*********************
Purpose: to efficiently correct multithreading
in the matrixMultiplication (matrixT) program
and the sumArray (sumT) program.
matrixT: no synchronizationtechnique was needed.
Just incldued a statement that checks what size
NUM_THREADS is and executes code accordingly
sumT: Made use of mutex synchronization to be
able to correctly handle odd number of threads
by locking the computation of each thread before
summing all array values. Once finished the end
result of summation, regardless of thread number
is always 500000500000.

Date: Oct 8 2024