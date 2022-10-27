/**
 * @file myLibrary.c
 * @author Davide Arcolini (davide.arcolini@studenti.polito.it)
 * @brief This file contains the definitions of the functions used in merge_OP.c
 * @version 0.1
 * @date 2022-08-24
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "myLibrary.h"

#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))

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
    bool is_ready;
} thread_struct_t;

/**
 * @brief Result of the merging operaton
 * 
 */
int *output = NULL;
int output_length = 0;

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
 * @brief Given two arrays, returns 0 if the minimum value is in arrayA, 1 if the minimum value
 *        is in arrayB.
 * 
 * @param arrayA first array to search
 * @param arrayB second array to search
 * @param lengthA length of the first array
 * @param lengthB length of the second array
 * @return zero or one
 */
static int find_index_of_array_with_min_value_OPT(int *arrayA, int *arrayB, int lengthA, int lengthB) {
    
    /* CHECKING INPUT PARAMETERS */
    if (arrayA == NULL) {
        return 1;
    } else if (arrayB == NULL) {
        return 0;
    }

    /* DATA USED */
    int min_value = arrayA[0];

    /* FINDING MIN VALUE OF ARRAYA */
    for (int i = 0; i < lengthA; i++) {
        if (arrayA[i] < min_value) {
            min_value = arrayA[i];
        }
    }

    /* IF WE FIND A NEW MIN VALUE, RETURN IMMEDIATLY */
    for (int i = 0; i < lengthB; i++) {
        if (arrayB[i] < min_value) {
            return 1;
        }
    }

    return 0;
}

/**
 * @brief Merge number_of_arrays ordered arrays without reordering from scratch
 * 
 * @param thread_RESULT the result of the threads routine, storing the arrays to be merged
 * @param output_file the output file where the result will be stored
 * @param number_of_arrays the number of arrays to be merged
 * @return 0 on success
 */
int merge_arrays(thread_struct_t *thread_RESULT, char *output_file, int number_of_arrays) {

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

    /* OPENING OUTPUT FILE */   
    int output_FD = open(output_file, O_WRONLY);
    if (output_FD == -1) {
        fprintf(stderr, "[ERROR] open() failed execution\n");
        return -1;
    }
    //fprintf(stdout, "[DEBUG] output fd is: %d\n", output_FD);

    /* MERGING ARRAY WITHOUT REORDERING */
    int j = 0;
    int array_index;
    do {
        array_index = find_index_of_array_with_min_value(thread_RESULT, indices, number_of_arrays);
        if (array_index >= 0)
            output[j++] = thread_RESULT[array_index].output_array[indices[array_index]++];
    } while (array_index >= 0);

    /* WRITING RESULT TO OUTPUT FILE */
    //fprintf(stdout, "[DEBUG] total number of data: %X (%d)\n", total, total);
    if (write(output_FD, &total, sizeof(int)) == -1) {
        fprintf(stderr, "[ERROR] write() failed execution\n");
        return -1;
    }
    for (int i = 0; i < total; i++) {
        //fprintf(stdout, "[DEBUG] buf is: %X (%d)\n", output[i], output[i]);
        if (write(output_FD, &output[i], sizeof(int)) == -1) {
            fprintf(stderr, "[ERROR] write() failed execution\n");
            return -1;
        }

    }

    /* FUNCTION ENDS CORRECTLY */
    close(output_FD);
    return total;
}

/**
 * @brief Merge the new result into the output array
 * 
 * @param thread_RESULT data produced by the thread
 * @return zero on success, -1 in case of failure 
 */
int merge_arrays_OPT(thread_struct_t thread_RESULT) {

    /* COUNTING SIZE OF RESULT */
    int total = 0;
    if (output_length == 0) {
        output = (int *) malloc(1 * sizeof(int));
        if (output == NULL) {
            return -1;
        }
    }
    total = output_length + thread_RESULT.length;
    // fprintf(stdout, "[DEBUG] total is: %d\n", total);
    // fprintf(stdout, "[DEBUG] output_length is: %d\n", output_length);
    // fprintf(stdout, "[DEBUG] thread_RESULT.length is: %d\n\n", thread_RESULT.length);

    /* ALLOCATING SPACE FOR RESULT */
    int *tmp = (int *) malloc(total * sizeof(int));
    if (tmp == NULL) {
        fprintf(stdout, "[ERROR] malloc() failed execution\n");
        return -1;
    }

    /* MERGING TWO ARRAYS */
    int indexA = 0;
    int indexB = 0;
    int indexC = 0;
    while (indexA < output_length && indexB < thread_RESULT.length) {
        if (output[indexA] < thread_RESULT.output_array[indexB]) {
            tmp[indexC++] = output[indexA++];
        } else {
            tmp[indexC++] = thread_RESULT.output_array[indexB++];
        }
    }
    while (indexA < output_length) {
        tmp[indexC++] = output[indexA++];
    }
    while (indexB < thread_RESULT.length) {
        tmp[indexC++] = thread_RESULT.output_array[indexB++];
    }

    /* COPYING BACK RESULT */
    output = (int *) realloc(output, total * sizeof(int));
    output_length = total;
    if (output == NULL) {
        fprintf(stderr, "[ERROR] malloc() failed execution\n");
        return -1;
    }
    for (int i = 0; i < total; i++) {
        output[i] = tmp[i];
    }

    /* TASK COMPLETED SUCCESSFULLY */
    free(tmp);
    return 0;

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
    thread_DATA->is_ready = true;
    pthread_exit((void *) EXIT_SUCCESS);
}

/**
 * @brief Wrapper function to solve the exercise
 * 
 * @param list_of_files the input files
 * @param output_file the output file
 * @param number_of_files the number of input files in order to allocate the threads
 * @return 0 on success
 */
int solver(char **list_of_files, char *output_file, int number_of_files) {

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

    /* MERGING ARRAY WITHOUT REORDERING */
    int total;
    if ((total = merge_arrays(threads_DATA, output_file, number_of_files)) == -1) {
        fprintf(stderr, "[ERROR] merging_arrays() failed execution\n");
        return -1;
    }

    /* FREEING MEMORY */
    free(threads_ID);
    free(threads_DATA);

    /* FUNCTION ENDS CORRECTLY */
    return 0;
}

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
int solver_OPT(char **list_of_files, char *output_file, int number_of_files) {

    /* ALLOCATING THREADS INFORMATION */
    pthread_t *threads_ID = (pthread_t *) malloc(number_of_files * sizeof(pthread_t));
    thread_struct_t *threads_DATA = (thread_struct_t *) malloc(number_of_files * sizeof(thread_struct_t));
    if (threads_ID == NULL || threads_DATA == NULL) {
        fprintf(stderr, "[ERROR] malloc() failed execution\n");
        return -1;
    }  
    for (int i = 0; i < number_of_files; i++) {
        threads_DATA[i].length = 0;
        threads_DATA[i].is_ready = false;
    }

    /* GENERATING THREADS */
    for (int id = 0; id < number_of_files; id++) {

        /* INITIALIZING THREADS_DATA */
        threads_DATA[id].filename = list_of_files[id];

        if (pthread_create(&threads_ID[id], NULL, routine, (void *) &threads_DATA[id]) != 0) {
            fprintf(stderr, "pthread_create() failed execution\n");
            return -1;
        }

        if (pthread_detach(threads_ID[id]) == -1) {
            fprintf(stderr, "[ERROR] pthread_detach() failed execution\n");
            return -1;
        }
    }

    /* MAIN THREAD CHECKS IN BUSY WAITING IF THERE ARE DATA READY */
    int counter = 0;
    int total = 0;
    while (true) {
        for (int i = 0; i < number_of_files; i++) {
            if (threads_DATA[i].is_ready) {
                if (merge_arrays_OPT(threads_DATA[i]) == -1) {
                    fprintf(stderr, "[ERROR] merge_arrays_OPT() failed execution\n");
                    return -1;
                }
                total += threads_DATA[i].length;
                threads_DATA[i].is_ready = false;
                counter++;
            }
        }

        if (counter == number_of_files) break;
    }

    /* OPENING OUTPUT FILE */   
    int output_FD = open(output_file, O_WRONLY);
    if (output_FD == -1) {
        fprintf(stderr, "[ERROR] open() failed execution\n");
        return -1;
    }

    /* WRITING RESULT TO OUTPUT FILE */
    //fprintf(stdout, "[DEBUG] total number of data: %X (%d)\n", total, total);
    if (write(output_FD, &total, sizeof(int)) == -1) {
        fprintf(stderr, "[ERROR] write() failed execution\n");
        return -1;
    }
    for (int i = 0; i < total; i++) {
        // fprintf(stdout, "[DEBUG] buf is: %X (%d)\n", output[i], output[i]);
        if (write(output_FD, &output[i], sizeof(int)) == -1) {
            fprintf(stderr, "[ERROR] write() failed execution\n");
            return -1;
        }

    }

    /* FREEING MEMORY */
    free(threads_ID);
    free(threads_DATA);

    /* FUNCTION ENDS CORRECTLY */
    close(output_FD);       
    return 0;
}