# OS161 - System and Device Programming

## LAB03 - Locks and Conditional Variables implementation

* **Author**: `Davide Arcolini`
* **Date**: `2022-06-05`

## Table of contents
1. [Testing semaphores (`sy1`)](#testing-semaphores)
2. [Testing locks (`sy2`)](#testing-locks)
    - [Lock create](#lock-creation)
    - [Lock acquire](#lock-acquire)
    - [Lock release](#lock-release)

> **NB**: all over the file, `os161-base_2.0.3` is referred as `src`. This is to simplified paths as `os161/os161-base_2.0.3/...` to `os161/src`. Keep it in mind!

## Testing semaphores
Let's start by creating another version of the kernel (from `DUMBVM` is fine, we do not need any particular configuration) and then run the operating system and gets to the menu (at this point, it should be a trivial task). Here it is possible to call procedure which will make use of the already-implemented **semaphores** in order to check their correct behavior. This is not part of the laboratory yet. This introduction is done to understand how the calls `sy1`, `sy2`, etc... works. The laboratory starts at [Testing locks](#testing-locks).

`sy1` performs a synchronization test by means of semaphores. Semaphores are implemented in **non-busy waiting** mode. To implement them, **wait channels** are used. Wait channels are a type of condition variable made in OS161, associated with a **spinlock**. Since spinlocks are locks with *busy waiting*, they are suitable for cases where (within the kernel) there is a limited wait. Read (and trace with the debugger) the creation of a semaphore: you will observe that the spinlock protects (by mutual exclusion) the access to the counter inside the semaphore, while the wait channel is used to implement waiting and signaling (i.e. a thread waits to be awakened, by reporting, from another thread).

Let's call the `sy1` procedure and follow the flow. First, from the menu we will reach the `semtest()` function (*path*: `os161/src/kern/test/synchtest.c`). This function performs the following operation in order to test the semaphore:
1. **Initialization of the semaphore**
    At the very beginning, the function will call `inititems()`, a function which will take care of the creations of all the structures used to run all the tests (in this case, we are interested in the part which will actually create the semaphore).
    ```C
    if (testsem == NULL) {
		testsem = sem_create("testsem", 2);
		if (testsem == NULL) {
			panic("synchtest: sem_create failed\n");
		}
	}
    ```
    Two things to take into accounts:
    1. `semtest` is a global semaphore variable (`static struct semaphore *semtest`). A semaphore struct is defined in `synch.h` (*path*: `os161/src/kern/include/synch.h`) as:
        ```C
        struct semaphore {
            char *sem_name;
            struct wchan *sem_wchan;
            struct spinlock sem_lock;
            volatile unsigned sem_count;
        };
        ```

        where:
            - `sem_name` is a string containing the name of the semaphore (useful for debugging);
            - `sem_wchan` is a pointer to a `wchan` struct. A `wchan` (i.e. **waiting channel**) is a particular structure which contains the name of the channel (again, a string useful for debugging) and a `threadlist`. A `threadlist` is, unexpectedly, a list of threads! A thread set to sleep is placed inside the `threadlist`, waiting to get waken up. Therefore, a `wchan` is nothing more than a memory place where the thread sleeps.
            - `sem_lock` is a `spinlock` variable. It's where the magic happens. It stores the address of memory where the thread spins (i.e. waits) and the pointer to the `CPU` struct which holds that thread.
            - `sem_count` indicates the number of resources that can be obtained from that semaphore at the very beginning.
    2. `sem_create()` initializes the semaphore as: 
        ```C
        struct semaphore *sem_create(const char *name, unsigned initial_count) {
            struct semaphore *sem;

            sem = kmalloc(sizeof(*sem));        /* initialization of the semaphore */
            if (sem == NULL) {
                    return NULL;
            }

            sem->sem_name = kstrdup(name);      /* initialization of the name */
            if (sem->sem_name == NULL) {
                    kfree(sem);
                    return NULL;
            }

            sem->sem_wchan = wchan_create(sem->sem_name);       /* initialization of the wchan */
            if (sem->sem_wchan == NULL) {
                kfree(sem->sem_name);
                kfree(sem);
                return NULL;
            }

            spinlock_init(&sem->sem_lock);      /* initialization of the spinlock */
            sem->sem_count = initial_count;     /* initialization of the count */

            return sem;
        }
        ```
2. **Testing initialization of the semaphore**
After the semaphore has been initialized, a quick test is run in order to check if everything works fine:

    ```C
    kprintf("Starting semaphore test...\n");
	kprintf("If this hangs, it's broken: ");
	P(testsem);
	P(testsem);
	kprintf("ok\n");
    ```
    Here we have to understand how `P()` and `V()` works: 
    - `P()` is used to acquire a resource from the semaphore (a sort of `wait()`). It works as expected, i.e. it waits in the spinlock if the no resource is available, otherwise it acquires the resource and then release the spinlock:
        ```C
        void P(struct semaphore *sem) {
            KASSERT(sem != NULL);

            /*
            * May not block in an interrupt handler.
            *
            * For robustness, always check, even if we can actually
            * complete the P without blocking.
            */
            KASSERT(curthread->t_in_interrupt == false);

            /* Use the semaphore spinlock to protect the wchan as well. */
            spinlock_acquire(&sem->sem_lock);
            while (sem->sem_count == 0) {
                /*
                *
                * Note that we don't maintain strict FIFO ordering of
                * threads going through the semaphore; that is, we
                * might "get" it on the first try even if other
                * threads are waiting. Apparently according to some
                * textbooks semaphores must for some reason have
                * strict ordering. Too bad. :-)
                *
                * Exercise: how would you implement strict FIFO
                * ordering?
                */
                wchan_sleep(sem->sem_wchan, &sem->sem_lock);
            }
            
            KASSERT(sem->sem_count > 0);
            sem->sem_count--;
            spinlock_release(&sem->sem_lock);
        }
        ```
    - `V()` is used to release back a resource to the semaphore (a sort of `signal()`). It works as expected, again. It acquires the spinlock, wakes the thread sleeping the waiting channel, release the resource (incrementing the counter of the semaphore) and release again the spinlock:
        ```C
        void V(struct semaphore *sem) {
            KASSERT(sem != NULL);

            spinlock_acquire(&sem->sem_lock);

            sem->sem_count++;
            KASSERT(sem->sem_count > 0);
            wchan_wakeone(sem->sem_wchan, &sem->sem_lock);

            spinlock_release(&sem->sem_lock);
        }
        ```
    Now we have almost all the pieces to understand how this test works.
3. **Creating the threads to run the test**
Threads are generated with the `semtestthread` routine, which receives as argument an incremental number (based on the thread) from $0$ to $63$: 
    ```C
    for (i=0; i<NTHREADS; i++) {
		result = thread_fork("semtest", NULL, semtestthread, NULL, i);
		if (result) {
			panic("semtest: thread_fork failed: %s\n", strerror(result));
		}
	}
    ```
    Meanwhile, the main thread will call a `V()` to let the first of the new-generated threads start, and then waits for its finish (exploiting another semaphore, just like the `semtest`): 
    ```C
    for (i=0; i<NTHREADS; i++) {
		V(testsem);
		P(donesem);
	}
    ```

4. **Thread routine**
Each thread will simply acquire a resource from the semaphore, print some character and then, release back a resource to the semaphore, signalling also the main thread which manage the start of the routine-thread:
    ```C
    static void semtestthread(void *junk, unsigned long num) {
        int i;
        (void)junk;

        /*
        * Only one of these should print at a time.
        */
        P(testsem);
        kprintf("Thread %2lu: ", num);
        for (i=0; i<NSEMLOOPS; i++) {
            kprintf("%c", (int)num+64);
        }
        kprintf("\n");
        V(donesem);
    }
    ```

5. **Ending test**
At the end, semaphore is reset to the initial state, ready for another test.
    ```C
    /* so we can run it again */
	V(testsem);
	V(testsem);

	kprintf("Semaphore test done.\n");
	return 0;
    ```

## Testing locks
`sy2` performs a synchronization test by means of locks. However locks are not implemented in the basic version of *os161*. We are asked to implement locks in two different ways:
1. **using semaphores**: a lock is indeed equivalent to a binary semaphore (counter with maximum value 1). We need to change/complete `struct lock` in `kern/include/synch.h`, and complete functions `lock_create()`, `lock_destroy()`, `lock_acquire()`, `lock_release()`, `lock_do_i_hold()` in `kern/thread/synch.c`.
2. **using directly `wchan` and `spinlock`**. This is certainly the preferred version, for which it is advisable to refer, adapting it appropriately, to the implementation of semaphores.
> **WARNING**: a lock is not just a binary semaphore. Unlike the semaphore, the lock has the concept of "ownership", i.e. `lock_release()` can only be called by the thread that obtained the lock through `lock_acquire()` and currently owns it. This implies that the `struct lock` must have a pointer to the thread owner: remember that the `curthread` symbol is available (it is not a global variable but a `#define` made in `current.h`, usable as a global variable). The attempt to `lock_release()` by a non-owner thread can (optionally) be treated as a fatal error (`KASSERT` or `panic`) or simply be ignored, not releasing the lock. `KASSERT` is the recommended choice because, since it is a kernel thread calling it, an incorrect `lock_release()` attempt is a real error in the kernel. Also note that the `lock_do_i_hold()` function must be implemented. The function should return to the calling program whether or not the current thread is the owner of a lock received as a parameter. In all probability, it is a matter of reading a pointer stored in the struct lock: reading should therefore be done with this pointer in mutual exclusion (or at least it is necessary to consider whether multiple access from multiple threads is critical or not). Attention, if a spinlock is used to implement `lock_do_i_hold()` as a critical section, to the fact that *os161* does not allow a thread to wait on `wchan_sleep()` while owning more that one spinlock (i.e. the only spinlock owned must be the one to be released and later acquired again): so beware when calling `spinlock_acquire()` on multiple spinlocks before calling `wchan_sleep()`.


First thing first, we need to define some state variable which will indicates the kernel which implementation exploit:
```C
/*
 *      LAB03 (locks and conditional variables implementations)
 */
#include "opt-lab03.h" 

/**
 * @brief indicates whether locks has been implemented using binary
 *        semaphores (1) or using wchan and spinlocks (0)
 * 
 */
#define IMPLEMENTED_WITH_BINARY_SEMAPHORES 1 
```

Therefore, the lock structure will be implemented in the following way:
```C
/*
 * Simple lock for mutual exclusion.
 *
 * When the lock is created, no thread should be holding it. Likewise,
 * when the lock is destroyed, no thread should be holding it.
 *
 * The name field is for easier debugging. A copy of the name is
 * (should be) made internally.
 */
struct lock {
    char *lk_name;
    HANGMAN_LOCKABLE(lk_hangman);   /* Deadlock detector hook. */
                                    /* Do not care for now... It deals with debugger options */
        
    /**
     * NB: a simple implementation would be to add a semaphore with value 1. However, with this
     * solution, there is no management for lock's ownership. Indeed, anyone can release the semaphore
     * even if it is not the one (process/thread) that acquires the lock (i.e. the semaphore).
     * 
     * In order to provide ownerships it is necessary to add some more structure, such as (maybe)
     * a pointer to the thread who got the lock.
     */

#if OPT_LAB03
#if IMPLEMENTED_WITH_BINARY_SEMAPHORES
        struct semaphore *lk_sem;
#else
        struct wchan *lk_wchan;
#endif
        struct spinlock lk_lock;
        volatile struct thread *lk_owner;     /* pointer to the thread who owns the lock */
#endif
};
```

Again, let's call the `sy2` procedure and follow the flow. First, from the menu we will reach the `locktest()` function (*path*: `os161/src/kern/test/synchtest.c`). This function performs the following operation in order to test the locks:

### Lock creation
1.  **Initialization of the locks**
    At the very beginning, the function will call `inititems()`, the same function as before, which will take care of the creations of all the structures used to run all the tests (in this case, we are interested in the part which will actually create the locks).
    ```C
    if (testlock==NULL) {
		testlock = lock_create("testlock");
		if (testlock == NULL) {
			panic("synchtest: lock_create failed\n");
		}
	}
    ```
    Two things to take into accounts:
    1. `testlock` is the global lock variable (`static struct lock *testlock`) that we have just defined in `synch.h` (*path*: `os161/src/kern/include/synch.h`).
    2. `lock_create()` initializes the locks. It exploits the global variable previously defined to implement one of the two solutions: 
        ```C
        struct lock *lock_create(const char *name) {
            struct lock *lock;

            lock = kmalloc(sizeof(*lock));          /* structure init */
            if (lock == NULL) {
                return NULL;
            }

            lock->lk_name = kstrdup(name);          /* name assignement */
            if (lock->lk_name == NULL) {
                kfree(lock);
                return NULL;
            }

            HANGMAN_LOCKABLEINIT(&lock->lk_hangman, lock->lk_name);

        #if OPT_LAB03
        #if IMPLEMENTED_WITH_BINARY_SEMAPHORES

            /* creating the new binary semaphore and checking */
            lock->lk_sem = sem_create(lock->lk_name, 1);
            if (lock->lk_sem == NULL) {
        #else
            /* creating the new waiting channel and checking */
            lock->lk_wchan = wchan_create(lock->lk_name);
            if (lock->lk_wchan == NULL) {
        #endif
                /* an error occured while creating the semaphore/wchan */
                    kfree(lock->lk_name);
                    kfree(lock);
                    return NULL;
            }

            /* initialize owner and spinlock */
            lock->lk_owner = NULL; /* at the beginning, no owner owns this lock */
            spinlock_init(&lock->lk_lock);

        #endif
            return lock;
        }
        ```
    Before proceeding, let's implement the `lock_destroy()`, which will be useful later on. This function simply free all the memory related to the lock based on the implementation we have chosen. It exploits `spinlock_cleanup()` to clear the spinlock, `sem_destroy()` to kill the semaphore and `wchan_destroy()` to kill the waiting channel. Nothing more.
    ```C
    void lock_destroy(struct lock *lock){
        KASSERT(lock != NULL);

    #if OPT_LAB03
        spinlock_cleanup(&lock->lk_lock);
    #if IMPLEMENTED_WITH_BINARY_SEMAPHORES
        sem_destroy(lock->lk_sem);
    #else 
        wchan_destroy(lock->lk_wchan);
    #endif
    #endif
        kfree(lock->lk_name);
        kfree(lock);
    }
    ``` 

2. **Creating the threads to run the test**
Threads are generated with the `locktestthread` routine, which receives as argument an incremental number (based on the thread) from $0$ to $63$: 
    ```C
    for (i = 0; i < NTHREADS; i++) {
		result = thread_fork("synchtest", NULL, locktestthread, NULL, i);
		if (result) {
			panic("locktest: thread_fork failed: %s\n", strerror(result));
		}
	}
    ```
    Meanwhile, the main thread will call a `P()` to wait for the tests to finish (exploiting another semaphore, just like the `semtest`): 
    ```C
    for (i = 0; i < NTHREADS; i++) {
		P(donesem);
	}
    ```

3. **Thread routine** 
Each thread executes a series of tests based only on the initial number passed as argument to the routine. This value is used to defined the values of three global variables (`testval1`, `testval2` and `testval3`) which cannot be accessed concurrently (or the tests will fail). Therefore, each test suite is wrapped inside the functions `lock_acquire()` and `lock_release()`, which need to be implemented.
```C
static void locktestthread(void *junk, unsigned long num) {
	int i;
	(void)junk;

	for (i = 0; i < NLOCKLOOPS; i++) {
		lock_acquire(testlock);
		testval1 = num;
		testval2 = num*num;
		testval3 = num%3;

		if (testval2 != testval1*testval1) {
			fail(num, "testval2/testval1");
		}

		if (testval2%3 != (testval3*testval3)%3) {
			fail(num, "testval2/testval3");
		}

		if (testval3 != testval1%3) {
			fail(num, "testval3/testval1");
		}

		if (testval1 != num) {
			fail(num, "testval1/num");
		}

		if (testval2 != num*num) {
			fail(num, "testval2/num");
		}

		if (testval3 != num%3) {
			fail(num, "testval3/num");
		}

		lock_release(testlock);
	}
	V(donesem);
}
```

### Lock acquire
4. **Implementation of lock acquire**
At the very beginning of the thread routine, the thread **must** acquire the lock in order to pass the tests, otherwise another thread may modify the global variables used and lead to a tests failure. In order to do so, we need to implement the lock acquire function. The function has to:
    - check whether the current thread is not the **owner of the lock**. It should not be the owner if it asks for acquiring the lock...
    - check whether **interrupts** have been disabled for the current thread.
    - sleep/wait in `wchan`/semaphore
    - acquire the spinlock.
    - acquire the ownership of the lock.
    - release the spinlock.

    Now, the current thread is the **owner of the lock**, nice.
    The function is implemented as follows: 
```C
/**
 * NOTES: 
 * lock_acquire() works also during the bootstrap process. It is therfore important not to
 * mess it up.
 */

void
lock_acquire(struct lock *lock)
{
	/* Call this (atomically) before waiting for a lock */
	//HANGMAN_WAIT(&curthread->t_hangman, &lock->lk_hangman);

#if OPT_LAB03
    /* asserting lock to exist */
    KASSERT(lock != NULL);

    /* assert the current thread not to be the owner of the lock already */
    KASSERT(lock_do_i_hold(lock) == false);

    /* assert the current thread to have interrupts disabled */
    KASSERT(curthread->t_in_interrupt == false);

#if IMPLEMENTED_WITH_BINARY_SEMAPHORES
    /* acquire the lock */

    /*
    *  G.Cabodi - 2019: P BEFORE(!!!) spinlock acquire. OS161 forbids sleeping/realeasing
    *  the CPU while owning a spinlocks: this could be a cause of deadlock. 
    *  THE spinlock passed to wchan_wait is the only one allowed. 
    *  This is checked in various parts of the code (see for instance wchan_sleep.
    *  as P may result in "wait", it cannot be called while owning the spinlock.
    */
    P(lock->lk_sem);
    spinlock_acquire(&lock->lk_lock);

#else
    /* acquiring spinlock and sleep if the lock is owned by someone else */
    spinlock_acquire(&lock->lk_lock); 
    while (lock->lk_owner != NULL) {
	    wchan_sleep(lock->lk_wchan, &lock->lk_lock);
    }

#endif
    /* get lock ownership */
    KASSERT(lock->lk_owner == NULL);
    lock->lk_owner = curthread;

    /* releasing the spinlock */
	spinlock_release(&lock->lk_lock);

#endif
    (void)lock;  // suppress warning until code gets written

	/* Call this (atomically) once the lock is acquired */
	//HANGMAN_ACQUIRE(&curthread->t_hangman, &lock->lk_hangman);
}
```
The `lock_acquire()` makes use of another utility function called `lock_do_i_hold()`. This function check whether the current thread is the owner of the lock passed as argument. It's crucial that we assert in `lock_acquire()` that the thread who is trying to acquire the lock is not the owner of the same lock. Therefore, the `lock_do_i_hold()` is defined to check it and return a boolean value. 
> This could possibly work without spinlock for mutual exclusion, which could simplify the semaphore-based solution, by removing the spinlock. Whenever the current thread owns the lock, the test is safe without guaranteeing mutual exclusion. If not the owner, a wrong verdict could happen (very low chance!) by wrongly reading a `pointer == curthread`. However, using the spinlock is good practice for shared data.

```C
bool lock_do_i_hold(struct lock *lock) {

#if OPT_LAB03
    bool result;

    /* check whether the current thread is the owner of the lock */
    spinlock_acquire(&lock->lk_lock);
	result = (lock->lk_owner == curthread);
	spinlock_release(&lock->lk_lock);

#endif
    (void)lock;  // suppress warning until code gets written
    return result; // dummy until code gets written
}
```

### Lock release
Once we have completed the operations in the critical section, we must release the lock to allow other threads to enter the critical section. To do so, we implement the `lock_release()` function such that: 
- check whether the current thread is the **owner of the lock**. It should be the owner if it asks for releasing the lock...
- acquire the spinlock.
- clear the ownership of the lock.
- wakeup/signal the `wchan`/semaphore
- release the spinlock.

The function is defined as follows:
```C
void lock_release(struct lock *lock) {
    /* Call this (atomically) when the lock is released */
//HANGMAN_RELEASE(&curthread->t_hangman, &lock->lk_hangman);

#if OPT_LAB03
    /* asserting lock to exist */
    KASSERT(lock != NULL);

    /* assert the current thread not to be the owner of the lock already */
    KASSERT(lock_do_i_hold(lock) == true);

    /* acquire the lock and clear the ownership */
    spinlock_acquire(&lock->lk_lock);
    lock->lk_owner = NULL;
        
    /*  G.Cabodi - 2019: no problem here owning a spinlock, as V/wchan_wakeone do not lead to wait state */

#if IMPLEMENTED_WITH_BINARY_SEMAPHORES
    V(lock->lk_sem);
#else
    wchan_wakeone(lock->lk_wchan, &lock->lk_lock);
#endif
    /* release the spinlock */
    spinlock_release(&lock->lk_lock);
#endif
    (void)lock;  // suppress warning until code gets written
}
``` 