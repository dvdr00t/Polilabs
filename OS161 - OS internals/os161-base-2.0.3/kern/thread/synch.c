/*
 * Copyright (c) 2000, 2001, 2002, 2003, 2004, 2005, 2008, 2009
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
 * Synchronization primitives.
 * The specifications of the functions are in synch.h.
 */

#include <types.h>
#include <lib.h>
#include <spinlock.h>
#include <wchan.h>
#include <thread.h>
#include <current.h>
#include <synch.h>

////////////////////////////////////////////////////////////
//
// Semaphore.

struct semaphore *
sem_create(const char *name, unsigned initial_count)
{
        struct semaphore *sem;

        sem = kmalloc(sizeof(*sem));
        if (sem == NULL) {
                return NULL;
        }

        sem->sem_name = kstrdup(name);
        if (sem->sem_name == NULL) {
                kfree(sem);
                return NULL;
        }

	sem->sem_wchan = wchan_create(sem->sem_name);
	if (sem->sem_wchan == NULL) {
		kfree(sem->sem_name);
		kfree(sem);
		return NULL;
	}

	spinlock_init(&sem->sem_lock);
        sem->sem_count = initial_count;

        return sem;
}

void
sem_destroy(struct semaphore *sem)
{
        KASSERT(sem != NULL);

	/* wchan_cleanup will assert if anyone's waiting on it */
	spinlock_cleanup(&sem->sem_lock);
	wchan_destroy(sem->sem_wchan);
        kfree(sem->sem_name);
        kfree(sem);
}

void
P(struct semaphore *sem)
{
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

void
V(struct semaphore *sem)
{
        KASSERT(sem != NULL);

	spinlock_acquire(&sem->sem_lock);

        sem->sem_count++;
        KASSERT(sem->sem_count > 0);
	wchan_wakeone(sem->sem_wchan, &sem->sem_lock);

	spinlock_release(&sem->sem_lock);
}

////////////////////////////////////////////////////////////
//
// Lock.

struct lock *
lock_create(const char *name)
{
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
        lock->lk_owner = NULL;
        spinlock_init(&lock->lk_lock);

#endif
        return lock;
}

void
lock_destroy(struct lock *lock)
{
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

void
lock_release(struct lock *lock)
{
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

bool
lock_do_i_hold(struct lock *lock)
{

#if OPT_LAB03
        bool result;

        /* check whether the current thread is the owner of the lock */
        spinlock_acquire(&lock->lk_lock);
	result = (lock->lk_owner == curthread);
	spinlock_release(&lock->lk_lock);
        return result;

#endif
        (void)lock;  // suppress warning until code gets written
        return true; // dummy until code gets written
}

////////////////////////////////////////////////////////////
//
// CV


struct cv *
cv_create(const char *name)
{
        struct cv *cv;

        cv = kmalloc(sizeof(*cv));              /* CV initialization            */
        if (cv == NULL) {
                return NULL;
        }

        cv->cv_name = kstrdup(name);            /* CV name initialization       */
        if (cv->cv_name==NULL) {
                kfree(cv);
                return NULL;
        }

#if OPT_LAB03
        /* initialization of the waiting channel */
        cv->cv_wchan = wchan_create(cv->cv_name);
        if (cv->cv_wchan == NULL) {
                kfree(cv->cv_name);
                kfree(cv);
                return NULL;
        }

        /* iniitalization of the spinlock */
        spinlock_init(&cv->cv_lock);

#endif

        return cv;
}

void
cv_destroy(struct cv *cv)
{
        KASSERT(cv != NULL);

#if OPT_LAB03
	spinlock_cleanup(&cv->cv_lock);
	wchan_destroy(cv->cv_wchan);
#endif

        kfree(cv->cv_name);
        kfree(cv);
}

void
cv_wait(struct cv *cv, struct lock *lock)
{
#if OPT_LAB03
        /* assert lock and cv to exist */
        KASSERT(cv != NULL);
        KASSERT(lock != NULL);

        /* assert the current thread to be the owner of the lock */
        KASSERT(lock_do_i_hold(lock) == true);

        /* acquiring the lock and get to sleep */
        spinlock_acquire(&cv->cv_lock);
	/* 
                G.Cabodi - 2019: spinlock already owned as atomic lock_release+wchan_sleep needed 
        */
	lock_release(lock);
	wchan_sleep(cv->cv_wchan,&cv->cv_lock);
	spinlock_release(&cv->cv_lock);
	/* 
                G.Cabodi - 2019: spinlock already  released to avoid ownership while (possibly) going 
                to wait state in lock_acquire. Atomicity wakeup+lock_acquire not guaranteed (but not 
                necessary!) 
        */
	lock_acquire(lock);
#endif
        (void)cv;    // suppress warning until code gets written
        (void)lock;  // suppress warning until code gets written
}

void
cv_signal(struct cv *cv, struct lock *lock)
{
#if OPT_LAB03
        /* assert lock and cv to exist */
        KASSERT(cv != NULL);
        KASSERT(lock != NULL);

        /* assert the current thread to be the owner of the lock */
        KASSERT(lock_do_i_hold(lock) == true);
	/* 
                G.Cabodi - 2019: here the spinlock is NOT required, as no atomic operation 
                has to be done. The spinlock is just acquired because needed by wakeone 
        */

        /* acquiring the lock and wake the first thread up */
	spinlock_acquire(&cv->cv_lock);
	wchan_wakeone(cv->cv_wchan,&cv->cv_lock);
	spinlock_release(&cv->cv_lock);
#endif
	(void)cv;    // suppress warning until code gets written
	(void)lock;  // suppress warning until code gets written
}

void
cv_broadcast(struct cv *cv, struct lock *lock)
{
#if OPT_LAB03
        /* assert lock and cv to exist */
        KASSERT(cv != NULL);
        KASSERT(lock != NULL);

        /* assert the current thread to be the owner of the lock */
        KASSERT(lock_do_i_hold(lock) == true);
	/* 
                G.Cabodi - 2019: here the spinlock is NOT required, as no atomic operation 
                has to be done. The spinlock is just acquired because needed by wakeone 
        */

        /* acquiring the lock and wake all threads up */
	spinlock_acquire(&cv->cv_lock);
	wchan_wakeall(cv->cv_wchan,&cv->cv_lock);
	spinlock_release(&cv->cv_lock);
#endif
	(void)cv;    // suppress warning until code gets written
	(void)lock;  // suppress warning until code gets written
}
