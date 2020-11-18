#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

int main(int args, char *argv[]) {

    /*
    * Checking that the number of arguments passed to the program in 0.   
    * Only the program name should be written.
    * If not, close the program with errors.
    */
    if(args != 1) {
        fprintf(stderr, "Error! Usage: ./progname\n");
        exit(EXIT_FAILURE);
    }

    /* VARIABLES */
    int number_inserted = -1;
    bool stop_iteration = false;
    

    /* Reading integer numbers */
    while (!stop_iteration) {
        fscanf(stdin, "%d\n", &number_inserted);

        /* Closing execution if the number is zero */
        if (number_inserted == 0) {
            exit(EXIT_SUCCESS);
        }

        if (number_inserted % 2 == 0) {
            fprintf(stdout, "%d is EVEN, redirected from stdout.\n", number_inserted);
        } else {
            fprintf(stderr, "%d is ODD, redirected from stderr.\n", number_inserted);
        }
    }
    
    exit(EXIT_SUCCESS);
}