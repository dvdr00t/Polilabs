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
  return (0);
}