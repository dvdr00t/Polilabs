#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>

#define MAX_DEPTH 100

/**
 * @brief Data type containing information about the current thread's history.
 * 
 */
typedef struct history {
	pthread_t history[MAX_DEPTH];
	int current_depth;
	int destination_depth;
} history_t;

void* routine(void* data);
history_t history_INIT(int current_depth, int destination_depth);
int thread_generation(history_t history);

/* MAIN FUNCTION */
int main(int args, char *argv[]) {

	/* CHECKING INPUT FROM COMMAND LINE */
	if (args != 2) {
		fprintf(stderr, "[USAGE] ./prg <degree_n>\n\n");
		exit(EXIT_FAILURE);
	}

	/* EXECUTING THE CORE PROGRAM */
	int n = atoi(argv[1]);
	history_t history = history_INIT(0, n);
	if (thread_generation(history) != 0) {
		fprintf(stderr, "[ERROR] thread_generation() failed execution.\n");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}

/**
 * @brief Given the history of the current thread, it generates two more theads, recording itself in the history
 * and then waits for the new threads generated to join.
 * 
 * @param history 
 * @return int 
 */
int thread_generation(history_t history) {

	/* ALLOCATING SPACE FOR THE THREADS */
	pthread_t *threads_ID = (pthread_t*) malloc(2* sizeof(pthread_t));
	if (threads_ID == NULL) {
		fprintf(stderr, "[ERROR] malloc() failed execution while allocating space in the heap for the threads.\n\n");
		exit(EXIT_FAILURE);
	}

	/* THREAD STARTS THE GENERATION */
	history.history[history.current_depth] = pthread_self();
	history.current_depth++;
	history_t history_1 = history;
	if (pthread_create(&threads_ID[0], NULL, routine, (void *) &history_1) != 0) {
        fprintf(stderr, "[ERROR] pthread_create() failed execution.\n");
        exit(EXIT_FAILURE);
    }
	history_t history_2 = history;
	if (pthread_create(&threads_ID[1], NULL, routine, (void *) &history_2) != 0) {
        fprintf(stderr, "[ERROR] pthread_create() failed execution.\n");
        exit(EXIT_FAILURE);
    }

	/* MAIN THREAD WAITS TO JOIN THE OTHER THREADS */
	void* exit_status;
	if (pthread_join(threads_ID[0], &exit_status) != 0) {
        fprintf(stderr, "[ERROR] pthread_join() failed execution.\n");
        exit(EXIT_FAILURE);
    }
	if (pthread_join(threads_ID[1], &exit_status) != 0) {
        fprintf(stderr, "[ERROR] pthread_join() failed execution.\n");
        exit(EXIT_FAILURE);
    }

	/* FREEING SPACE AND RETURNING SUCCESS */
	free(threads_ID);
	return EXIT_SUCCESS;
}

/**
 * @brief Initialize a history with a specific depth of threads generation and maximum depth.
 * 
 * @param current_depth 
 * @param destination_depth 
 * @return history_t 
 */
history_t history_INIT(int current_depth, int destination_depth) {
	history_t history;

	/* INITIALIZATION PROCESS */
	history.current_depth = current_depth;
	history.destination_depth = destination_depth;

	return history;
}

/**
 * @brief Each thread can perform two operation: if its current depth in the thread generation tree is
 * equal to the maximal depth, it has to print the history of the generation and terminates (it is like
 * a leaf). On the other hand, if it has not reached the maximal depth, it has to generates two more threads
 * and wait them to join (through the thread_generation() function). 
 * 
 * @param data 
 * @return void* 
 */
void* routine(void* data) {
	
	/* CONVERTING INPUT DATA TO RESPECTIVE TYPE */
	history_t* history = (history_t*) data;

	/* CHECKING OPERATIONS TO BE PERFORMED */
	if (history->current_depth == history->destination_depth) {
		char string_to_print[MAX_DEPTH];
		int index = 0;
		for (int i = 0; i < history->destination_depth; i++) {
			index += sprintf(&string_to_print[index], "%ld ", (long) history->history[i]);
		}
		sprintf(&string_to_print[index], "%ld\n", (long) pthread_self());
		fprintf(stdout, "%s", string_to_print);
	} else {
		history_t myhistory = *history;
		if (thread_generation(myhistory) != 0) {
			fprintf(stderr, "[ERROR] thread_generation() failed execution.\n");
			exit(EXIT_FAILURE);
		}
	}

	/* CLOSING THREAD */
	pthread_exit(NULL);
}