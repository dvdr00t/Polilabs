#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <aio.h>

#define HEADER_SIZE 16

/* GLOBAL DATA */
bool pipe_1_PENDING         = false;        /* Defines the status of the asynchronous operation on pipe_1           */
bool pipe_2_PENDING         = false;        /* Defines the status of the asynchronous operation on pipe_2           */
int count_messages          = 0;            /* Counts the number of messages read on the pipes                      */
int pipe_1[2];                              /* Pipe                                                                 */
int pipe_2[2];                              /* Pipe                                                                 */
int STR_SIZE;                               /* Size of the messages defined by the user as input                    */
char header1[HEADER_SIZE];
char header2[HEADER_SIZE];


/* UTILITIES FUNCTIONS */
int generate_random_string(char *string, int size);
int toupper_string(char *string, int str_len);

/* MANAGES COMMUNICATION BETWEEN ASYNCHRONOUS OPERATION */
void communication_protocol(int signal);
void signal_manager_1(int signal);
void signal_manager_2(int signal);

/* PROCESS ROUTINE */
int child_routine(int pipe, int WAIT_TIME, int STR_NUM, int STR_SIZE);
int parent_routine(int pipe_1, int pipe_2, int STR_NUM, int STR_SIZE);
int parent_process_reading(int pipe, char *header);

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
    STR_SIZE = atoi(argv[4]);

    /* PIPES INITIALIZATION */
    if (pipe(pipe_1) != 0 || pipe(pipe_2) != 0) {
        fprintf(stderr, "[ERROR] pipe() failed execution\n");
        exit(EXIT_FAILURE);
    }

    /* SIGNAL HANDLER INITIALIZATION */
    if (signal(SIGCONT, communication_protocol) == SIG_ERR || signal(SIGUSR1, signal_manager_1) == SIG_ERR || signal(SIGUSR2, signal_manager_2) == SIG_ERR) {
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
        kill(getppid(), SIGCONT);
        pause();
    }

    /* FREEING MEMORY */
    free(message);

    /* ROUTINE EXECUTED SUCCESSFULLY */
    return 0;
}
int parent_routine(int pipe_1, int pipe_2, int STR_NUM, int STR_SIZE) {

    /* INITIALIZING AIO CONTROL BLOCKS */
    struct aiocb control_block_1, control_block_2;

    control_block_1.aio_fildes                  = pipe_1;               /* File Descriptor                                              */
    control_block_1.aio_offset                  = 0;                    /* Read or writes start at the offset specified                 */
    control_block_1.aio_buf                     = header1;              /* Buffer used to transfer data for a read or write operation   */
    control_block_1.aio_nbytes                  = HEADER_SIZE;          /* Size of the buffer pointed to by aio_buf                     */
    control_block_1.aio_reqprio                 = 0;                    /* Determine the priority for execution of this I/O request     */
    control_block_1.aio_sigevent.sigev_notify   = SIGEV_SIGNAL;         /* Signal specified by the sigev_signo field is generated when 
                                                                           the asynchronous I/O request completes                       */
    control_block_1.aio_sigevent.sigev_signo    = SIGUSR1;              /* Signal adopted                                               */

    control_block_2.aio_fildes                  = pipe_2;               /* File Descriptor                                              */
    control_block_2.aio_offset                  = 0;                    /* Read or writes start at the offset specified                 */
    control_block_2.aio_buf                     = header2;              /* Buffer used to transfer data for a read or write operation   */
    control_block_2.aio_nbytes                  = HEADER_SIZE;          /* Size of the buffer pointed to by aio_buf                     */
    control_block_2.aio_reqprio                 = 0;                    /* Determine the priority for execution of this I/O request     */
    control_block_2.aio_sigevent.sigev_notify   = SIGEV_SIGNAL;         /* Signal specified by the sigev_signo field is generated when 
                                                                           the asynchronous I/O request completes                       */
    control_block_2.aio_sigevent.sigev_signo    = SIGUSR2;              /* Signal adopted                                               */

    /* INITIATE ASYNCHRONOUS OPERATION */
    bool running = true;
    while (running) {

        /* ASYNCHRONOUS READING ON PIPE 1 (IF NOT PENDING) */
        if (!pipe_1_PENDING) {

            /* INITIATE OPERATION */
            fprintf(stdout, "[!] scheduling async reading on pipe1\n");
            aio_read(&control_block_1);


            /* OPERATION HERE IS PENDING */
            pipe_1_PENDING = true;
        }

        /* ASYNCHRONOUS READING ON PIPE 2 (IF NOT PENDING) */
        if (!pipe_2_PENDING) {

            /* INITIATE OPERATION */
            fprintf(stdout, "[!] scheduling async reading on pipe2\n");
            aio_read(&control_block_2);

            /* OPERATION HERE IS PENDING */
            pipe_2_PENDING = true;

        }

        /* CHECKING IF ALL THE MESSAGES HAVE BEEN RECEIVED */
        if (count_messages == 2*STR_NUM) {
            running = false;
        }
    }

    /* ROUTINE EXECUTED SUCCESSFULLY */
    return 0;
}

void communication_protocol(int signal) {

    /* DO NOTHING, JUST MANAGE THE SIGNAL TRANSMISSION */
    return;
}

void signal_manager_1(int signal) {

    /* PERFORMING READING OPERATION */
    fprintf(stdout, "[!] Ready to read from pipe1\n");
    if (parent_process_reading(pipe_1[0], header1) != 0) {
        fprintf(stderr, "[ERROR] parent_process_reading() failed execution\n");
        exit(EXIT_FAILURE);
    }

    /* COUNTING MESSAGES */
    count_messages++;

    /* OPERATION COMPLETED SUCCESSFULLY */
    pipe_1_PENDING = false;
    return;
}

void signal_manager_2(int signal) {

    /* PERFORMING READING OPERATION */
    fprintf(stdout, "[!] Ready to read from pipe2\n");
    if (parent_process_reading(pipe_2[0], header2) != 0) {
        fprintf(stderr, "[ERROR] parent_process_reading() failed execution\n");
        exit(EXIT_FAILURE);
    }

    /* COUNTING MESSAGES */
    count_messages++;
    
    /* OPERATION COMPLETED SUCCESSFULLY */
    pipe_2_PENDING = false;
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

int parent_process_reading(int pipe, char *header) {

    /* DATA USED */
    pid_t process_PID;
    int message_size;
    char *message = (char *) malloc(STR_SIZE * sizeof(char));
    if (message == NULL) {
        fprintf(stderr, "[ERROR] malloc() failed execution\n");
        return -1;
    }
    sscanf(header, "%d %d", &process_PID, &message_size);

    /* SENDING ACK MESSAGE TO CHILD PROCESS */
    kill(process_PID, SIGCONT);
    pause();

    /* READING MESSAGE */
    if (read(pipe, message, message_size) == -1) {
        fprintf(stderr, "[ERROR] read() failed execution\n");
        return -1;
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
    kill(process_PID, SIGCONT);

    /* FREEING MEMORY */
    free(message);

    /* OPERATION COMPLETED SUCCESSFULLY */
    return 0;
}