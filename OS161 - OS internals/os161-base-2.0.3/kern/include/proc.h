/*
 * Copyright (c) 2013
 *	The President and Fellows of Harvard College.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE UNIVERSITY AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE UNIVERSITY OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef _PROC_H_
#define _PROC_H_

/*
 * Definition of a process.
 *
 * Note: curproc is defined by <current.h>.
 */

#include <spinlock.h>

#include "opt-lab04.h" //optional include for LAB04
#include "opt-lab05.h" //optional include for LAB05

#if OPT_LAB05
#include <limits.h>				// to use OPEN_MAX
#include "syscalls_LAB05.h"		// to use openfile
#endif

struct addrspace;
struct thread;
struct vnode;

/**
 *    LAB04: waitpid() system call
 * ==================================
 */
#if OPT_LAB04
/**
 * IMPLEMENTED_WAIT_WITH_SEMAPHORES == 1 --> Enable implementation with semaphores
 * IMPLEMENTED_WAIT_WITH_SEMAPHORES == 0 --> Enable implementation with wait channels
 */
#define IMPLEMENTED_WAIT_WITH_SEMAPHORES 1

/**
 * IMPLEMENTED_WAITPID == 1 --> Enable implementation of getpid() and sys_waitpid()
 * IMPLEMENTED_WAITPID == 0 --> Enable wait() instead of getpid() and sys_waitpid()
 */
#define IMPLEMENTED_WAITPID 1
#endif


/*
 * Process structure.
 *
 * Note that we only count the number of threads in each process.
 * (And, unless you implement multithreaded user processes, this
 * number will not exceed 1 except in kproc.) If you want to know
 * exactly which threads are in the process, e.g. for debugging, add
 * an array and a sleeplock to protect it. (You can't use a spinlock
 * to protect an array because arrays need to be able to call
 * kmalloc.)
 *
 * You will most likely be adding stuff to this structure, so you may
 * find you need a sleeplock in here for other reasons as well.
 * However, note that p_addrspace must be protected by a spinlock:
 * thread_switch needs to be able to fetch the current address space
 * without sleeping.
 */
struct proc {
	char *p_name;			/* Name of this process */
	struct spinlock p_lock;		/* Lock for this structure */
	unsigned p_numthreads;		/* Number of threads in this process */

	/* VM */
	struct addrspace *p_addrspace;	/* virtual address space */

	/* VFS */
	struct vnode *p_cwd;		/* current working directory */

#if OPT_LAB04
	int p_status; 				/* current process status */
#if IMPLEMENTED_WAITPID
	pid_t p_pid;				/* current process pid */
#endif
#if IMPLEMENTED_WAIT_WITH_SEMAPHORES
	struct semaphore *p_sem;	/* semaphore implementation of waitpid() */
#else
	struct cv *p_cv;			/* wait channels implementation of waitpid() */
    struct lock *p_locklock;
#endif
#endif
#if OPT_LAB05
	struct openfile *fileTable[OPEN_MAX];
#endif
};

/* This is the process structure for the kernel and for kernel-only threads. */
extern struct proc *kproc;

/* Call once during system startup to allocate data structures. */
void proc_bootstrap(void);

/* Create a fresh process for use by runprogram(). */
struct proc *proc_create_runprogram(const char *name);

/* Destroy a process. */
void proc_destroy(struct proc *proc);

/* Attach a thread to a process. Must not already have a process. */
int proc_addthread(struct proc *proc, struct thread *t);

/* Detach a thread from its process. */
void proc_remthread(struct thread *t);

/* Fetch the address space of the current process. */
struct addrspace *proc_getas(void);

/* Change the address space of the current process, and return the old one. */
struct addrspace *proc_setas(struct addrspace *);


/**
 *    LAB04: waitpid() system call
 * ==================================
 * Support function for waitpid()
*/

/**
 * @brief Support the waitpid() function
 * 
 * @param p 
 * @return int the exit status of
 */
int proc_wait(struct proc *proc);

/**
 * @brief Retrieves the PID of a given process
 * 
 * @param proc the process associated to the PID
 * @return pid_t 
 */
pid_t proc_getpid(struct proc *proc);

/**
 * @brief Wait for a specific process identified by pid
 * 
 * @param pid the identifier of the process
 * @param exit_status the exit status of the process
 * @return pid_t 
 */
pid_t proc_waitpid(pid_t pid, int *exit_status);

/**
 * @brief retrieve the process associated to pid in the process table
 * 
 * @param pid pid of the process
 * @return struct proc* 
 */
struct proc *proc_search_pid(pid_t pid);




#endif /* _PROC_H_ */
