#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>
#include <fcntl.h>
#include <sys/select.h>

#define HEADER_SIZE 16


/* UTILITIES FUNCTIONS */
int generate_random_string(char *string, int size);
int toupper_string(char *string, int str_len);

/* MANAGES COMMUNICATION BETWEEN CHILDREN AND PARENT PROCESSES */
void signal_manager(int signal);

/* PROCESS ROUTINE */
int child_routine(int pipe, int WAIT_TIME, int STR_NUM, int STR_SIZE);
int parent_routine(int pipe_1, int pipe_2, int STR_NUM, int STR_SIZE);
int parent_process_message(int pipe, char *header, int STR_SIZE);

int main(int args, char *argv[]) {

    /* CHECKING USER INPUT */
    if (args != 5) {
        fprintf(stderr, "[USAGE] ./exe_A WAIT_TIME_1 WAIT_TIME_2 STR_NUM STR_SIZE\n");
        exit(EXIT_FAILURE);
    }

    /* PARSING USER INPUT */
    int WAIT_TIME_1 = atoi(argv[1]); 
    int WAIT_TIME_2 = atoi(argv[2]);
    int STR_NUM = atoi(argv[3]);
    int STR_SIZE = atoi(argv[4]);

    /* PIPES INITIALIZATION */
    int pipe_1[2], pipe_2[2];
    if (pipe(pipe_1) != 0 || pipe(pipe_2) != 0) {
        fprintf(stderr, "[ERROR] pipe() failed execution\n");
        exit(EXIT_FAILURE);
    }

    /* SIGNAL HANDLER INITIALIZATION */
    if (signal(SIGUSR1, signal_manager) == SIG_ERR) {
        fprintf(stderr, "[ERROR] signal() failed execution\n");
        exit(EXIT_FAILURE);
    }

    /* REMOVE BUFFERING FROM STDOUT */
    setbuf(stdout, 0);

    /* GENERATING CHILD C1 AND C2 */
    int C1_PID, C2_PID;
    C1_PID = fork();
    if (C1_PID < 0) {
        fprintf(stderr, "[ERROR] fork() failed execution\n");
        exit(EXIT_FAILURE);
    } else if (C1_PID == 0) {

        /* CHILD C1 */
        if (close(pipe_1[0]) != 0) {
            fprintf(stderr, "[ERROR] close() failed execution\n");
            exit(EXIT_FAILURE);
        }

        srand(time(NULL)); //random seed initialization
        if (child_routine(pipe_1[1], WAIT_TIME_1, STR_NUM, STR_SIZE) != 0) {
            fprintf(stderr, "[ERROR] child_routine() failed execution\n");
            exit(EXIT_FAILURE);
        }
    } else {

        /* PARENT */
        C2_PID = fork();
        if (C2_PID < 0) {
            fprintf(stderr, "[ERROR] fork() failed execution\n");
            exit(EXIT_FAILURE);
        } else if (C2_PID == 0) {

            /* CHILD C2 */
            if (close(pipe_2[0]) != 0) {
                fprintf(stderr, "[ERROR] close() failed execution\n");
                exit(EXIT_FAILURE);
            }

            srand(time(NULL) + 1); //random seed initialization
            if (child_routine(pipe_2[1], WAIT_TIME_2, STR_NUM, STR_SIZE) != 0) {
                fprintf(stderr, "[ERROR] child_routine() failed execution\n");
                exit(EXIT_FAILURE);
            }
        } else {

            /* PARENT */
            if (close(pipe_1[1]) != 0 || close(pipe_2[1]) != 0) {
                fprintf(stderr, "[ERROR] close() failed execution\n");
                exit(EXIT_FAILURE);
            }
            if (parent_routine(pipe_1[0], pipe_2[0], STR_NUM, STR_SIZE) != 0) {
                fprintf(stderr, "[ERROR] parent_routine() failed execution\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    /* PROGRAM EXECUTED CORRECTLY */
    exit(EXIT_SUCCESS);
}

int child_routine(int pipe, int WAIT_TIME, int STR_NUM, int STR_SIZE) {

    /* DATA USED */
    char header[HEADER_SIZE];
    char *message = (char *) malloc(STR_SIZE * sizeof(char));
    if (message == NULL) {
        fprintf(stderr, "[ERROR] malloc() failed execution\n");
        return -1;
    }
    
    /* GENERATING STR_NUM STRING */
    for (int counter = 0; counter < STR_NUM; counter++) {

        /* SLEEPING WAIT_TIME SECONDS */
        sleep(WAIT_TIME);

        /* GENERATING STRING */                                                     
        int size = rand() % STR_SIZE;
        if (generate_random_string(message, size) != 0) {
            fprintf(stderr, "[ERROR] generate_random_string() failed execution\n");
            return -1;
        }  

        /*
        *   SENDING MESSAGE OVER THE PIPE
        *   -----------------------------
        * 
        *   Communication protocol:
        *       - HEADER: the length of the string is transmitted in order to:
        *                   - let the parent discover the size of the string it will receive;
        *                   - let the parent allocate memory for the string.
        *       - BODY: the string itself is transmitted to the parent.
        * 
        *   The children pause(), waiting for an ACK received by the parent.
        */

        /* HEADER */
        sprintf(header, "%06d %08d", getpid(), (int) strlen(message));
        if (write(pipe, header, HEADER_SIZE) == -1) {
            fprintf(stderr, "[ERROR] write() failed execution\n");
            return -1;
        }
        pause();

        /* BODY MESSAGE */
        if (write(pipe, message, strlen(message)) == -1) {
            fprintf(stderr, "[ERROR] write() failed execution\n");
            return -1;
        }
        pause();
    }

    /* FREEING MEMORY */
    free(message);

    /* ROUTINE EXECUTED SUCCESSFULLY */
    return 0;
}
int parent_routine(int pipe_1, int pipe_2, int STR_NUM, int STR_SIZE) {

    /* DEFINING READING SET */
    fd_set reading_set;
    int maxfdp1 = (pipe_1 > pipe_2) ? pipe_1 : pipe_2 +1;      // largest value in [pipe_1, pipe_2]          

    /* INITIATE MULTIPLEXING OPERATION */
    char header[HEADER_SIZE];
    int count_messages = 0;
    bool running = true;
    while (running) {

        /* RESETTING SETS */
        FD_ZERO(&reading_set);
        FD_SET(pipe_1, &reading_set);
        FD_SET(pipe_2, &reading_set);

        /* MULTIPLEXING THE PIPES */
        if (select(maxfdp1, &reading_set, NULL, NULL, NULL) == -1) {
            fprintf(stderr, "[ERROR] select() failed execution\n");
            return -1;
        }

        /* CHECKING PIPE 1 */
        if (FD_ISSET(pipe_1, &reading_set)) {

            /* READING HEADER */
            if (read(pipe_1, header, HEADER_SIZE) == -1) {
                fprintf(stderr, "[ERROR] read() failed execution\n");
                return -1;
            }

            /* PROCESSING MESSAGE */
            if (parent_process_message(pipe_1, header, STR_SIZE) != 0) {
                fprintf(stderr, "[ERROR] parent_process_manager() failed execution\n");
                return -1;
            }

            /* COUNTING MESSAGE */
            count_messages++;
        }

        /* CHECKING PIPE 2 */
        if (FD_ISSET(pipe_2, &reading_set)) {

            /* READING HEADER */
            if (read(pipe_2, header, HEADER_SIZE) == -1) {
                fprintf(stderr, "[ERROR] read() failed execution\n");
                return -1;
            }

            /* PROCESSING MESSAGE */
            if (parent_process_message(pipe_2, header, STR_SIZE) != 0) {
                fprintf(stderr, "[ERROR] parent_process_manager() failed execution\n");
                return -1;
            }

            /* COUNTING MESSAGE */
            count_messages++;
        }

        /* CHECKING IF ALL THE MESSAGES HAVE BEEN RECEIVED */
        if (count_messages == 2*STR_NUM) {
            running = false;
        }
    }

    /* ROUTINE EXECUTED SUCCESSFULLY */
    return 0;
}

void signal_manager(int signal) {

    /* DO NOTHING, JUST MANAGE THE SIGNAL TRANSMISSION */
    return;
}

int toupper_string(char *string, int str_len) {
    for (int i = 0; i < str_len; i++) {
        string[i] = toupper(string[i]);
    }
    return 0;
}

int generate_random_string(char *string, int size) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyz ";
    if (size != 0) {
        size--;
        for (int n = 0; n < size; n++) {
            int key = rand() % (int) (sizeof charset - 1);
            string[n] = charset[key];
        }
        string[size] = '\0';
    } else {
        string = "default";
    }
    return 0;
}

int parent_process_message(int pipe, char *header, int STR_SIZE) {

    /* DATA USED */
    pid_t process_PID;
    int message_size;
    char *message = (char *) malloc(STR_SIZE * sizeof(char));
    if (message == NULL) {
        fprintf(stderr, "[ERROR] malloc() failed execution\n");
        return -1;
    }

    /* READING HEADER */
    sscanf(header, "%d %d", &process_PID, &message_size);

    /* SENDING ACK MESSAGE TO CHILD PROCESS */
    kill(process_PID, SIGUSR1);

    /* READING MESSAGE */
    int res = read(pipe, message, message_size);
    while (res <= 0) {
        res = read(pipe, message, message_size);
    }

    /* PARSING MESSAGE */
    message[message_size] = '\0';
    toupper_string(message, message_size);

    /* DISPLAYING MESSAGE TO SDOUT */
    fprintf(stdout, "> [HEADER]\n");
    fprintf(stdout, "> %s\n", header);
    fprintf(stdout, "> [MESSAGE]\n");
    fprintf(stdout, "> %s\n", message);
    fprintf(stdout, "------------------\n\n");

    /* SENDING ACK MESSAGE TO CHILD PROCESS */
    kill(process_PID, SIGUSR1);

    /* FREEING MEMORY */
    free(message);

    /* OPERATION COMPLETED SUCCESSFULLY */
    return 0;
}