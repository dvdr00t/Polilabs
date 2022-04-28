# EXERCISE 03
## Code analysis: fork, exec, system

Given the following program, draw the process generation tree and indicate
what it displays and for what reason.

```C
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main () {
  int i, j=-1;

  for (i=0; i<5; i++) {
    printf ("i=%d\n", i);
    if (!fork()) {
      for (j=2; j>0; j--) {
        fork();
        execlp ("echo", "i", "j", (char *) 0);
      }
    } else {
      printf ("j=%d \n", j);
    }
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
  |           +-----+
  |    +----+
  + -- | C2 |
  |    +----+
  |      |
  |      |    +-----+
  |      + -- | C21 |
  |           +-----+
  |    +----+
  + -- | C3 |
  |    +----+
  |      |
  |      |    +-----+
  |      + -- | C31 |
  |           +-----+
  |    +----+
  + -- | C4 |
  |    +----+
  |      |
  |      |    +-----+
  |      + -- | C41 |
  |           +-----+
  |    +----+
  + -- | C5 |
       +----+
         |
         |    +-----+
         + -- | C51 |
              +-----+
```
The output of the program is the following:
```console
i=0
i=i
i=1
i=1
i=1
i=i
i=i
i=i
```
NB: the order in which the `echo` are called and the output is produced may vary between executions. This is due to the fact that, while, after the first `fork()`, the parent process is busy with another `fork()` system call, the children is free to run to the next iteration of the cicle, ending up in a **race condition**.