#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define MAX_BUFFER_SIZE 75
#define MAX_NAME_LENGTH 31

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
 * @brief Menu for the user.
 * 
 * @param fd 
 * @return 0 on success.
 */
int menu (int fd);

/**
 * @brief Read from the file associated to the file descriptor fd, the record corresponding to the user identified by the identifier.
 * 
 * @param fd 
 * @param identifier 
 * @return 0 on success.
 */
int read_from_file(int fd, int identifier);

/**
 * @brief Read from stdin all exam information and write them to the file.
 * 
 * @param fd 
 * @param identifier
 * @return 0 on success. 
 */
int write_to_file(int fd, int identifier);


int lock_region (int fd, int cmd, int type, off_t offset, int whence, off_t len); 

/**
 * @brief Main driver code.
 * 
 * @param args 
 * @param argv 
 * @return int 
 */
int main (int args, char *argv[]) {

    /* CHECKING USER INPUT */
    if (args != 2) {
        fprintf(stderr, "[USAGE] ./exe02 filename\n\n");
        exit(EXIT_FAILURE);
    }

    /* PARSING USER INPUT */
    char *filename_input_1 = argv[1];       

    /* OPENING USER FILES */
    int fd_input = open(filename_input_1, O_RDWR);
    if (fd_input == -1) {
        fprintf(stderr, "[ERROR] opening %s failed.\n\n", filename_input_1);
		exit(EXIT_FAILURE);
    }

    /* CALLING MENU */
    int running = TRUE;
    while (running) {
        if (menu(fd_input) != 0) {
            running = FALSE;
        }
    }

    /* CLOSING FILES */
    close(fd_input);

    exit(EXIT_SUCCESS);
}

int menu (int fd) {
    
    /* DISPLAYING THE MENU */
    fprintf(stdout, "WELCOME: \n\n");
    fprintf(stdout, "> R n  [Read from file the record representing student number n and display this information on standard output]\n");
    fprintf(stdout, "> W n  [Read from standard input all data fields for the student number n and write them on the file in position n]\n");
    fprintf(stdout, "> E    [End the program]\n\n");
    fprintf(stdout, "Operation: ");

    /* READING CHOICE */
    char user_input[MAX_BUFFER_SIZE];
    fgets(user_input, MAX_BUFFER_SIZE, stdin);

    /* PERFORMING OPERATIONS */
    if (user_input[0] == 'E') {
        fprintf(stdout, "\n... ending program execution ...\n\n");
        return -1;
    } else if (user_input[0] == 'R') {

        /* EXTRACTING THE SECOND PARAMETER PASSED */
        char *token = strtok(user_input, " ");
        token = strtok(NULL, " ");
        
        /* EXECUTING RECORD READING */
        if (read_from_file(fd, atoi(token)) != 0) {
            fprintf(stderr, "[ERROR] Reading record from file failed execution.\n\n");
            return -1;
        }
        return 0;

    } else if (user_input[0] == 'W') {

        /* EXTRACTING THE SECOND PARAMETER PASSED */
        char *token = strtok(user_input, " ");
        token = strtok(NULL, " ");
        
        /* EXECUTING RECORD READING */
        if (write_to_file(fd, atoi(token)) != 0) {
            fprintf(stderr, "[ERROR] Reading record from file failed execution.\n\n");
            return -1;
        }
        return 0;

    }

    /* DEFAULT CASE */
    return -1;
}

int read_from_file(int fd, int identifier) {

    exam_outcome_t exam_tmp;

    /* SETTING NEW OFFSET FOR READING */
    int offset = (identifier-1)*sizeof(exam_outcome_t);
    if (lseek(fd, offset, SEEK_SET) == -1) {
        fprintf(stderr, "[ERROR] Setting offset failed execution.\n\n");
        return -1;
    }

    /* LOCKING RECORD */
    lock_region(fd, F_SETLK, F_RDLCK, offset, SEEK_SET, sizeof(exam_outcome_t));

    /* READING CORRESPONDING DATA */
    if (read(fd, &exam_tmp, sizeof(exam_outcome_t)) != sizeof(exam_tmp)) {
        fprintf(stderr, "[ERROR] Reading from file failed execution.\n");
        return -1;
    }

    /* SHOWING CORRESPONDING DATA */
    fprintf(stdout, ">>> Read from file: %d %ld %s %s %d\n\n", exam_tmp.identifier, exam_tmp.register_number, exam_tmp.name, exam_tmp.surname, exam_tmp.score);

    /* UNLOCKING RECORD */
    lock_region(fd, F_SETLK, F_UNLCK, offset, SEEK_SET, sizeof(exam_outcome_t));
    
    return 0;
}

int write_to_file(int fd, int identifier) {

    exam_outcome_t exam_tmp;
    exam_tmp.identifier = identifier;

    /* SETTING NEW OFFSET FOR WRITING */
    int offset = (identifier-1)*sizeof(exam_outcome_t);
    if (lseek(fd, offset, SEEK_SET) == -1) {
        fprintf(stderr, "[ERROR] Setting offset failed execution.\n\n");
        return -1;
    }

    /* LOCKING RECORD */
    lock_region(fd, F_SETLK, F_WRLCK, offset, SEEK_SET, sizeof(exam_outcome_t));

    /* PRINTING INFORMATION FOR THE USER */
    fprintf(stdout, "Insert data corresponding to record %d.\n", identifier);
    fprintf(stdout, "Format is [register_number name surname score].\n");
    fprintf(stdout, "Insert: ");

    /* READING INFORMATION FROM THE INPUT */
    char user_input[MAX_BUFFER_SIZE];
    fgets(user_input, MAX_BUFFER_SIZE, stdin);

    /* SPLITTING STRING */
    char *token = strtok(user_input, " ");
    exam_tmp.register_number = atoi(token);
    token = strtok(NULL, " ");
    strcpy(exam_tmp.name, token);
    token = strtok(NULL, " ");
    strcpy(exam_tmp.surname, token);
    token = strtok(NULL, " ");
    exam_tmp.score = atoi(token);

    /* WRITING INFORMATION BACK TO FILE */
    if (write(fd, &exam_tmp, sizeof(exam_outcome_t)) != sizeof(exam_outcome_t)) {
        fprintf(stderr, "[ERROR] write operation failed execution\n\n");
        return -1;
    }

    /* UNLOCKING RECORD */
    lock_region(fd, F_SETLK, F_UNLCK, offset, SEEK_SET, sizeof(exam_outcome_t));

    fprintf(stdout, "... writing completed ...\n");
    return 0;
}

int lock_region (int fd, int cmd, int type, off_t offset, int whence, off_t len) {

    struct flock lock;

    lock.l_type = type;
    lock.l_start = offset;
    lock.l_whence = whence;
    lock.l_len = len;

    return (fcntl(fd, cmd, &lock)); 
}