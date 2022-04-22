System and Device Programming
Laboratory number 04 (System and Programming part)



Learning goals
--------------

Binary files, file systems, and multi-threaded programming with
"advanced" synchronization (semaphores, etc.).



Requested background
--------------------

- At least one sorting algorithm (i.e., such as insertion sort,
  selection sort, ..., quick sort).
- The merge algorithm (i.e., merging two ordered sequences of values
  into a unique ordered sequence of values without "reordering"
  all data from scratch).
  Note that the cost of such an algorithm is linear in the final array
  size not quadratic or linearithmic as a standard ordering strategy.



Exercise 01
Synchronization with barriers
-----------------------------

Given an array v of 2^n elements v[i], i.e.,

v = { v[0], v[1], ... , v[2^n − 1] }

its k-th prefix sum (the cumulative sum vector) is the sum of its first k+1
elements, i.e.,
v[0] + v[1] + ... + v[k]

Write a C program using Pthreads that:
* Receives an integer value n as an argument of the command line
* Randomly generates an array v of 2^n integer elements in the range [1-9]
* Computes the prefix sum of v as follows.

For an array v of 2^n elements, it is possible to compute all values of
the prefix sum in n steps, by means of 2^n−1 concurrent threads, each
associated with an array element.

For example for n=3 and 2^3 = 8 elements, each step consists of each
thread taking two elements, and computing a result:
* In the first step, each thread computes the sums of adjacent elements
  (i.e., gap = 1=2^0). 
* In the second step, each thread computes the sums of the elements
  that are 2 elements away (i.e., gap = 2 = 2^1).
* In the third step, each thread computes the sums of the elements
  that are 4 elements away (i.e., gap = 4 = 2^2).
After 3 steps, all sum prefix values are computed.

In general, if we have 2^n elements in v, step i computes the sums of
elements that are 2^(i-1) elements away.
After n steps, all cumulative sum values are computed.

For example, let us suppose the program is run as:

> cumulative_sum  3

It should compute (and eventually display) the following:

Initial array v:  2  4  6  1  3  5  8  7
v after step 1 :  2  6 10  7  4  8 13 15
                  Computed as: v[0]=v[0], v[1]=v[0]+v[1],
		               v[2]=v[1]+v[2], etc., v[n-1]=v[n-2]+v[n-1]
                               (sum of elements at distance 2^0=1)
v after step 2 :  2  6 12 13 14 15 17 23
                  Computed as: v[0]=v[0], v[1]=v[1], v[2]=v[0]+v[2],
                               v[3]=v[1]+v[3], etc., v[n-1]=v[n-3]+v[n-1]
                               (sum of elements at distance 2^1=2)
v after step 3 :  2  6 12 13 16 21 29 36
                  Computed as: v[0]=v[0], v[1]=v[1], v[2]=v[2], v[3]=v[4],
                               v[4]=v[0]+v[4], v[5]=v[1]+v[5], etc.,
                               v[n-1]=v[n-5]+v[n-1]
                               (sum of elements at distance 2^2=4)
There is no step 4 as in step 4 the procedure should sum elements at a
distance 2^3=8 (which is the size of the array).

Notice that in the final array:
- v[7]=36 is the sum of elements v[0]+v[1]+...+v[7]
- v[6]=29 is the sum of elements v[0]+v[1]+...+v[6]
- v[5]=21 is the sum of elements v[0]+v[1]+...+v[5]
- etc.

Moreover, notice that:
1. The main thread allocates, fills, and prints the initial content of
   the array v
2. All threads are initially created by the main thread, they have all
   the same code and they must synchronize their activity according to the
   procedure previously described, exploiting the maximum possible
   concurrency.
3. The array v must be updated in place, i.e., you cannot allocate
   auxiliary arrays.
4. Each thread must loop for no more than n times (the number of
   steps) before exiting.
5. Some threads terminate before the n-th step.
   In particular thread i terminates after step i, with i in the
   range [1–n].
6. The main thread prints immediately the value in v[0], then it waits
   the termination of each thread i, in order of creation, and
   immediately prints the value stored in v[i].

Suggestions
-----------

* First design and write the pseudo-code of a solution excluding rule 5.
* Then, update your solution including rule 5.
  Please notice that:
  - If you refer to v[i-gap], index i-gap must be non-negative
  - You must properly manage a variable that stores the number of
    active threads, which must be decremented according to rule 5.



Exercise 02
Concurrent file sorting and merging
-----------------------------------

A file stores integer values in binary format on 32 bits.
The first integer stored in the file indicates the number of values
following the first one.
For example, the following three lines specify (obviously in ASCII
format, to be user-readable) the content of three possible files
(that are stored in, or must be converted into, binary format):

File 1: 5 23 45 67 9 23
File 2: 12 90 65 34 24 12 0 89 29 54 12 78 3
File 3: 3 9 5 1

Write an application which:

- Receives a variable number of strings, let us say n strings, on the
  command line.
  The first (n-1) strings are input file names.
  The last string is an output file name.
  Each input file has the format previously described.

- Runs one thread for each input file passing to each of them one of
  the input file names.
  We will refer to these (n-1) working threads as "ordering" threads.

- After running all ordering threads, the main application awaits for
  the termination of all of them.

- When the main thread waits, each ordering thread:
  - opens "its own" input file
  - reads the first integer value
  - allocates a dynamic array of integers to store all other integers
    numbers stored in the file
  - read those numbers into the array
  - orders the array (in ascending order) using whatever ordering
    algorithm it is deemed appropriate
  - ends (returning the control to the main application thread).

- The main application thread, once collected the termination of all
  ordering threads, merges all ordered arrays into a unique array,
  i.e., a unique sequence of integers.

- It stores the final ordered array into the output file, using the
  same (binary) format of the input files.

For the previous input files the output file (again, in its ASCII
version) should be:
20 0 1 3 5 9 9 12 15 23 23 24 29 34 45 54 65 67 78 89 90



Exercise 02
Concurrent visit and comparison of directory trees
--------------------------------------------------

A C program is run with N parameters.
Each parameter indicates a relative or an absolute path to a
filesystem directory tree.

The program has to compare the content of all directories trees to
decide whether they have the same content or not.

Two directory trees have the same content *if and only if* all
directory entries (files, special files, and sub-directories) have
the same name (excluding the path leading to the root, which differs
but for self-comparisons).

Upon termination, the program has to state whether all directory trees
have the same content or not.

Suggestions
-----------

- The main program runs one "reading" thread for each directory tree
  plus one unique "comparing" thread.
- Each "reading" thread visits one of the directory trees.
  It is reasonable to suppose (even if this is not explicitly stated
  by the specification of the system call opendir and readdir) that in
  case of equivalent directory trees, all visits proceed using the same
  order, i.e., they deliver all directory entries in the same order.
- Reading threads synchronize themselves for each entry they find,
  waiting for each other before moving to the next entry.
- For each entry, the last "reading" threads activate the "comparing"
  thread.
- The "comparing" thread compares the name of all entries received.
  It stops all other threads (and the program) in case the entries are
  not equal. Otherwise, it returns the control to all "reading" threads.

Observations
------------

Notice that there are at least 3 possible termination conditions
to manage:
- Directories are indeed equivalent.
  This should lead to a successful termination.
- Directories differ for some specific entry names.
  This can be intercepted by the comparing thread and force a program
  termination.
- Directories are (partially) equivalent but they include a
  different number of entries.
  In this case, one thread will terminate its task before all
  others which may be waiting for it forever.
  This situation should be avoided and managed properly.
