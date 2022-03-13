#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
  pid_t pid;
  int i;
  for (i=1; i<=3; i++){
    switch (i) {
      case 1: 
        fork(); 
        break;

      case 2: 
        pid=fork(); 
        if (pid!=0)
            system ("echo case 2");
        break;

      case 3: 
        execlp ("echo", "myPgrm", "case 3", NULL); 
        break;
    }
  }
  return (0);
} 