/**
 * @file syscalls_LAB04.h
 * @author Davide Arcolini (davide.arcolini@studenti.polito.it)
 * @brief Contains the declarations of the system calls used in LAB04
 * @version 0.1
 * @date 2022-06-11
 * 
 * @copyright Copyright (c) 2022
 * 
*/

#ifndef _SYSCALLS_LAB04_H_
#define _SYSCALLS_LAB04_H_

#include "opt-lab04.h"

/**
 * @brief fork() system call. It will create a new identical process, given a process
 * 
 * @param ctf trapframe of the given process
 * @param retval pid of the new process
 * @return 0 on success
 */
#if OPT_LAB04
int sys_fork_LAB04(struct trapframe *ctf, pid_t *retval);
#endif

/**
 * @brief Starts the new generated thread
 * 
 * @param tfv trapframe
 * @param dummy not used
 */
#if OPT_LAB04
void call_enter_forked_process(void *tfv, unsigned long dummy);
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
int sys_waitpid_LAB04(pid_t pid, userptr_t statusp, int options);
#endif

/**
 * @brief Retrieve the pid of the current process
 * 
 * @return pid_t 
 */
#if OPT_LAB04
pid_t sys_getpid_LAB04(void);
#endif

#endif /* _SYSCALLS_LAB04_H_ */