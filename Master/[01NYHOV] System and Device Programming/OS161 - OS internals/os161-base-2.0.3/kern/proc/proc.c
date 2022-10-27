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

/*
 * Process support.
 *
 * There is (intentionally) not much here; you will need to add stuff
 * and maybe change around what's already present.
 *
 * p_lock is intended to be held when manipulating the pointers in the
 * proc structure, not while doing any significant work with the
 * things they point to. Rearrange this (and/or change it to be a
 * regular lock) as needed.
 *
 * Unless you're implementing multithreaded user processes, the only
 * process that will have more than one thread is the kernel process.
 */

#include <types.h>
#include <spl.h>
#include <proc.h>
#include <current.h>
#include <addrspace.h>
#include <vnode.h>

/**
 *    LAB04: waitpid() system call
 * ==================================
 * Support function for waitpid()
 */
#if OPT_LAB04
#include <synch.h>

/**
 * PROCESS TABLE DEFINITION
 * The structure of the process table is defined here, in order to exploit the
 * possibility to have pid associated to struct proc.
 */
#define MAX_PROC 100
static struct _process_table {
	int active;							/* Initial value is 0 			*/
	struct proc *proc[MAX_PROC+1]; 		/* [0] not used. pids are >= 1 	*/
	int last_pid;           			/* index of last allocated pid 	*/
	struct spinlock lk;					/* Lock for this table 			*/
} processTable;

#endif

/*
 * The process for the kernel; this holds all the kernel-only threads.
 */
struct proc *kproc;


/**
 *    LAB04: waitpid() system call
 * ==================================
 * Initialize support function for waitpid()
*/
struct proc *proc_search_pid(pid_t pid) {
#if IMPLEMENTED_WAITPID

  	KASSERT(pid >= 0 && pid < MAX_PROC);
  	struct proc *proc = processTable.proc[pid];
  	KASSERT(proc->p_pid == pid);
  	return proc;
#else
  	(void) pid;
  	return NULL;
#endif
}

static void init_process_waitpid(struct proc *proc, const char *name) {
#if OPT_LAB04
#if IMPLEMENTED_WAITPID
	/* acquiring the lock */
  	spinlock_acquire(&processTable.lk);
  	int index = processTable.last_pid + 1;

  	proc->p_pid = 0;

	/* search a free index in table using a circular strategy */
	index = (index > MAX_PROC) ? 1 : index;
  	while (index != processTable.last_pid) {
		if (processTable.proc[index] == NULL) {
			processTable.proc[index] = proc;
			processTable.last_pid = index;
			proc->p_pid = index;
			break;
		}
     	index++;
		index = (index > MAX_PROC) ? 1 : index;
  	}

	/* releasing the spinnlock */
  	spinlock_release(&processTable.lk);
  	if (proc->p_pid == 0) {
    	panic("too many processes. proc table is full\n");
  	}

  	proc->p_status = 0;
#endif
#if IMPLEMENTED_WAIT_WITH_SEMAPHORES
	proc->p_sem = sem_create(name, 0);	/* NB: semaphore is initialized with 0 resources */
	KASSERT(proc->p_sem != NULL);
#else
	proc->p_cv = cv_create(name);
  	proc->p_locklock = lock_create(name);
	KASSERT(proc->p_cv != NULL);
	KASSERT(proc->p_locklock != NULL);
#endif
#else 

	/* does nothing if LAB04 options is not enabled */
	(void) proc;	/* suppress os161 warnings */
	(void) name;	/* suppress os161 warnings */
#endif
}

/**
 *    LAB04: waitpid() system call
 * ==================================
 * Destroy support function for waitpid()
*/
static void destroy_process_waitpid(struct proc *proc) {
#if OPT_LAB04
#if IMPLEMENTED_WAITPID
	/* acquiring the spinlock */
  	spinlock_acquire(&processTable.lk);

	/* acquiring the process pid */
  	int index = proc->p_pid;
  	KASSERT(index > 0 && index <= MAX_PROC);

	/* setting the correposing entry in the table as free */
  	processTable.proc[index] = NULL;

	/* releasing the spinlock */
  	spinlock_release(&processTable.lk);
#endif
#if IMPLEMENTED_WAIT_WITH_SEMAPHORES
	sem_destroy(proc->p_sem);
#else 
	cv_destroy(proc->p_cv);
  	lock_destroy(proc->p_locklock);
#endif
#else 
	/* does nothing if LAB04 options is not enabled */
	(void) proc;	/* suppress os161 warnings */
#endif
}

/*
 * Create a proc structure.
 */
static
struct proc *
proc_create(const char *name)
{
	struct proc *proc;

	proc = kmalloc(sizeof(*proc));
	if (proc == NULL) {
		return NULL;
	}
	proc->p_name = kstrdup(name);
	if (proc->p_name == NULL) {
		kfree(proc);
		return NULL;
	}

	proc->p_numthreads = 0;
	spinlock_init(&proc->p_lock);

	/* VM fields */
	proc->p_addrspace = NULL;

	/* VFS fields */
	proc->p_cwd = NULL;

	/* LAB04: initialization of stuff used to wait */
	init_process_waitpid(proc, name);

	/* LAB05: initialization of the fileTable */
#if OPT_LAB05
	bzero(proc->fileTable, OPEN_MAX*sizeof(struct openfile*));
#endif

	return proc;
}

/*
 * Destroy a proc structure.
 *
 * Note: nothing currently calls this. Your wait/exit code will
 * probably want to do so.
 */
void
proc_destroy(struct proc *proc)
{
	/*
	 * You probably want to destroy and null out much of the
	 * process (particularly the address space) at exit time if
	 * your wait/exit design calls for the process structure to
	 * hang around beyond process exit. Some wait/exit designs
	 * do, some don't.
	 */

	KASSERT(proc != NULL);
	KASSERT(proc != kproc);

	/*
	 * We don't take p_lock in here because we must have the only
	 * reference to this structure. (Otherwise it would be
	 * incorrect to destroy it.)
	 */

	/* VFS fields */
	if (proc->p_cwd) {
		VOP_DECREF(proc->p_cwd);
		proc->p_cwd = NULL;
	}

	/* VM fields */
	if (proc->p_addrspace) {
		/*
		 * If p is the current process, remove it safely from
		 * p_addrspace before destroying it. This makes sure
		 * we don't try to activate the address space while
		 * it's being destroyed.
		 *
		 * Also explicitly deactivate, because setting the
		 * address space to NULL won't necessarily do that.
		 *
		 * (When the address space is NULL, it means the
		 * process is kernel-only; in that case it is normally
		 * ok if the MMU and MMU- related data structures
		 * still refer to the address space of the last
		 * process that had one. Then you save work if that
		 * process is the next one to run, which isn't
		 * uncommon. However, here we're going to destroy the
		 * address space, so we need to make sure that nothing
		 * in the VM system still refers to it.)
		 *
		 * The call to as_deactivate() must come after we
		 * clear the address space, or a timer interrupt might
		 * reactivate the old address space again behind our
		 * back.
		 *
		 * If p is not the current process, still remove it
		 * from p_addrspace before destroying it as a
		 * precaution. Note that if p is not the current
		 * process, in order to be here p must either have
		 * never run (e.g. cleaning up after fork failed) or
		 * have finished running and exited. It is quite
		 * incorrect to destroy the proc structure of some
		 * random other process while it's still running...
		 */
		struct addrspace *as;

		if (proc == curproc) {
			as = proc_setas(NULL);
			as_deactivate();
		}
		else {
			as = proc->p_addrspace;
			proc->p_addrspace = NULL;
		}
		as_destroy(as);
	}

	KASSERT(proc->p_numthreads == 0);
	spinlock_cleanup(&proc->p_lock);

	/* LAB04: end of stuff used to wait */
	destroy_process_waitpid(proc);

	kfree(proc->p_name);
	kfree(proc);
}

/*
 * Create the process structure for the kernel.
 */
void
proc_bootstrap(void)
{
	kproc = proc_create("[kernel]");
	if (kproc == NULL) {
		panic("proc_create for kproc failed\n");
	}
#if OPT_LAB04
	spinlock_init(&processTable.lk);
	/* kernel process is not registered in the table */
	processTable.active = 1;
#endif
}

/*
 * Create a fresh proc for use by runprogram.
 *
 * It will have no address space and will inherit the current
 * process's (that is, the kernel menu's) current directory.
 */
struct proc *
proc_create_runprogram(const char *name)
{
	struct proc *newproc;

	newproc = proc_create(name);
	if (newproc == NULL) {
		return NULL;
	}

	/* VM fields */

	newproc->p_addrspace = NULL;

	/* VFS fields */

	/*
	 * Lock the current process to copy its current directory.
	 * (We don't need to lock the new process, though, as we have
	 * the only reference to it.)
	 */
	spinlock_acquire(&curproc->p_lock);
	if (curproc->p_cwd != NULL) {
		VOP_INCREF(curproc->p_cwd);
		newproc->p_cwd = curproc->p_cwd;
	}
	spinlock_release(&curproc->p_lock);

	return newproc;
}

/*
 * Add a thread to a process. Either the thread or the process might
 * or might not be current.
 *
 * Turn off interrupts on the local cpu while changing t_proc, in
 * case it's current, to protect against the as_activate call in
 * the timer interrupt context switch, and any other implicit uses
 * of "curproc".
 */
int
proc_addthread(struct proc *proc, struct thread *t)
{
	int spl;

	KASSERT(t->t_proc == NULL);

	spinlock_acquire(&proc->p_lock);
	proc->p_numthreads++;
	spinlock_release(&proc->p_lock);

	spl = splhigh();
	t->t_proc = proc;
	splx(spl);

	return 0;
}

/*
 * Remove a thread from its process. Either the thread or the process
 * might or might not be current.
 *
 * Turn off interrupts on the local cpu while changing t_proc, in
 * case it's current, to protect against the as_activate call in
 * the timer interrupt context switch, and any other implicit uses
 * of "curproc".
 */
void
proc_remthread(struct thread *t)
{
	struct proc *proc;
	int spl;

	proc = t->t_proc;
	KASSERT(proc != NULL);

	spinlock_acquire(&proc->p_lock);
	KASSERT(proc->p_numthreads > 0);
	proc->p_numthreads--;
	spinlock_release(&proc->p_lock);

	spl = splhigh();
	t->t_proc = NULL;
	splx(spl);
}

/*
 * Fetch the address space of (the current) process.
 *
 * Caution: address spaces aren't refcounted. If you implement
 * multithreaded processes, make sure to set up a refcount scheme or
 * some other method to make this safe. Otherwise the returned address
 * space might disappear under you.
 */
struct addrspace *
proc_getas(void)
{
	struct addrspace *as;
	struct proc *proc = curproc;

	if (proc == NULL) {
		return NULL;
	}

	spinlock_acquire(&proc->p_lock);
	as = proc->p_addrspace;
	spinlock_release(&proc->p_lock);
	return as;
}

/*
 * Change the address space of (the current) process. Return the old
 * one for later restoration or disposal.
 */
struct addrspace *
proc_setas(struct addrspace *newas)
{
	struct addrspace *oldas;
	struct proc *proc = curproc;

	KASSERT(proc != NULL);

	spinlock_acquire(&proc->p_lock);
	oldas = proc->p_addrspace;
	proc->p_addrspace = newas;
	spinlock_release(&proc->p_lock);
	return oldas;
}


/**
 *    LAB04: waitpid() system call
 * ==================================
 * Support function for waitpid()
*/


/**
 * @brief Support the waitpid() function
 * 
 * @param proc process data structure
 * @return the exit status of the terminated process
 */
int proc_wait(struct proc *proc) {
#if OPT_LAB04

	int return_status;

	/* NULL and kernel processes are not allowed here */
	KASSERT(proc != NULL);
	KASSERT(proc != kproc);

#if IMPLEMENTED_WAIT_WITH_SEMAPHORES
	P(proc->p_sem);			/* Remember: semaphore has 0 resources at the beginning */
#else
	lock_acquire(proc->p_locklock);
    cv_wait(proc->p_cv, proc->p_locklock);
    lock_release(proc->p_locklock);
#endif

	return_status = proc->p_status;
	//kprintf("[!] return_status: %d\n", return_status);
	proc_destroy(proc);
	return return_status;
#else
    (void) *proc; /* suppress os161 warnings */
    return 0;  /* return success by default */
#endif
}

/**
 * @brief Retrieves the PID of a given process
 * 
 * @param proc the process associated to the PID
 * @return pid_t 
 */
pid_t proc_getpid(struct proc *proc) {
#if IMPLEMENTED_WAITPID
	KASSERT(proc != NULL);
	return proc->p_pid;
#else
	(void) proc;
	return -1;
#endif
}

/**
 * @brief Wait for a specific process identified by pid
 * 
 * @param pid the identifier of the process
 * @param exit_status the exit status of the process
 * @return pid_t 
 */
pid_t proc_waitpid(pid_t pid, int *exit_status) {
#if IMPLEMENTED_WAITPID

	/* retrieve the process given the pid */
    struct proc *proc = proc_search_pid(pid);
	if (proc == NULL) {
		return -1;
	}

	/* waiting for the process */
	pid_t proc_pid = proc_getpid(proc);
    *exit_status = proc_wait(proc);

	/* returning the pid of the process */
	return proc_pid;
#else
	(void) pid;
	*exit_status = 1;
	return -1;
#endif
}