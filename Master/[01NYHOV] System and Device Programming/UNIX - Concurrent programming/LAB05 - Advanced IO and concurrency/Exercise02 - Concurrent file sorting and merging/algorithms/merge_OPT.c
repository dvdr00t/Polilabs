/**
 * @file solution.c
 * @author Davide Arcolini (davide.arcolini@studenti.polito.it)
 * @brief This file contains the solution to exercise02
 * @version 0.1
 * @date 2022-04-22
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>
#include <time.h>

/* Inclusion of personalized libraries */
#include "../lib/myLibrary.h"

#define MAX_FILENAME_LENGTH 50

/**
 * @brief Driver code
 * 
 * @param args number of arguments
 * @param argv list of arguments
 * @return 0 on success.
 */
int main(int args, char *argv[]) {

    /* CHECKING USER INPUT */
    if (args < 3) {
        fprintf(stderr, "[USAGE] ./merge_notOPT input_file_1.bin <input_file_n.bin> output_file.bin\n");
        exit(EXIT_FAILURE);
    }

    /* DELETING BUFFER TO STDOUT */
    setbuf(stdout, NULL);       

    /* TIME MANAGEMENT */
    clock_t t;

    /* PARSING USER INPUT */
    int number_of_files = args - 2;
    char **list_of_files = (char **) malloc(number_of_files * sizeof(char *));
    if (list_of_files == NULL) {
        fprintf(stderr, "[ERROR] malloc() failed execution\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < number_of_files; i++) {
        list_of_files[i] = argv[i+1];       
    }

    /* CALLING SOLVER */
    t = clock();
    if (solver_OPT(list_of_files, argv[number_of_files+1], number_of_files) != 0) {
        fprintf(stderr, "[ERROR] solver() failed execution\n");
        exit(EXIT_FAILURE);
    }
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // calculate the elapsed time
    fprintf(stdout, "The program took %f seconds to execute", time_taken);

    /* PROGRAM EXECUTED CORRECTLY */
    fprintf(stdout, "\n... done! ...\n");
    exit(EXIT_SUCCESS);
}