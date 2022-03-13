# EXERCISE 06
## Code implementation with threads (a variant of exercise 04, but with threads)

Implement a C program, `thread_generation`, that receives a command line parameter
$n$.
The parent thread creates two threads and waits for their termination.
Each further thread creates the other two threads, and it awaits their termination. 
Tread creation stops after $2^n$ threads have been created, i.e., the ones that stand
on the leaves of a tree with $2^n$ leaves.

For example, if $n=3$:
- the main thread creates two threads,
- each one of these $2$ threads creates other two threads,
- each one of these $4$ threads creates other two threads at this point, $8$ leaf treads are running and the program must stop.

Each leaf thread must print its generation tree, i.e., the sequence of thread identifiers from the main thread (the tree root) to the leaf thread (tree leaf).

The following is an example of the program execution: 

```console
quer@quer-VirtualBox:~/current/sdp$ ./l01e05 3
140051327870720 140051311085312 140051224717056 
140051327870720 140051311085312 140051224717056 
140051327870720 140051311085312 140051233109760 
140051327870720 140051319478016 140051207931648 
140051327870720 140051311085312 140051233109760 
140051327870720 140051319478016 140051207931648 
140051327870720 140051319478016 140051216324352 
140051327870720 140051319478016 140051216324352 
```

## Suggestion
Print (and store) thread identifiers (tids) as "long integer" values.
