#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>

#define N_THREADS 4

/* Monomial data structure */
typedef struct monomial_s {
    float coefficient;
    float base;
    int exponent;
} monomial_t;


/* Data structure: each thread see its own packet of data */
typedef struct threads_data_s {
    pthread_t thread;
    int thread_ID;
    char *filename;
    float result;
    sem_t sem;
    bool end;
} threads_data_t;
threads_data_t threads[N_THREADS-1];

/* Threads routine */
static void *reading_monomial(void *);
static void *computing_polynomial(void *);


int main(int args, char *argv[]) {

    /* Checking arguments */
    if (args != 2) {
        fprintf(stderr, ">>> [USAGE] ./progname binaryfile\n");
        fflush(stderr);
        exit(EXIT_FAILURE);
    }

    /* Polynomial thread */
    threads[0].thread_ID = 0;
    threads[0].filename = NULL;
    threads[0].end = false;
    threads[0].result = 0;
    sem_init(&threads[0].sem, 0, 0);
    if (pthread_create(&(threads[0].thread), NULL, computing_polynomial, (void *) &threads[0].thread_ID) != 0) {
        fprintf(stderr, ">>> [ERROR] Some errors occured while trying to create the threads\n");
        fflush(stderr);
        exit(EXIT_FAILURE);
    }

    /* Monomial threads */
    for (int i = 1; i < N_THREADS; i++) {

        threads[i].thread_ID = i;
        threads[i].filename = argv[1];
        threads[i].end = false;
        threads[i].result = 0;
        sem_init(&(threads[i].sem), 0, 1);
        if (pthread_create(&(threads[i].thread), NULL, reading_monomial, (void *) &threads[i].thread_ID) != 0) {
            fprintf(stderr, ">>> [ERROR] Some errors occured while trying to create the threads\n");
            fflush(stderr);
            exit(EXIT_FAILURE);
    }     
    }

    /* Joining the threads */
    for (int i = 0; i < N_THREADS; i++) {
        if (pthread_join(threads[i].thread, NULL) != 0) {
            fprintf(stderr, ">>> Some errors occured while trying to joining the threads\n");
            fflush(stderr);
            exit(EXIT_FAILURE);
        }
    }

    /* Main process ends successfully */
    exit(EXIT_SUCCESS);
}   

void *reading_monomial(void *arg) {

    /* DATA */
    monomial_t monomial;
    int thread_ID = *((int *) arg);
    int input_file;
    int n = 1;
    int offset, offset_counter;

    /* Opening file */
    input_file = open(threads[thread_ID].filename, O_RDONLY);
    if (input_file == -1) {
        fprintf(stderr, ">>> [ERROR] Some errors occured while trying to open file %s\n", threads[thread_ID].filename);
        fflush(stderr);
        exit(EXIT_FAILURE);
    }

    /* Main loop */
    offset_counter = 0;
    while (n > 0) {

        /* Waiting the other threads to end (first iteration: go) */
        if (sem_wait(&threads[thread_ID].sem) != 0) {
            fprintf(stderr, ">>> [ERROR] Some errors occured while managing semaphores\n");
            fflush(stderr);
            exit(EXIT_FAILURE);
        }

        /* Setting offset for reading the file */
        offset = (offset_counter * 3 + threads[thread_ID].thread_ID - 1) * sizeof(monomial_t);
        lseek(input_file, offset, SEEK_SET);

        /* Reading data from file */
        n = read(input_file, &monomial, sizeof(monomial_t));
        if (n > 0) {

            /* Computing result */
            threads[thread_ID].result = monomial.coefficient;
            for (int j = 0; j < monomial.exponent; j++) {
                threads[thread_ID].result = threads[thread_ID].result * monomial.base;
            }

            /* Printing records */
            fprintf(stdout, "thread_ID: %2d - read_record: %2d - read_data: [%.2f %.2f %2d] - result: %.2f\n", 
                threads[thread_ID].thread_ID, offset_counter+1, monomial.coefficient, monomial.base, monomial.exponent, threads[thread_ID].result);
            fflush(stdout);

            /* Continuing execution */
            threads[thread_ID].end = false;

        } else {
            /* Stopping execution */
            threads[thread_ID].result = 0;
            threads[thread_ID].end = true;
        }

        /* Incrementing offset */
        offset_counter++;

        /* Signalling other threads */
        if (sem_post(&(threads[0].sem)) != 0) {
            fprintf(stderr, ">>> [ERROR] Some errors occured while managing semaphores\n");
            fflush(stderr);
            exit(EXIT_FAILURE);
        }
    }
    
    /* Closing file */
    close(input_file);

    /* Closing the thread */
    pthread_exit(NULL);
}

void *computing_polynomial(void *arg) {

    /* DATA */
    int thread_ID = *((int *) arg);
    float result = 0;
    int counter = 0;

    while (counter == 0) {
        /* Waiting for the other threads */
        for (int i = 0; i < N_THREADS -1; i++) {
            if (sem_wait(&(threads[thread_ID].sem)) != 0) {
                fprintf(stderr, ">>> [ERROR] Some errors occured while managing semaphores\n");
                fflush(stderr);
                exit(EXIT_FAILURE);
            }
        }

        /* Resetting counter */
        counter = 0;

        /* Computin polynomial result */
        for (int i = 1; i < N_THREADS; i++) {
            if (threads[i].end == true) {
                counter++;
            } else {
                result = result + threads[i].result;
            }
        }

        /* Closing execution if no data have been read */
        if (counter == N_THREADS-1) {
            pthread_exit(NULL);
        } else {

            /* Printing result */
            fprintf(stdout, "thread_ID: %2d - result: %.2f\n", threads[thread_ID].thread_ID, result);
            fflush(stdout);

            /* Signalling other threads */
            for (int i = 1; i < N_THREADS; i++) {
                if (sem_post(&(threads[i].sem)) != 0) {
                    fprintf(stderr, ">>> [ERROR] Some errors occured while managing semaphores\n");
                    fflush(stderr);
                    exit(EXIT_FAILURE);
                }
            }
        }
    }

    /* Closing thread */
    pthread_exit(NULL);
}