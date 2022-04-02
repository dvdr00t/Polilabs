System and Device Programming
Laboratory number 02



Learning goals
--------------

File and filesystem basics with thread concurrency.



Exercise 01
Binary files
------------

An ASCII file has lines with the following format:
- An identifier, i.e., an integer value varying from 1 to the number
  of rows in the file (e.g., 1, 2, etc.)
- A register number, i.e., a long integer of 6 digits (e.g., 164678)
- A surname, i.e., a string of a maximum of 30 characters (e.g., Rossi)
- A name, i.e., a string of a maximum of 30 characters (e.g., Mario)
- An examination mark, i.e., an integer value.

The following is a correct example of such a file:

1 100000 Romano Antonio 25
2 150000 Fabrizi Aldo 22
3 200000 Verdi Giacomo 15
4 250000 Rossi Luigi 30

Write a C program in UNIX which is run with 3 file names on the
command line:
file_1  file_2  file_3
and which can:
- Read the *text* file file_1 (with the previous format) and
  store the same information into file file_2 in *binary* format.
  The binary format file must have *fixed-length* records (i.e.,
  each record must include an integer + a long integer + 31 characters
  + 31 characters + an integer).
- Read back the *binary* file file_2 and store it as a *text*
  into file file_3 (with the same format of file_1).

Suggestions
-----------
Manipulate text files using C library functions and binary files
using the I/O UNIX library.
As an alternative, use only the UNIX library *but* operate on the ASCII
file on a byte-by-byte basis (and then move forward and backward to
the record-by-record representation).

Verify that files file_input_1 and file_output_2 are indeed equivalent
using the shell command "wc" and "diff".



Exercise 02
Random File Walks
-----------------

File stores *binary* information with *fixed-length* records
using the format of file_2 of Exercise 01.
Write a program to implement a user menu with the following options:

- R n: Where R is the character 'R', and n is an integer value.
       Read from file the record representing student number n and
       display this information on standard output.

- W n: Where W is the character 'W', and n is an integer value.
       Read from standard input all data fields for the student number
       n and write them on the file in position n.
       
- E: End the program.

The input file name is given on the command line.

The following is an example of execution (using the file presented in
Exercise 01):

> R 3     // Read info for student 3
3 200000 Verdi Giacomo 15
> R 1     // Read info for student 1
1 100000 Romano Antonio 25
> W 1     // Over-write info for student 1
          // with info read from standard input
          // Read input data
Data: 1 100000 Romano Antonio 27
          // Overwrite record on file ...
> W 5     // Add info for student in position 5
          // with info read from standard input
          // Read input data
Data: 5 157143 White House 30
          // Overwrite record on file ...
> E       // End program

where
- "//" specifies explanatory comments not program I/O lines
- "R 3", "R 1", "W 1", etc. are the user inputs
- all other characters belong to the program output.

Future development
Once file locking has been introduced
--------------------------------------
Lock each record before reading (or writing) it, and release the
same record as soon as the operation has been performed.
(Notice that locking, for now at least, is useless, but it will be useful
with multi-threaded applications).



Exercise 03
Directory copy with file modification
-------------------------------------
Implement (simulate) the Linux shell command
cp  -R  <source>  <destination>
(with some extra feature) writing a C program with the following
specification

A C program is run with 2 strings on the command line:
dir_1  dir_2
where the two C strings indicate relative or absolute paths
of any two directory trees.
The program must copy the directory tree rooted in dir_1 into an isomorphic
directory tree dir_2.
For each file copied from the first to the second directory tree, the
program has to add two data fields on top of the destination file
specifying:
- the first one, the name of the file (the name only, not the path, as a
  C string) 
- the second one, the size of the source (original) file
  (an integer value).



Exercise 04 (versions A, B, and C)
Parallel directory visit
----------------------------------

A C program receives N parameters on the command line.
Each parameter indicates a relative or an absolute path to
a directory tree.
The program has to run N threads (one for each argument).
Each one of these threads recursively visits one of the directories,
and, for each directory entry, it displays its thread identifier
and the directory entry name.
The main thread waits for the termination of all threads.
When all threads have visited their directory also the program
ends.

Version A
As all output lines are generated independently by each single
thread, printing messages from different threads are interleaved
on standard output.

Version B
To avoid the problem of Version A, Version B uses files.
In other words, each thread stores its output on a file whose name
is generated (to avoid conflicts) based on the process and the
thread identifiers.
When all working threads end, the main thread copies all files on
standard output with the desired strategy (and no interruption).

Version C for Future development
Once synchronization primitives have been introduced
----------------------------------------------------
Generalize version B by using a centralized thread (the "output thread")
dedicated to generating the entire application output.
All threads resort to the centralized and dedicated output thread to
perform all operations on standard output.
The output thread is executed within a critical section, and
- it just awaits for more output to come in
- when the input arrives, it displays it.
Notice that by "centralizing" the output the application can optimize it.
For example, the application may decide to store messages on files (like
the program for Version B), it may use a dedicated data structure to
store messages and sort them somehow, it may decide which verbosity level
to use, it may compute output statistics, etc.




