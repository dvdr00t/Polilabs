#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>

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

    /* VARIABLES */
    bool stop_iteration = false;
    int count_iteration = 0;

    if (signal(SIGUSR1, signal_manager) == SIG_ERR) {
        fprintf(stderr, "Some errors occurred while trying to define signal handler.\n");
        exit(EXIT_FAILURE);
    }
    pid_t PID = fork();

    /* Checking fork() execution */
    if (PID < 0) {
        fprintf(stderr, "Some errors occurred while trying to fork the process.\n");
        exit(EXIT_FAILURE);
    }

    while (!stop_iteration) {

        if (count_iteration > 5) {
            stop_iteration = true;
        }
        
        /* PARENT */
        if (PID > 0) {
            pause();
            fprintf(stdout, ">>> Parent %d woke-up ...\n", getpid());
            sleep(1);
            kill(PID, SIGUSR1);
        }
        /* CHILD */
        else if (PID == 0) {
            fprintf(stdout, ">>> Child %d woke-up ...\n", getpid());
            sleep(1);
            kill(getppid(), SIGUSR1);
            pause();
        }

        count_iteration++;  
        if (count_iteration > 5) {
            stop_iteration = true;
        }      
    }

    exit(EXIT_SUCCESS);
}
void signal_manager(int signal) {
    return;
}