<'
# Copyright (c) 2020 LAAS/CNRS
# All rights reserved.
#
# Redistribution  and  use  in  source  and binary  forms,  with  or  without
# modification, are permitted provided that the following conditions are met:
#
#   1. Redistributions of  source  code must retain the  above copyright
#      notice and this list of conditions.
#   2. Redistributions in binary form must reproduce the above copyright
#      notice and  this list of  conditions in the  documentation and/or
#      other materials provided with the distribution.
#
# THE SOFTWARE  IS PROVIDED "AS IS"  AND THE AUTHOR  DISCLAIMS ALL WARRANTIES
# WITH  REGARD   TO  THIS  SOFTWARE  INCLUDING  ALL   IMPLIED  WARRANTIES  OF
# MERCHANTABILITY AND  FITNESS.  IN NO EVENT  SHALL THE AUTHOR  BE LIABLE FOR
# ANY  SPECIAL, DIRECT,  INDIRECT, OR  CONSEQUENTIAL DAMAGES  OR  ANY DAMAGES
# WHATSOEVER  RESULTING FROM  LOSS OF  USE, DATA  OR PROFITS,  WHETHER  IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR  OTHER TORTIOUS ACTION, ARISING OUT OF OR
# IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
#
#                                           Anthony Mallet on Wed Apr 15 2020
#

if {[llength $argv] != 1} { error "expected arguments: component" }
lassign $argv component

# compute handy shortcuts
set comp [$component name]

lang c
'>
#include "autoconf/acheader.h"

#include <assert.h>
#include <err.h>
#include <errno.h>
#include <pthread.h>
#include <sched.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "genom3/c/exception.h"
#include "<"$comp">_prof.h"

int genom_<"$comp">_prof_enable = 0;

/* --- local data ---------------------------------------------------------- */

#define PROF_EVQ_SIZE	512 /* events */
#if PROF_EVQ_SIZE & (PROF_EVQ_SIZE-1)
# error "PROF_EVQ_SIZE must be a power of two"
#endif

struct prof_item {
  atomic_bool dirty;
  struct prof_event event;
};

static struct {
  const char *instance;
  FILE *fout;
  bool autolog;
  pthread_t writer;

  struct {
    struct prof_item *item;
    atomic_uint head, tail;
  } eventq;
} pcontext;

static inline void	prof_log(const struct timespec *tenter,
                                const struct timespec *tstart,
                                const struct timespec *tleave,
                                const char *task, const char *service,
                                const char *codel, const char *from,
                                const char *to);
static void *		genom_<"$comp">_prof_writer(void *data);


/* <"[--- genom_${comp}_prof_init ---------------------------------------]"> */

int
genom_<"$comp">_prof_init(const char *instance, const char *filename,
                          int autolog)
{
  if (!genom_<"$comp">_prof_enable) return 0;

  /* init context */
  pcontext.instance = instance;
  pcontext.fout = NULL;
  pcontext.autolog = !!autolog;
  pcontext.eventq.item = malloc(PROF_EVQ_SIZE * sizeof(*pcontext.eventq.item));
  if (pcontext.eventq.item == NULL) { warn("malloc"); return 1; }

  atomic_store(&pcontext.eventq.head, 0);
  atomic_store(&pcontext.eventq.tail, 0);
  for(unsigned i = 0; i < PROF_EVQ_SIZE; i++)
    atomic_store(&pcontext.eventq.item[i].dirty, true);

  /* open report */
  char path[512];

  if (snprintf(path, sizeof(path), "%s.out", filename) >= (int)sizeof(path)) {
    errno = ENAMETOOLONG;
    warn("%s", filename);
    goto err;
  }
  pcontext.fout = fopen(path, "w");
  if (pcontext.fout == NULL) { warn("%s", path); goto err; }

  /* writer thread */
  if (pthread_create(
        &pcontext.writer, NULL, genom_<"$comp">_prof_writer, NULL)) {
    warn("profiling writer thread");
    goto err;
  }

  return 0;

err: {
    int e = errno;
    free(pcontext.eventq.item);
    if (pcontext.fout) fclose(pcontext.fout);
    return e;
  }
}


/* <"[--- genom_${comp}_prof_record -------------------------------------]"> */

/* lock-free multiple writer push: multiple threads may increment the head
 * pointer, so it must be incremented before writing an item contents. The
 * reader is synchronized to the 'dirty' flag and will not process
 * an item until it is marked as not dirty. */

void
genom_<"$comp">_prof_record(struct prof_event *event)
{
  /* next free item */
  unsigned int head = atomic_fetch_add(&pcontext.eventq.head, 1);
  struct prof_item *i = &pcontext.eventq.item[head % PROF_EVQ_SIZE];

  /* the queue shall not be full */
  while (__builtin_expect(
           head >= atomic_load(&pcontext.eventq.tail) + PROF_EVQ_SIZE,
           0))
    sched_yield();

  /* must be dirty by construction */
  assert(__builtin_expect(atomic_load(&i->dirty), true));

  /* store to the queue and mark as clean */
  i->event = *event;
  atomic_store(&i->dirty, false);
}


/* <"[--- genom_${comp}_prof_fini ---------------------------------------]"> */

void
genom_<"$comp">_prof_fini(void)
{
  struct timespec tenter, tleave;

  if (!genom_<"$comp">_prof_enable) return;

  /* disable profiling */
  genom_<"$comp">_prof_enable = 0;

  /* wait for writer */
  pthread_cancel(pcontext.writer);
  pthread_join(pcontext.writer, NULL);

  /* flush events queue */
  if (pcontext.autolog) clock_gettime(CLOCK_REALTIME, &tenter);
  while (atomic_load(&pcontext.eventq.tail)
         != atomic_load(&pcontext.eventq.head)) {
    const struct prof_item *i = &pcontext.eventq.item[
      atomic_fetch_add(&pcontext.eventq.tail, 1) % PROF_EVQ_SIZE];

    assert(__builtin_expect(!atomic_load(&i->dirty), true));
    prof_log(&i->event.tenter, &i->event.tstart, &i->event.tleave,
             i->event.task, i->event.service, i->event.codel,
             i->event.from, i->event.to);
  }

  /* leave */
  if (pcontext.autolog) {
    clock_gettime(CLOCK_REALTIME, &tleave);
    prof_log(&tenter, &tenter, &tleave,
             "profiling", "writer", "genom_<"$comp">_prof_writer",
             "wakeup", "done");
  }
  fclose(pcontext.fout);
  free(pcontext.eventq.item);
}


/* --- prof_log ------------------------------------------------------------ */

static inline void
prof_log(const struct timespec *tenter,
         const struct timespec *tstart,
         const struct timespec *tleave,
         const char *task, const char *service, const char *codel,
         const char *from, const char *to)
{
  fprintf(pcontext.fout,
          "%lld.%.9ld %lld.%.9ld %lld.%.9ld %s %s %s %s %s %s\n",
          (long long)tenter->tv_sec, tenter->tv_nsec,
          (long long)tstart->tv_sec, tstart->tv_nsec,
          (long long)tleave->tv_sec, tleave->tv_nsec,
          pcontext.instance, task, service, codel,
          from ? from : "::genom::ok",
          to ? to : "::genom::ok");
}


/* <"[--- genom_${comp}_prof_writer -------------------------------------]"> */

void *
genom_<"$comp">_prof_writer(void *unused)
{
  struct timespec tsleep = { .tv_sec = 0, .tv_nsec = 10000000 /* 10ms */ };
  unsigned int tail = atomic_load(&pcontext.eventq.tail);
  struct timespec tenter, tleave;
  unsigned int qlen;

  if (pcontext.autolog) clock_gettime(CLOCK_REALTIME, &tenter);

  /* loop until pthread_cancel(3) */
  pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &(int){0});
  pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &(int){0});
  while(1) {
    struct prof_item *i = &pcontext.eventq.item[tail % PROF_EVQ_SIZE];

    /* if there is no item */
    if (tail == atomic_load(&pcontext.eventq.head)) {

      /* flush previous batch */
      fflush(pcontext.fout);

      /* log profiling event related to profiling */
      if (pcontext.autolog) {
        clock_gettime(CLOCK_REALTIME, &tleave);
        prof_log(&tenter, &tenter, &tleave,
                 "profiling", "writer", "genom_<"$comp">_prof_writer",
                 "wakeup", "sleep");
      }

      /* sleep until ready or canceled */
      qlen = 0;
      while (tail == atomic_load(&pcontext.eventq.head)) {
        struct timespec s = tsleep, r;

        pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, unused);
        while(nanosleep(&s, &r) < 0 && errno == EINTR)
          s = r;
        pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, unused);

        /* increase sleep time up to 500ms if the queue is <10% empty */
        qlen = atomic_load(&pcontext.eventq.head) - tail;
        if (qlen < PROF_EVQ_SIZE/10) {
          tsleep.tv_nsec += tsleep.tv_nsec/5;
          if (tsleep.tv_nsec > 500000000) tsleep.tv_nsec = 500000000;
        }
      }

      if (pcontext.autolog) clock_gettime(CLOCK_REALTIME, &tenter);

      /* decrease sleep time down to 1ms if the queue was filled >50% */
      if (qlen > PROF_EVQ_SIZE/2) {
        tsleep.tv_nsec -= tsleep.tv_nsec/5;
        if (tsleep.tv_nsec < 1000000) tsleep.tv_nsec = 1000000;
      }
    }

    /* wait for the dirty flag to clear */
    while (__builtin_expect(atomic_load(&i->dirty), false))
      sched_yield();

    /* log data */
    prof_log(&i->event.tenter, &i->event.tstart, &i->event.tleave,
             i->event.task, i->event.service, i->event.codel,
             i->event.from, i->event.to);

    /* mark as processed */
    atomic_store(&i->dirty, true);
    atomic_store(&pcontext.eventq.tail, ++tail);
  }

  /*NOTREACHED*/
  return NULL;
}
