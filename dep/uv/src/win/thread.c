/* Copyright Joyent, Inc. and other Node contributors. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <assert.h>
#include <limits.h>

#include "uv.h"
#include "internal.h"


#define HAVE_SRWLOCK_API() (pTryAcquireSRWLockShared != NULL)
#define HAVE_CONDVAR_API() (pInitializeConditionVariable != NULL)

#ifdef _MSC_VER /* msvc */
# define inline __inline
# define NOINLINE __declspec (noinline)
#else  /* gcc */
# define inline inline
# define NOINLINE __attribute__ ((noinline))
#endif



inline static int uv__rwlock_srwlock_init(uv_rwlock_t* rwlock);
inline static void uv__rwlock_srwlock_destroy(uv_rwlock_t* rwlock);
inline static void uv__rwlock_srwlock_rdlock(uv_rwlock_t* rwlock);
inline static int uv__rwlock_srwlock_tryrdlock(uv_rwlock_t* rwlock);
inline static void uv__rwlock_srwlock_rdunlock(uv_rwlock_t* rwlock);
inline static void uv__rwlock_srwlock_wrlock(uv_rwlock_t* rwlock);
inline static int uv__rwlock_srwlock_trywrlock(uv_rwlock_t* rwlock);
inline static void uv__rwlock_srwlock_wrunlock(uv_rwlock_t* rwlock);

inline static int uv__rwlock_fallback_init(uv_rwlock_t* rwlock);
inline static void uv__rwlock_fallback_destroy(uv_rwlock_t* rwlock);
inline static void uv__rwlock_fallback_rdlock(uv_rwlock_t* rwlock);
inline static int uv__rwlock_fallback_tryrdlock(uv_rwlock_t* rwlock);
inline static void uv__rwlock_fallback_rdunlock(uv_rwlock_t* rwlock);
inline static void uv__rwlock_fallback_wrlock(uv_rwlock_t* rwlock);
inline static int uv__rwlock_fallback_trywrlock(uv_rwlock_t* rwlock);
inline static void uv__rwlock_fallback_wrunlock(uv_rwlock_t* rwlock);


inline static int uv_cond_fallback_init(uv_cond_t* cond);
inline static void uv_cond_fallback_destroy(uv_cond_t* cond);
inline static void uv_cond_fallback_signal(uv_cond_t* cond);
inline static void uv_cond_fallback_broadcast(uv_cond_t* cond);
inline static void uv_cond_fallback_wait(uv_cond_t* cond, uv_mutex_t* mutex);
inline static int uv_cond_fallback_timedwait(uv_cond_t* cond,
    uv_mutex_t* mutex, uint64_t timeout);

inline static int uv_cond_condvar_init(uv_cond_t* cond);
inline static void uv_cond_condvar_destroy(uv_cond_t* cond);
inline static void uv_cond_condvar_signal(uv_cond_t* cond);
inline static void uv_cond_condvar_broadcast(uv_cond_t* cond);
inline static void uv_cond_condvar_wait(uv_cond_t* cond, uv_mutex_t* mutex);
inline static int uv_cond_condvar_timedwait(uv_cond_t* cond,
    uv_mutex_t* mutex, uint64_t timeout);



int uv_thread_join(uv_thread_t *tid) {
  if (WaitForSingleObject(*tid, INFINITE))
    return -1;
  else {
    CloseHandle(*tid);
    *tid = 0;
    return 0;
  }
}


int uv_mutex_init(uv_mutex_t* mutex) {
  InitializeCriticalSection(mutex);
  return 0;
}


void uv_mutex_destroy(uv_mutex_t* mutex) {
  DeleteCriticalSection(mutex);
}


void uv_mutex_lock(uv_mutex_t* mutex) {
  EnterCriticalSection(mutex);
}


int uv_mutex_trylock(uv_mutex_t* mutex) {
  if (TryEnterCriticalSection(mutex))
    return 0;
  else
    return -1;
}


void uv_mutex_unlock(uv_mutex_t* mutex) {
  LeaveCriticalSection(mutex);
}

int uv_rwlock_init(uv_rwlock_t* rwlock) {

  if (HAVE_SRWLOCK_API())
    return uv__rwlock_srwlock_init(rwlock);
  else
    return uv__rwlock_fallback_init(rwlock);
}


void uv_rwlock_destroy(uv_rwlock_t* rwlock) {
  if (HAVE_SRWLOCK_API())
    uv__rwlock_srwlock_destroy(rwlock);
  else
    uv__rwlock_fallback_destroy(rwlock);
}


void uv_rwlock_rdlock(uv_rwlock_t* rwlock) {
  if (HAVE_SRWLOCK_API())
    uv__rwlock_srwlock_rdlock(rwlock);
  else
    uv__rwlock_fallback_rdlock(rwlock);
}


int uv_rwlock_tryrdlock(uv_rwlock_t* rwlock) {
  if (HAVE_SRWLOCK_API())
    return uv__rwlock_srwlock_tryrdlock(rwlock);
  else
    return uv__rwlock_fallback_tryrdlock(rwlock);
}


void uv_rwlock_rdunlock(uv_rwlock_t* rwlock) {
  if (HAVE_SRWLOCK_API())
    uv__rwlock_srwlock_rdunlock(rwlock);
  else
    uv__rwlock_fallback_rdunlock(rwlock);
}


void uv_rwlock_wrlock(uv_rwlock_t* rwlock) {
  if (HAVE_SRWLOCK_API())
    uv__rwlock_srwlock_wrlock(rwlock);
  else
    uv__rwlock_fallback_wrlock(rwlock);
}


int uv_rwlock_trywrlock(uv_rwlock_t* rwlock) {
  if (HAVE_SRWLOCK_API())
    return uv__rwlock_srwlock_trywrlock(rwlock);
  else
    return uv__rwlock_fallback_trywrlock(rwlock);
}


void uv_rwlock_wrunlock(uv_rwlock_t* rwlock) {
  if (HAVE_SRWLOCK_API())
    uv__rwlock_srwlock_wrunlock(rwlock);
  else
    uv__rwlock_fallback_wrunlock(rwlock);
}


int uv_sem_init(uv_sem_t* sem, unsigned int value) {
  *sem = CreateSemaphore(NULL, value, INT_MAX, NULL);
  return *sem ? 0 : -1;
}


void uv_sem_destroy(uv_sem_t* sem) {
  if (!CloseHandle(*sem))
    abort();
}


void uv_sem_post(uv_sem_t* sem) {
  if (!ReleaseSemaphore(*sem, 1, NULL))
    abort();
}


void uv_sem_wait(uv_sem_t* sem) {
  if (WaitForSingleObject(*sem, INFINITE) != WAIT_OBJECT_0)
    abort();
}


int uv_sem_trywait(uv_sem_t* sem) {
  DWORD r = WaitForSingleObject(*sem, 0);

  if (r == WAIT_OBJECT_0)
    return 0;

  if (r == WAIT_TIMEOUT)
    return -1;

  abort();
  return -1; /* Satisfy the compiler. */
}


inline static int uv__rwlock_srwlock_init(uv_rwlock_t* rwlock) {
  pInitializeSRWLock(&rwlock->srwlock_);
  return 0;
}


inline static void uv__rwlock_srwlock_destroy(uv_rwlock_t* rwlock) {
  (void) rwlock;
}


inline static void uv__rwlock_srwlock_rdlock(uv_rwlock_t* rwlock) {
  pAcquireSRWLockShared(&rwlock->srwlock_);
}


inline static int uv__rwlock_srwlock_tryrdlock(uv_rwlock_t* rwlock) {
  if (pTryAcquireSRWLockShared(&rwlock->srwlock_))
    return 0;
  else
    return -1;
}


inline static void uv__rwlock_srwlock_rdunlock(uv_rwlock_t* rwlock) {
  pReleaseSRWLockShared(&rwlock->srwlock_);
}


inline static void uv__rwlock_srwlock_wrlock(uv_rwlock_t* rwlock) {
  pAcquireSRWLockExclusive(&rwlock->srwlock_);
}


inline static int uv__rwlock_srwlock_trywrlock(uv_rwlock_t* rwlock) {
  if (pTryAcquireSRWLockExclusive(&rwlock->srwlock_))
    return 0;
  else
    return -1;
}


inline static void uv__rwlock_srwlock_wrunlock(uv_rwlock_t* rwlock) {
  pReleaseSRWLockExclusive(&rwlock->srwlock_);
}


inline static int uv__rwlock_fallback_init(uv_rwlock_t* rwlock) {
  if (uv_mutex_init(&rwlock->fallback_.read_mutex_))
    return -1;

  if (uv_mutex_init(&rwlock->fallback_.write_mutex_)) {
    uv_mutex_destroy(&rwlock->fallback_.read_mutex_);
    return -1;
  }

  rwlock->fallback_.num_readers_ = 0;

  return 0;
}


inline static void uv__rwlock_fallback_destroy(uv_rwlock_t* rwlock) {
  uv_mutex_destroy(&rwlock->fallback_.read_mutex_);
  uv_mutex_destroy(&rwlock->fallback_.write_mutex_);
}


inline static void uv__rwlock_fallback_rdlock(uv_rwlock_t* rwlock) {
  uv_mutex_lock(&rwlock->fallback_.read_mutex_);

  if (++rwlock->fallback_.num_readers_ == 1)
    uv_mutex_lock(&rwlock->fallback_.write_mutex_);

  uv_mutex_unlock(&rwlock->fallback_.read_mutex_);
}


inline static int uv__rwlock_fallback_tryrdlock(uv_rwlock_t* rwlock) {
  int ret;

  ret = -1;

  if (uv_mutex_trylock(&rwlock->fallback_.read_mutex_))
    goto out;

  if (rwlock->fallback_.num_readers_ == 0)
    ret = uv_mutex_trylock(&rwlock->fallback_.write_mutex_);
  else
    ret = 0;

  if (ret == 0)
    rwlock->fallback_.num_readers_++;

  uv_mutex_unlock(&rwlock->fallback_.read_mutex_);

out:
  return ret;
}


inline static void uv__rwlock_fallback_rdunlock(uv_rwlock_t* rwlock) {
  uv_mutex_lock(&rwlock->fallback_.read_mutex_);

  if (--rwlock->fallback_.num_readers_ == 0)
    uv_mutex_unlock(&rwlock->fallback_.write_mutex_);

  uv_mutex_unlock(&rwlock->fallback_.read_mutex_);
}


inline static void uv__rwlock_fallback_wrlock(uv_rwlock_t* rwlock) {
  uv_mutex_lock(&rwlock->fallback_.write_mutex_);
}


inline static int uv__rwlock_fallback_trywrlock(uv_rwlock_t* rwlock) {
  return uv_mutex_trylock(&rwlock->fallback_.write_mutex_);
}


inline static void uv__rwlock_fallback_wrunlock(uv_rwlock_t* rwlock) {
  uv_mutex_unlock(&rwlock->fallback_.write_mutex_);
}



/* This condition variable implementation is based on the SetEvent solution
 * (section 3.2) at http://www.cs.wustl.edu/~schmidt/win32-cv-1.html
 * We could not use the SignalObjectAndWait solution (section 3.4) because
 * it want the 2nd argument (type uv_mutex_t) of uv_cond_wait() and
 * uv_cond_timedwait() to be HANDLEs, but we use CRITICAL_SECTIONs.
 */

inline static int uv_cond_fallback_init(uv_cond_t* cond) {
  /* Initialize the count to 0. */
  cond->fallback.waiters_count = 0;

  InitializeCriticalSection(&cond->fallback.waiters_count_lock);

  /* Create an auto-reset event. */
  cond->fallback.signal_event = CreateEvent(NULL,  /* no security */
                                            FALSE, /* auto-reset event */
                                            FALSE, /* non-signaled initially */
                                            NULL); /* unnamed */
  if (!cond->fallback.signal_event)
    goto error2;

  /* Create a manual-reset event. */
  cond->fallback.broadcast_event = CreateEvent(NULL,  /* no security */
                                               TRUE,  /* manual-reset */
                                               FALSE, /* non-signaled */
                                               NULL); /* unnamed */
  if (!cond->fallback.broadcast_event)
    goto error;

  return 0;

error:
  CloseHandle(cond->fallback.signal_event);
error2:
  DeleteCriticalSection(&cond->fallback.waiters_count_lock);
  return -1;
}


inline static int uv_cond_condvar_init(uv_cond_t* cond) {
  pInitializeConditionVariable(&cond->cond_var);
  return 0;
}


int uv_cond_init(uv_cond_t* cond) {
  if (HAVE_CONDVAR_API())
    return uv_cond_condvar_init(cond);
  else
    return uv_cond_fallback_init(cond);
}


inline static void uv_cond_fallback_destroy(uv_cond_t* cond) {
  if (!CloseHandle(cond->fallback.broadcast_event))
    abort();
  if (!CloseHandle(cond->fallback.signal_event))
    abort();
  DeleteCriticalSection(&cond->fallback.waiters_count_lock);
}


inline static void uv_cond_condvar_destroy(uv_cond_t* cond) {
  /* nothing to do */
}


void uv_cond_destroy(uv_cond_t* cond) {
  if (HAVE_CONDVAR_API())
    uv_cond_condvar_destroy(cond);
  else
    uv_cond_fallback_destroy(cond);
}


inline static void uv_cond_fallback_signal(uv_cond_t* cond) {
  int have_waiters;

  /* Avoid race conditions. */
  EnterCriticalSection(&cond->fallback.waiters_count_lock);
  have_waiters = cond->fallback.waiters_count > 0;
  LeaveCriticalSection(&cond->fallback.waiters_count_lock);

  if (have_waiters)
    SetEvent(cond->fallback.signal_event);
}


inline static void uv_cond_condvar_signal(uv_cond_t* cond) {
  pWakeConditionVariable(&cond->cond_var);
}


void uv_cond_signal(uv_cond_t* cond) {
  if (HAVE_CONDVAR_API())
    uv_cond_condvar_signal(cond);
  else
    uv_cond_fallback_signal(cond);
}


inline static void uv_cond_fallback_broadcast(uv_cond_t* cond) {
  int have_waiters;

  /* Avoid race conditions. */
  EnterCriticalSection(&cond->fallback.waiters_count_lock);
  have_waiters = cond->fallback.waiters_count > 0;
  LeaveCriticalSection(&cond->fallback.waiters_count_lock);

  if (have_waiters)
    SetEvent(cond->fallback.broadcast_event);
}


inline static void uv_cond_condvar_broadcast(uv_cond_t* cond) {
  pWakeAllConditionVariable(&cond->cond_var);
}


void uv_cond_broadcast(uv_cond_t* cond) {
  if (HAVE_CONDVAR_API())
    uv_cond_condvar_broadcast(cond);
  else
    uv_cond_fallback_broadcast(cond);
}


inline int uv_cond_wait_helper(uv_cond_t* cond, uv_mutex_t* mutex,
    DWORD dwMilliseconds) {
  DWORD result;
  int last_waiter;
  HANDLE handles[2] = {
    cond->fallback.signal_event,
    cond->fallback.broadcast_event
  };

  /* Avoid race conditions. */
  EnterCriticalSection(&cond->fallback.waiters_count_lock);
  cond->fallback.waiters_count++;
  LeaveCriticalSection(&cond->fallback.waiters_count_lock);

  /* It's ok to release the <mutex> here since Win32 manual-reset events */
  /* maintain state when used with <SetEvent>. This avoids the "lost wakeup" */
  /* bug. */
  uv_mutex_unlock(mutex);

  /* Wait for either event to become signaled due to <uv_cond_signal> being */
  /* called or <uv_cond_broadcast> being called. */
  result = WaitForMultipleObjects(2, handles, FALSE, dwMilliseconds);

  EnterCriticalSection(&cond->fallback.waiters_count_lock);
  cond->fallback.waiters_count--;
  last_waiter = result == WAIT_OBJECT_0 + 1
      && cond->fallback.waiters_count == 0;
  LeaveCriticalSection(&cond->fallback.waiters_count_lock);

  /* Some thread called <pthread_cond_broadcast>. */
  if (last_waiter) {
    /* We're the last waiter to be notified or to stop waiting, so reset the */
    /* the manual-reset event. */
    ResetEvent(cond->fallback.broadcast_event);
  }

  /* Reacquire the <mutex>. */
  uv_mutex_lock(mutex);

  if (result == WAIT_OBJECT_0 || result == WAIT_OBJECT_0 + 1)
    return 0;

  if (result == WAIT_TIMEOUT)
    return -1;

  abort();
  return -1; /* Satisfy the compiler. */
}


inline static void uv_cond_fallback_wait(uv_cond_t* cond, uv_mutex_t* mutex) {
  if (uv_cond_wait_helper(cond, mutex, INFINITE))
    abort();
}


inline static void uv_cond_condvar_wait(uv_cond_t* cond, uv_mutex_t* mutex) {
  if (!pSleepConditionVariableCS(&cond->cond_var, mutex, INFINITE))
    abort();
}


void uv_cond_wait(uv_cond_t* cond, uv_mutex_t* mutex) {
  if (HAVE_CONDVAR_API())
    uv_cond_condvar_wait(cond, mutex);
  else
    uv_cond_fallback_wait(cond, mutex);
}


// inline static int uv_cond_fallback_timedwait(uv_cond_t* cond,
//     uv_mutex_t* mutex, uint64_t timeout) {
//   return uv_cond_wait_helper(cond, mutex, (DWORD)(timeout / 1e6));
// }
// 
// 
// inline static int uv_cond_condvar_timedwait(uv_cond_t* cond,
//     uv_mutex_t* mutex, uint64_t timeout) {
//   if (pSleepConditionVariableCS(&cond->cond_var, mutex, (DWORD)(timeout / 1e6)))
//     return 0;
//   if (GetLastError() != ERROR_TIMEOUT)
//     abort();
//   return -1;
// }
// 
// 
// int uv_cond_timedwait(uv_cond_t* cond, uv_mutex_t* mutex,
//     uint64_t timeout) {
//   if (HAVE_CONDVAR_API())
//     return uv_cond_condvar_timedwait(cond, mutex, timeout);
//   else
//     return uv_cond_fallback_timedwait(cond, mutex, timeout);
// }


// int uv_barrier_init(uv_barrier_t* barrier, unsigned int count) {
//   barrier->n = count;
//   barrier->count = 0;
// 
//   if (uv_mutex_init(&barrier->mutex))
//     return -1;
// 
//   if (uv_sem_init(&barrier->turnstile1, 0))
//     goto error2;
// 
//   if (uv_sem_init(&barrier->turnstile2, 1))
//     goto error;
// 
//   return 0;
// 
// error:
//   uv_sem_destroy(&barrier->turnstile1);
// error2:
//   uv_mutex_destroy(&barrier->mutex);
//   return -1;
// 
// }
// 
// 
// void uv_barrier_destroy(uv_barrier_t* barrier) {
//   uv_sem_destroy(&barrier->turnstile2);
//   uv_sem_destroy(&barrier->turnstile1);
//   uv_mutex_destroy(&barrier->mutex);
// }
// 
// 
// void uv_barrier_wait(uv_barrier_t* barrier) {
//   uv_mutex_lock(&barrier->mutex);
//   if (++barrier->count == barrier->n) {
//     uv_sem_wait(&barrier->turnstile2);
//     uv_sem_post(&barrier->turnstile1);
//   }
//   uv_mutex_unlock(&barrier->mutex);
// 
//   uv_sem_wait(&barrier->turnstile1);
//   uv_sem_post(&barrier->turnstile1);
// 
//   uv_mutex_lock(&barrier->mutex);
//   if (--barrier->count == 0) {
//     uv_sem_wait(&barrier->turnstile1);
//     uv_sem_post(&barrier->turnstile2);
//   }
//   uv_mutex_unlock(&barrier->mutex);
// 
//   uv_sem_wait(&barrier->turnstile2);
//   uv_sem_post(&barrier->turnstile2);
// }
