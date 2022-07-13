# DiningPhilosophers

  Offers the solution for the following problem:

Implement the philosophers' dinner problem based on the monitor model,
so there is no possibility of deadlock or starvation.
Every philosopher must be on a thread and the resources they use are required
shared (the sticks). Deploy on Linux using C.

  About the code:
The code follows an English variable naming pattern, with comments
in Portuguese. The file "philosophers.c" contains main and is the consumer program
of the abstract data structure "monitor" (implemented in the file of the same name).

To run the code, you need to use the following commands in the terminal
(provided that in the directory containing the files):
$ make all
$ make run
