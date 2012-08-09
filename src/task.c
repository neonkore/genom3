/*
 * Copyright (c) 2012 LAAS/CNRS
 * All rights reserved.
 *
 * Redistribution  and  use  in  source  and binary  forms,  with  or  without
 * modification, are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of  source  code must retain the  above copyright
 *      notice and this list of conditions.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice and  this list of  conditions in the  documentation and/or
 *      other materials provided with the distribution.
 *
 * THE SOFTWARE  IS PROVIDED "AS IS"  AND THE AUTHOR  DISCLAIMS ALL WARRANTIES
 * WITH  REGARD   TO  THIS  SOFTWARE  INCLUDING  ALL   IMPLIED  WARRANTIES  OF
 * MERCHANTABILITY AND  FITNESS.  IN NO EVENT  SHALL THE AUTHOR  BE LIABLE FOR
 * ANY  SPECIAL, DIRECT,  INDIRECT, OR  CONSEQUENTIAL DAMAGES  OR  ANY DAMAGES
 * WHATSOEVER  RESULTING FROM  LOSS OF  USE, DATA  OR PROFITS,  WHETHER  IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR  OTHER TORTIOUS ACTION, ARISING OUT OF OR
 * IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 *                                           Anthony Mallet on Sat Aug 11 2012
 */
#include "acgenom.h"

#include <assert.h>
#include <err.h>
#include <errno.h>
#include <stdlib.h>

#include "genom.h"


/* --- local data ---------------------------------------------------------- */

struct task_s {
  tloc loc;
  const char *name;
  comp_s component;

  hash_s props;
  hash_s fsm;
};

tloc		task_loc(task_s t) { assert(t); return t->loc; }
const char *	task_name(task_s t) { assert(t); return t->name; }
comp_s		task_comp(task_s t) { assert(t); return t->component; }
hash_s		task_props(task_s t) { assert(t); return t->props; }
hash_s		task_fsm(task_s t) { assert(t); return t->fsm; }




/* --- task_create --------------------------------------------------------- */

/** create a task in component
 */
task_s
task_create(tloc l, const char *name, hash_s props)
{
  comp_s comp = comp_active();
  hiter i;
  task_s t;
  codel_s c;
  prop_s p;
  int e;
  assert(comp && name);

  /* create empty property list if none has been defined */
  if (!props) {
    props = hash_create("property list", 0);
    if (!props) return NULL;
  }
  e = 0;

  /* check unwanted properties */
  for(hash_first(props, &i); i.current; hash_next(&i))
    switch(prop_kind(i.value)) {
      case PROP_DOC: case PROP_PERIOD: case PROP_PRIORITY:
      case PROP_SCHEDULING: case PROP_STACK: case PROP_FSM_CODEL:
        break;

      case PROP_THROWS:
        /* register internal events */
        if (comp_addievs(l, prop_hash(i.value), 1)) e = errno;
        break;

      case PROP_DELAY:
        p = hash_find(props, prop_strkind(PROP_PERIOD));
        if (!p) {
          parserror(prop_loc(p), "delay without period is not allowed");
          e = 1; break;
        }
        if (type_constvalue(prop_value(i.value)).f >=
            type_constvalue(prop_value(p)).f) {
          parserror(prop_loc(i.value), "delay must be less than period");
          parsenoerror(prop_loc(p), " period declared here");
          e = 1; break;
        }
        break;

      case PROP_SIMPLE_CODEL:
        parserror(l, "simple codel not allowed in task %s", name);
        parsenoerror(prop_loc(i.value), " %s declared here",
                     prop_name(i.value));
        e = 1; break;

      case PROP_IDS: case PROP_VERSION: case PROP_LANG: case PROP_EMAIL:
      case PROP_REQUIRE: case PROP_CODELS_REQUIRE: case PROP_CLOCKRATE:
      case PROP_TASK: case PROP_VALIDATE: case PROP_INTERRUPTS:
      case PROP_BEFORE: case PROP_AFTER: case PROP_EXTENDS: case PROP_PROVIDES:
      case PROP_USES:
        parserror(prop_loc(i.value), "property %s is not suitable for tasks",
                  prop_strkind(prop_kind(i.value)));
        e = 1; break;
    }
  if (e) return NULL;


  /* create */
  t = malloc(sizeof(*t));
  if (!t) {
    warnx("memory exhausted, cannot create task");
    return NULL;
  }

  t->loc = l;
  t->name = string(name);
  t->component = comp;
  t->props = props;

  /* build task's fsm */
  t->fsm = codel_fsmcreate(l, t->props);
  if (!t->fsm) {
    free(t);
    return NULL;
  }

  /* set codel's parent task and (NULL) service */
  for(hash_first(props, &i); i.current; hash_next(&i))
    if (prop_kind(i.value) == PROP_FSM_CODEL) {
      c = prop_codel(i.value);
      *codel_task(c) = t;
      *codel_service(c) = NULL;
    }

  /* register */
  e = hash_insert(comp_tasks(comp), t->name, t, (hrelease_f)task_destroy);
  switch(e) {
    case 0: break;

    case EEXIST:
      parserror(t->loc, "duplicate task %s", t->name);
      task_s u = comp_task(comp, name);
      if (u) parsenoerror(u->loc, " task %s declared here", u->name);
      /*FALLTHROUGH*/
    default:
      free(t);
      return NULL;
  }

  xwarnx("created task %s in %s %s",
         task_name(t), comp_strkind(comp_kind(comp)), comp_name(comp));
  return t;
}


/* --- task_clone ---------------------------------------------------------- */

/** clone a task
 */
task_s
task_clone(task_s task)
{
  task_s t;
  hash_s prop;

  /* clone properties */
  prop = hash_create("property list", 0);
  if (!prop || prop_merge_list(prop, task->props, 0/*ignore_dup*/))
    return NULL;

  /* create */
  t = task_create(task->loc, task->name, prop);
  if (!t) hash_destroy(prop, 1);

  return t;
}


/* --- task_destroy -------------------------------------------------------- */

/** destroy task
 */
void
task_destroy(task_s t)
{
  if (t) {
    hash_destroy(t->props, 1);
    free(t);
  }
}
