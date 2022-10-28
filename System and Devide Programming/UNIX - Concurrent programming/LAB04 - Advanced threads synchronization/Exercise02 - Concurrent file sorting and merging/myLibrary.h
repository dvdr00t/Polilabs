/**
 * @file myLibrary.h
 * @author Davide Arcolini (davide.arcolini@studenti.polito.it)
 * @brief This file contains the declarations of the functions used in solution.c
 * @version 0.1
 * @date 2022-04-22
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
 * @param number_of_files the number of input files in order to allocate the threads
 * @return 0 on success
 */
int solver(char **list_of_files, int number_of_files);

#endif /* MYLIBRARY_H */