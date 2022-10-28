#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

#define N_ITERATIONS 1
#define N_THREADS 9

/* Threads routines */
void *routine_A();
void *routine_B();
void *routine_C();
void *routine_D();
void *routine_E();
void *routine_F();
void *routine_G();
void *routine_H();
void *routine_I();
void *(*routine[N_THREADS])(void);

/* Semaphores */
sem_t sem_A;
sem_t sem_BCD;
sem_t sem_EF;
sem_t sem_G;
sem_t sem_H;
sem_t sem_I;

int main(int args, char *argv[]) {

    /* Checking arguments from command line */
    if (args != 1) {
        fprintf(stderr, ">>> [USAGE] ./progname\n");
        fflush(stderr);
        exit(EXIT_FAILURE);
    }

    /* DATA */
    pthread_t threads_ID[N_THREADS];

    /* Initializing semaphores */
    sem_init(&sem_A, 0, 1);
    sem_init(&sem_BCD, 0, 0);
    sem_init(&sem_EF, 0, 0);
    sem_init(&sem_G, 0, 0);
    sem_init(&sem_H, 0, 0);
    sem_init(&sem_I, 0, 0);

    /* Initializing array of function pointers */
    routine[0] = routine_A;
    routine[1] = routine_B;
    routine[2] = routine_C;
    routine[3] = routine_D;
    routine[4] = routine_E;
    routine[5] = routine_F;
    routine[6] = routine_G;
    routine[7] = routine_H;
    routine[8] = routine_I;

    /* Creating the threads */
    for (int n = 0; n < N_THREADS; n++) {
        if (pthread_create(&threads_ID[n], NULL, (void *)routine[n], NULL) != 0) {
            fprintf(stderr, ">>> [ERROR] Some errors occurred while trying to create the threads\n");
            fflush(stderr);
            exit(EXIT_FAILURE);
        }
    }

    /* Joining the threads */
    for (int n = 0; n < N_THREADS; n++) {
        if (pthread_join(threads_ID[n], NULL) != 0) {
            fprintf(stderr, ">>> [ERROR] Some errors occurred while trying to joining the threads\n");
            fflush(stderr);
            exit(EXIT_FAILURE);
        }
    }

    /* Destroying semaphores */
    sem_destroy(&sem_A);
    sem_destroy(&sem_BCD);
    sem_destroy(&sem_EF);
    sem_destroy(&sem_G);
    sem_destroy(&sem_H);
    sem_destroy(&sem_I);

    /* Closing execution with success */
    exit(EXIT_SUCCESS);
}

void *routine_A() {
    for (int i = 0; i < N_ITERATIONS; i++) {
        /* Waiting for semaphore A */
        sem_wait(&sem_A);

        /* Working */
        fprintf(stdout, "... Executing thread A ...\n");
        fflush(stdout);

        /* Posting the other semaphores */
        for (int i = 0; i < 3; i++) {
            sem_post(&sem_BCD);
        }
    }
}
void *routine_B() {
    for (int i = 0; i < N_ITERATIONS; i++) {
        /* Waiting for semaphore BCD */
        sem_wait(&sem_BCD);

        /* Working */
        fprintf(stdout, "... Executing thread B ...\n");
        fflush(stdout);

        /* Posting the other semaphores */
        sem_post(&sem_I);
    }
}
void *routine_C() {
    for (int i = 0; i < N_ITERATIONS; i++) {
        /* Waiting for semaphore BCD */
        sem_wait(&sem_BCD);

        /* Working */
        fprintf(stdout, "... Executing thread C ...\n");
        fflush(stdout);

        /* Posting the other semaphores */
        for (int i = 0; i < 2; i++) {
            sem_post(&sem_EF);
        }
    }
}
void *routine_D() {
    for (int i = 0; i < N_ITERATIONS; i++) {
        /* Waiting for semaphore BCD */
        sem_wait(&sem_BCD);

        /* Working */
        fprintf(stdout, "... Executing thread D ...\n");
        fflush(stdout);

        /* Posting the other semaphores */
        sem_post(&sem_H);
    }
}
void *routine_E() {
    for (int i = 0; i < N_ITERATIONS; i++) {
        /* Waiting for semaphore EF */
        sem_wait(&sem_EF);

        /* Working */
        fprintf(stdout, "... Executing thread E ...\n");
        fflush(stdout);

        /* Posting the other semaphores */
        sem_post(&sem_G);
    }
}
void *routine_F() {
    for (int i = 0; i < N_ITERATIONS; i++) {
        /* Waiting for semaphore EF */
        sem_wait(&sem_EF);

        /* Working */
        fprintf(stdout, "... Executing thread F ...\n");
        fflush(stdout);

        /* Posting the other semaphores */
        sem_post(&sem_G);
    }
}
void *routine_G() {
    for (int i = 0; i < N_ITERATIONS; i++) {
            /* Waiting for semaphore G */
            sem_wait(&sem_G);
            sem_wait(&sem_G);

            /* Working */
            fprintf(stdout, "... Executing thread G ...\n");
            fflush(stdout);

            /* Posting the other semaphores */
            sem_post(&sem_I);
    }
}
void *routine_H() {
    for (int i = 0; i < N_ITERATIONS; i++) {
        /* Waiting for semaphore H */
        sem_wait(&sem_H);

        /* Working */
        fprintf(stdout, "... Executing thread H ...\n");
        fflush(stdout);

        /* Posting the other semaphores */
        sem_post(&sem_I);
    }
}
void *routine_I() {
    for (int i = 0; i < N_ITERATIONS; i++) {
        /* Waiting for semaphore I */
        sem_wait(&sem_I);
        sem_wait(&sem_I);
        sem_wait(&sem_I);

        /* Working */
        fprintf(stdout, "... Executing thread I ...\n");
        fflush(stdout);

        /* Posting the other semaphores */
        sem_post(&sem_A);
    }
}