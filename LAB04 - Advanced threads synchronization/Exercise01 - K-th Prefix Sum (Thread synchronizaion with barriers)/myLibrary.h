/**
 * @file myLibrary.h
 * @author Davide Arcolini (davide.arcolini@studenti.polito.it)
 * @brief This file contains the declarations of the functions used in solution.c
 * @version 0.1
 * @date 2022-04-19
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
#include <math.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>

/**
 * @brief Check is a string represents a numeric value or not
 * 
 * @param string value to be checked
 * @return 0 on success.
 */
int is_numeric (char *string);

/**
 * @brief Print to the stdout the array of values passed as arguments
 * 
 * @param array array of values to be printed
 * @param length number of values in the array
 */
void show_array (int *array, int length);

/**
 * @brief Generates an array of N random values in the range 1 to 9 
 * 
 * @param length number of elements in the array
 * @return a pointer to the array or NULL on failure
 */
int* generate_random_array (int length);

/**
 * @brief Compute the k-th prefix sum array
 * 
 * @param array the starting array
 * @param length the length of the array
 * @return 0 on success
 */
int kth_prefix_sum (int *array, int length);

#endif /* MYLIBRARY_H */