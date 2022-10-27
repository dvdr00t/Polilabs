# OS161 - System and Device Programming

## LAB05 - File System support
* **Author**: `Davide Arcolini`
* **Date**: `2022-06-12`

## Table of contents
1. [Introduction](#introduction)
    - [Virtual File System](#virtual-file-system-vfs)
    - [open and close a file in `runprogram()`](#open-and-close-a-file)
    - [read and write a file in `load_elf()`](#read-and-write-a-file)
2. [System calls](#system-calls)
    - [`open()` system call and `close()` system call](#open-system-call-and-close-system-call)
    - [`read()` system call and `write()` system call](#read-system-call-and-write-system-call)

> **NB**: all over the file, `os161-base_2.0.3` is referred as `src`. This is to simplified paths as `os161/os161-base_2.0.3/...` to `os161/src`. Keep it in mind!

## Introduction
*os161* lacks support for the **file system**, intended as the set of system calls that provide file operations, such as: `open()`, `read()`, `write()`, `lseek()`, `close()`, `dup2()`, `chdir()`, `__getcwd()`. This support, in addition to requiring the implementation of individual functions, also needs appropriate data structures (tables for open files, directories, etc.) that allow the search of files and their identification starting from an integer `id`.

### Virtual File System (VFS)
A **virtual file system** or *virtual filesystem switch* is an abstract layer on top of a more concrete file system. The purpose of a VFS is to allow client applications to access different types of concrete file systems in a uniform way. (*source*: [wikipedia](https://en.wikipedia.org/wiki/Virtual_file_system))

In *os161*, the VFS is defined in `vfs.h` (*path*: `os161/src/kern/include/vfs.h`). In this context, the `struct vnode` (path: `os161/src/kern/include/vnode.h`) represents a **file** and it is defined as follows:
```C
/*
 * A struct vnode is an abstract representation of a file.
 *
 * It is an interface in the Java sense that allows the kernel's
 * filesystem-independent code to interact usefully with multiple sets
 * of filesystem code.
 */

/*
 * Abstract low-level file.
 *
 * Note: vn_fs may be null if the vnode refers to a device.
 */
struct vnode {
	int vn_refcount;                /* Reference count */
	struct spinlock vn_countlock;   /* Lock for vn_refcount */

	struct fs *vn_fs;               /* Filesystem vnode belongs to */

	void *vn_data;                  /* Filesystem-specific data */

	const struct vnode_ops *vn_ops; /* Functions on this vnode */
};
```

Moreover, `vfs.c` contains the definitions of functions like `vfs_open()` (which open a file), `vfs_close()` (which close a file) and so many others in order to manage files.

As suggested by the text of the laboratory, let's begin by following the process of:
- `runprogram()` in order to understand how files are opened and closed;
- `load_segment()` in order to understand how file are written and read.

### open and close a file
In `common_prog()` function, inside the routine `cmd_progthread()` when a new kernel thread is generated, id has to open a specific program to run. The function `runprogram()` loads program `progname` and starts running it in *usermode*. It does not return except on error. Here, the function calls `vfs_open` on `progname` and thus may destroy it.

The function is defined as follows:
```C
int
runprogram(char *progname)
{
	struct addrspace *as;
	struct vnode *v;
	vaddr_t entrypoint, stackptr;
	int result;

	/* Open the file. */
	result = vfs_open(progname, O_RDONLY, 0, &v);
	if (result) {
		return result;
	}

	/* We should be a new process. */
	KASSERT(proc_getas() == NULL);

	/* Create a new address space. */
	as = as_create();
	if (as == NULL) {
		vfs_close(v);
		return ENOMEM;
	}

	/* Switch to it and activate it. */
	proc_setas(as);
	as_activate();

	/* Load the executable. */
	result = load_elf(v, &entrypoint);
	if (result) {
		/* p_addrspace will go away when curproc is destroyed */
		vfs_close(v);
		return result;
	}

	/* Done with the file now. */
	vfs_close(v);

	/* Define the user stack in the address space */
	result = as_define_stack(as, &stackptr);
	if (result) {
		/* p_addrspace will go away when curproc is destroyed */
		return result;
	}

	/* Warp to user mode. */
	enter_new_process(0 /*argc*/, NULL /*userspace addr of argv*/,
			  NULL /*userspace addr of environment*/,
			  stackptr, entrypoint);

	/* enter_new_process does not return. */
	panic("enter_new_process returned\n");
	return EINVAL;
}
```
and it performs the following operation:
- **open a file** with `vfs_open()`. The file contains, in this case, the executable program to be run in the current process.
- create a new address space and activate it.
- **load the executable** in memory with `load_elf()` (see [read and write a file in `load_elf()`](#read-and-write-a-file)).
- **close the file** with `vfs_close()`.
- define user stack.
- switch to user mode.

### read and write a file
on the other hand, an example of reading and writing is performed in `load_elf()`, which loads an ELF executable user program into the current address space and returns the entry point (initial PC) for the program in `ENTRYPOINT`. I will not report here the entire function, but what is important are the function involved. 

These functions support data transfer between files, kernel memory and user memory thanks to the `uio` module. The functions of main interest in this context are: `uio_kinit()` (followed by `VOP_READ` or `VOP_WRITE`) for **kernel IO** (for IO with buffers in **userspace** a manual variant of `uio_kinit()` is required) and `uiomove()` (called by `copyout()`, `copyin()` and `copystr()`), which transfers data between user and kernel memory. Note that `copyout()` and `copyin()` are used to copy data between user memory and kernel memory safely, that is, without the kernel crashing in case of an incorrect user pointer.




## System calls
Now, that we have understand a bit more how to open and close a file, we can create the system calls that perform these operations. 

These system calls can be tested by running, from the menu, the command `p testbin/filetest` which will performs the following operations (among others) a couple of times:
- open a file (it is a test file and not a file passed as argument, since passing arguments to main will be performed in the optional section):
    ```C
    fd = open(file, O_WRONLY|O_CREAT|O_TRUNC, 0664);
	if (fd <0 ) {
		err(1, "%s: open for write", file);
	}
    ```
- close a file:
    ```C
    rv = close(fd);
	if (rv < 0) {
		err(1, "%s: close (1st time)", file);
	}
    ```

Let's implement those system calls.
### open system call and close system call
For a given user process, it is necessary to generate a table of `vnode` pointers or a table of struct where one of the fields is a `vnode` pointer. Let's go with the second option.

First of all, we need to define the table structure. Let's create the header file in `os161/src/kern/include` as `syscalls_LAB05.h` (as we did for the previous LABs). Here we can define the building block of the table struct. Each item in the array (i.e. each open file for a given user process) will contain:
- a pointer to a `struct vnode`;
- the corresponding offset indicating the buffer position in the file;
- an integer value counting the number of process which are concurrently using the file.

The struct is defined as:
```C
/**
 * @brief struct defining a pointer to a specific vnode.
 *        Used to build an array of pointers to vnodes (i.e. files) opened for
 *        a given user process.
 */
#if OPT_LAB05
struct openfile;
#endif
```

and, in `syscalls_LAB05.c` (*path*: `os161/src/kern/syscalls/syscalls_LAB05.c`):
```C
/**
 * @brief struct defining a pointer to a specific vnode.
 *        Used to build an array of pointers to vnodes (i.e. files) opened for
 *        a given user process.
 */
#if OPT_LAB05
struct openfile {
    struct vnode *vn;
    off_t offset;	
    unsigned int countRef;
};
#endif
```

Now, in `proc.h` (*path*: `os161/src/kern/include/proc.h`), after importing the required library:
```C
#if OPT_LAB05
#include <limits.h>				// to use OPEN_MAX
#include "syscalls_LAB05.h"		// to use openfile
#endif
```

we can edit the process data structure, adding the `openfile` array of files:
```C
struct proc {

    /* ... */

#if OPT_LAB05
    struct openfile *fileTable[OPEN_MAX];
#endif
};
```

We also need to initialize it when a process is generated with `proc_create()`:
```C
static struct proc *proc_create(const char *name) {

    /* ... */

/* LAB05: initialization of the fileTable */
#if OPT_LAB05
	bzero(proc->fileTable, OPEN_MAX*sizeof(struct openfile*));
#endif
}
```

Now, we have to defined a system file table in order to associated to each file an unique identifier (the process is very similar to the one implemented in LAB04 when we wanted to associated an unique `pid` to a specific process). Let's define the array of system files in `syscalls_LAB05.c` as:
```C
struct openfile systemFileTable[SYSTEM_OPEN_MAX];
```
where `SYSTEM_OPEN_MAX` is defined previously as:
```C
/**
 * @brief Max number of system wide open files
 */
#define SYSTEM_OPEN_MAX (10*OPEN_MAX)
```

At this point we can defined the system calls. Let's start from the opening of a file. The declaration of the system call is done in `syscalls_LAB05.h` as:
```C
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
```
and its definition:
```C
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
                kprintf("[DEBUG] you got here with fd == %d\n", fd);
	            return fd;
            }
        }

        *errp = EMFILE;
    }
    
    vfs_close(v);
    return -1;
}
```

> Note the call to `vfs_open()`.

Now, the close system call declaration:
```C
/**
 * @brief close() system call
 * 
 * @param fd file descriptor
 * @return int 
 */
#if OPT_LAB05
int sys_close_LAB05(int fd);
#endif
```
and definition:
```C
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
```
> Note the call to `vfs_close()`.

### read system call and write system call
We already implemented `read()` and `write()` system calls in LAB02, but the only supported the reading and writing from `STDIN`, `STDOUT` and `STDERR`. It is time to expand them.

First of all, we will implement them once by exploiting the **kernel buffer** and once not.

1. **Exploiting kernel buffer**
    ```C
    #if USE_KERNEL_BUFFER
    /**
     * @brief Read from a specific file using the kernel buffer.
     * 
     * @param fd file descriptor
     * @param buf_ptr buffer pointer
     * @param size size of the raeading performed
     * @return 0 on success 
     */
    static int file_read_LAB05(int fd, userptr_t buf_ptr, size_t size) {
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
    static int file_write_LAB05(int fd, userptr_t buf_ptr, size_t size) {
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
    ```

    > Note the calls to `VOP_READ()` and `VOP_WRITE()`. Moreover, note the calls to `copyin()` and `uio_kinit()` which allows the operations with the kernel buffer.

2. Exploiting user buffer
On the other hand:
    ```C
    /**
    * @brief Read from a specific file without using the kernel buffer.
    * 
    * @param fd file descriptor
    * @param buf_ptr buffer pointer
    * @param size size to read
    * @return int 
    */
    static int file_read_LAB05(int fd, userptr_t buf_ptr, size_t size) {
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

    static int
    /**
    * @brief Write to a specific file without using the kernel buffer.
    * 
    * @param fd file descriptor
    * @param buf_ptr buffer pointer
    * @param size size to write
    * @return int 
    */
    file_write(int fd, userptr_t buf_ptr, size_t size) {
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
    ```

3. **Adding functions to system calls**
Finally, we can add these functions to the system calls defined in LAB02 and see the result of the `testbin/filetest`.
```C
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
```

