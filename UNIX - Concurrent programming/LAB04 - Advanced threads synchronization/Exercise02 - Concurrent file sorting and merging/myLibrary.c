/**
 * @file myLibrary.c
 * @author Davide Arcolini (davide.arcolini@studenti.polito.it)
 * @brief This file contains the definitions of the functions used in solution.c
 * @version 0.1
 * @date 2022-04-22
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "myLibrary.h"

/*
+----------------+
| DATA STRUCTURE |
+----------------+
*/

/**
 * @brief Data structure used as thread's routine function argument
 * 
 */
typedef struct thread_struct_s {
    char *filename;
    int *output_array;
    int length;
} thread_struct_t;

/**
 * @brief Result of the merging operaton
 * 
 */
int *output;

/*
+-----------+
| FUNCTIONS |
+-----------+
*/

/**
 * @brief Quick sort recursive function
 * 
 * @param array the array to be sorted
 * @param start the left value
 * @param end the rigth value
 * @return 0 on success
 */
static int quick_sort_wrapped(int *array, int start, int end) {
    
    /* DATA USED */
    int i, j;
    int pivot;
    int temp;

    /* CHECKING CONDITION */
    if (start < end) {

        pivot = start;
        i = start;
        j = end;

        /* LOOPING */
        while (i < j) {
            while (array[i] <= array[pivot] && i < end) i++;
            while (array[j] > array[pivot]) j--;

            /* SWITCHING ELEMENTS */
            if (i < j) {
                temp = array[i];
                array[i] = array[j];
                array[j] = temp;
            }
        }
        
        /* SWITCHING ELEMENTS */
        temp = array[pivot];
        array[pivot] = array[j];
        array[j] = temp;

        /* CALLING RECURSIVELY */
        if (quick_sort_wrapped(array, start, j-1) != 0) {
            fprintf(stderr, "[ERROR] quick_sort_wrapped() failed execution\n");
            return -1;
        }
        if (quick_sort_wrapped(array, j+1, end) != 0) {
            fprintf(stderr, "[ERROR] quick_sort_wrapped() failed execution\n");
            return -1;
        }
    }
    return 0;
}

/**
 * @brief Interface with the quick sort function
 * 
 * @param array the array to be sorted
 * @param length the length of the array to be sorted
 * @return 0 on success
 */
static int quick_sort(int *array, int length) {
    if (quick_sort_wrapped(array, 0, length-1) != 0) {
        fprintf(stderr, "[ERROR] quick_sort_wrapped() failed execution\n");
        return -1;
    }
    return 0;
} 

/**
 * @brief Find the minimum value between a list of values
 * 
 * @param thread_RESULT the result of the thread routine, storing the arrays
 * @param indices the current point in the arrays in which is stored the candidate miin value
 * @param number_of_arrays number of candidate values
 * @return 0 on success 
 */
static int find_index_of_array_with_min_value(thread_struct_t *thread_RESULT, int *indices, int number_of_arrays) {

    /* DATA USED */
    int min_value;
    int min_index = -1;

    /* SEARCHING MIN VALUE */
    for (int i = 0; i < number_of_arrays; i++) {
        if (indices[i] < thread_RESULT[i].length) {
            if (min_index == -1 || thread_RESULT[i].output_array[indices[i]] < min_value) {
                min_index = i;
                min_value = thread_RESULT[i].output_array[indices[i]];
            }
        }
    }

    /* INDEX FOUND */
    return min_index;   
}

/**
 * @brief Merge number_of_arrays ordered arrays without reordering from scratch
 * 
 * @param thread_RESULT the result of the threads routine, storing the arrays to be merged
 * @param number_of_arrays the number of arrays to be merged
 * @return 0 on success
 */
int merge_arrays(thread_struct_t *thread_RESULT, int number_of_arrays) {

    /* COUNTING TOTAL VALUES */
    int total = 0;
    for (int i = 0; i < number_of_arrays; i++) {
        total = total + thread_RESULT[i].length;
    }

    /* ALLOCATING SPACE FOR OUTPUT ARRAY AND INDICES*/
    output = (int *) malloc(total * sizeof(int));
    int *indices = (int *) malloc(number_of_arrays * sizeof(int));
    if (output == NULL || indices ==  NULL) {
        fprintf(stderr, "[ERROR] malloc() failed execution\n");
        return -1;
    }

    /* INITIALIZING INDICES */
    for (int i = 0; i < number_of_arrays; i++) {
        indices[i] = 0;
    }

    /* MERGING ARRAY WITHOUT REORDERING */
    int j = 0;
    int array_index;
    do {
        array_index = find_index_of_array_with_min_value(thread_RESULT, indices, number_of_arrays);
        if (array_index >= 0)
            output[j++] = thread_RESULT[array_index].output_array[indices[array_index]++];
    } while (array_index >= 0);

    /* FUNCTION ENDS CORRECTLY */
    return total;
}

/**
 * @brief Thread's function routine
 * 
 * @param data the data structure passed as argument
 * @return void* 
 */
static void* routine (void *data) {

    /* CASTING INPUT DATA */
    thread_struct_t *thread_DATA = (thread_struct_t *) data;  

    /* OPENING INPUT FILE */    
    int input_FD = open(thread_DATA->filename, O_RDONLY);
    if (input_FD == -1) {
        fprintf(stderr, "[ERROR] open() failed execution\n");
        pthread_exit((void *) EXIT_FAILURE);
    }

    /* ALLOCATING SPACE FOR THE ARRAY */
    if (read(input_FD, &(thread_DATA->length), sizeof(int)) != sizeof(thread_DATA->length)) {
        fprintf(stderr, "[ERROR] read() failed execution\n");
        pthread_exit((void *) EXIT_FAILURE);
    }
    thread_DATA->output_array = (int *) malloc((thread_DATA->length) * sizeof(int));
    if (thread_DATA->output_array == NULL) {
        fprintf(stderr, "[ERROR] malloc() failed execution\n");
        pthread_exit((void *) EXIT_FAILURE);
    }

    /* READING DATA FROM FILE INTO ARRAY */
    for (int i = 0; i < thread_DATA->length; i++) {
        if (read(input_FD, &(thread_DATA->output_array[i]), sizeof(int)) != sizeof(thread_DATA->output_array[i])) {
            fprintf(stderr, "[ERROR] read() failed execution\n");
            pthread_exit((void *) EXIT_FAILURE);
        }
    }

    /* SORTING ARRAY */
    if (quick_sort(thread_DATA->output_array, thread_DATA->length) != 0) {
        fprintf(stderr, "[ERROR] quick_sort() failed execution\n");
        pthread_exit((void *) EXIT_FAILURE);
    }

    /* CLOSING INPUT FILE */
    if (close(input_FD) != 0) {
        fprintf(stderr, "[ERROR] close() failed execution\n");
        pthread_exit((void *) EXIT_FAILURE);
    }

    /* THREAD'S ROUTINE ENDS CORRECTLY */
    pthread_exit((void *) EXIT_SUCCESS);
}

/**
 * @brief Wrapper function to solve the exercise
 * 
 * @param list_of_files the input files
 * @param number_of_files the number of input files in order to allocate the threads
 * @return 0 on success
 */
int solver(char **list_of_files, int number_of_files) {

    /* ALLOCATING THREADS INFORMATION */
    pthread_t *threads_ID = (pthread_t *) malloc(number_of_files * sizeof(pthread_t));
    thread_struct_t *threads_DATA = (thread_struct_t *) malloc(number_of_files * sizeof(thread_struct_t));
    if (threads_ID == NULL || threads_DATA == NULL) {
        fprintf(stderr, "[ERROR] malloc() failed execution\n");
        return -1;
    }   

    /* GENERATING THREADS */
    for (int id = 0; id < number_of_files; id++) {

        /* INITIALIZING THREADS_DATA */
        threads_DATA[id].filename = list_of_files[id];

        if (pthread_create(&threads_ID[id], NULL, routine, (void *) &threads_DATA[id]) != 0) {
            fprintf(stderr, "pthread_create() failed execution\n");
            return -1;
        }
    }

    /* JOINING THREADS */
    int exit_status;
    for (int id = 0; id < number_of_files; id++) {
        if (pthread_join(threads_ID[id], (void *) &exit_status) != 0) {
            fprintf(stderr, "[ERROR] pthread_join() failed execution\n");
            return -1;
        }

        /* CHECKING EXITING STATUS */
        if (exit_status == EXIT_FAILURE) {
            fprintf(stderr, "[ERROR] thread %d ends with EXIT_FAILURE %d\n", id, exit_status);
            return -1;
        }
    }

    /* SHOWING SORTED ARRAYS */
    for (int i = 0; i < number_of_files; i++) {
        fprintf(stdout, "The sorted array is: ");
        for (int j = 0; j < threads_DATA[i].length; j++) {
            fprintf(stdout, "%d ", threads_DATA[i].output_array[j]);
        }
        fprintf(stdout, "\n");
    }

    /* MERGING ARRAY WITHOUT REORDERING */
    int total;
    if ((total = merge_arrays(threads_DATA, number_of_files)) == -1) {
        fprintf(stderr, "[ERROR] merging_arrays() failed execution\n");
    }

    /* SHOWING RESULT */
    fprintf(stdout, "The final ordered array is: ");
    for (int i = 0; i < total; i++) {
        fprintf(stdout, "%d ", output[i]);
    }
    fprintf(stdout, "\n");

    /* FREEING MEMORY */
    free(threads_ID);
    free(threads_DATA);

    /* FUNCTION ENDS CORRECTLY */
    return 0;
}