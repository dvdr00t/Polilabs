/**
 * @file myLibrary.c
 * @author Davide Arcolini (davide.arcolini@studenti.polito.it)
 * @brief This file contains the definitions of the functions used in solution.c
 * @version 0.1
 * @date 2022-04-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "myLibrary.h"

#define END_MESSAGE     "ENDOFDIRECTORY"
#define MAX_ENTRY_NAME  256

/*
+----------------+
| DATA STRUCTURE |
+----------------+
*/

/**
 * @brief Data structure used in the reading routine
 * 
 */
typedef struct reading_struct_s {
    int thread_ID;
    char current_directory_name[MAX_ENTRY_NAME];
    char current_file_name[MAX_ENTRY_NAME];
    sem_t reading_semaphore;
} reading_struct_t;

/**
 * @brief Data structure used in the comparing routine
 * 
 */
typedef struct comparing_struct_s {
    int N_reading_threads;                      /* NUMBER OF READING THREADS        */
    reading_struct_t *reading_threads;          /* POINTER TO THE READING THREADS   */
} comparing_struct_t;

/**
 * @brief Semaphore for the comparing thread.
 * 
 */
sem_t comparing_semaphore;

/*
+-----------+
| FUNCTIONS |
+-----------+
*/

/**
 * @brief Visit a directory entry recursively
 * 
 * @param thread_DATA the structure of the thread
 * @return 0 on success
 */
static int recursive_visit(reading_struct_t *thread_DATA) {

    /* DATA USED */
    DIR *dp_src;
    struct stat buffer;
    struct dirent *dirp;
    char old_entry_name[MAX_ENTRY_NAME];
    char new_entry_name[MAX_ENTRY_NAME];

    strcpy(old_entry_name, thread_DATA->current_directory_name);

    /* OPENING SOURCE DIRECTORY AND CHECKING ERRORS */
    if (lstat(old_entry_name, &buffer) < 0) {
        fprintf(stderr, "[ERROR] lstat() failed execution\n");
        return -1;
    } else if (S_ISDIR(buffer.st_mode) == 0) {
        fprintf(stderr, "[ERROR] not a directory\n");
        return -1;
    } else if ((dp_src = opendir(old_entry_name)) == NULL) {
        fprintf(stderr, "[ERROR] opendir() failed execution\n");
        return -1;
    }

    /* LOOPING THROUGH ALL ENTRIES */
    while((dirp = readdir(dp_src)) != NULL) {
        strcpy(thread_DATA->current_file_name, dirp->d_name);
        
        /* SIGNALLING COMPARING THREADS */
        if (sem_post(&comparing_semaphore) != 0) {
            fprintf(stderr, "[ERROR] sem_post() failed excution\n");
            return -1;
        }

        /* WAITING FOR ALL OTHER READING THREADS */
        if (sem_wait(&thread_DATA->reading_semaphore) != 0) {
            fprintf(stderr, "[ERROR] sem_wait() failed excution\n");
            return -1;
        }

        /* RECURSIVELY VISIT */
        if (S_ISDIR(buffer.st_mode) == 0) {

            /* EXCLUDING SYSTEM DIRECTORY */
            if (strcmp(dirp->d_name, ".") == 0 || strcmp(dirp->d_name, "..") == 0) {
                continue;
            } else {
                sprintf(old_entry_name, "%s", thread_DATA->current_directory_name);
                sprintf(new_entry_name, "%s", thread_DATA->current_directory_name);
                strcat(new_entry_name, "/");
                strcat(new_entry_name, dirp->d_name);
                strcpy(thread_DATA->current_directory_name, new_entry_name);

                if (recursive_visit(thread_DATA) != 0) {
                    fprintf(stderr, "[ERROR] recursive_visit() failed execution\n");
                    return -1;
                }

                strcpy(thread_DATA->current_file_name, old_entry_name);
            }
        }
    }

    /* CLOSING ENTRY */
    if (closedir(dp_src) != 0) {
        fprintf(stderr, "[ERROR] closedir() failed execution\n");
        return -1;
    }

    /* FUNCTION EXECUTED CORRECTLY */
    return 0;
}

/**
 * @brief Reading thread's routine
 * 
 * @param data 
 * @return void* 
 */
static void *reading_routine(void *data) {

    /* CASTING THREAD DATA */
    reading_struct_t *thread_DATA = (reading_struct_t *) data;

    /* VISIT DIRECTORY RECURSIVELY */
    if (recursive_visit(thread_DATA) != 0) {
        fprintf(stderr, "[ERROR] recursive_visit() failed execution\n");
        pthread_exit((void *) EXIT_FAILURE);
    }

    /* MESSAGING END OF DIRECTORY */
    strcpy(thread_DATA->current_file_name, END_MESSAGE);

    /* SIGNALLING COMPARING THREAD */
    if (sem_post(&comparing_semaphore) != 0) {
        fprintf(stderr, "[ERROR] sem_post() failed execution\n");
        pthread_exit((void *) EXIT_FAILURE);
    }

    /* THREAD ROUTINE ENDS CORRECTLY */
    pthread_exit((void *) EXIT_SUCCESS);
}

/**
 * @brief Comparing thread's routine
 * 
 * @param data comparing_struct_t
 * @return void* 
 */
static void *comparing_routine(void *data) {

    /* CASTING THREAD DATA */
    comparing_struct_t *thread_DATA = (comparing_struct_t *) data;

    /* DATA USED */
    int N_threads_ended;        /* counts the number of theads that has completed execution */

    /* LOOPING */
    bool running = true;
    while(running) {

        /* WAITING FOR ALL THE THREADS */
        for (int i = 0; i < thread_DATA->N_reading_threads; i++) {
            if (sem_wait(&comparing_semaphore) != 0) {
                fprintf(stderr, "[ERROR] sem_wait() failed execution\n");
                exit(EXIT_FAILURE);
            }
        }

        /* RESETTING ENDING THREADS COUNTER */
        N_threads_ended = 0;
        for (int i = 0; i < thread_DATA->N_reading_threads; i++) {
            if (strcmp(thread_DATA->reading_threads[i].current_file_name, END_MESSAGE) == 0) {
                N_threads_ended++;
            } else {
                if (i > 0 && strcmp(thread_DATA->reading_threads[0].current_file_name, thread_DATA->reading_threads[i].current_file_name) != 0) {
                    fprintf(stdout, "> %s DIFFER.\n", thread_DATA->reading_threads[i].current_file_name);
                    fprintf(stdout, "> ... NUCLEAR IMPACT (destroying all threads) ...\n");
                    exit(EXIT_FAILURE);
                }
            }
        }

        /* NO THREAD HAS ENDED OR ALL ENDED FINE */
        if (N_threads_ended == 0 || N_threads_ended == thread_DATA->N_reading_threads) {
            fprintf(stdout, "> ... it seems so quiet ...\n");
        } else {
            fprintf(stdout, "> ... NUCLEAR IMPACT (destroying all threads) ...\n");
            exit(EXIT_FAILURE);
        }

        /* RELEASING ALL READING THREADS */
        for (int i = 0; i < thread_DATA->N_reading_threads; i++) {
            if (sem_post(&thread_DATA->reading_threads[i].reading_semaphore) != 0) {
                fprintf(stderr, "[ERROR] sem_post() failed execution\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    /* THREAD ROUTINE ENDS CORRECTLY */
    pthread_exit((void *) EXIT_SUCCESS);
}

/**
 * @brief Wrapper function to solve the exercise
 * 
 * @param source_directories the list of directories to be compared
 * @param N_DIRS the number of directories to be compared
 * @return 0 on success
 */
int solver(char **source_directories, int N_DIRS) {

    /*
    * The main program runs one "reading" thread for each directory tree
    * plus one unique "comparing" thread.
    */

    /* ALLOCATING SPACE FOR THREADS UTILITIES */
    pthread_t *reading_threads_ID = (pthread_t *) malloc((N_DIRS) * sizeof(pthread_t));
    reading_struct_t *reading_threads_DATA = (reading_struct_t *) malloc((N_DIRS) * sizeof(reading_struct_t));
    if (reading_threads_ID == NULL || reading_threads_DATA == NULL) {
        fprintf(stderr, "[ERROR] malloc() failed execution\n");
        return -1;
    }

    /* INITIALIZING COMPARING SEMAPHORE */
    if (sem_init(&comparing_semaphore, 0, 0) != 0) {
        fprintf(stderr, "[ERROR] sem_init() failed execution\n");
    }

    /* GENERATING COMPARING THREAD */
    pthread_t comparing_thread_ID;
    comparing_struct_t comparing_thread_DATA;
    comparing_thread_DATA.N_reading_threads = N_DIRS;
    comparing_thread_DATA.reading_threads = reading_threads_DATA;
    if (pthread_create(&comparing_thread_ID, NULL, comparing_routine, (void *) &comparing_thread_DATA) != 0) {
        fprintf(stderr, "[ERROR] pthread_create() failed execution\n");
        return -1;
    }

    /* GENERATING READING THREADS THREADS */
    for (int id = 0; id < N_DIRS; id++) {

        /* THREAD DATA INITIALIZATION */
        reading_threads_DATA[id].thread_ID = id;
        strcpy(reading_threads_DATA[id].current_directory_name, source_directories[id]);
        if (sem_init(&(reading_threads_DATA[id].reading_semaphore), 0, 0) != 0) {
            fprintf(stderr, "[ERROR] sem_init() failed execution\n");
            return -1;
        }

        if (pthread_create(&reading_threads_ID[id], NULL, reading_routine, (void *) &reading_threads_DATA[id]) != 0) {
            fprintf(stderr, "[ERROR] pthread_create() failed execution\n");
            return -1;
        }
    }

    /* JOINING READING THREADS */
    int exit_status;
    for (int id = 0; id < N_DIRS; id++) {
        if (pthread_join(reading_threads_ID[id], (void *) &exit_status) != 0) {
            fprintf(stderr, "[ERROR] pthread_join() failed execution\n");
            return -1;
        }

        /* CHECKING EXIT STATUS */
        if (exit_status == EXIT_FAILURE) {
            fprintf(stderr, "[ERROR] some reading thread exits with error: %d\n", EXIT_FAILURE);
            return -1;
        }
    }

    /* FREEING MEMORY */
    free(reading_threads_ID);
    free(reading_threads_DATA);

    /* SOLVER EXECUTED CORRECTLY */
    fprintf(stdout, "> You must be a master of concurrent programming...\n");
    return 0;
}