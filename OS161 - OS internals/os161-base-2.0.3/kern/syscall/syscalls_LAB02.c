/**
 * @file syscalls_LAB02.c
 * @author Davide Arcolini (davide.arcolini@studenti.polito.it)
 * @brief Contains the definitions of the system calls used in LAB02 (and following)
 * @version 0.1
 * @date 2022-05-02
 * 
 * @copyright Copyright (c) 2022
 * 
*/

#include <types.h>
#include <kern/unistd.h>
#include <kern/errno.h>
#include <clock.h>
#include <copyinout.h>
#include <syscall.h>
#include <lib.h>
#include <proc.h>
#include <thread.h>
#include <addrspace.h>
#include <mips/trapframe.h>
#include <current.h>
#include <synch.h>

#include "syscalls_LAB02.h"
#include "syscalls_LAB04.h"
#include "syscalls_LAB05.h"

/**
 * @brief sys_write_LAB02() function attempts to write n_byte bytes from the buffer pointed to 
 *        by buf to the file associated with the open file descriptor, fd. If n_byte is 0, 
 *        sys_write_LAB02() will return 0 and have no other results if the file is a regular file; 
 *        otherwise, the results are unspecified. 
 * 
 * @param fd File descriptor: where to write
 * @param buf pointer to the buffer from which the sys_write_LAB02 will get the content to write
 * @param n_bytes number of bytes to be written
 * @return ssize_t number of bytes written
 */
#if OPT_LAB02
ssize_t sys_write_LAB02(int fd, const void *buf, ssize_t n_bytes) {

    /* 
        CHECK IMPLEMENTATION CONSTRAINT 
        -------------------------------
        NB: sys_write_LAB02 is defined to work only with stdout (and, eventually, stderr).
            If fd is not stdout or stderr, it will raise an error and exit.
    */
    if (fd != STDOUT_FILENO && fd != STDERR_FILENO) {
#if OPT_LAB05
        return file_write_LAB05(fd, (userptr_t) buf, n_bytes);
#else
        kprintf("[!] Ayo, I am quite dumb... I can work only with STDOUT and STDERR...\n");
        return -1;
#endif
    }

    /* PRINTING BYTES TO THE FILE DESCRIPTOR FROM THE BUFFER*/
    char *pointer = (char *) buf;
    for (int i = 0; i < n_bytes; i++) {
        putch(pointer[i]);  
    }

    /* RETURNING NUMBER OF BYTES WRITTEN */
    return n_bytes;
    
}
#endif


/**
 * @brief sys_read_LAB02() attempts to read up to n_bytes from file descriptor fd
          into the buffer starting at buf.
 * 
 * @param fd File descriptor: read from fd
 * @param buf pointer to the buffer to which the sys_read_LAB02 will store the content read
 * @param n_bytes number of bytes to be read
 * @return ssize_t number of bytes read
*/
#if OPT_LAB02
ssize_t sys_read_LAB02(int fd, void *buf, ssize_t n_bytes) {

    /* 
        CHECK IMPLEMENTATION CONSTRAINT 
        -------------------------------
        NB: sys_write_LAB02 is defined to work only with stdout (and, eventually, stderr).
            If fd is not stdout or stderr, it will raise an error and exit.
    */
    if (fd != STDIN_FILENO) {
 #if OPT_LAB05
        return file_read_LAB05(fd, (userptr_t) buf, n_bytes);
#else
        kprintf("[!] Ayo, I am quite dumb... I can work only with STDOUT and STDERR...\n");
        return -1;
#endif
    }

    /* READING BYTES FROM THE FILE DESCRIPTOR TO THE BUFFER*/
    char *pointer = (char *) buf;
    for (int i = 0; i < n_bytes; i++) {
        if ((pointer[i] = getch()) == -1) {
            return i;
        }
    }

    /* RETURNING THE NUMBER OF BYTES READ */
    return n_bytes;
}
#endif

/**
 * @brief Basic (simpler) implementation of _exit(). It should saves the state of the process
 *        ending (in order to be read from other processes), release memory and destroy the
 *        main thread. However, this simpler version only destroy the main thread and then exit.
 *      
 *        LAB04: we have now implemented wait() function in order to wait for the thread to terminate
 * 
 * @param status integer value indicating the result of the operation. Tipically EXIT_SUCCESS or
 *               EXIT_FAILURE 
 */
#if OPT_LAB02
void sys_exit_LAB02(int status) {
#if OPT_LAB04

    /* acquiring the current process information from current.h */
    //kprintf("[!] status is: %d\n", status);
    struct proc *proc = curproc;
    proc->p_status = status & 0xff;    /* return status is represented by the lower 8 bits */
    //proc_remthread(curthread);        /* remove thread from current process */
                                        /* NB: this operation is already done in thread_exit() */
    
#if IMPLEMENTED_WAIT_WITH_SEMAPHORES
    V(proc->p_sem);  /* adding a resource to the semaphore */
#else
    lock_acquire(proc->p_locklock);
    cv_signal(proc->p_cv, proc->p_locklock);
    lock_release(proc->p_locklock);
#endif
#else

    /* GET THE ADDRESS SPACE OF THE CURRENT PROCESS */
    struct addrspace *as = proc_getas();
    if (as == NULL) {
        kprintf("[!] proc_getas() returned NULL value.\n");
        return;
    }

    /* DESTORY THE ADDRESS SPACE */
    as_destroy(as);
    
    /* HERE YOU SHOULD SAVE SOME STATE BEFORE EXITING, BUT FOR NOW, WHO CARES... */
#endif
    /* MAIN THREAD TERMINATES HERE. BYE BYE */
    thread_exit();

    /* WAIT! YOU SHOULD NOT HAPPEN TO BE HERE */
    panic("[!] Wait! You should not be here. Some errors happened during thread_exit()...\n");

    (void) status;
}
#endif

