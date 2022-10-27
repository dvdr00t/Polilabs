#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int args, char *argv[]) {

    int i = 0;

    setbuf(stdout, 0);
    fprintf(stdout, ">>> Chief process PID: %d\n", getpid());

    while (i <= 2 && fork()) {

        /* PARENT */
        if (!fork()) {

            /* CHILD */
            fprintf(stdout, ">>> (Child) This process PID: %d\n", getpid());
            fprintf(stdout, "... running Exec by %d...\n", getpid());
            execlp("echo", "echo", "i*i", NULL);
        }

        fprintf(stdout, ">>> (Parent) This process PID: %d\n", getpid());
        i++;
        fprintf(stdout, "... running System by %d ...\n", getpid());
        system("echo i+i");
    }
    
    fprintf(stdout, ">>> (Child) This process PID: %d\n", getpid());
    fprintf(stdout, ">>> Yo, process PID: %d is terminating...\n", getpid());
    exit(EXIT_SUCCESS);
}