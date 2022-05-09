#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <pthread.h>

#define MAX_PATH_NAME 100

/*
*   + --------- +
*   | FUNCTIONS |
*   + --------- +
*/

/**
 * @brief Runs N threads. Each thread visits a source directory recursively and print stats in the stdout.
 * 
 * @param source_directories 
 * @param N 
 * @return 0 on success.  
 */
static int visit_directories(char *source_directories[], int N);

/**
 * @brief Thread routine function
 * 
 * @param data 
 * @return (void *) EXIT_SUCCESS on success.
 */
static void *routine(void *data);

/**
 * @brief Visits a source directory recursively and print stats in the stdout with lock management.
 * 
 * @param path  
 * @return 0 on success. 
 */
static int visit_recursively(char *path);

/**
 * @brief Mutex lock to manage critical section.
 * 
 */
pthread_mutex_t mutex;

/**
 * @brief Main driver code.
 * 
 * @param args 
 * @param argv 
 * @return int 
 */
int main (int args, char *argv[]) {

    /* CHECKING USER INPUT */
    if (args <= 1) {
        fprintf(stderr, "[USAGE] ./exe04 path_1 ... <path_x>");
        exit(EXIT_FAILURE);
    }

    /* PARSING USER INPUT */
    int N = args-1;
    struct stat buffer;
    char **source_directories = (char **) malloc(N * sizeof(char *));
    if (source_directories == NULL) {
        fprintf(stderr, "[ERROR] malloc() failed execution\n\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 1, j = 0; i <= N; i++, j++) {

        /* CHECKING IF USER INPUT EXISTS AS A DIRECTORY */
        if (stat(argv[i], &buffer) != 0) {
            fprintf(stderr, "[ERROR] stat() function failed execution\n\n");
            exit(EXIT_FAILURE);
        }
        if (!S_ISDIR(buffer.st_mode)) {
            fprintf(stderr, "[ERROR] %s is not a path to an existing direectory...\n\n", argv[i]);
            exit(EXIT_FAILURE);
        }

        /* CREATING SPACE FOR STORING THE USER INPUT */ 
        source_directories[j] = (char *) malloc(MAX_PATH_NAME * sizeof(char));
        if (source_directories[j] == NULL) {
            fprintf(stderr, "[ERROR] malloc() failed execution\n\n");
            exit(EXIT_FAILURE);
        }
        sprintf(source_directories[j], "%s", argv[i]);
    }

    /* EXECUTING RECURSIVE VISIT WITH THREADS (VERSION C) */
    if (visit_directories(source_directories, N) != 0) {
        fprintf(stderr, "[ERROR] visit_directories() failed execution\n\n");
        exit(EXIT_FAILURE);
    }


    /* FREEING MEMORY */
    pthread_mutex_destroy (&mutex);
    free(*source_directories);
    exit(EXIT_SUCCESS);
}

int visit_directories(char *source_directories[], int N) {

    /* INITIATING MUTEX */
    pthread_mutex_init(&mutex, NULL);
    
    /* GENERATING THREADS */
    pthread_t *threads_ID = (pthread_t *) malloc(N * sizeof(pthread_t));
    if (threads_ID == NULL) {
        fprintf(stderr, "[ERROR] malloc() failed execution\n\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < N; i++) {
        if (pthread_create(&threads_ID[i], NULL, routine, (void *) source_directories[i]) != 0) {				
			fprintf(stderr, "[ERROR] pthread_create() failed execution.\n\n");
			exit(EXIT_FAILURE);
		}
    }

    /* JOINING THREADS */
	int exit_status;
	for (int i = 0; i < N; i++) {
		if (pthread_join(threads_ID[i], (void*) &exit_status) != 0) {
			fprintf(stderr, "[ERROR] pthread_create() failed execution.\n\n");
			exit(EXIT_FAILURE);
		}
	}

    /* FREEING SPACE */
    free(threads_ID);

    return 0;
}

void *routine(void *data) {

    /* CAST TO CORRECT TYPE */
    char *path = (char *) data;

    /* DISPLAYING SOURCE DIRECTORY INFORMATION */
    pthread_mutex_lock (&mutex);
    fprintf(stdout, "> [THREAD ID: %u] directory: %s\n", pthread_self(), path);
    pthread_mutex_unlock (&mutex);

    /* EXECUTING ROUTINE OF THE THREAD */
    if (visit_recursively(path) != 0) {
        fprintf(stderr, "[ERROR] visit_recursive() failed execution\n\n");
        return (void *) EXIT_FAILURE;
    }

    return (void *) EXIT_SUCCESS;
}

static int visit_recursively(char *path) {

    char *src_path_TMP;     //Path of the current element in working process in the directory

    /* OPENING SOURCE DIRECTORY AND CHECKING ERRORS */
    DIR *dp_src = opendir(path);
    if (dp_src == NULL) {
        fprintf(stderr, "[ERROR] opendir() function failed execution\n\n");
        return -1;
    }

    /* VISIT RECURSIVELY ALL THE TREE */
    struct dirent *dirp;
    struct stat buffer;
    while ((dirp = readdir(dp_src)) != NULL) {

        /*
        * ALLOCATING SPACE IN MEMORY FOR STORING TARGET PATH.
        * ---------------------------------------------------
        * Checking that the allocation has been successfully completed and then storing the argument 
        * passed by the user.
        */
        src_path_TMP = (char*) malloc((strlen(path) + strlen(dirp->d_name) + 1) * sizeof(char));
        if (src_path_TMP == NULL) {
            fprintf(stderr, "[ERROR] malloc() function failed execution\n\n");
            return -1;
        }
        sprintf(src_path_TMP, "%s/%s", path, dirp->d_name);

        /* CHECKING IF CURRENT VISITED NODE IS A FILE OR A DIRECTORY */
        if (lstat(src_path_TMP, &buffer) < 0) {
            fprintf(stderr, "[ERROR] Reading the content of %s failed during execution\n\n", src_path_TMP);
            return -1;
        }
        if (!S_ISDIR(buffer.st_mode)) {

            /* DISPLAYING FILE TO STDOUT */
            pthread_mutex_lock (&mutex);
            fprintf(stdout, "> [THREAD ID: %u] filename: %s\n", pthread_self(), dirp->d_name);
            pthread_mutex_unlock (&mutex);

        } else {
            if (strcmp(dirp->d_name, ".") != 0 && strcmp(dirp->d_name, "..") != 0) {

                /* DISPLAYING DIRECTORY TO STDOUT */
                pthread_mutex_lock (&mutex);
                fprintf(stdout, "> [THREAD ID: %u] directory: %s\n", pthread_self(), dirp->d_name);
                pthread_mutex_unlock (&mutex);

                /* VISIT RECURSIVELY */
                if (visit_recursively(src_path_TMP) != 0) {
                    fprintf(stderr, "[ERROR] visit_recursive() failed execution\n\n");
                    return -1;
                }
            }
        }
        /* FREEING MEMORY */
        free(src_path_TMP);
    }

    /* CLOSING SOURCE DIRECTORY AND CHECKING ERRORS */
    if (closedir(dp_src) != 0) {
        fprintf(stderr, "[ERROR] closedir() function failed execution\n\n");
        return -1;
    }
    return 0;
}