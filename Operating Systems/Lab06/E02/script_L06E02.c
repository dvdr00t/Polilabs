#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <string.h>
#include <ctype.h>

#define MESSAGE_LENGTH 2

void parent_routine(int iteration, int pipe_descriptor_write, int pipe_descriptor_read);
void child_routine(int pipe_descriptor_write, int pipe_descriptor_read);
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

    /* VARIABLES */
    int pipe_descriptor_PARENT[2];
    int pipe_descriptor_CHILD[2];
    pid_t pid_CHILD;
    int iteration;

    /* Asking for number of iteration */
    fprintf(stdout, "How many iteration do you want to see: ");
    fscanf(stdin, "%d", &iteration);

    /* Creating the pipe */
    int pipe_result_PARENT = pipe(pipe_descriptor_PARENT);
    int pipe_result_CHILD = pipe(pipe_descriptor_CHILD);
    if (pipe_result_PARENT == -1 || pipe_result_CHILD == -1) {
        fprintf(stderr, "Some errors occurred while trying to pipe the process.\n");
        exit(EXIT_FAILURE);
    }

    /* Creating the child process */
    pid_CHILD = fork();
    
    if (pid_CHILD < 0) {
        fprintf(stderr, "Some errors occurred while trying to fork the process.\n");
        exit(EXIT_FAILURE);
    }
    else if (pid_CHILD == 0) {

        /* CHILD */
        close(pipe_descriptor_CHILD[1]);
        close(pipe_descriptor_PARENT[0]);

        child_routine(pipe_descriptor_PARENT[1], pipe_descriptor_CHILD[0]);

    }
    else {

        /* PARENT */
        close(pipe_descriptor_CHILD[0]);
        close(pipe_descriptor_PARENT[1]);

        parent_routine(iteration, pipe_descriptor_CHILD[1], pipe_descriptor_PARENT[0]);
    }

    exit(EXIT_SUCCESS);
}
void parent_routine(int iteration, int pipe_descriptor_write, int pipe_descriptor_read) {

    /* VARIABLES */
    bool stop_iteration = false;
    int counter_iteration = 0;
    char message_received[MESSAGE_LENGTH+1];

    /* Main loop */
    while (!stop_iteration) {

        ++counter_iteration;

        /* Printing message in the stdout */
        fprintf(stdout, ">>> (PID: %d) I am the father ...\n", (int) getpid());
        //sleep(1);

        /* Sending message to child */
        if (counter_iteration == iteration) {
            write(pipe_descriptor_write, "XX", MESSAGE_LENGTH);
            break;
        } else {
            write(pipe_descriptor_write, "GO", MESSAGE_LENGTH);
        }

        /* Waiting for child to finish process */
        read(pipe_descriptor_read, message_received, MESSAGE_LENGTH+1);
    }

    exit(EXIT_SUCCESS);
}
void child_routine(int pipe_descriptor_write, int pipe_descriptor_read) {

    /* VARIABLES */
    bool stop_iteration = false;
    char message_received[MESSAGE_LENGTH+1];

    /* Main loop */
    while (!stop_iteration) {

        /* Waiting for parent to finish process */
        read(pipe_descriptor_read, message_received, MESSAGE_LENGTH+1);

        if (strcmp(message_received, "XX") == 0) {
            stop_iteration = true;
        }

        /* Printing message in the stdout */
        fprintf(stdout, ">>> (PID: %d) I am the child ...\n", (int) getpid());
        //sleep(1);

        /* Sending message to parent */
        write(pipe_descriptor_write, "GO", MESSAGE_LENGTH);
    }

    exit(EXIT_SUCCESS);
}