# EXERCISE 01
## Code analysis: fork, exec, system

Given the following program, draw the process generation tree and indicate
what it displays and for what reason.

```C
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
  pid_t pid;
  int i;
  for (i=1; i<=3; i++){
    switch (i) {
      case 1: fork(); break;
      case 2: pid=fork(); if (pid!=0) system ("echo case 2"); break;
      case 3: execlp ("echo", "myPgrm", "case 3", NULL); break;
    }
  }
  return (0);
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
  |
  |
  |    +----+
  + -- | C2 |
       +----+
```

The output of the program is the following:
```console
case 2
case 2
case 3
case 3
case 3
case 3
```
NB: the order in which the `echo` are called and the output is produced may vary between executions. This is due to the fact that, while, after the first `fork()`, the parent process is busy with the `system()` system call, the children is free to run to the next iteration of the cicle, ending up in a **race condition**.