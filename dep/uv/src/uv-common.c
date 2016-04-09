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

#include "uv.h"
#include "uv-common.h"

#include <stdio.h>
#include <assert.h>
#include <stddef.h> /* NULL */
#include <stdlib.h> /* malloc */
#include <string.h> /* memset */


#ifdef _WIN32
static UINT __stdcall uv__thread_start(void *ctx_v)
#else
static void *uv__thread_start(void *ctx_v)
#endif
{
  void (*entry)(void *arg);
  void *arg;

  struct {
    void (*entry)(void *arg);
    void *arg;
  } *ctx;

  ctx = ctx_v;
  arg = ctx->arg;
  entry = ctx->entry;
  free(ctx);
  entry(arg);

  return 0;
}


int uv_thread_create(uv_thread_t *tid, void (*entry)(void *arg), void *arg) {
  struct {
    void (*entry)(void *arg);
    void *arg;
  } *ctx;

  if ((ctx = malloc(sizeof *ctx)) == NULL)
    return -1;

  ctx->entry = entry;
  ctx->arg = arg;

#ifdef _WIN32
  *tid = (HANDLE) _beginthreadex(NULL, 0, uv__thread_start, ctx, 0, NULL);
  if (*tid == 0) {
#else
  if (pthread_create(tid, NULL, uv__thread_start, ctx)) {
#endif
    free(ctx);
    return -1;
  }

  return 0;
}