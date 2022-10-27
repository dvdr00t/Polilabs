#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <pthread.h>

#define MAX_LENGTH 100

typedef struct files {
    char file_IN[MAX_LENGTH];
    char file_OUT[MAX_LENGTH];
} files_t;

int partition(int *data, int left, int right);
void swap(int *a, int *b);
void sorting(int *data, int left, int right);
void *routine(void* files);
int main(int args, char *argv[]) {

    /*
    * Checking the number of arguments.
    * Usage ./progname N files1_in.txt files2_in.txt ... filesN_in.txt files1_out.txt files2_out.txt ... filesN_out.txt
    * If the numebr is different from 2, exit with error.
    */
    if (args != 2*atoi(argv[1])+2) {
        fprintf(stderr, ">>> [ERROR] Usage: ./progname N files1_in.txt files2_in.txt ... filesN_in.txt files1_out.txt files2_out.txt ... filesN_out.txt\n");
        exit(EXIT_FAILURE);
    }

    /*
    *   V A R I A B L E S
    */
    int N = atoi(argv[1]);
    files_t *files;
    pthread_t *threads_ID;
    void *exitstatus;

    // Allocating memory for array of threads 
    threads_ID = (pthread_t *) malloc(N * sizeof(pthread_t));
    if (threads_ID == NULL) {
        fprintf(stderr, ">>> [ERROR] Some errors occurred while trying to allocate memory.\n");
        exit(EXIT_FAILURE);
    }

    // Copying files name
    files = (files_t *) malloc(N * sizeof(files_t));
    if (files == NULL) {
        fprintf(stderr, ">>> [ERROR] Some errors occurred while trying to allocate memory.\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < N; i++) {
        strcpy(files[i].file_IN, argv[i+2]);
        strcpy(files[i].file_OUT, argv[i+N+2]);
    }

    // Generating threads
    for (int i = 0; i < N; i++) {
        if (pthread_create(&threads_ID[i], NULL, routine, (void *) &files[i]) != 0) {
            fprintf(stderr, ">>> [ERROR] Some errors occurred while trying to generate threads\n");
            exit(EXIT_FAILURE);
        }
        if (pthread_join(threads_ID[i], &exitstatus) != 0) {
            fprintf(stderr, ">>> [ERROR] Some errors occurred while trying to generate threads\n");
            exit(EXIT_FAILURE);
        }
    }

    exit(EXIT_SUCCESS);
}
void *routine(void* files) {

    /* VARIABLES */
    files_t *files_new = (files_t *) files;
    char filename_IN[MAX_LENGTH];
    char filename_OUT[MAX_LENGTH];
    FILE *fp_IN = NULL;
    FILE *fp_OUT = NULL;    
    int N;
    int *data;

    // Opening files
    strcpy(filename_IN, files_new->file_IN);
    strcpy(filename_OUT, files_new->file_OUT);
    fp_IN = fopen(filename_IN, "r");
    fp_OUT = fopen(filename_OUT, "w");
    if (fp_IN == NULL || fp_OUT == NULL) {
        fprintf(stderr, ">>> [ERROR] Some errors occurred while trying to managing file\n");
        exit(EXIT_FAILURE);
    }

    // Reading data from file
    fscanf(fp_IN, "%d\n", &N);
    data = (int *) malloc(N * sizeof(int));
    if (data == NULL) {
        fprintf(stderr, ">>> [ERROR] Some errors occurred while trying to allocate data in memory.\n");
        exit(EXIT_FAILURE);
    }

    // Reading data from file
    for (int i = 0; i < N; i++) {
        fscanf(fp_IN, "%d\n", &data[i]);
    }

    // Sorting data
    sorting(data, 0, N-1);

    // Printing data to file
    for (int i = 0; i < N; i++) {
        fprintf(fp_OUT, "%d\n", data[i]);
    }

    free(data);
    fclose(fp_IN);
    fclose(fp_OUT);
    pthread_exit(NULL);
}
void sorting(int *data, int left, int right) {

    if (left >= right) {
        return;
    }

    int pivot = partition(data, left, right);
    sorting(data, left, pivot-1);
    sorting(data, pivot+1, right);
}
int partition(int *data, int left, int right) {
    int pivot = data[right];
    int i = left-1;

    for (int j = left; j <= right-1; j++) {
        if (data[j] < pivot) {
            i++;
            swap(&data[i], &data[j]);
        }
    }
    swap(&data[i+1], &data[right]);
    return i+1;
}
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}