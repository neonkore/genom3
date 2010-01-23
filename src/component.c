/*
 * Copyright (c) 2009-2010 LAAS/CNRS
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
 *                                           Anthony Mallet on Tue Dec 29 2009
 */
#include "acgenom.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <err.h>

#include "genom.h"


/* --- local data ---------------------------------------------------------- */

struct comp_s {
  tloc loc;
  const char *name;

  hash_s props;
  hash_s tasks;
  hash_s ports;
  hash_s services;
};

tloc		comp_loc(comp_s c) { assert(c); return c->loc; }
const char *	comp_name(comp_s c) { assert(c); return c->name; }
hash_s		comp_props(comp_s c) { assert(c); return c->props; }
hash_s		comp_tasks(comp_s c) { assert(c); return c->tasks; }
hash_s		comp_ports(comp_s c) { assert(c); return c->ports; }
hash_s		comp_services(comp_s c) { assert(c); return c->services; }

struct task_s {
  tloc loc;
  const char *name;

  hash_s props;
};

tloc		task_loc(task_s t) { assert(t); return t->loc; }
const char *	task_name(task_s t) { assert(t); return t->name; }
hash_s		task_props(task_s t) { assert(t); return t->props; }

struct port_s {
  tloc loc;
  portkind kind;
  const char *name;
  idltype_s type;
};

tloc		port_loc(port_s p) { assert(p); return p->loc; }
const char *	port_name(port_s p) { assert(p); return p->name; }
portkind	port_kind(port_s p) { assert(p); return p->kind; }
idltype_s	port_type(port_s p) { assert(p); return p->type; }


struct service_s {
  tloc loc;
  const char *name;

  hash_s props;
  hash_s params;
};

tloc		service_loc(service_s s) { assert(s); return s->loc; }
const char *	service_name(service_s s) { assert(s); return s->name; }
hash_s		service_props(service_s s) { assert(s); return s->props; }
hash_s		service_params(service_s s) { assert(s); return s->params; }


/** the component of a dotgen file */
static comp_s dotgen;

comp_s	comp_dotgen() { return dotgen; }


/* --- comp_create --------------------------------------------------------- */

/** create a component
 */
comp_s
comp_create(tloc l, const char *name, hash_s props)
{
  hiter i;
  int e, k;
  assert(name && props);

  if (dotgen) {
    parserror(l, "only one component declaration is allowed");
    parsenoerror(dotgen->loc, " component %s declared here", dotgen->name);
    return NULL;
  }
  e = 0;

  /* check mandatory properties */
  static const propkind m[] = {
    PROP_IDS, PROP_VERSION, PROP_LANG
  };
  for(k = 0; k<sizeof(m)/sizeof(m[0]); k++)
    if (!hash_find(props, prop_strkind(m[k]))) {
      parserror(l, "missing %s definition for component %s",
		prop_strkind(m[k]), name);
      e = 1;
    }

  /* check unwanted properties */
  for(hash_first(props, &i); i.current; hash_next(&i))
    switch(prop_kind(i.value)) {
      case PROP_DOC: case PROP_IDS: case PROP_VERSION: case PROP_LANG:
      case PROP_EMAIL: case PROP_REQUIRE: case PROP_BUILD_REQUIRE:
	break;

      case PROP_PERIOD: case PROP_DELAY: case PROP_PRIORITY: case PROP_STACK:
      case PROP_CODEL: case PROP_THROWS: case PROP_TASK: case PROP_INTERRUPTS:
      case PROP_BEFORE: case PROP_AFTER:
	parserror(prop_loc(i.value),
		  "property %s is not suitable for components",
		  prop_strkind(prop_kind(i.value)));
	e = 1; break;
    }
  if (e) return NULL;

  /* create */
  dotgen = malloc(sizeof(*dotgen));
  if (!dotgen) {
    warnx("memory exhausted, cannot create component");
    return NULL;
  }

  dotgen->loc = l;
  dotgen->name = string(name);
  dotgen->props = props;
  dotgen->tasks = hash_create(strings(name, " tasks", NULL), 2);
  if (!dotgen->tasks) {
    free(dotgen);
    return dotgen = NULL;
  }
  dotgen->ports = hash_create(strings(name, " ports", NULL), 2);
  if (!dotgen->ports) {
    hash_destroy(dotgen->tasks);
    free(dotgen);
    return dotgen = NULL;
  }
  dotgen->services = hash_create(strings(name, " services", NULL), 2);
  if (!dotgen->services) {
    hash_destroy(dotgen->tasks);
    hash_destroy(dotgen->ports);
    free(dotgen);
    return dotgen = NULL;
  }

  xwarnx("created component %s", dotgen->name);
  return dotgen;
}


/* --- comp_ids ------------------------------------------------------------ */

/** return component's ids
 */
idltype_s
comp_ids(comp_s c)
{
  prop_s p;
  assert(c);
  p = hash_find(c->props, prop_strkind(PROP_IDS));
  return p ? prop_type(p) : NULL;
}


/* --- comp_port ----------------------------------------------------------- */

/** return component's port
 */
port_s
comp_port(comp_s c, const char *name)
{
  assert(c && name);
  return hash_find(c->ports, name);
}


/* --- comp_task ----------------------------------------------------------- */

/** return component's task
 */
task_s
comp_task(comp_s c, const char *name)
{
  assert(c && name);
  return hash_find(c->tasks, name);
}


/* --- comp_addtask -------------------------------------------------------- */

/** create a task in component
 */
task_s
comp_addtask(tloc l, const char *name, hash_s props)
{
  hiter i;
  task_s t;
  prop_s p, d;
  int e;
  assert(name && props);

  /* control task is a reserved name */
  if (!strcmp(name, CNTRL_TASK_NAME)) {
    parserror(l, "'%s' is a reserved task name", name);
    return NULL;
  }

  /* a component must exist */
  if (!dotgen) {
    parserror(l, "missing component declaration before task %s", name);
    return NULL;
  }
  e = 0;

  /* check consitency of some properties */
  p = hash_find(props, prop_strkind(PROP_PERIOD));
  if (p && (
	const_convert(prop_value(p), CST_FLOAT) ||
	prop_value(p)->f < 0.)) {
    parserror(prop_loc(p),
	      "invalid numeric value for %s", prop_strkind(PROP_PERIOD));
    e = 1;
  }

  d = hash_find(props, prop_strkind(PROP_DELAY));
  if (d && (
	const_convert(prop_value(d), CST_FLOAT) ||
	prop_value(d)->f < 0.)) {
    parserror(prop_loc(d),
	      "invalid numeric value for %s", prop_strkind(PROP_DELAY));
    e = 1;
  }

  if (!e && d && p) {
    if (prop_value(d)->f >= prop_value(p)->f) {
      parserror(prop_loc(d), "delay must be less than period");
      parsenoerror(prop_loc(p), " period declared here");
      e = 1;
    }
  }

  /* check unwanted properties */
  for(hash_first(props, &i); i.current; hash_next(&i))
    switch(prop_kind(i.value)) {
      case PROP_DOC: case PROP_PERIOD: case PROP_DELAY: case PROP_PRIORITY:
      case PROP_STACK: case PROP_CODEL: case PROP_THROWS:
	break;

      case PROP_IDS: case PROP_VERSION: case PROP_LANG: case PROP_EMAIL:
      case PROP_REQUIRE: case PROP_BUILD_REQUIRE: case PROP_TASK:
      case PROP_INTERRUPTS: case PROP_BEFORE: case PROP_AFTER:
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
  t->props = props;

  e = hash_insert(dotgen->tasks, t->name, t, (hrelease_f)task_destroy);
  switch(e) {
    case 0: break;

    case EEXIST:
      parserror(l, "duplicate task %s", t->name);
      task_s u = hash_find(dotgen->tasks, name);
      if (u) parserror(u->loc, " task %s declared here", u->name);
      /*FALLTHROUGH*/
    default:
      free(t);
      return NULL;
  }

  xwarnx("created task %s", t->name);
  return t;
}


/* --- comp_addport -------------------------------------------------------- */

/** create a port in component
 */
port_s
comp_addport(tloc l, portkind k, const char *name, idltype_s t)
{
  port_s p;
  int e;
  assert(name);

  /* a component must exist */
  if (!dotgen) {
    parserror(l, "missing component declaration before port %s", name);
    return NULL;
  }

  /* create */
  p = malloc(sizeof(*p));
  if (!p) {
    warnx("memory exhausted, cannot create port");
    return NULL;
  }

  p->loc = l;
  p->kind = k;
  p->name = string(name);
  p->type = t;

  e = hash_insert(dotgen->ports, p->name, p, (hrelease_f)port_destroy);
  switch(e) {
    case 0: break;

    case EEXIST:
      parserror(l, "duplicate port %s", p->name);
      port_s u = hash_find(dotgen->ports, name);
      if (u) parserror(u->loc, " port %s declared here", u->name);
      /*FALLTHROUGH*/
    default:
      free(p);
      return NULL;
  }

  xwarnx("created port %s", p->name);
  return p;
}


/* --- comp_addservice ----------------------------------------------------- */

/** create a service in component
 */
service_s
comp_addservice(tloc l, const char *name, hash_s params, hash_s props)
{
  hiter i;
  service_s s;
  int e, k;
  assert(name && params && props);

  /* all is a reserved name */
  if (!strcmp(name, ALL_SERVICE_NAME)) {
    parserror(l, "'%s' is a reserved service name", name);
    return NULL;
  }

  /* a component must exist */
  if (!dotgen) {
    parserror(l, "missing component declaration before service %s", name);
    return NULL;
  }
  e = 0;

  /* check mandatory properties */
  static const propkind m[] = {
    PROP_TASK
  };
  for(k = 0; k<sizeof(m)/sizeof(m[0]); k++)
    if (!hash_find(props, prop_strkind(m[k]))) {
      parserror(l, "missing %s definition for service %s",
		prop_strkind(m[k]), name);
      e = 1;
    }

  /* check unwanted properties */
  for(hash_first(props, &i); i.current; hash_next(&i))
    switch(prop_kind(i.value)) {
      case PROP_DOC: case PROP_TASK: case PROP_CODEL: case PROP_THROWS:
      case PROP_INTERRUPTS: case PROP_BEFORE: case PROP_AFTER:
	break;

      case PROP_IDS: case PROP_VERSION: case PROP_LANG: case PROP_EMAIL:
      case PROP_REQUIRE: case PROP_BUILD_REQUIRE: case PROP_PERIOD:
      case PROP_DELAY: case PROP_PRIORITY: case PROP_STACK:
	parserror(prop_loc(i.value), "property %s is not suitable for services",
		  prop_strkind(prop_kind(i.value)));
	e = 1; break;
    }
  if (e) return NULL;

  /* create */
  s = malloc(sizeof(*s));
  if (!s) {
    warnx("memory exhausted, cannot create service");
    return NULL;
  }

  s->loc = l;
  s->name = string(name);
  s->props = props;
  s->params = params;

  e = hash_insert(dotgen->services, s->name, s, (hrelease_f)service_destroy);
  switch(e) {
    case 0: break;

    case EEXIST:
      parserror(l, "duplicate service %s", s->name);
      service_s u = hash_find(dotgen->services, name);
      if (u) parserror(u->loc, " service %s declared here", u->name);
      /*FALLTHROUGH*/
    default:
      free(s);
      return NULL;
  }

  xwarnx("created service %s", s->name);
  return s;
}


/* --- port_strkind -------------------------------------------------------- */

/** Return a port kind as a string
 */
const char *
port_strkind(portkind k)
{
  switch(k) {
    case PORT_IN:		return "in";
    case PORT_OUT:		return "out";
    case PORT_EVENT:		return "event";
  }

  assert(0);
  return NULL;
}


/* --- task_destroy -------------------------------------------------------- */

/** destroy task
 */
void
task_destroy(task_s t)
{
  if (t) {
    hash_destroy(t->props);
    free(t);
  }
}


/* --- port_destroy -------------------------------------------------------- */

/** destroy port
 */
void
port_destroy(port_s p)
{
  if (p) free(p);
}


/* --- service_destroy ----------------------------------------------------- */

/** destroy service
 */
void
service_destroy(service_s s)
{
  if (s) {
    hash_destroy(s->props);
    hash_destroy(s->params);
    free(s);
  }
}
