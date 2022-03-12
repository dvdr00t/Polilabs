# EXERCISE 04
## Code implementation with processes

Write a C program that receives two integer values on the command line,
$h$ and $n$, and it generates a process tree of height $h$ and degree $n$.

For example, if $h=2$ and $n=2$:
- the main process creates two processes
- each one of these $2$ processes creates other two processes
- each one of these $4$ processes creates other two processes
at this point, $8$ leaf processes run and the program must stop.

More in detail each node of the tree is a process.
The initial process generates $n$ child jobs and ends.
The same thing must be done by all child processes, generating a
number of processes on the leaves of the tree equal to $n^h$.
Processes on the leaves must all display their own `PID` and end.
