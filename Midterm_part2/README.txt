**********************ABOUT***********************
Midterm Part2: Parallelizing the Nqueens program

mainQ_Pthread.c uses the thread syncronization
technique of mutex to lock and unlock resources to
execute the program efficiently and correctly each
time.
mainQ_OpenMP.c uses the concept of openMP to
parallelize the program.  I used the '#pragma omp 
parallel for' line to denote openMP parallelzation.
I also ended up using a header file to split the 
work rather than doing it all in the source file.

Date: Oct 21, 2024