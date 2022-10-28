#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define N_ITERATIONS 1

/* Defining the new type for the pipe semaphore */
typedef int* pipe_sem_t; 

/* pipe_sem_t related functions */
pipe_sem_t pipe_sem_create(int value);
void pipe_sem_destroy(pipe_sem_t sem);
int pipe_sem_wait(pipe_sem_t sem);
int pipe_sem_signal(pipe_sem_t sem);

/* Defining the semaphores */
pipe_sem_t sem_A;
pipe_sem_t sem_BCD;
pipe_sem_t sem_EF;
pipe_sem_t sem_G;
pipe_sem_t sem_H;
pipe_sem_t sem_I;

/* Defining the processes routine */
int process_A(); 
int process_B(); 
int process_C(); 
int process_D(); 
int process_E(); 
int process_F(); 
int process_G();
int process_H(); 
int process_I();

/* Generates the process from the main by means of fork() and starts the routine */
int spawn_process(int (*routine)(void), char name); 

int main(int args, char *argv[]) {

    /* Checking the number of arguemnts */
    if (args != 1) {
        fprintf(stderr, ">>> [USAGE]: ./progname\n");
        fflush(stderr);
        exit(EXIT_FAILURE);
    }

    /* Initializing the semaphores */
    sem_A = pipe_sem_create(1);
    sem_BCD = pipe_sem_create(0);
    sem_EF = pipe_sem_create(0);
    sem_G = pipe_sem_create(0);
    sem_H = pipe_sem_create(0);
    sem_I = pipe_sem_create(0);

    /*
    * The main process creates and manages all the other process for 
    * N_ITERATIONS times.
    * All the process are generated as child of the main process and 
    * use semaphores to accomplish synchronization.  
    */
    for (int i = 0; i < N_ITERATIONS; i++) {

        //spawning the processes
        if (spawn_process(process_A, 'A') == -1) {
            exit(EXIT_FAILURE);
        }
        if (spawn_process(process_B, 'B') == -1) {
            exit(EXIT_FAILURE);
        }
        if (spawn_process(process_C, 'C') == -1) {
            exit(EXIT_FAILURE);
        }
        if (spawn_process(process_D, 'D') == -1) {
            exit(EXIT_FAILURE);
        }
        if (spawn_process(process_E, 'E') == -1) {
            exit(EXIT_FAILURE);
        }
        if (spawn_process(process_F, 'F') == -1) {
            exit(EXIT_FAILURE);
        }
        if (spawn_process(process_G, 'G') == -1) {
            exit(EXIT_FAILURE);
        }
        if (spawn_process(process_H, 'H') == -1) {
            exit(EXIT_FAILURE);
        }
        process_I();
    }

    /* Deallocating memory for the semaphores */
    pipe_sem_destroy(sem_A);
    pipe_sem_destroy(sem_BCD);
    pipe_sem_destroy(sem_EF);
    pipe_sem_destroy(sem_G);
    pipe_sem_destroy(sem_H);
    pipe_sem_destroy(sem_I);

    //closing main with success
    exit(EXIT_SUCCESS);
}
int spawn_process(int (*routine)(void), char name) {
    pid_t pid_ID;
    pid_ID = fork();

    //checking if the fork() worked fine
    if (pid_ID == 0) {

        /* CHILD */
        if (routine() == -1) {
            //something went wrong in the process
            return -1;
        }
        
        /* Killing the CHILD process */
        exit(EXIT_SUCCESS);
    } else if (pid_ID > 0) {
        /* PARENT DOES NOTHING */
    } else {
        fprintf(stderr, ">>> [ERROR %c] some errors occured while trying to fork the process\n", name);
        fflush(stderr);
        return -1;
    }
    
    return 1;
}

/* pipe_sem_t related functions */
pipe_sem_t pipe_sem_create(int n_resources) {
    pipe_sem_t pipe_FD;

    //allocating memory for the file descriptor
    pipe_FD = (pipe_sem_t) malloc(2 * sizeof(int));
    if (pipe_FD == NULL) {
        fprintf(stderr, ">>> [ERROR] some erros occurred while allocating memory\n");
        fflush(stderr);
        exit(EXIT_FAILURE);
    }

    //creating the pipe
    if (pipe(pipe_FD) == -1) {
        fprintf(stderr, ">>> [ERROR] some erros occurred while managing pipe\n");
        fflush(stderr);
        exit(EXIT_FAILURE);
    }

    //allocating resources (writing in the pipe) for the semaphore
    for (int i = 0; i < n_resources; i++) {
        pipe_sem_signal(pipe_FD);
    }

    //returning the semaphore ready
    return pipe_FD;
}
void pipe_sem_destroy(pipe_sem_t sem) {
    close(sem[0]);
    close(sem[1]);
    free(sem);
}
int pipe_sem_wait(pipe_sem_t sem) {
    char token;
    if (read(sem[0], (void *) &token, 1) == -1) {
        fprintf(stderr, ">>> [ERROR] some errors occurred while trying to read from the pipe\n");
        fflush(stderr);
        return -1;
    }
    return 1;
}
int pipe_sem_signal(pipe_sem_t sem) {
    char token = '*';
    if (write(sem[1], (void *) &token, 1) == -1) {
        fprintf(stderr, ">>> [ERROR] some errors occurred while trying to write to the pipe\n");
        fflush(stderr);
        return -1;
    }
    return 1;
}


/* Processes routine */
int process_A() {
    /* 
    * Process A has to wait that I terminates the execution.
    * However, it starts with value 1 since it is the first process.
    */
    if (pipe_sem_wait(sem_A) == -1) {
        return -1;
    }

    /* Executing code */
    fprintf(stdout, "... process A is executing ...\n");
    fflush(stdout);

    /* Process A can now free resources for processes B, C and D */
    for (int i = 0; i < 3; i++) {
        if (pipe_sem_signal(sem_BCD) == -1) {
            return -1;
        }
    }

    /* Exiting the process with success */
    return 0;
}
int process_B() {
    /* Process B has to wait that A terminates the execution */
    if (pipe_sem_wait(sem_BCD) == -1) {
        return -1;
    }

    /* Operative code */
    fprintf(stdout, "... process B is executing ...\n");
    fflush(stdout);

    /* Process B can now free resources for I */
    if (pipe_sem_signal(sem_I) == -1) {
        return -1;
    }
    return 0;
}
int process_C() {
    /* Process C has to wait that B terminates the execution */
    pipe_sem_wait(sem_BCD);

    /* Operative code */
    fprintf(stdout, "... process C is executing ...\n");
    fflush(stdout);

    /* Process C can now free resources for E and F */
    for (int i = 0; i < 2; i++) {
        if (pipe_sem_signal(sem_EF) == -1) {
            return -1;
        }
    }
    return 0;
}
int process_D() {
    /* Process D has to wait that C terminates the execution */
    pipe_sem_wait(sem_BCD);

    /* Operative code */
    fprintf(stdout, "... process D is executing ...\n");
    fflush(stdout);

    /* Process D can now free resources for H */
    if (pipe_sem_signal(sem_H) == -1) {
        return -1;
    }
    return 0;
}
int process_E() {
    /* Process E has to wait that D terminates the execution */
    pipe_sem_wait(sem_EF);

    /* Operative code */
    fprintf(stdout, "... process E is executing ...\n");
    fflush(stdout);

    /* Process E can now free resources for G */
    if (pipe_sem_signal(sem_G) == -1) {
        return -1;
    }
    return 0;
}
int process_F() {
    /* Process F has to wait that E terminates the execution */
    pipe_sem_wait(sem_EF);

    /* Operative code */
    fprintf(stdout, "... process F is executing ...\n");
    fflush(stdout);

    /* Process F can now free resources for G */
    if (pipe_sem_signal(sem_G) == -1) {
        return -1;
    }
    return 0;

}
int process_G() {
    /* Process G has to wait that F terminates the execution */
    pipe_sem_wait(sem_G);
    pipe_sem_wait(sem_G);

    /* Operative code */
    fprintf(stdout, "... process G is executing ...\n");
    fflush(stdout);

    /* Process G can now free resources for I */
    if (pipe_sem_signal(sem_I) == -1) {
        return -1;
    }
    return 0;

}
int process_H() {
    /* Process H has to wait that G terminates the execution */
    pipe_sem_wait(sem_H);

    /* Operative code */
    fprintf(stdout, "... process H is executing ...\n");
    fflush(stdout);

    /* Process H can now free resources for I */
    if (pipe_sem_signal(sem_I) == -1) {
        return -1;
    }
    return 0;

}
int process_I() {
    /* Process I has to wait that H terminates the execution */
    pipe_sem_wait(sem_I);
    pipe_sem_wait(sem_I);
    pipe_sem_wait(sem_I);

    /* Operative code */
    fprintf(stdout, "... process I is executing ...\n");
    fflush(stdout);
    sleep(1);

    /* Process I can now free resources for A */
    if (pipe_sem_signal(sem_A) == -1) {
        return -1;
    }
    return 0;

}

