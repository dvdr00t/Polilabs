/**
 * @file myLibrary.h
 * @author Davide Arcolini (davide.arcolini@studenti.polito.it)
 * @brief This file contains the declarations of the functions used in solution.c
 * @version 0.1
 * @date 2022-04-23
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
#include <dirent.h>
#include <sys/stat.h>
#include <pthread.h>
#include <semaphore.h>


/**
 * @brief Wrapper function to solve the exercise
 * 
 * @param source_directories the list of directories to be compared
 * @param N_DIRS the number of directories to be compared
 * @return 0 on success
 */
int solver(char **source_directories, int N_DIRS);

#endif /* MYLIBRARY_H */