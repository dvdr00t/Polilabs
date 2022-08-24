/**
 * @file myLibrary.h
 * @author Davide Arcolini (davide.arcolini@studenti.polito.it)
 * @brief This file contains the declarations of the functions used in merge_OP.c
 * @version 0.1
 * @date 2022-08-24
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _MYLIBRARY_H_
#define _MYLIBRARY_H_

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <fcntl.h>
#include <math.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>

/**
 * @brief Wrapper function to solve the exercise
 * 
 * @param list_of_files the input files
 * @param output_file the output file
 * @param number_of_files the number of input files in order to allocate the threads
 * @return 0 on success
 */
int solver(char **list_of_files, char *output_file, int number_of_files);

/**
 * @brief Wrapper function to solve the exercise in an OPTIMIZED way, i.e. the main thread
 *        does not wait for the termination of ALL threads before merging, but starts merging
 *        as soon as it has data to use.
 * 
 * @param list_of_files the input files
 * @param output_file the output file
 * @param number_of_files the number of input files in order to allocate the threads
 * @return 0 on success
 */
int solver_OPT(char **list_of_files, char *output_file, int number_of_files);

#endif /* MYLIBRARY_H */