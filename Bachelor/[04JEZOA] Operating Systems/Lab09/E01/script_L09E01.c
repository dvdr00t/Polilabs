#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>


/*
* Supppose to have this atomic function given and use it to write the
* sem_init(), pthread_mutex_lock() and phtread_mutex_unlock()
*/
int atomicIncrement (int *var) {
  int tmp = *var;
  *var = tmp + 1;
  return (*var);
}

/*
* Struct defining the reservation number of the thread that is trying to access
* to the critical section
*/
typedef struct my_lock_s {
    int reservation_number; //ticket number, assigned to the process when trying to enter to theh CS
    int turn_number;    //attual number that is accessing to the CS
} my_lock_t;

/* 
* Rewriting function sem_init by means of the given int atomicIncrement(int *var)
* 
* Param: the semaphore, in this case a mutex semaphore called lock
* Return: success/failure value
*/
int my_sem_init (my_lock_t lock) {
    lock.reservation_number = 0;
    lock.turn_number = 0;
    return 1;
}

/* 
* Rewriting function pthread_mutex_lock by means of the given 
* int atomicIncrement(int *var)
* 
* Param: the semaphore, in this case a mutex semaphore called lock
* Return: void
*/
void my_lock (my_lock_t *lock) {
    int my_turn = atomicIncrement(&lock->reservation_number);
    while (lock->turn_number != my_turn) {
        //thread waits here until the turn number is increased to my_turn value
    }
}

/* 
* Rewriting function pthread_mutex_unlock by means of the given 
* int atomicIncrement(int *var)
* 
* Param: the semaphore, in this case a mutex semaphore called lock
* Return: void
*/
void my_unlock (my_lock_t *lock) {
    atomicIncrement(&(lock->turn_number));
}

/* Some routine functions for the threads */
void *routine_A();
void *routine_B();
my_lock_t my_mutex;
int main(int args, char *argv[]) {

    /* Testing the function created */
    pthread_t thread_A;
    pthread_t thread_B;
    
    /* Initializing the mutex */
    if (my_sem_init(my_mutex) != 1) {
        fprintf(stderr, ">>> [ERROR] Some errors occured while trying to initialize the mutex\n");
        exit(EXIT_FAILURE);
    }

    pthread_create(&thread_A, NULL, routine_A, NULL);
    pthread_create(&thread_B, NULL, routine_B, NULL);
   
    pthread_join(thread_A, NULL);
    pthread_join(thread_B, NULL);

    exit(EXIT_SUCCESS);
}
void *routine_A(/*void  *my_mutex_void*/) {
    fprintf(stdout, "A\n");
    my_unlock(&my_mutex);   //unlocking the semaphore
    pthread_exit(NULL);
}
void *routine_B() {
    my_lock(&my_mutex); //waiting the semaphore
    fprintf(stdout, "B\n");
    pthread_exit(NULL);
}