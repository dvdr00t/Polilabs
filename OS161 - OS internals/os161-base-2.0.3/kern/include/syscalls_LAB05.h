/**
 * @file syscalls_LAB05.h
 * @author Davide Arcolini (davide.arcolini@studenti.polito.it)
 * @brief Contains the declarations of the system calls used in LAB05
 * @version 0.1
 * @date 2022-06-12
 * 
 * @copyright Copyright (c) 2022
 * 
*/

#ifndef _SYSCALLS_LAB05_H_
#define _SYSCALLS_LAB05_H_

#include "opt-lab05.h"

/**
 * @brief struct defining a pointer to a specific vnode.
 *        Used to build an array of pointers to vnodes (i.e. files) opened for
 *        a given user process.
 */
#if OPT_LAB05
struct openfile;
#endif

#if OPT_LAB05
void openfileIncrRefCount(struct openfile *of);
#endif

/**
 * @brief open() system call
 * 
 * @param path filename
 * @param openflags optional flags specifying how to open the file (only read, only write, etc...)
 * @param mode specify the access mode to the file (granting access to specific users or groups)
 * @param errp error value in case of failure
 * @return int 
 */
#if OPT_LAB05
int sys_open_LAB05(userptr_t path, int openflags, mode_t mode, int *errp);
#endif

/**
 * @brief close() system call
 * 
 * @param fd file descriptor
 * @return int 
 */
#if OPT_LAB05
int sys_close_LAB05(int fd);
#endif

#if OPT_LAB05
/**
 * @brief Read from a specific file without using the kernel buffer.
 * 
 * @param fd file descriptor
 * @param buf_ptr buffer pointer
 * @param size size to read
 * @return int 
 */
int file_read_LAB05(int fd, userptr_t buf_ptr, size_t size);
#endif

#if OPT_LAB05
/**
 * @brief Write to a specific file without using the kernel buffer.
 * 
 * @param fd file descriptorù
 * @param buf_ptr buffer pointer
 * @param size size to write
 * @return int 
 */
int file_write_LAB05(int fd, userptr_t buf_ptr, size_t size);
#endif

#endif /* _SYSCALLS_LAB05_H_ */