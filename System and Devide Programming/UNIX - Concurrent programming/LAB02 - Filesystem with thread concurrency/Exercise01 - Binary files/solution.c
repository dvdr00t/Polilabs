#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_NAME_LENGTH 31
#define READ_ONLY "r"
#define WRITE_ONLY "w"

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

/*
*   + --------- +
*   | FUNCTIONS |
*   + --------- +
*/

/**
 * @brief Convert the content of the first ASCII file into a binary file.
 * 
 * @param fp_ASCII 
 * @param fp_BIN 
 * @return 0 on success 
 */
int from_ASCII_to_BIN(FILE *fp_ASCII, int fp_BIN);

/**
 * @brief Convert the content of the first binary file into a ASCII file.
 * 
 * @param fp_BIN 
 * @param fp_ASCII 
 * @return int 
 */
int from_BIN_to_ASCII(int fp_BIN, FILE *fp_ASCII);

/**
 * @brief Main driver code.
 * 
 * @param args 
 * @param argv 
 * @return int 
 */
int main(int args, char *argv[]) {
    
    /* CHECKING USER INPUT */
    if (args != 4) {
        fprintf(stderr, "[USAGE] ./exe01 file_1 file_2 file_3\n\n");
        exit(EXIT_FAILURE);
    }

    /* PARSING USER INPUT */
    char *filename_input_1 = argv[1];       // ASCII INPUT FILE
    char *filename_input_2 = argv[2];       // BINARY FILE
    char *filename_input_3 = argv[3];       // ASCII OUTPUT FILE

    /* OPENING USER FILES */
    FILE *fp_input_1 = fopen(filename_input_1, READ_ONLY);
    int fp_input_2 = open(filename_input_2, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    FILE *fp_input_3 = fopen(filename_input_3, WRITE_ONLY);
    if (fp_input_1 == NULL) {
		fprintf(stderr, "[ERROR] opening %s failed.\n\n", filename_input_1);
		exit(EXIT_FAILURE);
	} else if (fp_input_2 == -1) {
		fprintf(stderr, "[ERROR] opening %s failed.\n\n", filename_input_2);
		exit(EXIT_FAILURE);
	} else if (fp_input_3 == NULL) {
		fprintf(stderr, "[ERROR] opening %s failed.\n\n", filename_input_3);
		exit(EXIT_FAILURE);
	}

    /* CONVERTING ASCII INPUT FILE TO BINARY FILE */
    if (from_ASCII_to_BIN(fp_input_1, fp_input_2) != 0) {
        fprintf(stderr, "[ERROR] Conversion from ASCII to BINARY failed execution\n\n");
        exit(EXIT_FAILURE);
    }
    
    /* CLOSING FILES */
    fclose(fp_input_1);
    close(fp_input_2);

    /* OPENING USER FILES */
    fp_input_2 = open(filename_input_2, O_RDONLY);
    if (fp_input_2 == -1) {
		fprintf(stderr, "[ERROR] opening %s failed.\n\n", filename_input_2);
		exit(EXIT_FAILURE);
	}

    /* CONVERTING BINARY FILE BACK TO ASCII FILE */
    if (from_BIN_to_ASCII(fp_input_2, fp_input_3) != 0) {
        fprintf(stderr, "[ERROR] Conversion from BINARY to ASCII failed execution\n\n");
        exit(EXIT_FAILURE);
    }
    
    /* CLOSING FILES */
    close(fp_input_2);
    fclose(fp_input_3);

    exit(EXIT_SUCCESS);
}

int from_ASCII_to_BIN(FILE *fp_ASCII, int fp_BIN) {

    /* READING ASCII FILE UNTIL EOF */
    exam_outcome_t exam_tmp;
    while (fscanf(fp_ASCII, "%d %ld %s %s %d\n", &exam_tmp.identifier, &exam_tmp.register_number, exam_tmp.name, exam_tmp.surname, &exam_tmp.score) != EOF) {
        if (write(fp_BIN, &exam_tmp, sizeof(exam_outcome_t)) != sizeof(exam_outcome_t)) {
            fprintf(stderr, "[ERROR] write operation failed execution\n\n");
            return -1;
        }
    }
    return 0;
}

int from_BIN_to_ASCII(int fp_BIN, FILE *fp_ASCII) {
    /* READING BINARY FILE UNTIL EOF */
    exam_outcome_t exam_tmp;
    while (read(fp_BIN, &exam_tmp, sizeof(exam_outcome_t)) == sizeof(exam_tmp)) {
        fprintf(fp_ASCII, "%d %ld %s %s %d\n", exam_tmp.identifier, exam_tmp.register_number, exam_tmp.name, exam_tmp.surname, exam_tmp.score);
    }
    return 0;
}