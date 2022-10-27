#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <ctype.h>
#include <semaphore.h>

#define MAX_LENGTH 100

/* Data structure */
typedef struct thread_data_s {
    FILE *input_file;
    FILE *output_file;
    char input_line[MAX_LENGTH];
    char output_line[MAX_LENGTH];
    bool end;
} thread_data_t;

/* Threads routines */
void *reading_line(thread_data_t *packet);
void *reversing_line(thread_data_t *packet);
void *writing_line(thread_data_t *packet);

/* Semaphores */
sem_t sem_reading;
sem_t sem_reversing;
sem_t sem_writing;

int main(int args, char *argv[]) {

    /* Checking arguments */
    if (args != 3) {
        fprintf(stderr, ">>> [USAGE] ./progname inputname outputname\n");
        fflush(stderr);
        exit(EXIT_FAILURE);
    }

    /* DATA */
    thread_data_t *packet;
    pthread_t threads_ID[3];

    /* Allocating memory for packet data  */
    packet = (thread_data_t *) malloc(sizeof(thread_data_t));
    if (packet == NULL) {
	fprintf(stderr, ">>> [ERROR] Some errors occurred while trying to allocate space in memory\n");
	fflush(stderr);
	exit(EXIT_FAILURE);
    }
    packet->end = false;

    /* Opening files */
    packet->input_file = fopen(argv[1], "r");
    packet->output_file = fopen(argv[2], "w");
    if (packet->input_file == NULL || packet->output_file == NULL) {
        fprintf(stderr, ">>> [ERROR] Some errors occurred while trying to open the files\n");
        fflush(stderr);
        exit(EXIT_FAILURE);
    }

    /* Initializing semaphores */
    sem_init(&sem_reading, 0, 0);
    sem_init(&sem_reversing, 0, 0);
    sem_init(&sem_writing, 0, 0);

    /* Creating the threads */
    if (pthread_create(&threads_ID[0], NULL, (void *)reading_line, packet) != 0 ||
        pthread_create(&threads_ID[1], NULL, (void *)reversing_line, packet) != 0 ||
        pthread_create(&threads_ID[2], NULL, (void *)writing_line, packet) != 0) {
            fprintf(stderr, ">>> [ERROR] Some errors occurred while trying to create the threads\n");
            fflush(stderr);
            exit(EXIT_FAILURE);
    }

    /* Joining threads */
    for (int i = 0; i < 3; i++) {
        if (pthread_join(threads_ID[i], NULL) != 0) {
            fprintf(stderr, ">>> [ERROR] Some errors occurred while trying to join the threads\n");
            fflush(stderr);
            exit(EXIT_FAILURE);
        }
    }

    /* Destroying the semaphores */
    sem_destroy(&sem_reading);
    sem_destroy(&sem_reversing);
    sem_destroy(&sem_writing);

    /* Closing files  */
    fclose(packet->input_file);
    fclose(packet->output_file);

    /* Closing the program successfully */
    exit(EXIT_SUCCESS);
}

/* Threads routines */
void *reading_line(thread_data_t *packet) {

    /* Reading line from file until it reaches the EOF */
    while (fgets(packet->input_line, MAX_LENGTH, packet->input_file) != NULL) {
        /* Posting on the reversing function */
        if (sem_post(&sem_reversing) != 0) {
            fprintf(stderr, ">>> [ERR0R] Some errors occurred with the semaphore\n");
            fflush(stderr);
            exit(EXIT_FAILURE);
        }

        /* Waiting */
        if (sem_wait(&sem_reading) != 0) {
            fprintf(stderr, ">>> [ERR0R] Some errors occurred with the semaphore\n");
            fflush(stderr);
            exit(EXIT_FAILURE);
        }
    }

    /* Terminating the other threads */
    packet->end = true;
    if (sem_post(&sem_reversing) != 0) {
        fprintf(stderr, ">>> [ERR0R] Some errors occurred with the semaphore\n");
        fflush(stderr);
        exit(EXIT_FAILURE);
    }
    if (sem_post(&sem_writing) != 0) {
        fprintf(stderr, ">>> [ERR0R] Some errors occurred with the semaphore\n");
        fflush(stderr);
        exit(EXIT_FAILURE);
    }

    pthread_exit(NULL);
}
void *reversing_line(thread_data_t *packet) {
    /* Checking end of execution */
    while (packet->end == false) {

        /* Waiting */
        if (sem_wait(&sem_reversing) != 0) {
            fprintf(stderr, ">>> [ERR0R] Some errors occurred with the semaphore\n");
            fflush(stderr);
            exit(EXIT_FAILURE);
        }

        /* Reversing the line */
        int i, j;
        for (i = 0, j = strlen(packet->input_line); i < strlen(packet->input_line)-1; i++, j--) {
            packet->output_line[i] = toupper(packet->input_line[j-2]);
        }

        /* Deleting the rest of the line */
	    for (int k = i; k < MAX_LENGTH; k++) {
	        packet->output_line[k] = '\0';
	    }

        /* Posting on writing function */
        if (sem_post(&sem_writing) != 0) {
            fprintf(stderr, ">>> [ERR0R] Some errors occurred with the semaphore\n");
            fflush(stderr);
            exit(EXIT_FAILURE);
        }
    }
    pthread_exit(NULL);
}
void *writing_line(thread_data_t *packet) {
    /* Checking end of execution */
    while (packet->end == false) {

        /* Waiting */
        if (sem_wait(&sem_writing) != 0) {
            fprintf(stderr, ">>> [ERR0R] Some errors occurred with the semaphore\n");
            fflush(stderr);
            exit(EXIT_FAILURE);
        }

        /* Writing the new line on the output file */
        fprintf(packet->output_file, "%s\n", packet->output_line);

        /* Posting on reading function */
        if (sem_post(&sem_reading) != 0) {
            fprintf(stderr, ">>> [ERR0R] Some errors occurred with the semaphore\n");
            fflush(stderr);
            exit(EXIT_FAILURE);
        }
    }
    pthread_exit(NULL);
}