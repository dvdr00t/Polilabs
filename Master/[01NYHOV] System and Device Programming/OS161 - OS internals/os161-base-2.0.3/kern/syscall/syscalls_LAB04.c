/**
 * @file syscalls_LAB04.c
 * @author Davide Arcolini (davide.arcolini@studenti.polito.it)
 * @brief Contains the definitions of the system calls used in LAB04
 * @version 0.1
 * @date 2022-06-11
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


#include "syscalls_LAB04.h"

/**
 * @brief Starts the new generated thread
 * 
 * @param tfv trapframe
 * @param dummy not used
 */
#if OPT_LAB04
void call_enter_forked_process(void *tfv, unsigned long dummy) {
#if IMPLEMENTED_FORK
    struct trapframe *tf = (struct trapframe *)tfv;
    (void)dummy;
    enter_forked_process(tf); 
 
    panic("enter_forked_process returned (should not happen)\n");
#endif
}
#endif


/**
 * @brief Retrieve the pid of the current process
 * 
 * @return pid_t 
 */
#if OPT_LAB04
pid_t sys_getpid_LAB04(void) {
#if IMPLEMENTED_WAITPID
    KASSERT(curproc != NULL);
    return curproc->p_pid;
#else
    return -1;
#endif
}
#endif

/**
 * @brief waitpid() system call
 * 
 * @param pid process identifier of the process 
 * @param statusp status 
 * @param options options (not handled here)
 * @return int 
 */
#if OPT_LAB04
int sys_waitpid_LAB04(pid_t pid, userptr_t statusp, int options) {
#if IMPLEMENTED_WAITPID

    /* retrieve the process given the pid */
    struct proc *proc = proc_search_pid(pid);
	if (proc == NULL) {
		return -1;
	}
    (void)options; /* not handled */
	/* waiting for the process */
    int s = proc_wait(proc);

    /* update status */
	if (statusp!=NULL) 
        *(int*)statusp = s;

    return pid;
#else
    (void)options; /* not handled */
    (void)pid;
    (void)statusp;
    return -1;
#endif
}
#endif

/**
 * @brief fork() system call. It will create a new identical process, given a process
 * 
 * @param ctf trapframe of the given process
 * @param retval pid of the new process
 * @return 0 on success
 */
#if OPT_LAB04
int sys_fork_LAB04(struct trapframe *ctf, pid_t *retval) {
    struct trapframe *tf_child;
    struct proc *newp;
    int result;

    /* assert current process to actually exists */
    KASSERT(curproc != NULL);

    /* create a new process */
    newp = proc_create_runprogram(curproc->p_name);
    if (newp == NULL) {
        return ENOMEM;
    }

    /* done here as we need to duplicate the address space of thbe current process */
    as_copy(curproc->p_addrspace, &(newp->p_addrspace));
    if(newp->p_addrspace == NULL){
        proc_destroy(newp); 
        return ENOMEM; 
    }

    /* we need a copy of the parent's trapframe */
    tf_child = kmalloc(sizeof(struct trapframe));
    if(tf_child == NULL){
        proc_destroy(newp);
        return ENOMEM; 
    }

    /* copying actual memory */
    memcpy(tf_child, ctf, sizeof(struct trapframe));

    /* TO BE DONE: linking parent/child, so that child terminated on parent exit */

    /* generating new thread */
    result = thread_fork(curthread->t_name, newp, call_enter_forked_process, (void *)tf_child, (unsigned long)0 /*unused*/);
    if (result){
        proc_destroy(newp);
        kfree(tf_child);
        return ENOMEM;
  }

    *retval = newp->p_pid;

    return 0;
}
#endif
