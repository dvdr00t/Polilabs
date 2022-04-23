/**
 * @file solution.c
 * @author Davide Arcolini (davide.arcolini@studenti.polito.it)
 * @brief This file contains the solution to exercise03
 * @version 0.1
 * @date 2022-04-23
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

#define MAX_PATH_NAME 100

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
        fprintf(stderr, "[USAGE] ./exe03 dir_path_1 dir_path_2 <dir_path_n>\n");
        exit(EXIT_FAILURE);
    }

    /* ALLOCATING SPACE FOR USER INPUTS */
    int N_DIRS = args - 1;
    char **source_directories = (char **) malloc(N_DIRS * sizeof(char *));
    if (source_directories == NULL) {
        fprintf(stderr, "[ERROR] malloc() failed execution\n");
        exit(EXIT_FAILURE);
    }

    /* PARSING USER INPUT */
    struct stat buffer;
    for (int arg_index = 1, dir_index = 0; arg_index <= args - 1; arg_index++) {

        /* CHECKING IF PATH PROVIDED LEADS TO SOMETHING THAT ACTUALLY EXISTS */
        if (stat(argv[arg_index], &buffer) != 0) {
            fprintf(stderr, "[ERROR] %s is not a path to an existing directory...\n\n", argv[arg_index]);
            exit(EXIT_FAILURE);
        }
        if (!S_ISDIR(buffer.st_mode)) {
            fprintf(stdout, ">>> %s is not a directory. Skipping...\n", argv[arg_index]);
            N_DIRS--;
        } else {
            /* CREATING SPACE FOR STORING THE USER INPUT */ 
            source_directories[dir_index] = (char *) malloc(MAX_PATH_NAME * sizeof(char));
            if (source_directories[dir_index] == NULL) {
                fprintf(stderr, "[ERROR] malloc() failed execution\n\n");
                exit(EXIT_FAILURE);
            }
            sprintf(source_directories[dir_index], "%s", argv[arg_index]);
            dir_index++;
        }
    }

    /* CALLING SOLVER FUNCTION */
    if (solver(source_directories, N_DIRS) != 0) {
        fprintf(stderr, "[ERROR] solver() failed execution\n");
        exit(EXIT_FAILURE);
    }
    
    /* FREEING MEMORY */
    free(source_directories);

    /* PROGRAM EXECUTED CORRECTLY */
    fprintf(stdout, "\n... well done! ...\n");
    exit(EXIT_SUCCESS);
}