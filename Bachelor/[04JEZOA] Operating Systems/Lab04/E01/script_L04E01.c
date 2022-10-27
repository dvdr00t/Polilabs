#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int args, char *argv[]) {

    char str[100];
    int i;

    setbuf(stdout, 0);
    fork();

    //Both child and parente
    for (i = 0; i < 2; i++) {

        /* PARENT */
        if (fork() != 0) {
            sprintf(str, "echo system with: i = %d", i);
            system(str);
        } else {

            /* CHILD */
            sprintf(str, "exec with i = %d", i);
            execlp("echo", "myPrgm", str, NULL);
        }
    }

    exit(EXIT_SUCCESS);
}