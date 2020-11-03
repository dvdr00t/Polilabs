#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <signal.h>

void generating_tree(int n, pid_t main_PID);
int main (int args, char *argv[]) {

    /*
    * Checking arguments passed from the command line.
    * The program should receive two integer values.
    * If not, a message error is sent to the user.
    */ 
    if (args != 3) {
        fprintf(stderr, ">>> Two and only two integer values are expected after the program name!\n\n");
        exit(EXIT_FAILURE);
    }

    /* VARIABLES */
    pid_t main_PID = getpid();    // Main process that will be killed as the least one
    int n = atoi(argv[1]);   // 2^n processes will be generated
    int t = atoi(argv[2]);   // Seconds wait by every leaf processes before terminating

    /* Generating the process tree */
    generating_tree(n, main_PID);

    /* 
    * If the process is not the main process that the program had identified in the beginning, 
    * the program is in a leaf of the tree and it will print the results.
    * The it will be killed.
    */
    if (getpid() != main_PID) {
        fprintf(stdout, "... Process %d sleeps for %d seconds ...\n", getpid(), t);
        sleep(t);
        fprintf(stdout, ">>> Process %d - END\n", getpid());
        exit(EXIT_SUCCESS);
    } 
    
    /*
    * If the process, instead, is the main process, the program waits for the user to kill it manually
    * pressing ctrl+C
    */
    else {
        fprintf(stdout, "Press ctrl+C to exit\n\n");
        while (1) {
            sleep(10);
            fprintf(stdout, "Ehi man, u sleeping?\n");
        }
    }
}
void generating_tree(int n, pid_t main_PID) {

    /* TERMINAL CONDITION */
    if (n == 0) {
        return;
    }

    /* Forking the process in order to generate the CHILD 1 */
    pid_t pid = fork();

    /* PARENT */
    if (pid != 0) {

        /* Forking the process in order to generate the CHILD 2 */
        pid = fork();

        /* 
        * STILL PARENT (the same as before obv) 
        * If this "parent" is the main process (i.e. getpid() is equal to the main_PID), the process
        * won't be killed but just return to the main function.
        * 
        * If this "parent" is a child of the main process (i.e. getpid() is not equal to the main_PID), 
        * the process will be killed.
        */
        if (pid != 0 && getpid() != main_PID) {
            exit(EXIT_SUCCESS);
        } else if (pid != 0 && getpid() == main_PID) {
            return;
        }
    }

    /* Only children arrives to this line */
    generating_tree(n-1, main_PID);
}