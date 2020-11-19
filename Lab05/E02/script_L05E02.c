#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/wait.h>

#define MAX_FILENAME_LENGTH 100

int sorting_simulation(int fp_BIN, char *filename_BIN, int number_of_process_required, int number_of_elements);
int creating_binary_file(int *fp_DST, char *filename_DST, FILE *fp_SRC, char *filename_SRC);
int main(int args, char *argv[]) {

    /*
    * Checking the number of arguments passed to the program by the user.
    * If the number of arguments is different from 2, exit with an error.
    */ 
    if (args != 2) {
        fprintf(stderr, ">>> ERROR! Usage: \"./prog_name file_name\"\n");
        exit(EXIT_FAILURE);
    }

    /* VARIABLES */
    int number_of_elements = 0; //Number of elements. It's the first number written in the file
    char filename_ASCII[MAX_FILENAME_LENGTH];   //Name of the ASCII file
    char *filename_BIN = "fileBIN.bin";         //Name of the BIN file
    FILE *fp_ASCII = NULL;  //File pointer of the ASCII file
    int fp_BIN = 0; //File pointer (actually, an integer value since it's going to be a BIN file) of the BIN file

    strcpy(filename_ASCII, argv[1]);

    /*
    * * * * * * * * * * * * * * * * * * * * * * * * *
    *                                               *
    *   W R I T I N G    B I N A R Y    F I L E     *
    *                                               *
    * * * * * * * * * * * * * * * * * * * * * * * * *
    */ 
    
    /* Generating the binary file containing the values */
    number_of_elements = creating_binary_file(&fp_BIN, filename_BIN, fp_ASCII, filename_ASCII);


    /*
    * * * * * * * * * * * * * * * * * * * * * * * * *
    *                                               *
    *   S O R T I N G    B I N A R Y    F I L E     *
    *                                               *
    * * * * * * * * * * * * * * * * * * * * * * * * *
    */ 

    /* Running bubble sort */

    /* Opening FILE and checking */
    fp_BIN = open(filename_BIN, O_RDWR);
    if (fp_BIN == -1) {
        fprintf(stderr, ">>> Some errors occurred while trying to open %s file.\n", filename_BIN);
        return -1;
    }

    /* Bubblesort */
    int j;
    for (int i = 0; i < number_of_elements - 1; i++) {

        /* Generating children */
        for (j = 0; j < number_of_elements - 1 - i; j++) {
            if (fork() > 0) {
                /* PARENT WAITS */
                wait((pid_t *) 0);
                break;
            }
        }

        /* CHILDREN ARRIVE HERE IN ORDER*/
        if (sorting_simulation(fp_BIN, filename_BIN, j, number_of_elements) == -1) {
            fprintf(stderr, ">>> Some errors occurred while trying to bubble sorting\n");
            exit(EXIT_FAILURE);
        }

        /* Terminating execution when parent arrives here */
        if (j > 0) {
            exit(EXIT_SUCCESS);
        }
    }
    close(fp_BIN);

    /*
    * * * * * * * * * * * * * * * * * * * * * * * * *
    *                                               *
    *   R E A D I N G    O R D E R E D    F I L E   *
    *                                               *
    * * * * * * * * * * * * * * * * * * * * * * * * *
    */ 

    /* Opening FILE and checking */
    fp_BIN = open(filename_BIN, O_RDONLY);
    if (fp_BIN == -1) {
        fprintf(stderr, ">>> Some errors occurred while trying to open %s file.\n", filename_BIN);
        return -1;
    }
    fprintf (stdout, ">>> File after ordering: ");

    /* Reading value from BIN file */
    int value_read;
    for (int i = 0; i < number_of_elements; i++) {

        /* Offset */
        lseek (fp_BIN, i*sizeof(int), SEEK_SET);

        /* Reading */
        int result = read (fp_BIN, &value_read, sizeof(int));
        if (result <= 0) {
            fprintf (stderr, ">>> Error reading file\n");
            exit(EXIT_FAILURE);
        }
        fprintf (stdout, "%d ", value_read);
    }
    fprintf (stdout, "\n");

    /* Terminate execution */
    close(fp_BIN);
    exit(EXIT_SUCCESS);
}
int creating_binary_file(int *fp_DST, char *filename_DST, FILE *fp_SRC, char *filename_SRC) {

    /* Opening FILE and checking for errors */
    fp_SRC = fopen(filename_SRC, "r");
    *fp_DST = creat(filename_DST, 0666);
    if (fp_SRC == NULL) {
        fprintf(stderr, ">>> Some errors occurred while trying to open %s file.\n", filename_SRC);
        return -1;
    }
    if (*fp_DST == -1) {
        fprintf(stderr, ">>> Some errors occurred while trying to open %s file.\n", filename_DST);
        return -1;
    }

    /* VARIABLES */
    int number_of_elements = 0;
    int tmp_value = 0;
    
    /* Reading data from file and checking for errors */
    fscanf(fp_SRC, "%d\n", &number_of_elements);
    if (number_of_elements <= 0 || isalpha(number_of_elements)) {
        fprintf(stderr, ">>> File %s contains errors. The first value should be a number greater than zero.\n", filename_SRC);
       return -1;
    }

    /* Copying values to binary file */
    fprintf(stdout, ">>> Number of element: %d\n", number_of_elements);
    fprintf(stdout, ">>> File before ordering: ");

    for (int i = 0; i < number_of_elements; i++) {
        fscanf(fp_SRC, "%d", &tmp_value);
        fprintf(stdout, "%d ", tmp_value);
        write(*fp_DST, &tmp_value, sizeof(int));
    }
    fprintf(stdout, "\n");
    fflush(stdout);

    fclose(fp_SRC);
    close(*fp_DST);
    return number_of_elements;
}
int sorting_simulation(int fp_BIN, char *filename_BIN, int number_of_process_required, int number_of_elements) {

    if (number_of_process_required >= number_of_elements -1) {
        return 0;
    }

    /* VARIABLES */
    int first_value, first_result;
    int second_value, second_result;

    /* Reading first and second variable in the BIN file and comparing them */
    lseek(fp_BIN, number_of_process_required * sizeof(int), SEEK_SET);  //Moving buffer in the BIN file
    first_result = read(fp_BIN, &first_value, sizeof(int)); //Reading
    lseek(fp_BIN, (number_of_process_required+1) * sizeof(int), SEEK_SET);  //Moving again buffer
    second_result = read(fp_BIN, &second_value, sizeof(int));   //Reading

    /* Checking that all the reading worked fine */
    if (first_result <= 0 || second_result <= 0) {
        fprintf(stderr, ">>> Some errors occurred while reading from %s file.\n", filename_BIN);
        exit(EXIT_FAILURE);
    }

    /* Swapping variable if the former is greater than the latter */
    if (first_value > second_value) {
        lseek(fp_BIN, number_of_process_required * sizeof(int), SEEK_SET);
        first_result = write(fp_BIN, &second_value, sizeof(int));
        lseek(fp_BIN, (number_of_process_required+1) * sizeof(int), SEEK_SET);
        second_result = write(fp_BIN, &first_value, sizeof (int));
    }

    /* Terminating execution */
    return 0;

}