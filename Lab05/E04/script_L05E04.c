#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <string.h>
#include <ctype.h>

int toupper_string(char *string, int str_len);
void signal_manager(int signal);
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
    FILE *fp_transmission = NULL;
    char *filename = "transmission.txt";

    bool stop_iteration = false;
    char string_lower[1000];
    char string_upper[1000];

    pid_t consumer_PID, producer_PID;

    /* Generating producer and consumer */
    consumer_PID = fork();

    /* Checking fork() execution */
    if (consumer_PID < 0) {
        fprintf(stderr, "Some errors occurred while trying to fork the process.\n");
        exit(EXIT_FAILURE);
    }
    else if (consumer_PID == 0) {

        /* CHILD (CONSUMER) */  
        while (!stop_iteration) {

            //Waiting for producer to execute
            pause();

            /* Opening FILE and checking */
            fp_transmission = fopen(filename, "r");
            if (fp_transmission == NULL) {
                fprintf(stderr, "Some errors occurred while trying to open %s file.\n", filename);
            }

            /* Reading from FILE */
            fscanf(fp_transmission, "%d %s\n", &producer_PID, string_upper);

            /* Closing FILE */
            fclose(fp_transmission);

            /* Checking for "end" string */
            if (strcmp(string_upper, "end") == 0) {
                kill(producer_PID, SIGKILL);
                exit(EXIT_SUCCESS);
            }

            toupper_string(string_upper, strlen(string_upper));
            fprintf(stdout, ">>> (PID: %d) Converted string: %s\n", (int) getpid(), string_upper);

            //Seinding signal to producer
            kill(producer_PID, SIGUSR1);
        }
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

            /* CHILD (PRODUCER) */
            while (!stop_iteration) {

                fprintf(stdout, ">>> (PID: %d) Insert a string (\"end\" to terminate): ", (int) getpid());
                fscanf(stdin, "%s", string_lower);

                /* Opening FILE and checking */
                fp_transmission = fopen(filename, "w");
                if (fp_transmission == NULL) {
                    fprintf(stderr, "Some errors occurred while trying to open %s file.\n", filename);
                }

                /* Printing to FILE */
                fprintf(fp_transmission, "%d %s\n", getpid(), string_lower);

                /* Closinf FILE */
                fclose(fp_transmission);

                //Sending signal to consumer
                kill(consumer_PID, SIGUSR1);

                //Waiting for consumer to execute
                pause();
            }
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