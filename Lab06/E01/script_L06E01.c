#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <string.h>
#include <ctype.h>

#define BUF_SIZE 16
#define MAX_LENGTH_STRING 1000

int toupper_string(char *string, int str_len);
void signal_manager(int signal);
void producer_routine(int fileDescr_write);
void consumer_routine(int fileDescr_read);
int main(int args, char *argv[]) {

    /*
    * Checking the number of arguments passed to the program.
    * The number of arguments should be zero.
    * Terminating the program if there are more arguments.
    */
    if (args != 1) {
        fprintf(stderr, "No arguments is required. Usage: ./progname\n");
        exit(EXIT_FAILURE);
    }

    /* Defining the signal handler */
    if (signal(SIGUSR1, signal_manager) == SIG_ERR) {
        fprintf(stderr, "Some errors occurred while trying to define signal handler.\n");
        exit(EXIT_FAILURE);
    }

    /* VARIABLES */
    int fileDescr[2];
    pid_t consumer_PID, producer_PID;


    /* Creating the pipe */
    int pipe_RES = pipe(fileDescr);
    if (pipe_RES == -1) {
        fprintf(stderr, "Some errors occurred while trying to pipe the process.\n");
        exit(EXIT_FAILURE);
    }

    /* Generating producer and consumer */
    consumer_PID = fork();

    /* Checking fork() execution */
    if (consumer_PID < 0) {
        fprintf(stderr, "Some errors occurred while trying to fork the process.\n");
        exit(EXIT_FAILURE);
    }
    else if (consumer_PID == 0) {

        /* Closing pipe stream for reading */
        close(fileDescr[1]);
        
        /* CHILD (CONSUMER) */  
        consumer_routine(fileDescr[0]);
    }
    else {

        /* PARENT */
        producer_PID = fork();

        /* Parent waits for producer and consumer to end execution */
        if (producer_PID > 0) {
            wait((pid_t *) 0);
            fprintf(stdout, "... Finishing execution ...\n");
            exit(EXIT_SUCCESS);
        }

        /* Checking fork() execution */
        if (producer_PID < 0) {
            fprintf(stderr, "Some errors occurred while trying to fork the process.\n");
            exit(EXIT_FAILURE);
        }
        else if (producer_PID == 0) {

            /* Closing pipe stream for reading */
            close(fileDescr[0]);

            /* CHILD (PRODUCER) */
            producer_routine(fileDescr[1]);

        }
    }

    exit(EXIT_SUCCESS);
}
void signal_manager(int signal) {
    return;
}
int toupper_string(char *string, int str_len) {
    for (int i = 0; i < str_len; i++) {
        string[i] = toupper(string[i]);
    }

    return 1;
}
void consumer_routine(int fileDescr_read) {

    /* VARIABLES */
    bool stop_routine = false;
    char string_upper[MAX_LENGTH_STRING];
    char buf_consumer[BUF_SIZE];
    pid_t producer_PID;
    int string_length;

    while (!stop_routine) {
        
        /* Waiting the producer to send the length of the string */
        read(fileDescr_read, buf_consumer, BUF_SIZE);

        /* Extracting information from the buffer */
        sscanf(buf_consumer, "%d %d", &producer_PID, &string_length);

        /* Sending the ACK back to the producer and waiting for the string to come */
        kill(producer_PID, SIGUSR1);
        read(fileDescr_read, string_upper, string_length);
        string_upper[string_length] = '\0';

        /* Checking for "end" string */
        if (strcmp(string_upper, "end") == 0) {
            kill(producer_PID, SIGKILL);
            break;
        }

        toupper_string(string_upper, string_length);
        fprintf(stdout, ">>> (PID: %d) Converted string: %s\n", (int) getpid(), string_upper);

        //Sending ACK to producer
        kill(producer_PID, SIGUSR1);

    }
    
    exit(EXIT_SUCCESS);
}
void producer_routine(int fileDescr_write) {

    /* VARIABLES */
    bool stop_routine = false;
    char string_lower[MAX_LENGTH_STRING];
    char buf_producer[BUF_SIZE];

    /* MAIN LOOP */
    while (!stop_routine) {

        // Receiving the string from the stdin
        fprintf(stdout, ">>> (PID: %d) Insert a string (\"end\" to terminate): ", (int) getpid());
        fscanf(stdin, "%s", string_lower);

        /* 
        * Preparing the pipe transmission by using a communication protocol.
        * 
        * Firstly, the length of the string is transmitted in order to:
        *   - Let the consumer discovering the size of the string it will receive;
        *   - Let the consumer allocating memory for the string.
        * 
        * Secondly, the string itself is transmitted to the consumer.
        * 
        * The produce pause(), waiting for an ACK received by the consumer
        */

        // Transmit the length of the string and wait for the consumer's ACK
        sprintf(buf_producer, "%06d %08d", getpid(), (int) strlen(string_lower));
        write(fileDescr_write, buf_producer, BUF_SIZE);
        pause();

        // Transmit the string and wait for the consumer's ACK
        write(fileDescr_write, string_lower, strlen(string_lower));
        pause();

    }
}
