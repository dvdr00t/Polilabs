# EXERCISE 02
## Code analysis: fork, exec, system

Given the following program, draw the process generation tree and indicate
what it displays and for what reason.

```C
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main () {
  int i;
  for (i=0; i<2; i++) {
    printf ("i=%d\n", i);
    if (fork())             // Call # 1
      fork ();              // Call # 2
    else
      system ("echo i=i");  // Call # 3
  }
  return (1);
}
```

## PROCESS GENERATION TREE
```bash
+---+
| P |
+---+
  |
  |    +----+
  + -- | C1 |
  |    +----+
  |      |
  |      |    +-----+
  |      + -- | C11 |
  |      |    +-----+
  |      |    +-----+
  |      + -- | C12 |
  |           +-----+
  |
  |    +----+
  + -- | C2 |
  |    +----+
  |      |
  |      |    +-----+
  |      + -- | C21 |
  |      |    +-----+
  |      |    +-----+
  |      + -- | C22 |
  |           +-----+
  |
  |    +----+
  + -- | C3 |
  |    +----+
  |
  |    +----+
  + -- | C4 |
       +----+
```
The output of the program is the following:
```console
i=0
j=-1 
j
j
i=1
j=-1 
j
j
i=2
j=-1 
j
j
i=3
j=-1 
j
j
i=4
j=-1 
j
j
```
NB: the order in which the `echo` and `printf()` are called and the output is produced may vary between executions. This is due to **race conditions**.