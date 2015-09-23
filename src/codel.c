/*
 * Copyright (c) 2009-2013,2015 LAAS/CNRS
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
 *                                           Anthony Mallet on Wed Dec 30 2009
 */
#include "acgenom.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <err.h>

#include "genom.h"


/* --- local data ---------------------------------------------------------- */

struct codel_s {
  tloc loc;
  const char *name;
  codelkind kind;

  hash_s params;	/**< parameter list */
  hash_s yields;	/**< transitions */
  hash_s triggers;	/**< start conditions */

  task_s task;		/**< codel's task */
  service_s service;	/**< codel's service */

  struct {
    int start:1, stop:1, ether:1;
  } connected;		/**< reached by start or stop or yields to ether */
};

tloc		codel_loc(codel_s c) { assert(c); return c->loc; }
const char *	codel_name(codel_s c) { assert(c); return c->name; }
codelkind	codel_kind(codel_s c) { assert(c); return c->kind; }
hash_s		codel_params(codel_s c) { assert(c); return c->params; }
hash_s		codel_triggers(codel_s c) { assert(c); return c->triggers; }
hash_s		codel_yields(codel_s c) { assert(c); return c->yields; }
task_s *	codel_task(codel_s c) { assert(c); return &c->task; }
service_s *	codel_service(codel_s c) { assert(c); return &c->service; }

void	codel_setkind(codel_s c, codelkind k) { assert(c); c->kind = k; }

static codel_s	codel_find(const char *name);
static void	connectivity(hash_s fsm, comp_s comp, codel_s node, int start);


/* --- codel_create -------------------------------------------------------- */

/** create new codel
 */
codel_s
codel_create(tloc l, const char *name, codelkind kind, hash_s triggers,
             hash_s yields, hash_s params)
{
  codel_s c;
  assert(name && params);

  /* prevent duplicates */
  c = codel_find(name);
  if (c) {
    if (!param_list_equal(codel_params(c), params)) {
      parserror(l, "conflicting types for codel %s", name);
      parsenoerror(codel_loc(c), " codel %s declared here", name);
      return NULL;
    }
  }

  /* create data structure */
  c = malloc(sizeof(*c));
  if (!c) {
    warnx("memory exhausted, cannot create codel '%s'", name);
    return NULL;
  }
  c->loc = l;
  c->name = string(name);
  c->kind = kind;

  c->params = params;
  c->yields = yields ? yields : hash_create("yields list", 0);
  c->triggers = triggers ? triggers : hash_create("triggers list", 0);

  c->task = NULL;
  c->service = NULL;

  c->connected.start = c->connected.stop = c->connected.ether = 0;

  xwarnx("created codel %s", c->name);
  return c;
}


/* --- codel_clone --------------------------------------------------------- */

/** clone a codel
 */
codel_s
codel_clone(codel_s codel)
{
  codel_s c;
  param_s p;
  hiter i;
  assert(codel);

  c = malloc(sizeof(*c));
  if (!c) {
    warnx("memory exhausted, cannot create codel '%s'", codel_name(codel));
    return NULL;
  }
  c->loc = codel_loc(codel);
  c->name = codel_name(codel);
  c->kind = codel_kind(codel);

  c->params = hash_create("parameter list", 0);
  if (!c->params) { free(c); return NULL; }
  for(hash_first(codel_params(codel), &i); i.current; hash_next(&i)) {
    p = param_clone(i.value);
    if (!p) { hash_destroy(c->params, 1); free(c); return NULL; }

    if (hash_insert(c->params, param_name(p), p, (hrelease_f)param_destroy)) {
      hash_destroy(c->params, 1); free(c); return NULL;
    }
  }

  c->yields = hash_create("yields list", 0);
  for(hash_first(codel_yields(codel), &i); i.current; hash_next(&i)) {
    if (hash_insert(c->yields, i.key, i.value, NULL)) {
      hash_destroy(c->yields, 1); hash_destroy(c->params, 1); free(c);
      return NULL;
    }
  }

  c->triggers = hash_create("triggers list", 0);
  for(hash_first(codel_triggers(codel), &i); i.current; hash_next(&i)) {
    if (hash_insert(c->triggers, i.key, i.value, NULL)) {
      hash_destroy(c->triggers, 1); hash_destroy(c->yields, 1);
      hash_destroy(c->params, 1); free(c); return NULL;
    }
  }

  if (codel->task)
    c->task = comp_task(comp_active(), task_name(codel->task));
  else
    c->task = NULL;
  if (codel->service)
    c->service = comp_service(comp_active(), service_name(codel->service));
  else
    c->service = NULL;

  xwarnx("created codel %s", c->name);
  return c;
}


/* --- codel_find ---------------------------------------------------------- */

static codel_s
codel_find(const char *name)
{
  comp_s c;
  codel_s codel;
  hiter i, j;

  for(c = comp_first(); c; c = comp_next(c)) {
    /* look for tasks codels */
    for(hash_first(comp_tasks(c), &i); i.value; hash_next(&i)) {
      for(hash_first(task_props(i.value), &j); j.value; hash_next(&j)) {
        if (prop_kind(j.value) != PROP_FSM_CODEL &&
            prop_kind(j.value) != PROP_SIMPLE_CODEL &&
            prop_kind(j.value) != PROP_VALIDATE)
          continue;
        codel = prop_codel(j.value);
        if (!strcmp(name, codel_name(codel))) return codel;
      }
    }

    /* look for service codels */
    for(hash_first(comp_services(c), &i); i.value; hash_next(&i)) {
      for(hash_first(service_props(i.value), &j); j.value; hash_next(&j)) {
        if (prop_kind(j.value) != PROP_FSM_CODEL &&
            prop_kind(j.value) != PROP_SIMPLE_CODEL &&
            prop_kind(j.value) != PROP_VALIDATE)
          continue;
        codel = prop_codel(j.value);
        if (!strcmp(name, codel_name(codel))) return codel;
      }
    }
  }

  return NULL;
}


/* --- codel_fsmcreate ----------------------------------------------------- */

/** Build a fsm hash from existing codels in a list of properties
 */
hash_s
codel_fsmcreate(tloc l, comp_s comp, hash_s props)
{
  hash_s fsm;
  const char *e;
  codel_s c, u;
  idltype_s ev;
  hiter i, t;
  int s;

  /* build fsm */
  fsm = hash_create("fsm", 1);
  if (!fsm) { parserror(l, "not enough memory"); return NULL; }

  for(hash_first(props, &i); i.current; hash_next(&i))
    switch(prop_kind(i.value)) {
      case PROP_FSM_CODEL:
        c = prop_codel(i.value);

        for(hash_first(codel_triggers(c), &t); t.current; hash_next(&t)) {
          assert(type_kind(t.value) == IDL_EVENT);
          e = type_fullname(t.value);
          s = hash_insert(fsm, e, c, NULL);
          switch(s) {
            case 0: break;

            case EEXIST:
              /* only one codel per event is allowed */
              parserror(codel_loc(c), "duplicate codel<%s>", e);
              u = hash_find(fsm, e);
              if (u) parsenoerror(codel_loc(u), " codel<%s> declared here", e);
              break;

            default:
              hash_destroy(fsm, 1);
              return NULL;
          }
        }
        break;

      default: break;
    }

  /* check fsm connectivity if not empty and in a regular component */
  if (hash_first(fsm, &i) || comp_kind(comp) != COMP_REGULAR) return fsm;

  ev = scope_findtype(comp_scope(comp), "start");
  assert(ev);
  c = hash_find(fsm, type_fullname(ev));
  if (!c) {
    parserror(l, "undefined codel<%s>", type_fullname(ev));
    hash_destroy(fsm, 1);
    return NULL;
  }
  connectivity(fsm, comp, c, 1);

  ev = scope_findtype(comp_scope(comp), "stop");
  assert(ev);
  c = hash_find(fsm, type_fullname(ev));
  if (c) {
    connectivity(fsm, comp, c, 0);
    if (!c->connected.ether) {
      parserror(codel_loc(c), "codel<%s> must eventually yield to ether",
                type_fullname(ev));
      hash_destroy(fsm, 1);
      return NULL;
    }
  }

  for(hash_first(fsm, &i); i.current; hash_next(&i)) {
    c = i.value;
    if (!c->connected.start && !c->connected.stop)
      parserror(codel_loc(c), "codel<%s> not reached", i.key);
  }

  return fsm;
}

static void
connectivity(hash_s fsm, comp_s comp, codel_s node, int start)
{
  idltype_s stop, ether;
  const char *e;
  codel_s n;
  hiter y;
  int r;

  stop = scope_findtype(comp_scope(comp), "stop");
  ether = scope_findtype(comp_scope(comp), "ether");
  assert(stop && ether);

  if (start) node->connected.start = 1; else node->connected.stop = 1;
  for(hash_first(codel_yields(node), &y); y.current; hash_next(&y)) {
    assert(type_kind(y.value) == IDL_EVENT);
    e = type_fullname(y.value);

    n = hash_find(fsm, e);
    if (!n) {
      if (!strcmp(e, type_fullname(ether))) {
        node->connected.ether = 1; continue;
      }

      parserror(codel_loc(node), "undefined codel<%s>", e);
      continue;
    }

    r = start ? n->connected.start : n->connected.stop;
    if (start) n->connected.start = 1; else n->connected.stop = 1;

    /* successor has not yet been visited; recurse on it */
    if (!r) connectivity(fsm, comp, n, start);

    if (n->connected.ether) node->connected.ether = 1;
  }
}


/* --- codel_strkind ------------------------------------------------------- */

/** Return a codel kind as a string
 */
const char *
codel_strkind(codelkind k)
{
  switch(k) {
    case CODEL_SYNC:		return "sync";
    case CODEL_ASYNC:		return "async";
  }

  assert(0);
  return NULL;
}
