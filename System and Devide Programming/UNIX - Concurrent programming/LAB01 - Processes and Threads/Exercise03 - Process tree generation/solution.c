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

  return (0);
}