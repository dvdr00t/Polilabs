#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <pthread.h>

#define MAX_LENGTH 100
#define MAX_SIZE 10

/*
* Stores the name of the file and its ID number.
* For the first file passed as argument the ID is 1, for the second is 2, and so on...
*/
typedef struct myfile {
    char filename[MAX_LENGTH];
    int id;
} myfile_t;

/*
* Matrix defintion as a struct instead of an array of arrays.
* int size --> Stores the actual value of element in array.
*/
typedef struct mydata {
    int size;
    int array[MAX_SIZE];
} mydata_t;

/* GLOBAL VARIABLE */
mydata_t *mat;

/* QUICKSORT utility functions */
int partition(int *data, int left, int right);
void swap(int *a, int *b);
void sorting(int *data, int left, int right);

void shift(int *support_data, int pos, int size);   // Shift an array of one alement to the left
void merge_values(int *support_data, int id, int size); // Add the values stored in the global variable to the support array
void *routine(void* file_IN);   // Thread function
int main(int args, char *argv[]) {

    /*
    * Checking the number of arguments.
    * Usage ./progname N files1_in.txt files2_in.txt ... filesN_in.txt files_out.txt
    * If the numebr is different from 2, exit with error.
    */
    if (args != atoi(argv[1])+3) {
        fprintf(stderr, ">>> [ERROR] Usage: ./progname N files1_in.txt files2_in.txt ... filesN_in.txt files1_out.txt files2_out.txt ... filesN_out.txt\n");
        exit(EXIT_FAILURE);
    }

    /*
    *   V A R I A B L E S
    */
    int N = atoi(argv[1]);   // Number of files passed as argument
    myfile_t *files_IN;     // Array containing the name of the file and the file ID (1, 2, 3, ...)
    char file_OUT[MAX_LENGTH];  // Output filename
    FILE *fp_OUT;               // Output file pointer
    pthread_t *threads_ID;  // Array storing the IDs of the threads
    void *exitstatus;   // Catched by the join()
    int *support_data;  // Stores all the values merged from the global variable
    
    

    // Allocating memory for array of threads 
    threads_ID = (pthread_t *) malloc(N * sizeof(pthread_t));
    if (threads_ID == NULL) {
        fprintf(stderr, ">>> [ERROR] Some errors occurred while trying to allocate memory.\n");
        exit(EXIT_FAILURE);
    }

    // Allocating memory for array of data
    mat = (mydata_t *) malloc(N * sizeof(mydata_t));
    if (mat == NULL) {
        fprintf(stderr, ">>> [ERROR] Some errors occurred while trying to allocate memory.\n");
        exit(EXIT_FAILURE);
    }
    support_data = (int *) malloc(N*MAX_SIZE * sizeof(int));
    if (support_data == NULL) {
        fprintf(stderr, ">>> [ERROR] Some errors occurred while trying to allocate memory.\n");
        exit(EXIT_FAILURE);
    }

    // Copying files name
    files_IN = (myfile_t *) malloc(N * sizeof(myfile_t));
    if (files_IN == NULL) {
        fprintf(stderr, ">>> [ERROR] Some errors occurred while trying to allocate memory.\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < N; i++) {
        strcpy(files_IN[i].filename, argv[i+2]);
        files_IN[i].id = i;
    }
    strcpy(file_OUT, argv[N+2]);

    /* GENERATING THREADS */
    for (int i = 0; i < N; i++) {
        if (pthread_create(&threads_ID[i], NULL, routine, (void *) &files_IN[i]) != 0) {
            fprintf(stderr, ">>> [ERROR] Some errors occurred while trying to generate threads\n");
            exit(EXIT_FAILURE);
        }
        if (pthread_join(threads_ID[i], &exitstatus) != 0) {
            fprintf(stderr, ">>> [ERROR] Some errors occurred while trying to generate threads\n");
            exit(EXIT_FAILURE);
        }
        /* MERGING VALUES */
        merge_values(support_data, i, N*MAX_SIZE);
    }

    // Printing values to the file
    fp_OUT = fopen(file_OUT, "w");
    if (fp_OUT == NULL) {
        fprintf(stderr, ">>> [ERROR] Some errors occurred while trying to open file\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < N*MAX_SIZE; i++) {
        fprintf(fp_OUT, "%d\n", support_data[i]);
    }

    /* Free memory */
    free(mat);
    free(support_data);
    free(threads_ID);
    fclose(fp_OUT);
    exit(EXIT_SUCCESS);
}
void *routine(void* file_IN) {

    /* VARIABLES */
    myfile_t *files_new = (myfile_t *) file_IN; // Converting void* to specific type
    char filename_IN[MAX_LENGTH];   // Input filename
    int id_IN;  // Input ID
    FILE *fp_IN = NULL; 
    int N;  // Number of values in the file
    int *data;  // Stores the values in the file

    // Opening files
    strcpy(filename_IN, files_new->filename);   
    id_IN = files_new->id;
    fp_IN = fopen(filename_IN, "r");
    if (fp_IN == NULL) {
        fprintf(stderr, ">>> [ERROR] Some errors occurred while trying to managing file\n");
        exit(EXIT_FAILURE);
    }

    // Reading data from file
    fscanf(fp_IN, "%d\n", &N);
    mat[id_IN].size = N;
    data = (int *) malloc(N * sizeof(int));
    if (data == NULL) {
        fprintf(stderr, ">>> [ERROR] Some errors occurred while trying to allocate data in memory.\n");
        exit(EXIT_FAILURE);
    }

    // Reading values from file
    for (int i = 0; i < N; i++) {
        fscanf(fp_IN, "%d\n", &data[i]);
    }

    // Sorting values
    sorting(data, 0, N-1);

    // Adding values to global matrix
    for (int i = 0; i < N; i++) {
        mat[id_IN].array[i] = data[i];
    }

    free(data);
    fclose(fp_IN);
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
void merge_values(int *support_data, int id, int size) {
    int j;
    for (int i = 0; i < mat[id].size; i++) {
        for (j = 0; j < size; j++) {
            if (mat[id].array[i] > support_data[j]) {
                shift(support_data, j, size);
                support_data[j-1] = mat[id].array[i];
                break;
            }
        }
        support_data[j] = mat[id].array[i];
    }
}
void shift(int *support_data, int pos, int size) {
    for (int i = size; i > pos ; i--) {
        support_data[i-1] = support_data[i-2];
    }
}