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

#ifndef UV_UNIX_H
#define UV_UNIX_H

#include "ngx-queue.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <termios.h>
#include <pwd.h>

#include <semaphore.h>
#include <pthread.h>
#include <signal.h>

#if defined(__linux__)
# include "uv-linux.h"
#elif defined(__sun)
# include "uv-sunos.h"
#elif defined(__APPLE__)
# include "uv-darwin.h"
#elif defined(__DragonFly__)  || \
      defined(__FreeBSD__)    || \
      defined(__OpenBSD__)    || \
      defined(__NetBSD__)
# include "uv-bsd.h"
#endif


#ifndef UV_PLATFORM_SEM_T
# define UV_PLATFORM_SEM_T sem_t
#endif
typedef pthread_once_t uv_once_t;
typedef pthread_t uv_thread_t;
typedef pthread_mutex_t uv_mutex_t;
typedef pthread_rwlock_t uv_rwlock_t;
typedef UV_PLATFORM_SEM_T uv_sem_t;
typedef pthread_cond_t uv_cond_t;


// #if defined(__APPLE__) && defined(__MACH__)
// 
// typedef struct {
//   unsigned int n;
//   unsigned int count;
//   uv_mutex_t mutex;
//   uv_sem_t turnstile1;
//   uv_sem_t turnstile2;
// } uv_barrier_t;
// 
// #else /* defined(__APPLE__) && defined(__MACH__) */
// 
// typedef pthread_barrier_t uv_barrier_t;
// 
// #endif /* defined(__APPLE__) && defined(__MACH__) */
// 
// 
 #endif /* UV_UNIX_H */
