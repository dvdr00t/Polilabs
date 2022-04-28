/**
 * @file solution.c
 * @author Davide Arcolini (davide.arcolini@studenti.polito.it)
 * @brief This file contains the solution to exercise01
 * @version 0.1
 * @date 2022-04-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>

/* Inclusion of personalized libraries */
#include "myLibrary.h"

/**
 * @brief Driver code
 * 
 * @param args number of arguments
 * @param argv list of arguments
 * @return 0 on success.
 */
int main (int args, char *argv[]) {

    /* CHECKING USER INPUT */
    if (args != 2) {
        fprintf(stderr, "[USAGE] ./exe01 <N>\n");
        exit(EXIT_FAILURE);
    } else if (is_numeric(argv[1]) != 0 || atoi(argv[1]) <= 0) {
        fprintf(stderr, "[USAGE] N should be an integer, strictly positive, value!\n");
        exit(EXIT_FAILURE); 
    }

    /* PARSING USER INPUT */
    int N = atoi(argv[1]);                       /* 2^N is the number of elements in the array                */

    /* SETTING BUFFER TO STDOUT */
    setbuf(stdout, NULL);

    /* GENERATING RANDOM ARRAY */
    int length = pow(2, N);
    int *array_v = generate_random_array(length);     /* Generates an array of N random values in the range 1 to 9 */
    if (array_v == NULL) {
        fprintf(stderr, "[ERROR] generate_random_array() failed execution\n");
        exit(EXIT_FAILURE);
    }

    /* SHOWING INITIAL ARRAY */
    show_array(array_v, length);

    /* COMPUTE K-TH PREFIX SUM */
    fprintf(stdout, "[ARRAY] The k-th prefix sum array is: %d ", array_v[0]);
    if (kth_prefix_sum(array_v, length) != 0) {
        fprintf(stderr, "[ERROR] kth_prefix_sum() failed execution\n");
        exit(EXIT_FAILURE);
    }
    fprintf(stdout, "\n");

    /* FREEING MEMORY */
    free(array_v);
    
    /* PROGRAM EXECUTED CORRECTLY */
    fprintf(stdout, "\n... program executed correctly ...\n");
    exit(EXIT_SUCCESS);
}

