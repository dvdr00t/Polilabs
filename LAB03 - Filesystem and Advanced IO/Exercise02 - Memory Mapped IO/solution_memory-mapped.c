/**
 * @file solution_memory-mapped.c
 * @author Davide Arcolini (davide.arcolini@studenti.polito.it)
 * @brief Map a file to memory and run 2 threads to work on the local memory in parallel
 * @version 0.1
 * @date 2022-04-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <pthread.h>

#define MAX_NAME_LENGTH 31
#define N_THREADS       2
#define DIRECTION_UP    1
#define DIRECTION_DOWN  0

/*
*   + --------------- +
*   | DATA STRUCTURES |
*   + --------------- +
*/
/**
 * @brief Abstract Data Type (ADT) used to serialize the input file in a binary file.
 * 
 */
typedef struct exam_outcome_s {
    int identifier;                 // Integer value
    long int register_number;       // Long integer value
    char name[MAX_NAME_LENGTH];     // 31 character
    char surname[MAX_NAME_LENGTH];  // 31 character
    int score;                      // Integer value
} exam_outcome_t;

/**
 * @brief Abstract Data Type (ADT) used as argument of the thread's routine function
 * 
 */
typedef struct thread_data_s {
    void *source;
    size_t copy_size;
    int direction;
} thread_data_t;

/*
*   + --------- +
*   | FUNCTIONS |
*   + --------- +
*/
void *t_routine(void *data);
int show_file(char *filename);

int main(int args, char *argv[]) {

    /* CHECKING USER INPUT */
    if (args != 2) {
        fprintf(stderr, "[USAGE] ./exe02 binary_filename\n");
        exit(EXIT_FAILURE);
    }

    /* OPENING USER INPUT FILE */
    char *filename = argv[1];
    if (show_file(filename) != 0) {
        fprintf(stderr, "[ERROR] show_file() failed execution\n");
        exit(EXIT_FAILURE);
    }
    int fd_input = open(filename, O_RDWR);
    if (fd_input < 0) {
        fprintf(stderr, "[ERROR] open() failed execution\n");
        exit(EXIT_FAILURE);
    }

    /* RETRIEVE SIZE OF INPUT FILE */
    struct stat file_buffer;
    if (fstat(fd_input, &file_buffer) < 0) {
        fprintf(stderr, "[ERROR] fstat() failed execution\n");
        exit(EXIT_FAILURE);
    }

    /* MAPPING FILE TO MEMORY */
    void    *start_address      = 0;                        /* Specifies the address where we want the mapped region to start (0: the OS chooses)   */
    size_t  copy_size           = file_buffer.st_size;      /* Number of bytes that will be mapped in memory                                        */
    int     protections         = PROT_READ | PROT_WRITE;   /* Specifies the protection of the mapped region                                        */ 
    int     flags               = MAP_SHARED;               /* Affects various attributes of the region (MAP_SHARED: a store operation is
                                                               equivalent to a write to the file)                                                   */
    off_t   offset              = 0;                        /* The starting offset in the file of the bytes to map                                  */

    void *source = (exam_outcome_t *) mmap(start_address, copy_size, protections, flags, fd_input, offset);
    if (source == MAP_FAILED) {
        fprintf(stderr, "[ERROR] mmap() failed execution\n");
        exit(EXIT_FAILURE);
    } 

    /* GENERATING THREADS */
    pthread_t threads_ID[N_THREADS];
    thread_data_t thread_packet[N_THREADS];
    for (int index_ID = 0; index_ID < N_THREADS; index_ID++) {

        /* CONSTRUCTING THE DATA STRUCTURE OF THE THREAD'S ROUTINE */
        thread_packet[index_ID].source = source;
        thread_packet[index_ID].copy_size = copy_size; 
        thread_packet[index_ID].direction = (index_ID == 0) ? DIRECTION_UP : DIRECTION_DOWN;

        /* CREATION OF THE THREADS */
        if (pthread_create(&threads_ID[index_ID], NULL, t_routine, (void *) &thread_packet[index_ID]) != 0) {
            fprintf(stderr, "[ERROR] pthread_create() failed execution\n");
            exit(EXIT_FAILURE);
        }
    }
    
    /* JOINING THREADS */
    int exit_status;
    for (int index_ID = 0; index_ID < N_THREADS; index_ID++) {
        if (pthread_join(threads_ID[index_ID], (void *) &exit_status) != 0) {
            fprintf(stderr, "[ERROR] pthread_join() failed execution\n");
            exit(EXIT_FAILURE);
        }
    }

    
    /* UNMAPPING FILE FROM MEMORY */
    if (munmap(source, copy_size) != 0) {
        fprintf(stderr, "[ERROR] munmap() failed execution\n");
        exit(EXIT_FAILURE);
    }

    /* CLOSING FILE */
    if (close(fd_input) != 0) {
        fprintf(stderr, "[ERROR] close() failed execution\n");
        exit(EXIT_FAILURE);
    }

    /* SHOWING CHANGES */
    if (show_file(filename) != 0) {
        fprintf(stderr, "[ERROR] show_file() failed execution\n");
        exit(EXIT_FAILURE);
    }

    /* PROGRAM ENDS SUCCESSFULLY */
    fprintf(stdout, "Done!\n");
    exit(EXIT_SUCCESS);
}

void *t_routine(void *data) {

    /* CASTING ARGUMENT TO CORRECT TYPE */
    thread_data_t *thread_packet = (thread_data_t *) data;   

    /* SWITCHING OPERATION TO BE PERFORMED */
    int N_records = thread_packet->copy_size / sizeof(exam_outcome_t);  /* Number of records in the file                            */
    exam_outcome_t *exam_record = thread_packet->source;                   /* Pointer to the current record in the memory mapped       */
    if (thread_packet->direction == DIRECTION_UP) {

        /* ITERATING OVER ALL THE RECORDS */
        for (int index = 0; index < N_records; index++) {
            exam_record->register_number += 1;                          /* Modifying value directly thanks to memory mapping        */
            exam_record++;                                              /* Moving to the next record (as a pointer)                 */
        }
    } else if (thread_packet->direction == DIRECTION_DOWN) {

        /* SETTING STARTING POINTER */
        exam_record = exam_record + N_records - 1;                      /* Last record in the file                                 */

        /* ITERATING OVER ALL THE RECORDS */
        for (int index = 0; index < N_records; index++) {
            exam_record->score -= 1;                                    /* Modifying value directly thanks to memory mapping        */
            exam_record--;                                              /* Moving to the previous record (as a pointer)             */
        }
    }

    /* THREAD ROUTINE COMPLETED SUCCESSFULLY */
    pthread_exit((void *) EXIT_SUCCESS);
}

int show_file(char *filename) {

    /* OPENING FILE */
    int file_descriptor = open(filename, O_RDONLY);
    if (file_descriptor == -1) {
        fprintf(stderr, "[ERROR] open() failed execution\n");
        return -1;
    }

    fprintf(stdout, "------------------------------\n");
    fprintf(stdout, "FILE: %s\n", filename);
    fprintf(stdout, "------------------------------\n");

    /* READING CONTENT */
    exam_outcome_t exam_record;
    while (read(file_descriptor, &exam_record, sizeof(exam_outcome_t)) == sizeof(exam_record)) {
        fprintf(stdout, "%d %ld %s %s %d\n", exam_record.identifier, exam_record.register_number, exam_record.name, exam_record.surname, exam_record.score);
    }

    /* CLOSING FILE */
    if (close(file_descriptor) != 0) {
        fprintf(stderr, "[ERROR] close() failed execution\n");
        return -1;
    }

    fprintf(stdout, "\n\n");

    /* OPERATION COMPLETED SUCCESSFULLY */
    return 0;
}