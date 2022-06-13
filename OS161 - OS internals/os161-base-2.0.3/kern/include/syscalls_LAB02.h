/**
 * @file syscalls_LAB02.h
 * @author Davide Arcolini (davide.arcolini@studenti.polito.it)
 * @brief Contains the declarations of the system calls used in LAB02 (and following)
 * @version 0.1
 * @date 2022-05-02
 * 
 * @copyright Copyright (c) 2022
 * 
*/


#ifndef _SYSCALLS_LAB02_H_
#define _SYSCALLS_LAB02_H_

#include "opt-lab02.h"
#include "opt-lab04.h"

#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

typedef int ssize_t;

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
ssize_t sys_write_LAB02(int fd, const void *buf, ssize_t n_bytes);
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
ssize_t sys_read_LAB02(int fd, void *buf, ssize_t n_bytes);
#endif

/**
 * @brief Basic (simpler) implementation of _exit(). It should saves the state of the process
 *        ending (in order to be read from other processes), release memory and destroy the
 *        main thread. However, this simpler version only destroy the main thread and then exit.
 * 
 * @param status integer value indicating the result of the operation. Tipically EXIT_SUCCESS or
 *               EXIT_FAILURE 
 */
#if OPT_LAB02
void sys_exit_LAB02(int status);
#endif

#endif /* _SYSCALLS_LAB02_H_ */