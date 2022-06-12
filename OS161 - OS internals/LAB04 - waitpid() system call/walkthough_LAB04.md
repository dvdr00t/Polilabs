# OS161 - System and Device Programming

## LAB03 - Implementing `waitpid()` system call
* **Author**: `Davide Arcolini`
* **Date**: `2022-06-09`

## Table of contents
1. [Introduction](#introduction)
    - [`struct proc`](#process-data-structure)
2. [Waiting for process termination](#waiting-for-process-termination)
    - [`proc_wait()`](#implementation-of-the-wait)
    - [`signal()`](#implementation-of-the-signal)
3. [Destruction of process data structure](#destruction-of-process-data-structure)
4. [Process table and `waitpid()`](#process-table-and-waitpid)
    - [Process table implementation](#process-table-implementation)
    - [`waitpid` implementation](#waitpid-implementation)
5. [Implementing `getpid()` and `fork()`](#implementing-getpid-and-fork)
    - [`fork` system call](#fork-system-call)
    - [`getpid` system call](#getpid-system-call)

> **NB**: all over the file, `os161-base_2.0.3` is referred as `src`. This is to simplified paths as `os161/os161-base_2.0.3/...` to `os161/src`. Keep it in mind!

## Introduction
In this laboratory, we are required to implement the `waitpid()` system call in order to wait for the **termination of a process**. More generally, in a Unix/Linux kernel distribution there are functions, like `wait()`, that awaits any child process to **change state** and obtain information about the child whose state has changed.

A **state change** is considered to be: 
- the child terminated (the **only** case taken into account in this laboratory); 
- the child was stopped by a signal; 
- the child was resumed by a signal. 

In the case of a terminated child, performing a wait allows the system to release the resources associated with the child; if a wait is not performed, then the terminated child remains in a  **zombie state**.

The laboratory can be divided into four parts: 
- waiting for the termination of a user process, returning its exit status;
- destruction of the data structure of a process (`struct proc`);
- `pid` assignment to process, process table management and `waitpid()`;
- *(optional)* create system calls `getpid` and `fork`.

Before starting, let's review one more time the data structures we need to exploits in this laboratory.

### Process data structure
The process data structure contains all the useful information regarding a particular process, whether it is a kernel process or it is a user process. In *os161*, the process data structure is initially defined in `proc.h` (*path*: `os161/src/kern/include/proc.h`) as:
```C
struct proc {
	char *p_name;			        /* Name of this process */
	struct spinlock p_lock;		    /* Lock for this structure */
	unsigned p_numthreads;		    /* Number of threads in this process */

	/* VM */
	struct addrspace *p_addrspace;	/* virtual address space */

	/* VFS */
	struct vnode *p_cwd;		    /* current working directory */

	/* add more material here as needed */
};
```
where: 
- `p_name` is a string representing the name of the process, useful for debugging.
- `p_lock` is a **spinlock** (see: my [GitHub page](https://github.com/DavideArcolini/PoliTo_SDP.git), `OS161 - OS internals/LAB03` for more information) which is useful to **protect** the `p_addrspace`. This is due to `thread_switch`, which needs to be able to fetch the current address space without sleeping.
- `p_numthreads` stores the number of threads for this process. Note that we only count the number of threads in each process and, unless we implement multithreaded user processes, this number will not exceed 1 except in `kproc`. If you want to know exactly which threads are in the process, e.g. for debugging, add an array and a sleeplock to protect it (you can't use a spinlock to protect an array because arrays need to be able to call `kmalloc`).
- `p_addrspace` is a pointer to the a structure containing all the information about the virtual address space in which the process is stored.
- `p_cwd` is a pointer to the `vnode` storing the information about the current working directory.

## Waiting for process termination
Let's do a quick recap and see what happens when the user calls a process with the command `p <program_name>`. 
1. from the menu, the input is read ad passed to `menu_execute()`;
2. in `menu_execute()`, the input is parsed and passed to `cmd_dispatch()`;
3. in `cmd_dispatch()`, the command is looked up versus the command table and the arguments are passed to `cmd_prog()`;
4. in `cmd_prog()`, the startup of the program is passed to `common_prog()`, which is our **starting point** for this LAB.

The implementation of `common_prog()` is the following: 
```C
static int common_prog(int nargs, char **args) {
	struct proc *proc;
	int result;

	/* Create a process for the new program to run in. */
	proc = proc_create_runprogram(args[0] /* name */);
	if (proc == NULL) {
		return ENOMEM;
	}

	result = thread_fork(args[0] /* thread name */,
			proc /* new process */,
			cmd_progthread /* thread function */,
			args /* thread arg */, nargs /* thread arg */);
	if (result) {
		kprintf("thread_fork failed: %s\n", strerror(result));
		proc_destroy(proc);
		return result;
	}

	/*
	 * The new process will be destroyed when the program exits...
	 * once you write the code for handling that.
	 */

	return 0;
}
```

As it can be seen, the `common_prog()` function will handle the initialization of the process structure, the fork of the process thread into another kernel thread (which, as we already know from **LAB02**, it will become a user process) and then... it returns!

Here, there is no implementation of any waiting for the subprogram to finish, but the function returns immediately to the menu. This is usually not what we want, so we need to implement some form of `wait()`.

In the meanwhile, the new created kernel thread will perform some routines useful to run the program, such as loading the executable program in memory and switching to user mode. Now, the user program is running and at some point it will end by calling `exit()`. This exit will trigger the `sys_exit()` system call we have implemented in **LAB02** and kill the current thread, as we can see in the following schema: 
![wait](./images/wait.png) 

At this point, two things are needed in order to perform all the operations described above:
1. an implementation of some `wait()` in `common_prog()`, to wait the termination of the user process;
2. an implementation of some `signal()` in `sys_exit()`, to signal the termination of the user process to the thread which was waiting in `common_prog()`;


### Implementation of the wait
As suggested in the laboratory text, it is recommended to first create an `int proc_wait(struct proc *p)` function. The function waits (without the need to manage the `pid` and to support the system call `waitpid`) for the end (with a call to the `syscall_exit()`) of the process (whose related `struct proc` is) pointed to by `p`. Waiting can be implemented by means of a **semaphore** or **condition variable** (added as a field to the `struct proc`).

It is therefore a **kernel function**, usable only within it (because it uses the pointer to `struct proc`). This function could be implemented in `kern/proc/proc.c` and called by `common_prog`, after this has successfully called `thread_fork`, in order to wait for the end of the activated process (and not immediately return to the menu that would require another command). The `common_prog` function could then wait for the child process to finish by

```C
exit_code = proc_wait(proc);
```

and print the return code (the one received from `_exit` - `sys__exit` and saved in the `struct proc`) on the console before returning to the calling program.

1. **Creating the `wait_proc()` function**
First things first, we need to define the `int proc_wait(struct proc *p)` function, which will wait for the process termination and will be useful later on as support for the real `waitpid()` function.

    Let's define the prototype of the function in `proc.h` as:
    ```C
    /**
     * @brief Support the waitpid() function
     * 
     * @param p process data structure
     * @return the exit status of the terminated process
     */
    int proc_wait(struct proc *proc);
    ```
    and implement it in `proc.c` as:
    ```C
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

        /* we need to implement some stuff here */

        /* destroy the terminated process but save its exit status */
        return_status = proc->p_status;
	    return return_status;
    #else
        (void) *proc; /* suppress os161 warnings */
        return 0;  /* return success by default */
    #endif
    }
    
    ```
    The implementation of the function, as written in the comment, will define the return status value and assert the process not to be either not-existing (`NULL`) or a kernel process (`kproc`). It should be only a **user process**.

    Next thing we need is either a **semaphore** of a **wait channel** to stop by while waiting for the process termination;

    We should add some features to the process data structure.


2. **Adding feature to the process data structure**
We need to add some more fields to the process data structure `struct proc`, in order to support either the implementation with semaphores or the implementation with wait channels. 

    In `proc.h`, let's add new fields to the process structure:
    ```C
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

    #if IMPLEMENTED_WAIT_WITH_SEMAPHORES
        struct semaphore *p_sem;	/* semaphore implementation of waitpid() */
    #else
        struct cv *p_cv;			/* wait channels implementation of waitpid() */
        struct lock *p_locklock;
    #endif
    #endif
    };
    ```

    These new variables need to be initialized before they can be used by `proc_wait()`. 
    
3. **Initialization of the new features**
Let's define a new initialization function in `proc.c` that manages the set-up for the process data structure.
    ```C
    /**
    *    LAB04: waitpid() system call
    * ==================================
    * Initialize support function for waitpid()
    */
    static void init_process_waitpid(struct proc *proc, const char *name) {
    #if OPT_LAB04

        /* we will add more stuff here later on in the LAB */

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
    ```
    which will be called in `proc_create` as:
    ```C
    static struct proc *proc_create(const char *name) {

        /* ... */

        /* LAB04: initialization of stuff used to wait */
	    init_process_waitpid(proc, name);

        return proc;
    }
    ```
    Since we are already here, let's also define the function which will destroy the variables used. This function will be exploited later on in this laboratory (see [Destruction of process data structure](#destruction-of-process-data-structure)):
    ```C
    /**
    *    LAB04: waitpid() system call
    * ==================================
    * Destroy support function for waitpid()
    */
    static void destroy_process_waitpid(struc proc *proc) {
    #if OPTL_LAB04

        /* we will add more stuff here later on in the LAB */

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
    ```
    which will be called in `proc_destroy` as: 
    ```C
    void proc_destroy(struct proc *proc) {

        /* ... */

        /* LAB04: end of stuff used to wait */
	    destroy_process_waitpid(proc, name);

        return proc;
    }
    ```
4. **Completing `proc_wait()`**
Now we can proceed in using our new defined variables in the `proc_wait()` function we were implementing:
    ```C
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
        return return_status;
    #else
        (void) *proc; /* suppress os161 warnings */
        return 0;  /* return success by default */
    #endif
    }
    ```
    Our `proc_wait()` is now **ready to be used!** Let's add it in the `common_prog()` function call:
    ```C
    static int common_prog(int nargs, char **args) {

        /* ... */

        /**
        *    LAB04: waitpid() system call
        * ==================================
        */
        int exit_code = proc_wait(proc);
        kprintf("[!] exit_code is: %d\n", exit_code);   

        return 0;
    }
    ```

### Implementation of the signal
At this point, calling a user program like `p testbin/palin` will result in an infinite wait of the main thread in `proc_wait()`, since we have not defined yet a way to signal the thread waiting on the termination.

As already stated at the beginning, when a process ends, it will trigger the **exit system call**. We need to implement there the signaling to the waiting thread.

Let's go back to the `sys_exit_LAB02()` function we defined in **LAB02** and add the management of the exit status and the signaling to the waiting thread (**NB:** remember to add the right libraries at the beginning of the files, like `current.h` or `sync.h`, etc...):

```C
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
    struct proc *proc = curproc;
    proc->p_status = status & 0xff;    /* return status is represented by the lower 8 bits */
    // proc_remthread(curthread);                  /* remove thread from current process */
    
#if IMPLEMENTED_WAIT_WITH_SEMAPHORES
    V(proc->p_sem);  /* adding a resource to the semaphore */
#else
    lock_acquire(proc->p_locklock);
    cv_signal(p->p_cv, proc->p_locklock);
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
```
Now, the waiting process (i.e. the main thread) is able to move on once the terminating process ends, thanks to the signaling done by the terminated process.  

## Destruction of process data structure
Now it is time to move to the second part of this laboratory.

Once the process terminates and its exit status reaches the main thread which was waiting for him, it is time to **destroy the process**. Please, think about the fact that it is not possible to destroy the process data structure of a terminated process before another process wait for its termination. This is due to the fact that the terminated process contains some information which are useful to the waiting process. That's why a un-waited process becomes a **zombie**, looking for someone to wait for him and read its data before destruction.

Among the various possible solutions to free a `struct proc` through `proc_destroy`, it is suggested to call the latter inside `proc_wait`, after waiting on a semaphore or condition variable (i.e. the struct is not destroyed when the process ends, but later on, inside the `proc_wait` function, called by another process, possibly the kernel).

This is why the previous implementation of the `sys__exit` (the one done in **LAB02**) needs to be modified as we did. Now, it would no longer need to release the address space, but only to signal (to the semaphore or condition variable) the end of the process, before calling `thread_exit`. In other words, `sys__exit` terminates the thread, does not destroy the data structure of the process, but simply signals its termination.
The complete destruction of a process is performed by `proc_wait` after it has received the end signal. The `proc_wait` function also handles the return of the process exit status.

It's time to collect the work we did before. Let's move into the `proc_wait()` function and let's add a call to the function `proc_destroy()`:

```C
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
```

Now, `proc_wait()` is in charge of destroying the process data structure. Schematizing the flow of execution, more or less this is what happens:
1. user calls a program to run.
2. **main process** create a user process (from kernel thread);
3. **main process** waits...
4. **user process** starts and does its job;
5. **user process** ends triggering `sys_exit`.

Now: 
- **user process** HAS TO: 
    - signaling the waiting process;
    - terminates the thread execution.
- **main process** HAS TO: 
    - destroy the process data structure.

**Here it happens the tricky thing!**

The `proc_destroy()` function requires that the process being destroyed (data structure) no longer has active threads (see the process destroy code, which contains the assertion `KASSERT(proc->p_numthreads == 0)`). Since `sys__exit()` signals the end of the process before calling `thread_exit()`, it is possible that the kernel on hold (in `common_prog()`) is woken up and calls `proc_destroy()` before `thread_exit()` "detaches" the thread from the process. (see the `thread_exit()` code, especially the call to `proc_remthread()`).


The solution to avoid this race is not univocal. We suggest, as a possible option, to call `proc_remthread()` explicitly in the `sys__exit()`, "before" signalling the end of the process, and modify the `thread_exit()` function so that it accepts a thread already detached from the process (be careful, not to REQUIRE `thread_exit()` to ALWAYS see a "detached" thread: the `thread_exit()` is called also in other contexts, outside the `sys__exit()`).

Let's take a look at `thread_exit()`:
```C
/*
 * Cause the current thread to exit.
 *
 * The parts of the thread structure we don't actually need to run
 * should be cleaned up right away. The rest has to wait until
 * thread_destroy is called from exorcise().
 *
 * Does not return.
 */
void thread_exit(void) {

	struct thread *cur;

	cur = curthread;

	/*
	 * Detach from our process. You might need to move this action
	 * around, depending on how your wait/exit works.
	 */
	proc_remthread(cur);

	/* Make sure we *are* detached (move this only if you're sure!) */
	KASSERT(cur->t_proc == NULL);

	/* Check the stack guard band. */
	thread_checkstack(cur);

	/* Interrupts off on this processor */
        splhigh();
	thread_switch(S_ZOMBIE, NULL, NULL);
	panic("braaaaaaaiiiiiiiiiiinssssss\n");
}
```

As our Harvard's colleagues suggest, we may be required to move `proc_remthread(cur)` around. This is exactly our case. The call to `proc_remthread()` will be done explicitly in `sys_exit()` and a check will be done in `thread_exit()` only because this function is called in other part of the operating system.

```C
void thread_exit(void) {

	struct thread *cur;

	cur = curthread;

	/*
	 * LAB04: proc_remthread() is called already outside this function. 
	 * Therefore it is not necessary to call another time it here, but only
	 * check if we are in the specific case where we need to call it. 
	 */
	if (cur->t_proc != NULL) {
		proc_remthread(cur);
	}

	/* Make sure we *are* detached (move this only if you're sure!) */
	KASSERT(cur->t_proc == NULL);

	/* Check the stack guard band. */
	thread_checkstack(cur);

	/* Interrupts off on this processor */
        splhigh();
	thread_switch(S_ZOMBIE, NULL, NULL);
	panic("braaaaaaaiiiiiiiiiiinssssss\n");
}
```

## Process table and waitpid
The `proc_wait` function does not completely accomplish the work required by the `waitpid`, as it starts from a process pointer (instead of a `pid`, an integer value).
For the attribution of a `pid` (process id) to a process, it must be taken into account that it is a single integer (type `pid_t`), with a value between `PID_MIN` and `PID_MAX` (`kern/include/limits.h`), defined according to `__PID_MIN` and `__PID_MAX` (`kern/include/kern/limits.h`). In order to assign a `pid` and to go from process (pointer to `struct proc`) to `pid` and vice versa, it is necessary to **create a table**. For simplicity, it is recommended to create an array of `struct proc` pointers, in which the index corresponds to the `pid` (use an acceptable number as the maximum `pid`, e.g. 100), or an array of pairs (`pid`, process pointer). A suitable `pid` field in the struct proc can instead allow to find the `pid` starting from the pointer. Each newly created process must be added to the table, generating its `pid`, each destroyed process must be removed from the table (once a `wait/waitpid` is completed). For example, the table can be created as a global variable in `kern/proc/proc.c`. It is then necessary to create a possible `sys_waitpid` function to call in `syscall()` (it is recommended to use the same file, e.g. `proc_syscalls.c`, already used for the `sys__exit`).

First thing first, we need to add to the process data structure a new field, storing the `pid` of the current process. Please, note that `IMPLEMENTED_WAITPID` is a boolean identifier defined at the very beginning of `proc.h` which enables, over the compilation process, the parts of the program using `waitpid()` instead of `wait()`.
```C
struct proc {

    /* ... */

#if OPT_LAB04
	int p_status; 				/* current process status */
#if IMPLEMENTED_WAITPID
	pid_t p_pid;				/* current process pid */
#endif

    /* ... */

#endif
}
```

Next, we need to link a process to a pid and viceversa.

### Process table implementation
Let's start from the definition of the process table in `proc.c`. `processTable` is a global variable which stores:
- a boolean value (`active`) indicating if the table has been enabled or not;
- an array of processes. In this implementation, as already stated, exploiting linear array simplifies everything without reducing drastically the performances.
- an integer value corresponding to the process identifier (`pid`);
- a spinlock used to access atomically the table.

The definition is the following:
```C
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
```

We need, of course, to initialize it. We can do it in `proc_bootstrap()`, where the kernel process is set-up an no user processes have been already generated:
```C
void proc_bootstrap(void) {

    /* ... */

#if OPT_LAB04
	spinlock_init(&processTable.lk);
	/* kernel process is not registered in the table */
	processTable.active = 1;
#endif
}
```

Now it is necessary to implement the management of processes during creation and destruction. In particular, when a process is created it must be added to the process table. Let's add this management to the previously defined `init_proc_waitpid()`, called from `proc_create()`:
```C
static void init_process_waitpid(struct proc *proc, const char *name) {
#if OPT_LAB04

	/* acquiring the lock */
  	spinlock_acquire(&processTable.lk);
  	int index = processTable.last_pid + 1;

  	proc->p_pid = 0;

	/* search a free index in table using a circular strategy */
	index = (current_pid > MAX_PROC) ? 1 : index;
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

    /* ... */
}
```
This new implementation will performs the following operations:
1. acquire the spinlock to access the table;
2. search in the table (by means of a circular buffer) a free position in which the new created process can be inserted, obtaining a `pid`;
3. manages errors in case there are too many processes;
4. release the spinlock.


On the other hand, when the process is destroyed, the process table needs to be modified accordingly. Let's add the new implementation in `destroy_proc_waitpid()` called from `proc_destroy()`:
```C
static void destroy_process_waitpid(struct proc *proc) {
#if OPT_LAB04

	/* acquiring the spinlock */
  	spinlock_acquire(&processTable.lk);

	/* acquiring the process pid */
  	int index = proc->p_pid;
  	KASSERT(index > 0 && index <= MAX_PROC);

	/* setting the corresponding entry in the table as free */
  	processTable.proc[index] = NULL;

	/* releasing the spinlock */
  	spinlock_release(&processTable.lk);

    /* ... */
}
```
This new implementation will performs the following operations:
1. acquire the spinlock to access the table;
2. acquire the process `pid`;
3. setting the corresponding entry in the table as free;
4. release the spinlock.

### waitpid implementation
The `common_prog`, internal function of the kernel, does not need to manage the `pid` of a created process (it already has the pointer), therefore it does not need to wait for the end of the created process, supported by the `waitpid` (which requires to manage the process table). If it is the kernel that has the pointer waiting for the created process, the end of a process with `_exit` (and system call `sys__exit`) does not need `waitpid` (with process identified by `pid`), but `proc_wait` (process identified by pointer).
The `waitpid`, on the other hand, is necessary for user programs to explicitly manage processes. For example, `testbin/forktest` would allow you to check the `waitpid` operation. However, for this test program to work, it is necessary to implement the `getpid`, which obtains the `pid` of the current process (pointed to by `curproc`), and the fork, which allows you to generate a user-level child process.

An easy way to test (without the fork), not the `waitpid` directly, but the possible `sys_waitpid` called in `syscall` to support it, is to obtain the child process `pid` in `common_prog` (through `proc_getpid()` or other strategy), and subsequently wait with `sys_waitpid` instead of `proc_wait`.

The following figure shows a possible scenario to verify the correctness of the `getpid`/`waitpid` chain and process table management, not directly, but indirectly through the `proc_getpid()` and `sys_waitpid()` functions.
![getpid](./images/getpid.png)

However, this part will be covered in the next section ([Implementing getpid and fork](#implementing-getpid-and-fork)).


As for the moment, let's concentrate on editing the `common_prog()` function in order to enable the waiting of a process by it's `pid` identifier:
```C
static int common_prog(int nargs, char **args) {

    /* ... */

#if IMPLEMENTED_WAITPID

	/* Case with getpid() and sys_waitpid() enabled */
	pid_t pid = proc_getpid(proc);      // just a wrapper, proc->p_pid was sufficient
	pid_t retpid = proc_waitpid(pid, &exit_code);
	kprintf("[!] process %d exits with exit_code: %d\n", retpid, exit_code);

#else

	/* Case without getpid() and sys_waitpid() enabled */
	exit_code = proc_wait(proc);
	kprintf("[!] exit_code is: %d\n", exit_code);

#endif

    return 0;
}
```

And now, we need to define the two utility functions exploited in `common_prog()`. Let's write the prototypes in the `proc.h` file as:
```C
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
```

And their's implementation in `proc.c` as:
```C
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
    *exit_status = proc_wait(proc);

	/* returning the pid of the process */
	return proc_getpid(proc);
#else
	*exit_status = 1;
	return -1;
#endif
}
```

Finally, the `proc_search_pid()` function simply search the pid-associated process in the process table:
```C
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
```

Running a user process now (e.g. `testbin/palin`), it will ends writing the exit status and, eventually, the pid of the process:
![waitpid](./images/waitpid.gif)


## Implementing getpid and fork

Last part of this laboratory requires us to implement the `getpid()` (easy task) and `fork()` (not-so-easy task) functionality in order to manage kernel processes instead of user processes. 

Indeed, trying to run `p testbin/forktest` will result in a `unknown systemcall 5`. The `systemcall 5` is defined (as the other system calls), in the system call header file `syscall.h` (*path*: `os16/src/kern/include/kern/syscall.h`):
```C
#define SYS_getpid       5
```

Here is our starting point. Let's see what the `forktest` performs to understand what we need to implement. The `forktest` test is defined as follows. First, in `main()`, a preliminary set-up is performed. Then, the actual test is called:
```C
int main(int argc, char *argv[]) {

    /* ... */
    int nowait = 0;

    test(nowait);
    /* ... */

    return 0;
}
``` 
and the `test()` function is:
```C
static void test(int nowait) {

	int pid0, pid1, pid2, pid3;
	int depth = 0;

	/*
	 * Caution: This generates processes geometrically.
	 *
	 * It is unrolled to encourage gcc to registerize the pids,
	 * to prevent wait/exit problems if fork corrupts memory.
	 *
	 * Note: if the depth prints trigger and show that the depth
	 * is too small, the most likely explanation is that the fork
	 * child is returning from the write() inside putchar()
	 * instead of from fork() and thus skipping the depth++. This
	 * is a fairly common problem caused by races in the kernel
	 * fork code.
	 */

	pid0 = dofork();
	depth++;
	putchar('A');
	if (depth != 1) {
		warnx("depth %d, should be 1", depth);
	}
	check();

	pid1 = dofork();
	depth++;
	putchar('B');
	if (depth != 2) {
		warnx("depth %d, should be 2", depth);
	}
	check();

	pid2 = dofork();
	depth++;
	putchar('C');
	if (depth != 3) {
		warnx("depth %d, should be 3", depth);
	}
	check();

	pid3 = dofork();
	depth++;
	putchar('D');
	if (depth != 4) {
		warnx("depth %d, should be 4", depth);
	}
	check();

	/*
	 * These must be called in reverse order to avoid waiting
	 * improperly.
	 */
	dowait(nowait, pid3);
	dowait(nowait, pid2);
	dowait(nowait, pid1);
	dowait(nowait, pid0);

	putchar('\n');
}
```
Basically, it will perform 4 `fork` and 4 `waitpid` in order to print 4 characters.

We need, therefore, to implement the two system calls previously stated.

### fork system call
Let's start from the `fork` system call. Let's define it in the `syscalls_LAB04.h` file, which contains the definition of the others system calls (not the ones defined in **LAB02**). The prototype is the following:
```C
#if IMPLEMENTED_FORK
int sys_fork_LAB04(struct trapframe *ctf, pid_t *retval);
#endif
```

Please, note that `IMPLEMENTED_FORK` is defined at the very beginning of the file as:
```C
#include "opt-lab04.h" //optional include for LAB04
#if OPT_LAB04
/**
 * IMPLEMENTED_FORK == 1 --> Enable implementation with fork
 * IMPLEMENTED_FORK == 0 --> Enable implementation without fork
 */
#define IMPLEMENTED_FORK 1
#endif
```

The actual implementation of the function is defined in the `syscall_LAB04.c` file (path: `os161/src/kern/syscall/syscall.c`) as:
```C
/**
 * @brief fork() system call. It will create a new identical process, given a process
 * 
 * @param ctf trapframe of the given process
 * @param retval pid of the new process
 * @return 0 on success
 */
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
```
Basically, it will performs the following operations:
1. assert current process to actually exists. It cannot be `NULL`, of course;
2. create a new runnable process;
3. duplicate the address space of the current process;
4. create a copy of the parent's trapframe;
5. copy actual memory;
6. generate the new thread that will run the new process;
7. update the return value with the pid of the new process.

Please, note that: `thread_fork()` will pass as parameter a specific function, i.e. `call_enter_forked_process`, that we need to define. This function will simply enters in the new thread and starts it, without returning:
```C
/**
 * @brief Starts the new generated thread
 * 
 * @param tfv trapframe
 * @param dummy not used
 */
#if IMPLEMENTED_FORK
static void call_enter_forked_process(void *tfv, unsigned long dummy) {
  struct trapframe *tf = (struct trapframe *)tfv;
  (void)dummy;
  enter_forked_process(tf); 
 
  panic("enter_forked_process returned (should not happen)\n");
}
#endif
```

Therefore, we also need to define in `syscall.c` (*path*: `os161/src/kern/arch/mips/syscall/syscall.c`) the `enter_forked_process()` function:
```C
#if IMPLEMENTED_FORK

	// Duplicate frame so it's on stack
	struct trapframe forkedTf = *tf; // copy trap frame onto kernel stack

	forkedTf.tf_v0 = 0; 	// return value is 0
    forkedTf.tf_a3 = 0; 	// return with success
	forkedTf.tf_epc += 4; 	// return to next instruction

	as_activate();

	mips_usermode(&forkedTf);
#else
	(void)tf;
#endif
```

And now, finally, we cane activate the corresponding system call in the system call dispatcher:
```C
#if IMPLEMENTED_FORK	
		case SYS_fork:
			err = sys_fork_LAB04(tf, &retval);
        break;
#endif
```

### getpid system call
At this point, we have to complete the lab with an easy task: implement the `getpid()` system call. The function prototype is defined in `syscalls_LAB04.h`, the header file where we have defined also the `fork` system calls. The implementation is the following:
```C
/**
 * @brief Retrieve the pid of the current process
 * 
 * @return pid_t 
 */
pid_t sys_getpid(void) {
#if IMPLEMENTED_WAITPID
    KASSERT(curproc != NULL);
    return curproc->p_pid;
#else
    return -1;
#endif
}
```
and enable it in the system call dispatcher:
```C
#if IMPLEMENTED_WAITPID:
		case SYS_getpid:
			retval = sys_getpid_LAB04();
			if (retval <0 ) err = ENOSYS;
		break;
#endif
```

We need now to define also the `waitpid` system call, but this task is even more trivial since we did it already for the `proc_waitpid()` (the function is basically the same):
```C
/**
 * @brief waitpid() system call
 * 
 * @param pid process identifier of the process 
 * @param statusp status 
 * @param options options (not handled here)
 * @return int 
 */
int sys_waitpid_LAB04(pid_t pid, userptr_t statusp, int options) {
#if IMPLEMENTED_WAITPID

    /* retrieve the process given the pid */
    struct proc *proc = proc_search_pid(pid);
	if (proc == NULL) {
		return -1;
	}

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
```

Finally, we need to enable the system call in the system call dispatcher:
```C
void syscall(struct trapframe *tf) {

    /* ... */

#if IMPLEMENTED_WAITPID
		case SYS_waitpid:
			retval = sys_waitpid_LAB04(
				(pid_t) tf->tf_a0, 			/* pid 		*/
				(userptr_t) tf->tf_a1,		/* statusp 	*/
				(int)tf->tf_a2				/* options 	*/
				);
			err = 0;
			if (retval == -1) err = ENOSYS;
		break;
#endif

    /* ... */
}
```

Now we are able to run `p testbin/forktest`.
