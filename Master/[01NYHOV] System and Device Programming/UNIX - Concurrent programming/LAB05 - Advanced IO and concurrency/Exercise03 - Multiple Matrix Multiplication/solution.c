/**
 * @file solution.c
 * @author Davide Arcolini (davide.arcolini@studenti.polito.it)
 * @brief First solution of the exercise. Implemented as a sequential algorithm.
 * @version 0.1
 * @date 2022-08-24
 * 
 * @copyright Copyright (c) 2022
 * 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

/**
 * @brief Enable random generation of data structure (zero identifies hard-coded data structure
 *        just for the purpose of checking results).
 */
#define OPT_RANDOM 1

/*
 *  + ------------- +
 *  |   FUNCTIONS   |
 *  + ------------- +
*/

/**
 * @brief Create an array of size N and fill it with random values
 * 
 * @param N size of the array
 * @return float* the array
 */
float *array_init(int N) {

    /* ALLOCATING SOME SPACE */
    float *result = (float *) malloc(N * sizeof(float));
    if (result == NULL) {
        fprintf(stderr, "[ERROR] malloc() failed execution\n");
        return NULL;
    }

    /* RANDOM INITIALIZATON */
    for (int i = 0; i < N; i++) {
        result[i] = ((float) rand() / (float) RAND_MAX) - 0.5;
    }

    /* TASK COMPLETED SUCCESSFULLY */
    return result;
}

/**
 * @brief Create a square matrix of size N and fill it with random values
 * 
 * @param N size of the matrix
 * @return float** the matrix
 */
float **mat_init(int N) {

    /* ALLOCATING SOME SPACE */
    float **result = (float **) malloc(N * sizeof(float*));
    if (result == NULL) {
        fprintf(stderr, "[ERROR] malloc() failed execution\n");
        return NULL;
    }
    for (int i = 0; i < N; i++) {
        result[i] = (float *) malloc(N * sizeof(float));
        if (result[i] == NULL) {
            fprintf(stderr, "[ERROR] malloc() failed execution\n");
        return NULL;
        }
    }

    /* RANDOM INITIALIZATON */
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            result[i][j] = ((float) rand() / (float) RAND_MAX) - 0.5;
        }
    }


    /* TASK COMPLETED SUCCESSFULLY */
    return result;
}


/**
 * @brief Perform the multiplication between two arrays of size size
 * 
 * @param array1 first array
 * @param array2 second array
 * @param size size of the two arrays
 * @param retval result of the multiplication
 * @return zero on success, any other value on failure
 */
int array_mul(float *array1, float *array2, int size, float *retval) {
    
    float res = 0;

    /* SOME ASSERTIONS */
    if (array1 == NULL || array2 == NULL || retval == NULL) {
        fprintf(stderr, "[ERROR] invalid arguments for array_mul()\n");
        return -1;
    } else if (size <= 0) {
        fprintf(stderr, "[ERROR] invalid arguments for array_mul()\n");
        return -1;
    }

    /* PERFORMING MULTIPLICATION */
    for (int i = 0; i < size; i++) {
        res += array1[i] * array2[i];
    }

    /* TASK COMPLETED SUCCESSFULLY */
    *retval = res;
    return 0;
}


/**
 * @brief Driver code to solve the exercise
 * 
 * @param N size of the arrays and matrices
 * @return zero on success
 */
int solver(int N) {

    /* SOME ASSERTIONS */
    if (N <= 0) {
        fprintf(stderr, "[ERROR] non-positive size N\n");
        return -1;
    }

#if OPT_RANDOM

    /* INITIALIZATION OF THE DATA STRUCTURE */
    float *v1 = array_init(N);
    if (v1 == NULL) {
        fprintf(stderr, "[ERROR] array_init() failed execution\n");
        return -1;
    }
    float *v2 = array_init(N);
    if (v2 == NULL) {
        fprintf(stderr, "[ERROR] array_init() failed execution\n");
        return -1;
    }
    float **mat = mat_init(N);
    if (mat == NULL) {
        fprintf(stderr, "[ERROR] mat_init() failed execution\n");
        return -1;
    }

#else

    /* STATIC INIT JUST FOR CHECKING */
    float v1[] = {-0.0613, -0.1184, 0.2655, 0.2952, -0.3131};
    float v2[] = {-0.3235, 0.1948, -0.1829, 0.4502, -0.4656};
    float mat[5][5] = {
        {-0.3424, -0.3581, 0.1557, 0.2577, 0.2060},	
        {0.4706, -0.0782, -0.4643, 0.2431, -0.4682},
        {0.4572, 0.4157, 0.3491, 0.1078, -0.2231},
        {-0.0146, 0.2922, 0.4340, -0.1555, -0.4029},
        {0.3003, 0.4595, 0.1787, -0.3288, -0.4656}
    };

#endif
    
    /* COMPUTING RESULT res = v1^T * mat * v2 = (v1^T * mat) * v2 = -0.004680 */
    float *tmp = (float *) malloc(N * sizeof(float));
    if (tmp == NULL) {
        fprintf(stderr, "[ERROR] malloc() failed execution\n");
        return -1;
    }

    /* STEP 0: EXTRACTING COLUMN FROM MATRIX */
    float *column = (float *) malloc(N * sizeof(float));
    if (column == NULL) {
        fprintf(stderr, "[ERROR] malloc() failed execution\n");
        return -1;
    }

    /* STEP 1:  (v1^T * mat) */
    int err;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            column[j] = mat[j][i];
        }
        err = array_mul(v1, column, N, &tmp[i]);
        if (err) {
            fprintf(stderr, "[ERROR] array_mul() failed execution\n");
            return -1;
        }
    }

    /* STEP 2: tmp * v2 = -0.004680 */
    float res;
    err = array_mul(tmp, v2, N, &res);
    if (err) {
        fprintf(stderr, "[ERROR] array_mul() failed execution\n");
        return -1;
    }

    fprintf(stdout, "[!] result is: %f\n", res);
    
    /* TASK COMPLETED SUCCESSFULLY */
    return 0;
}

/**
 * @brief MAIN
 * 
 * @param args number of arguments
 * @param argv list of arguments
 * @return zero on success
 */
int main(int args, char *argv[]) {

    /* CHECKING ARGUMENTS */
    if (args != 2) {
        fprintf(stderr, "[USAGE] ./exe03 N\n");
        exit(EXIT_FAILURE);
    }

    /* CONVERTING ARGUMENTS TO INTEGER */
    int N = atoi(argv[1]);

    /* TIME MANAGEMENT */
    srand(getpid());
    clock_t t = clock();

    /* DRIVER CODE */
    if (solver(N) != 0) {
        fprintf(stderr, "[DEBUG] solver() failed execution\n");
        exit(EXIT_FAILURE);
    }
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // calculate the elapsed time
    fprintf(stdout, "The program took %f seconds to execute\n", time_taken);

    /* TASK COMPLETED SUCCESSFULLY */
    fprintf(stdout, "[!] Done!\n");
    exit(EXIT_SUCCESS);
}