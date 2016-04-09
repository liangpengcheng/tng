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

/* See http://nikhilm.github.com/uvbook/ for an introduction. */

#ifndef UV_H
#define UV_H
#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
  /* Windows - set up dll import/export decorators. */
# if defined(BUILDING_UV_SHARED)
    /* Building shared library. */
#   define UV_EXTERN __declspec(dllexport)
# elif defined(USING_UV_SHARED)
    /* Using shared library. */
#   define UV_EXTERN __declspec(dllimport)
# else
    /* Building static library. */
#   define UV_EXTERN /* nothing */
# endif
#elif __GNUC__ >= 4
# define UV_EXTERN __attribute__((visibility("default")))
#else
# define UV_EXTERN /* nothing */
#endif


#define UV_VERSION_MAJOR 0
#define UV_VERSION_MINOR 10


#if defined(_MSC_VER) && _MSC_VER < 1600
# include "uv-private/stdint-msvc2008.h"
#else
# include <stdint.h>
#endif

#include <sys/types.h> /* size_t */

#if defined(__SVR4) && !defined(__unix__)
# define __unix__
#endif

#if defined(__unix__) || defined(__POSIX__) || \
    defined(__APPLE__) || defined(_AIX)
# include "uv-private/uv-unix.h"
#else
# include "uv-private/uv-win.h"
#endif


typedef int uv_err_t;


typedef struct uv_cpu_info_s uv_cpu_info_t;
typedef struct uv_interface_address_s uv_interface_address_t;




struct uv_cpu_info_s {
  char* model;
  int speed;
  struct uv_cpu_times_s {
    uint64_t user;
    uint64_t nice;
    uint64_t sys;
    uint64_t idle;
    uint64_t irq;
  } cpu_times;
};



/*
 * This allocates cpu_infos array, and sets count.  The array
 * is freed using uv_free_cpu_info().
 */
UV_EXTERN uv_err_t uv_cpu_info(uv_cpu_info_t** cpu_infos, int* count);
UV_EXTERN void uv_free_cpu_info(uv_cpu_info_t* cpu_infos, int count);



/* Gets the executable path */
UV_EXTERN int uv_exepath(char* buffer, size_t* size);

/* Gets the current working directory */
UV_EXTERN uv_err_t uv_cwd(char* buffer, size_t size);

/* Changes the current working directory */
UV_EXTERN uv_err_t uv_chdir(const char* dir);

/* Gets memory info in bytes */
UV_EXTERN uint64_t uv_get_free_memory(void);
UV_EXTERN uint64_t uv_get_total_memory(void);





/*
 * The mutex functions return 0 on success, -1 on error
 * (unless the return type is void, of course).
 */
UV_EXTERN int uv_mutex_init(uv_mutex_t* handle);
UV_EXTERN void uv_mutex_destroy(uv_mutex_t* handle);
UV_EXTERN void uv_mutex_lock(uv_mutex_t* handle);
UV_EXTERN int uv_mutex_trylock(uv_mutex_t* handle);
UV_EXTERN void uv_mutex_unlock(uv_mutex_t* handle);

/*
 * Same goes for the read/write lock functions.
 */
UV_EXTERN int uv_rwlock_init(uv_rwlock_t* rwlock);
UV_EXTERN void uv_rwlock_destroy(uv_rwlock_t* rwlock);
UV_EXTERN void uv_rwlock_rdlock(uv_rwlock_t* rwlock);
UV_EXTERN int uv_rwlock_tryrdlock(uv_rwlock_t* rwlock);
UV_EXTERN void uv_rwlock_rdunlock(uv_rwlock_t* rwlock);
UV_EXTERN void uv_rwlock_wrlock(uv_rwlock_t* rwlock);
UV_EXTERN int uv_rwlock_trywrlock(uv_rwlock_t* rwlock);
UV_EXTERN void uv_rwlock_wrunlock(uv_rwlock_t* rwlock);

/*
 * Same goes for the semaphore functions.
 */
UV_EXTERN int uv_sem_init(uv_sem_t* sem, unsigned int value);
UV_EXTERN void uv_sem_destroy(uv_sem_t* sem);
UV_EXTERN void uv_sem_post(uv_sem_t* sem);
UV_EXTERN void uv_sem_wait(uv_sem_t* sem);
UV_EXTERN int uv_sem_trywait(uv_sem_t* sem);

/*
 * Same goes for the condition variable functions.
 */
UV_EXTERN int uv_cond_init(uv_cond_t* cond);
UV_EXTERN void uv_cond_destroy(uv_cond_t* cond);
UV_EXTERN void uv_cond_signal(uv_cond_t* cond);
UV_EXTERN void uv_cond_broadcast(uv_cond_t* cond);
/* Waits on a condition variable without a timeout.
 *
 * Note:
 * 1. callers should be prepared to deal with spurious wakeups.
 */
UV_EXTERN void uv_cond_wait(uv_cond_t* cond, uv_mutex_t* mutex);
// /* Waits on a condition variable with a timeout in nano seconds.
//  * Returns 0 for success or -1 on timeout, * aborts when other errors happen.
//  *
//  * Note:
//  * 1. callers should be prepared to deal with spurious wakeups.
//  * 2. the granularity of timeout on Windows is never less than one millisecond.
//  * 3. uv_cond_timedwait takes a relative timeout, not an absolute time.
//  */
// UV_EXTERN int uv_cond_timedwait(uv_cond_t* cond, uv_mutex_t* mutex,
//     uint64_t timeout);

// UV_EXTERN int uv_barrier_init(uv_barrier_t* barrier, unsigned int count);
// UV_EXTERN void uv_barrier_destroy(uv_barrier_t* barrier);
// UV_EXTERN void uv_barrier_wait(uv_barrier_t* barrier);

UV_EXTERN int uv_thread_create(uv_thread_t *tid,
    void (*entry)(void *arg), void *arg);
UV_EXTERN unsigned long uv_thread_self(void);
UV_EXTERN int uv_thread_join(uv_thread_t *tid);
#ifdef __cplusplus
}
#endif

#endif /* UV_H */
