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

#ifndef UV_UNIX_INTERNAL_H_
#define UV_UNIX_INTERNAL_H_

#include "uv-common.h"

#include <assert.h>
#include <stdlib.h> /* abort */

#if defined(__STRICT_ANSI__)
# define inline __inline
#endif

#if defined(__linux__)
# include "linux-syscalls.h"
#endif /* __linux__ */

#if defined(__sun)
# include <sys/port.h>
# include <port.h>
# define futimes(fd, tv) futimesat(fd, (void*)0, tv)
#endif /* __sun */

#if defined(__APPLE__) && !TARGET_OS_IPHONE
# include <CoreServices/CoreServices.h>
#endif

#define STATIC_ASSERT(expr)                                                   \
  void uv__static_assert(int static_assert_failed[1 - 2 * !(expr)])

#define ACCESS_ONCE(type, var)                                                \
  (*(volatile type*) &(var))

#define UNREACHABLE()                                                         \
  do {                                                                        \
    assert(0 && "unreachable code");                                          \
    abort();                                                                  \
  }                                                                           \
  while (0)

#define SAVE_ERRNO(block)                                                     \
  do {                                                                        \
    int _saved_errno = errno;                                                 \
    do { block; } while (0);                                                  \
    errno = _saved_errno;                                                     \
  }                                                                           \
  while (0)

#ifdef HAVE_DTRACE
#include "uv-dtrace.h"
#else
#define UV_TICK_START(arg0, arg1)
#define UV_TICK_STOP(arg0, arg1)
#endif

#endif /* UV_UNIX_INTERNAL_H_ */
