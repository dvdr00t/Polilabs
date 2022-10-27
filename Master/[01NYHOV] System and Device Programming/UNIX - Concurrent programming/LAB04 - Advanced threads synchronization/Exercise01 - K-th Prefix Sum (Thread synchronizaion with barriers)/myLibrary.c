/**
 * @file myLibrary.c
 * @author Davide Arcolini (davide.arcolini@studenti.polito.it)
 * @brief This file contains the definitions of the functions used in solution.c
 * @version 0.1
 * @date 2022-04-19
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
    int required_step;              /* Each thread must loop for no more than N (initial argument) times    */
    int referenced_index;           /* Each thread is associated with a specifici position in the array     */
    int *array;                     /* Pointer to the array which needs to be modified                      */
} thread_struct_t;

/**
 * @brief Barrier used in thread's routine function
 * 
 */
static pthread_barrier_t barrier;

/**
 * @brief Keep track of the number of active threads to reset the barrier at each step
 * 
 */
static int number_of_active_threads;

/**
 * @brief Each thread waits at this semaphore. The ending thread post this semaphore number_of_active_threads times.
 * 
 */
static sem_t semaphore;


/*
+-----------+
| FUNCTIONS |
+-----------+
*/

/**
 * @brief Thread's function routine
 * 
 * @param data the data structure passed as argument
 * @return void* 
 */
static void* routine (void *data) {

    /* CASTING TYPE */
    thread_struct_t *thread_DATA = (thread_struct_t *) data;

    /* DATA USED */
    int gap;            /* the current distant from the referenced index                                */
    int value_tmp;      /* result of the opeation. It will be written over the array after the barrier  */
    int ref_index = thread_DATA->referenced_index;

    /**/

    /**
     * @brief Each thread should loop for at most required_step. Eventually, ends earlier.
     *        At each step, it computes the sums of elements that are 2^(step-1) elements away.
     *        The formula is:
     *                              v[referenced_index] = v[referenced_index-gap] + v[referenced_index]
     * 
     * @param step current step in the execution.
     */
    for (int step = 0; step < thread_DATA->required_step; step++) {

        /* COMPUTING USEFUL VALUES FOR THE FORMULA */
        gap = pow(2, step);

        /* COMPUTING value_tmp */
        if (ref_index - gap < 0) {
            value_tmp = thread_DATA->array[ref_index];
        } else {
            value_tmp = thread_DATA->array[ref_index - gap] + thread_DATA->array[ref_index];
        }
        
        /* WAITING ALL THREADS BEFORE OVERWRITING THE ARRAY */
        pthread_barrier_wait(&barrier);
        thread_DATA->array[ref_index] = value_tmp;

        /* CHECKING IF I AM A THREAD WHICH NEEDS TO BE TERMINATED */
        if (thread_DATA->referenced_index == step + 1) {

            /* RESETTING THE BARRIER FOR THE OTHER THREADS */
            number_of_active_threads--;
            if (pthread_barrier_destroy(&barrier) != 0) {
                fprintf(stderr, "[ERROR] pthread_barrrier_destroy() failed execution\n");
                exit(EXIT_FAILURE);
            }
            if (pthread_barrier_init(&barrier, NULL, number_of_active_threads) != 0) {
                fprintf(stderr, "[ERROR] pthread_barrrier_init() failed execution\n");
                exit(EXIT_FAILURE);
            }
            
            /* THREAD'S ROUTINE ENDS SUCCESSFULLY */
            for (int i = 0; i < number_of_active_threads; i++) 
                sem_post(&semaphore);
            pthread_exit((void *) EXIT_SUCCESS);
        }

        /* WAIT ENDING THREAD TO CHANGE THE BARRIER */
        sem_wait(&semaphore);
        sleep(1);    /* just to show that values are printed as soon as thread terminates */
    }

    /* THREAD'S ROUTINE EXECUTED CORRECTLY */
    pthread_exit((void *) EXIT_SUCCESS);
}

/**
 * @brief Check is a string represents a numeric value or not
 * 
 * @param string value to be checked
 * @return 0 on success.
 */
int is_numeric (char *string) {
    for (int index = 0; index < strlen(string); index++) {
        if (!isdigit(string[index])) return -1;
    }
    return 0;
}

/**
 * @brief Generates an array of N random values in the range 1 to 9 
 * 
 * @param length number of elements in the array
 * @return a pointer to the array or NULL on failure
 */
int* generate_random_array(int length) {

    /* INITIALIZING SEED */
    srand(time(NULL));

    /* ALLOCATING THE ARRAY */
    int *array_v = (int *) malloc(length * sizeof(int));
    if (array_v == NULL) {
        fprintf(stderr, "[ERROR] malloc() failed execution\n");
        return NULL;
    }

    /* FILLING THE ARRAY WITH RANDOM VALUES */
    for (int index = 0; index < length; index++) {
        array_v[index] = rand() % 9 + 1;
    }

    /* ARRAY GENERATED SUCCESSFULLY */
    return array_v;
}

/**
 * @brief Print to the stdout the array of values passed as arguments
 *  
 * @param array array of values to be printed
 * @param length number of values in the array
 */
void show_array (int *array, int length) {

    fprintf(stdout, "[ARRAY] The array is: ");
    for (int index = 0; index < length; index++) {
        fprintf(stdout, "%d ", array[index]);
    }
    fprintf(stdout, "\n");
}

/**
 * @brief Compute the k-th prefix sum array
 * 
 * @param array the starting array
 * @param length the length of the array
 * @return 0 on success
 */
int kth_prefix_sum (int *array, int length) {

    /* DATA USED */
    int N_THREADS = length - 1;             /* Number of threads = 2^n - 1                  */
    number_of_active_threads = N_THREADS;   /* Initialization of the global variable        */

    /* ALLOCATING SPACE FOR THREADS DATA */
    pthread_t *threads_ID = (pthread_t *) malloc(N_THREADS * sizeof(pthread_t));
    thread_struct_t *thread_DATA = (thread_struct_t *) malloc(N_THREADS * sizeof(thread_struct_t));
    if (threads_ID == NULL || thread_DATA == NULL) {
        fprintf(stderr, "[ERROR] malloc() failed execution\n");
        return -1;
    }

    /* INITIALIZITATION OF THE BARRIER AND SEMAPHORE */
    pthread_barrier_init(&barrier, NULL, N_THREADS);
    sem_init(&semaphore, 0, 0);

    /* GENERATING THREADS */
    for (int id = 0; id < N_THREADS; id++) {

        /* INITIALIZING THREADS_DATA */
        thread_DATA[id].referenced_index = id+1;        /* threads_ID[0] is associated with array[1], etc...                */
        thread_DATA[id].array = array;                  /* passing the pointer so that each thread modify the same array    */
        thread_DATA[id].required_step = log2(length);   /* retrieving N = log2(length)                                      */

        if (pthread_create(&threads_ID[id], NULL, routine, (void *) &thread_DATA[id]) != 0) {
            fprintf(stderr, "[ERROR] pthread_create() failed execution\n");
            return -1;
        }
    }

    /* JOINING THREADS */
    int exit_status;
    for (int i = 0; i < N_THREADS; i++) {
        if (pthread_join(threads_ID[i], (void *) &exit_status) != 0) {
            fprintf(stderr, "[ERROR] pthread_join() failed execution\n");
            return -1;
        }

        /* SHOWING RESULT */
        fprintf(stdout, "%d ", array[i+1]);
    }
    
    /* FREEING MEMORY */
    free(threads_ID);
    free(thread_DATA);

    /* REMOVING BARRIER AND SEMAPHORE */
    pthread_barrier_destroy(&barrier);
    sem_destroy(&semaphore);

    /* FUNCTION EXECUTED CORRECTLY */
    return 0;
}

