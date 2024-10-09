**********************ABOUT***********************
Two programs making use of synchronization
techniques.
Neruomancer: used techniques mutex and conditional
variables.  Mutex makes it so that the only one
part of the program can access the information at 
one time. When in use, it locks and doesn't allow
other processes to use it until it has finished.
Also used conditional variables which makes the
program wait to continue code until some event has
occured.
Array Average: Uses mutex synchronization to lock
the calculation of partial sums before unlocking
and calculating the total sum and average.

Date: Oct 11, 2024