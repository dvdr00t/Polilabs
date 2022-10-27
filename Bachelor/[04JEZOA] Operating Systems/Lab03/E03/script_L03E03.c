/*
 Using the system calls fork and wait, realize the following precedence graph.

        P1
       /  \ 
      /    \ 
    P2      P3
    /\      /\
   /  \    /  \
 P4   P5  |   P6
   \  /    \  /
    \/      \/
    P7      P8
      \    /
       \  / 
        P9

Note that all arcs are oriented downwards. Each Pi process corresponds to a print message 
(print the string “Pi” and the PID of the process).
Check the precedence are respected by inserting the system call sleep in the 
various branches of the program.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void signal_handler(int sig);
void print_message(int process_N, pid_t pid_N);
int main(int argc, char *argv[]) {    

    /*
    * Checking arguments passed from the command line.
    * The program should receive zero arguments.
    * If not, a message error is sent to the user.
    */ 
    if (argc != 1) {
        fprintf(stderr, ">>> Zero and only zero integer values are expected after the program name!\n\n");
        exit(EXIT_FAILURE);
    }

    /* VARIABLES */
    pid_t pid = getpid();   // Main process pid
    signal(SIGALRM, signal_handler);

    /* EXECUTING >>> PROCESS P1 */
    print_message(1, pid);
    sleep(1);

    pid = fork();
    if (pid == 0) {

        /* CHILD --> P3 = P8 */

        //Waiting for P2 before proceeding
        pause();

        /* EXECUTING >>> PROCESS P3 */
        print_message(3, getpid());
        sleep(1);

        //Send a message to P4: P3 finished execution, time to go!
        kill(getppid(), SIGALRM);
        
        pid = fork();
        if (pid == 0) {

            /* CHILD --> P6 */

            //Waiting a message from P5 before proceeding (MESSAGE PATH: P5 -> P1 -> P3 -> P6)
            pause();

            /* EXECUTING >>> PROCESS P6 */
            print_message(6, getpid());
            sleep(1);

            //Send a message to P7 (MESSAGE PATH: P6 -> P3 -> P1 = P7): P6 finished execution, time to go!
            exit(EXIT_SUCCESS);

        } else if (pid > 0) {

            /* PARENT --> P3 = P8 */
            
            //Waiting for P4 to send a message to P6
            pause();
            kill(pid, SIGALRM);

            //Waiting for P6 to send a message to P1 = P7
            wait((int *) 0);

            //P3 sending a message to P1 = P7 --> READY TO GO!
            kill(getppid(), SIGALRM);
            pause();

            /* EXECUTING >>> PROCESS P8 */
            print_message(8, getpid());
            sleep(1);

        } else {

            /* HANDLE FORK ERROR */
            fprintf(stderr, "• Some errors occurred while trying to fork the process %d\n", getpid());
            exit(EXIT_FAILURE);
        }
        
        exit(0);

    } else if (pid > 0) {

        /* PARENT --> P1 = P2 = P4 = P7 = P9 */

        /* EXECUTING >>> PROCESS P2 */
        print_message(2, getpid());
        sleep(1);

        //Sending message to P3: P2 finished execution, time to go!
        kill(pid, SIGALRM);

        pid_t pid_P3 = pid;
        pid = fork();

        if (pid == 0) {

            /* CHILD --> P5 */
            
            /* PROCESS P5 */

            //Waiting for a message from P4
            pause();

            /* EXECUTING >>> PROCESS P5 */
            print_message(5, getpid());
            sleep(1);

            //Closing process
            exit(EXIT_SUCCESS);

        } else if (pid > 0) {

            /* PARENT --> P1 = P2 = P4 = P7 = P9 */

            //Waiting a message from P3
            pause();

            /* EXECUTING >>> PROCESS P4 */
            print_message(4, getpid());
            sleep(1);

            //Sending message to P5: P4 finished execution, time to go!
            kill(pid, SIGALRM);

            //Waiting for P5 to terminate
            wait(NULL);

            //Send a message to P6 (MESSAGE PATH: P4 -> P3 -> P6)
            kill(pid_P3, SIGALRM);

            //Waiting for a  message from P6 (MESSAGE PATH: P6 -> P3 -> P7)
            pause();

            /* EXECUTING >>> PROCESS P7 */
            print_message(7, getpid());
            sleep(1);

            //Sending a message to P3 = P8 (MESSAGE PATH: P7 = P1 -> P3 = P8)
            kill(pid_P3, SIGALRM);

        } else {

            /* HANDLE FORK ERROR */
            fprintf(stderr, "• Some errors occurred while trying to fork the process %d\n", getpid());
            exit(EXIT_FAILURE);
        }

        /* Waiting the exectution of process P8 */
        wait((int *) 0);

        /* EXECUTING >>> PROCESS P9 */
        print_message(9, getpid());
        
        exit(EXIT_SUCCESS);

    } else {

        /* HANDLE FORK ERROR */
        fprintf(stderr, "• Some errors occurred while trying to fork the process %d\n", getpid());
        exit(EXIT_FAILURE);
    }

}
void print_message(int process_N, pid_t pid_N) {
    fprintf(stdout, ">>> Executing process %d [PID: %d]\n", process_N, pid_N);
}
void signal_handler(int sig) {
    return;
}