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
'>
#ifndef H_GENOM_PROF
#define H_GENOM_PROF

#include <time.h>

#include "genom3/c/event.h"

/* profiling enabled */
extern int	genom_prof_enable;

/* profiling event */
struct prof_event {
  const char *instance, *task, *service, *codel;

  struct timespec tenter, tstart, tleave;
  genom_event from, to;
};

/* API */
#ifdef __cplusplus
extern "C" {
#endif
  int		genom_prof_init(const char *filename, int autolog);
  void		genom_prof_fini(void);
  void		genom_prof_record(struct prof_event *event);
#ifdef __cplusplus
}
#endif

#define genom_prof_decl(event)        \
  struct prof_event event

static inline void
genom_prof_enter(struct prof_event *event)
{
  if (__builtin_expect(!genom_prof_enable, 1)) return;

  clock_gettime(CLOCK_REALTIME, &event->tenter);
  event->tleave = event->tstart = event->tenter;
}

static inline void
genom_prof_start(struct prof_event *event)
{
  if (__builtin_expect(!genom_prof_enable, 1)) return;

  clock_gettime(CLOCK_REALTIME, &event->tstart);
  event->tleave = event->tstart;
}

static inline void
genom_prof_leave(struct prof_event *event)
{
  if (__builtin_expect(!genom_prof_enable, 1)) return;

  clock_gettime(CLOCK_REALTIME, &event->tleave);
}

static inline void
genom_prof_collect(struct prof_event *event, const char *instance,
                   const char *task, const char *service,
                   const char *codel, genom_event from, genom_event to)
{
  if (__builtin_expect(!genom_prof_enable, 1)) return;

  event->instance = instance;
  event->task = task;
  event->service = service;
  event->codel = codel;
  event->from = from;
  event->to = to;

  genom_prof_record(event);
}

#endif /* H_GENOM_PROF */
