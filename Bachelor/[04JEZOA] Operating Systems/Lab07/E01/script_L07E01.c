#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <math.h>

void generating_number(int *vet, int N);
int main(int args, char *argv[]) {

    /*
    * Checking the number of arguments.
    * Usage ./progname N
    * If the numebr is different from 2, exit with error.
    */
    if (args != 2) {
        fprintf(stderr, ">>> [ERROR] Usage: ./progname N\n");
        exit(EXIT_FAILURE);
    }

    /*
    *   V A R I A B L E S
    */
    int N = atoi(argv[1]);
    int *vet;
    vet = (int *) malloc(N * sizeof(int));
    if (vet == NULL) {  
        fprintf(stderr, ">>> [ERROR] Some errors occured while trying to allocate memory.\n");
        exit(EXIT_FAILURE);
    }

    /* Generating numbers */
    generating_number(vet, N);
    free(vet);

    exit(EXIT_SUCCESS);

}
void generating_number(int *vet, int N) {    
    /*
    *   V A R I A B L E S
    */
    pid_t pid_PARENT; 
    pid_t pid_CHILD;

    pid_PARENT = fork();
    if (pid_PARENT < 0) {
        fprintf(stderr, ">>> [ERROR] Some errors occured while trying to fork processes.\n");
        exit(EXIT_FAILURE);
    } else if (pid_PARENT == 0) {
        for (int i = 0; i <= N; i++) {
            if (i == N) {
                for (int j = 0; j < N; j++) {
                    fprintf(stdout, "%d", vet[j]);
                }
                fprintf(stdout, "\n");
            } else {
                pid_CHILD = fork();
                if (pid_CHILD < 0) {
                    fprintf(stderr, ">>> [ERROR] Some errors occured while trying to fork processes.\n");
                    exit(EXIT_FAILURE);
                } else if (pid_CHILD == 0) {
                    /* CHILD */
                    vet[i] = 0;
                } else {
                    /* PARENT */
                    vet[i] = 1;
                }
            }
        }
    } else {
        wait((pid_t *) 0);
    }

    exit(EXIT_SUCCESS);
} 