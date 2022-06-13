/**
 * @file syscalls_LAB05.c
 * @author Davide Arcolini (davide.arcolini@studenti.polito.it)
 * @brief Contains the definitions of the system calls used in LAB05
 * @version 0.1
 * @date 2022-06-12
 * 
 * @copyright Copyright (c) 2022
 * 
*/

#include <types.h>
#include <kern/unistd.h>
#include <kern/errno.h>
#include <clock.h>
#include <syscall.h>
#include <current.h>
#include <lib.h>
#include <copyinout.h>
#include <vnode.h>
#include <vfs.h>
#include <limits.h>
#include <uio.h>
#include <proc.h>

#include "syscalls_LAB05.h"

/**
 * @brief Max number of system wide open files
 */
#define SYSTEM_OPEN_MAX (10*OPEN_MAX)

/**
 * USE_KERNEL_BUFFER == 1 --> enable kernel buffer
 * USE_KERNEL_BUFFER == 0 --> disable kernel buffer
 */
#define USE_KERNEL_BUFFER 0


/**
 * @brief struct defining a pointer to a specific vnode.
 *        Used to build an array of pointers to vnodes (i.e. files) opened for
 *        a given user process.
 */
struct openfile {
    struct vnode *vn;
    off_t offset;	
    unsigned int countRef;
}; 
struct openfile systemFileTable[SYSTEM_OPEN_MAX];


#if USE_KERNEL_BUFFER
/**
 * @brief Read from a specific file using the kernel buffer.
 * 
 * @param fd file descriptor
 * @param buf_ptr buffer pointer
 * @param size size of the raeading performed
 * @return 0 on success 
 */
int file_read_LAB05(int fd, userptr_t buf_ptr, size_t size) {
    struct iovec iov;
    struct uio ku;
    int result, nread;
    struct vnode *vn;
    struct openfile *of;
    void *kbuf;

    if (fd < 0 || fd > OPEN_MAX) {
        return -1;
    }

    of = curproc->fileTable[fd];
    if (of == NULL) {
        return -1;
    }

    vn = of->vn;
    if (vn == NULL) {
        return -1;
    }

    kbuf = kmalloc(size);
    uio_kinit(&iov, &ku, kbuf, size, of->offset, UIO_READ);
    result = VOP_READ(vn, &ku);
    if (result) {
        return result;
    }

    of->offset = ku.uio_offset;
    nread = size - ku.uio_resid;
    copyout(kbuf, buf_ptr, nread);
    kfree(kbuf);

    return (nread);
}

/**
 * @brief Write to a specific file using the kernel buffer.
 * 
 * @param fd file descriptor
 * @param buf_ptr buffer pointer
 * @param size size to write
 * @return int 
 */
int file_write_LAB05(int fd, userptr_t buf_ptr, size_t size) {
    struct iovec iov;
    struct uio ku;
    int result, nwrite;
    struct vnode *vn;
    struct openfile *of;
    void *kbuf;

    if (fd < 0 || fd > OPEN_MAX) {
        return -1;
    }

    of = curproc->fileTable[fd];
    if (of == NULL) {
        return -1;
    }

    vn = of->vn;
    if (vn == NULL) {
        return -1;
    }

    kbuf = kmalloc(size);
    copyin(buf_ptr, kbuf,size);
    uio_kinit(&iov, &ku, kbuf, size, of->offset, UIO_WRITE);

    result = VOP_WRITE(vn, &ku);
    if (result) {
        return result;
    }

    kfree(kbuf);
    of->offset = ku.uio_offset;
    nwrite = size - ku.uio_resid;
    return (nwrite);
}

#else
/**
 * @brief Read from a specific file without using the kernel buffer.
 * 
 * @param fd file descriptor
 * @param buf_ptr buffer pointer
 * @param size size to read
 * @return int 
 */
int file_read_LAB05(int fd, userptr_t buf_ptr, size_t size) {
    struct iovec iov;
    struct uio u;
    int result;
    struct vnode *vn;
    struct openfile *of;

    if (fd < 0 || fd > OPEN_MAX) {
        return -1;
    }

    of = curproc->fileTable[fd];
    if (of == NULL) {
        return -1;
    }

    vn = of->vn;
    if (vn == NULL) {
        return -1;
    }
    
    iov.iov_ubase = buf_ptr;
    iov.iov_len = size;

    u.uio_iov = &iov;
    u.uio_iovcnt = 1;
    u.uio_resid = size;          // amount to read from the file
    u.uio_offset = of->offset;
    u.uio_segflg = UIO_USERISPACE;
    u.uio_rw = UIO_READ;
    u.uio_space = curproc->p_addrspace;

    result = VOP_READ(vn, &u);
    if (result) {
        return result;
    }

    of->offset = u.uio_offset;
    return (size - u.uio_resid);
}

/**
 * @brief Write to a specific file without using the kernel buffer.
 * 
 * @param fd file descriptor
 * @param buf_ptr buffer pointer
 * @param size size to write
 * @return int 
 */
int file_write_LAB05(int fd, userptr_t buf_ptr, size_t size) {
    struct iovec iov;
    struct uio u;
    int result, nwrite;
    struct vnode *vn;
    struct openfile *of;

    if (fd < 0 || fd > OPEN_MAX) {
        return -1;
    }

    of = curproc->fileTable[fd];
    if (of == NULL) {
        return -1;
    }

    vn = of->vn;
    if (vn == NULL) {
        return -1;
    }

    iov.iov_ubase = buf_ptr;
    iov.iov_len = size;

    u.uio_iov = &iov;
    u.uio_iovcnt = 1;
    u.uio_resid = size;          // amount to read from the file
    u.uio_offset = of->offset;
    u.uio_segflg = UIO_USERISPACE;
    u.uio_rw = UIO_WRITE;
    u.uio_space = curproc->p_addrspace;

    result = VOP_WRITE(vn, &u);
    if (result) {
        return result;
    }
    of->offset = u.uio_offset;
    nwrite = size - u.uio_resid;
    return (nwrite);
}
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
int sys_open_LAB05(userptr_t path, int openflags, mode_t mode, int *errp) {

    int fd;
    struct vnode *v;
    struct openfile *of = NULL;
    int result;

    /* opening file with VFS utilities function */
    result = vfs_open((char *)path, openflags, mode, &v);
    if (result) {
        *errp = ENOENT;
        return -1;
    }

    /* search system open file table */
    for (int i = 0; i < SYSTEM_OPEN_MAX; i++) {
        if (systemFileTable[i].vn == NULL) {
            of = &systemFileTable[i];
            of->vn = v;
            of->offset = 0; // TODO: handle offset with append
            of->countRef = 1;
            break;
        }
    }

    if (of == NULL) { 

        // no free slot in system open file table
        *errp = ENFILE;

    } else {

        for (fd = STDERR_FILENO+1; fd < OPEN_MAX; fd++) {
            if (curproc->fileTable[fd] == NULL) {
	            curproc->fileTable[fd] = of;
	            return fd;
            }
        }

        *errp = EMFILE;
    }
    
    vfs_close(v);
    return -1;
}

/**
 * @brief close() system call
 * 
 * @param fd file descriptor
 * @return int 
 */
int sys_close_LAB05(int fd) {

    struct openfile *of = NULL; 
    struct vnode *vn;

    if (fd < 0 || fd > OPEN_MAX) {
        return -1;
    }

    of = curproc->fileTable[fd];
    if (of == NULL) {
        return -1;
    }

    curproc->fileTable[fd] = NULL;

    if (--of->countRef > 0) {
        return 0; // just decrement ref cnt
    }

    vn = of->vn;
    of->vn = NULL;
    if (vn == NULL) {
        return -1;
    }

    vfs_close(vn);	
    return 0;
}
