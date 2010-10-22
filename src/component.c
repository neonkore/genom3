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
  idltype_s events;

  hash_s props;
  hash_s tasks;
  hash_s ports;
  hash_s services;
};

tloc		comp_loc(comp_s c) { assert(c); return c->loc; }
const char *	comp_name(comp_s c) { assert(c); return c->name; }
idltype_s	comp_eventtype(comp_s c) { assert(c); return c->events; }
hash_s		comp_props(comp_s c) { assert(c); return c->props; }
hash_s		comp_tasks(comp_s c) { assert(c); return c->tasks; }
hash_s		comp_ports(comp_s c) { assert(c); return c->ports; }
hash_s		comp_services(comp_s c) { assert(c); return c->services; }

struct task_s {
  tloc loc;
  const char *name;
  comp_s component;

  hash_s props;
};

tloc		task_loc(task_s t) { assert(t); return t->loc; }
const char *	task_name(task_s t) { assert(t); return t->name; }
comp_s		task_comp(task_s t) { assert(t); return t->component; }
hash_s		task_props(task_s t) { assert(t); return t->props; }

struct port_s {
  tloc loc;
  portkind kind;
  const char *name;
  comp_s component;
  idltype_s type;
};

tloc		port_loc(port_s p) { assert(p); return p->loc; }
const char *	port_name(port_s p) { assert(p); return p->name; }
portkind	port_kind(port_s p) { assert(p); return p->kind; }
comp_s		port_comp(port_s p) { assert(p); return p->component; }
idltype_s	port_type(port_s p) { assert(p); return p->type; }


struct service_s {
  tloc loc;
  const char *name;
  comp_s component;

  hash_s props;
  hash_s params;
};

tloc		service_loc(service_s s) { assert(s); return s->loc; }
const char *	service_name(service_s s) { assert(s); return s->name; }
comp_s		service_comp(service_s s) { assert(s); return s->component; }
hash_s		service_props(service_s s) { assert(s); return s->props; }
hash_s		service_params(service_s s) { assert(s); return s->params; }


/** the components of a dotgen file */
static struct comp_list_s {
  comp_s c;
  struct comp_list_s *next;
} *clist;

comp_s		comp_current() { return clist?clist->c:NULL; }

static idltype_s	comp_ievcreate(tloc l, const char *name);


/* --- comp_create --------------------------------------------------------- */

/** create a component
 */
comp_s
comp_create(tloc l, const char *name, hash_s props)
{
  struct comp_list_s *cl;
  comp_s c;
  prop_s p;
  hiter i;
  int e;
  assert(name);

  /* search for an existing component and move it to the front if it exists */
  for(c = comp_current(); c; c = comp_next(c))
    if (!strcmp(comp_name(c), name)) {
      struct comp_list_s *n;

      if (comp_current() == c) break;
      for(cl = clist; cl->next->c != c; cl = cl->next) /*empty body */;
      n = cl->next;
      cl->next = n->next;
      n->next = clist;
      clist = n;

      xwarnx("reopened component %s", c->name);
      break;
    }

  e = 0;

  /* check consitency of some properties */
  if (props) {
    p = hash_find(props, prop_strkind(PROP_CLOCKRATE));
    if (p) {
      cval c = type_constvalue(prop_value(p));
      if (const_convert(&c, CST_FLOAT) || c.f < 0.) {
	parserror(prop_loc(p), "invalid numeric value for %s",
		  prop_strkind(PROP_CLOCKRATE));
	e = 1;
      }
    }
  }

  /* check unwanted properties */
  if (props)
    for(hash_first(props, &i); i.current; hash_next(&i))
      switch(prop_kind(i.value)) {
	case PROP_DOC: case PROP_IDS: case PROP_ATTRIBUTE: case PROP_VERSION:
	case PROP_LANG: case PROP_EMAIL: case PROP_REQUIRE: case PROP_THROWS:
	case PROP_BUILD_REQUIRE: case PROP_CLOCKRATE:
	  break;

	case PROP_PERIOD: case PROP_DELAY: case PROP_PRIORITY:
	case PROP_SCHEDULING: case PROP_STACK: case PROP_VALIDATE:
	case PROP_CODEL: case PROP_TASK: case PROP_INTERRUPTS:
	case PROP_BEFORE: case PROP_AFTER:
	  parserror(prop_loc(i.value),
		    "property %s is not suitable for components",
		    prop_strkind(prop_kind(i.value)));
	  e = 1; break;
      }
  if (e) return NULL;

  /* merge properties when component already exists */
  if (c) {
    e = prop_merge(comp_props(c), props);
    return e?NULL:c;
  }

  /* create new component */
  c = malloc(sizeof(*c));
  if (!c) {
    warnx("memory exhausted, cannot create component");
    return NULL;
  }

  c->loc = l;
  c->name = string(name);
  c->props = props ? props : hash_create("property list", 0);
  c->tasks = hash_create(strings(name, " tasks", NULL), 2);
  if (!c->tasks) {
    free(c);
    return NULL;
  }
  c->ports = hash_create(strings(name, " ports", NULL), 2);
  if (!c->ports) {
    hash_destroy(c->tasks);
    free(c);
    return NULL;
  }
  c->services = hash_create(strings(name, " services", NULL), 2);
  if (!c->services) {
    hash_destroy(c->tasks);
    hash_destroy(c->ports);
    free(c);
    return NULL;
  }

  /* link component to others */
  cl = malloc(sizeof(*cl));
  if (!cl) {
    free(c);
    warnx("memory exhausted, cannot create component");
    return NULL;
  }
  cl->c = c;
  cl->next = clist;
  clist = cl;

  /* define internal event type */
  c->events = comp_ievcreate(l, name);
  if (!c->events) return NULL;
  p = hash_find(c->props, prop_strkind(PROP_THROWS));
  if (p) {
    /* register internal events */
    if (comp_addievs(l, prop_hash(p))) return NULL;
  }

  xwarnx("created component %s", c->name);
  return c;
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


/* --- comp_service -------------------------------------------------------- */

/** return component's service
 */
service_s
comp_service(comp_s c, const char *name)
{
  assert(c && name);
  return hash_find(c->services, name);
}


/* --- comp_next ----------------------------------------------------------- */

/** return the component after the one given
 */
comp_s
comp_next(comp_s c)
{
  struct comp_list_s *l;

  for(l = clist; l; l = l->next)
    if (l->c == c) return l->next?l->next->c:NULL;

  return NULL;
}


/* --- comp_addids --------------------------------------------------------- */

/** add IDS members in a component
 */
idltype_s
comp_addids(tloc l, scope_s s)
{
  idltype_s ids;
  comp_s c = comp_current();
  assert(s);

  /* a component must exist */
  if (!c) {
    scope_destroy(s);
    parserror(l, "missing component declaration before ids");
    errno = EINVAL;
    return NULL;
  }

  /* create or recreate ids type, so that it always appears to be declared at
   * this point. This is important if type declares other nested types,
   * because the later have to always appear before the main type. */
  ids = comp_ids(c);
  if (!ids) {
    scope_s p;
    prop_s idsp;

    p = scope_push(l, comp_name(c), SCOPE_MODULE);
    if (!p) return NULL;
    ids = type_newstruct(l, "ids", s);
    scope_pop();
    if (!ids) return NULL;

    idsp = prop_newids(l, ids);
    hash_insert(c->props, prop_name(idsp), idsp, (hrelease_f)prop_destroy);
  } else
    type_renew(ids);

  return ids;
}


/* --- comp_addattr -------------------------------------------------------- */

/** create an attribute for component
 */
int
comp_addattr(tloc l, hash_s attrs)
{
  hash_s h;
  hiter i;
  prop_s p;

  /* a component must exist */
  if (!comp_current()) {
    parserror(l, "missing component declaration before attribute");
    return errno = EINVAL;
  }

  /* append to existing attributes, or create one */
  p = hash_find(comp_current()->props, prop_strkind(PROP_ATTRIBUTE));
  if (p) {
    h = prop_hash(p);
    for (hash_first(attrs, &i); i.current; hash_first(attrs, &i)) {
      if (hash_insert(h, i.key, i.value, (hrelease_f)param_destroy)) {
	if (errno == EEXIST) {
	  param_s p = hash_find(h, i.key); assert(p);
	  parserror(l, "duplicate attribute '%s' declaration", i.key);
	  parsenoerror(param_loc(p), " %s declared here", param_name(p));
	} else
	  parserror(l, "dropped attribute '%s' declaration", i.key);
	hash_remove(attrs, i.key, 1);
      } else {
	xwarnx("created attribute '%s'", i.key);
	hash_remove(attrs, i.key, 0);
      }
    }
    hash_destroy(attrs);
  } else {
    p = prop_newhash(l, PROP_ATTRIBUTE, attrs);
    if (hash_insert(comp_current()->props,
		    prop_name(p), p, (hrelease_f)prop_destroy)) {
      parserror(l, "dropped attribute declaration");
      return errno;
    }
    for (hash_first(attrs, &i); i.current; hash_next(&i)) {
      xwarnx("created attribute '%s'", i.key);
    }
  }

  return 0;
}


/* --- comp_addtask -------------------------------------------------------- */

/** create a task in component
 */
task_s
comp_addtask(tloc l, const char *name, hash_s props)
{
  hiter i;
  task_s t;
  codel_s c;
  prop_s p, d;
  int e;
  assert(name && props);

  /* a component must exist */
  if (!comp_current()) {
    parserror(l, "missing component declaration before task %s", name);
    return NULL;
  }
  e = 0;

  /* check consitency of some properties */
  p = hash_find(props, prop_strkind(PROP_PERIOD));
  if (p) {
    cval c = type_constvalue(prop_value(p));
    if (const_convert(&c, CST_FLOAT) ||	c.f < 0.) {
      parserror(prop_loc(p),
		"invalid numeric value for %s", prop_strkind(PROP_PERIOD));
      e = 1;
    }
  }

  d = hash_find(props, prop_strkind(PROP_DELAY));
  if (d) {
    cval c = type_constvalue(prop_value(d));
    if (const_convert(&c, CST_FLOAT) ||	c.f < 0.) {
      parserror(prop_loc(d),
		"invalid numeric value for %s", prop_strkind(PROP_DELAY));
      e = 1;
    }
  }

  if (!e && d && p) {
    if (type_constvalue(prop_value(d)).f >= type_constvalue(prop_value(p)).f) {
      parserror(prop_loc(d), "delay must be less than period");
      parsenoerror(prop_loc(p), " period declared here");
      e = 1;
    }
  }
  if (!e && d && !p) {
    parsewarning(
      prop_loc(d), "ignoring delay in task %s with no period", name);
    hash_remove(props, prop_strkind(PROP_DELAY), 1/*release*/);
  }

  /* check unwanted properties */
  for(hash_first(props, &i); i.current; hash_next(&i))
    switch(prop_kind(i.value)) {
      case PROP_THROWS:
	/* register internal events */
	if (comp_addievs(l, prop_hash(i.value))) e = errno;
	break;

      case PROP_DOC: case PROP_PERIOD: case PROP_DELAY: case PROP_PRIORITY:
      case PROP_SCHEDULING: case PROP_STACK: case PROP_CODEL:
	break;

      case PROP_IDS: case PROP_ATTRIBUTE: case PROP_VERSION: case PROP_LANG:
      case PROP_EMAIL: case PROP_REQUIRE: case PROP_BUILD_REQUIRE:
      case PROP_CLOCKRATE: case PROP_TASK: case PROP_VALIDATE:
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
  t->component = comp_current();
  t->props = props;

  e = hash_insert(comp_current()->tasks, t->name, t, (hrelease_f)task_destroy);
  switch(e) {
    case 0: break;

    case EEXIST:
      /* control task is a reserved name */
      parserror(l, "duplicate task %s", t->name);
      task_s u = hash_find(comp_current()->tasks, name);
      if (u) parserror(u->loc, " task %s declared here", u->name);
      /*FALLTHROUGH*/
    default:
      free(t);
      return NULL;
  }

  /* set codel's parent task and (NULL) service */
  for(hash_first(props, &i); i.current; hash_next(&i))
    if (prop_kind(i.value) == PROP_CODEL) {
      c = prop_codel(i.value);
      *codel_task(c) = t;
      *codel_service(c) = NULL;
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
  if (!comp_current()) {
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
  p->component = comp_current();
  p->type = t;

  e = hash_insert(comp_current()->ports, p->name, p, (hrelease_f)port_destroy);
  switch(e) {
    case 0: break;

    case EEXIST:
      parserror(l, "duplicate port %s", p->name);
      port_s u = hash_find(comp_current()->ports, name);
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
  hiter i, j;
  service_s s;
  task_s t;
  codel_s c;
  int e;
  assert(name && params && props);

  /* all is a reserved name */
  if (!strcmp(name, ALL_SERVICE_NAME)) {
    parserror(l, "'%s' is a reserved service name", name);
    return NULL;
  }

  /* a component must exist */
  if (!comp_current()) {
    parserror(l, "missing component declaration before service %s", name);
    return NULL;
  }
  e = 0;

  /* check unwanted properties */
  t = NULL;
  for(hash_first(props, &i); i.current; hash_next(&i))
    switch(prop_kind(i.value)) {
      case PROP_TASK: /* remember task for later use (below) */
	t = prop_task(i.value);
	break;

      case PROP_THROWS:
	/* register internal events */
	if (comp_addievs(l, prop_hash(i.value))) e = errno;
	break;

      case PROP_VALIDATE:
	c = prop_codel(i.value);
	for(hash_first(codel_params(c), &j); j.current; hash_next(&j)) {
	  switch(param_dir(j.value)) {
	    case P_NODIR: case P_IN: case P_OUT: case P_INOUT:
	      break;

	    case P_INPORT: case P_OUTPORT:
	      parserror(prop_loc(j.value),
			"port %s is not allowed in validate codel %s",
			port_name(param_port(j.value)), codel_name(c));
	      e = 1; break;
	  }
	}
	break;

      case PROP_DOC: case PROP_CODEL: case PROP_INTERRUPTS:
      case PROP_BEFORE: case PROP_AFTER:
	break;

      case PROP_IDS: case PROP_ATTRIBUTE: case PROP_VERSION: case PROP_LANG:
      case PROP_EMAIL: case PROP_REQUIRE: case PROP_BUILD_REQUIRE:
      case PROP_CLOCKRATE: case PROP_PERIOD: case PROP_DELAY:
      case PROP_PRIORITY: case PROP_SCHEDULING: case PROP_STACK:
	parserror(prop_loc(i.value), "property %s is not suitable for services",
		  prop_strkind(prop_kind(i.value)));
	e = 1; break;
    }

  /* check parameters */
  for(hash_first(params, &i); i.current; hash_next(&i)) {
    switch(param_dir(i.value)) {
      case P_NODIR: case P_IN: case P_OUT: case P_INOUT:
	break;
      case P_INPORT: case P_OUTPORT:
	parserror(param_loc(i.value),
		  "%s parameter %s is not allowed in service %s",
		  param_strdir(param_dir(i.value)), param_name(i.value),
		  param_dir(i.value) == P_INPORT ? "input":"output");
	e = 1; break;
    }
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
  s->component = comp_current();
  s->props = props;
  s->params = params;

  e = hash_insert(comp_current()->services,
		  s->name, s, (hrelease_f)service_destroy);

  switch(e) {
    case 0: break;

    case EEXIST:
      parserror(l, "duplicate service %s", s->name);
      service_s u = hash_find(comp_current()->services, name);
      if (u) parserror(u->loc, " service %s declared here", u->name);
      /*FALLTHROUGH*/
    default:
      free(s);
      return NULL;
  }

  /* set codels parent task and service */
  for(hash_first(props, &i); i.current; hash_next(&i))
    switch (prop_kind(i.value)) {
      case PROP_CODEL:
	c = prop_codel(i.value);
	*codel_task(c) = t;
	*codel_service(c) = s;
	break;

      case PROP_VALIDATE:
	c = prop_codel(i.value);
	*codel_task(c) = NULL;
	*codel_service(c) = s;
	break;

      default: break;
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
    case PORT_INDATA:	return "data in";
    case PORT_INEVENT:	return "event in";
    case PORT_OUTDATA:	return "data out";
    case PORT_OUTEVENT:	return "event out";
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


/* --- comp_ievcreate ------------------------------------------------------ */

/** Create component internal event type
 */
static idltype_s
comp_ievcreate(tloc l, const char *name)
{
  const char *stdev[] = COMPONENT_EVENT_STD_NAMES;
  scope_s s, p;
  idltype_s t, e;
  hash_s h;
  int i;

  assert(scope_current() == scope_global());
  s = scope_push(l, name, SCOPE_MODULE);
  if (!s) {
    parserror(l, "the scope '::%1$s' is reserved for component '%1$s'", name);
    return NULL;
  }

  /* create standard events */
  h = hash_create("enumerator list", 5);
  if (!h) goto error;

  for(i=0; i<sizeof(stdev)/sizeof(stdev[0]); i++) {
    e = type_newenumerator(l, stdev[i]);
    if (!e) goto error;
    if (hash_insert(h, type_name(e), e, NULL)) goto error;
    xwarnx("created component standard event %s", stdev[i]);
  }

  /* create enum */
  t = type_newenum(l, COMPONENT_EVENTTYPE_NAME, h);
  if (!t) goto error;

  p = scope_pop();
  assert(p == s);

  xwarnx("created component internal event type %s %s",
	 type_strkind(type_kind(t)), type_fullname(t));
  return t;

error:
  scope_pop();
  parserror(l, "failed to create component internal event type '%s'",
	    strings(name, "::" COMPONENT_EVENTTYPE_NAME, NULL));
  return NULL;
}


/* --- comp_addiev --------------------------------------------------------- */

/** Add internal event to component
 */
int
comp_addievs(tloc l, hash_s h)
{
  idltype_s iev, e;
  scope_s s, p;
  hiter i;
  int r;
  assert(h);

  iev = comp_eventtype(comp_current()); assert(iev);

  s = scope_push(l, comp_name(comp_current()), SCOPE_MODULE);
  if (!s) return errno;

  r = 0;
  for(hash_first(h, &i); i.current; hash_next(&i)) {
    e = scope_findtype(s, i.value);
    if (!e || type_kind(e) != IDL_ENUMERATOR) {
      e = type_addenumerator(l, iev, i.value);
      if (e)
	xwarnx("added component internal event %s", type_name(e));
      else {
	parserror(l, "failed to create component internal event '%s'", i.value);
	r = r?r:EINVAL;
      }
    }
    hash_set(h, i.key, e);
  }

  p = scope_pop();
  assert(p == s);

  if (r) errno = r;
  return r;
}


/* --- comp_resolvesvc ----------------------------------------------------- */

/** Resolve service names in given hash.
 */
int
comp_resolvesvc(tloc l, comp_s c, hash_s h)
{
  hiter i;
  service_s s;
  int e;
  assert(c && h);

  e = 0;
  for(hash_first(h, &i); i.current; hash_next(&i)) {
    if (strcmp(i.value, ALL_SERVICE_NAME)) {
      s = comp_service(c, i.value);
      if (!s) {
	parserror(l, "no such service '%s'", i.value);
	e = 1;
      }
    }
  }

  return e;
}
