/*
 * Copyright (c) 2009-2012 LAAS/CNRS
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
  compkind kind;
  scope_s scope;
  scope_s idsscope;

  idltype_s events;
  hash_s props;
  hash_s tasks;
  hash_s ports;
  hash_s services;
  hash_s remotes;

  comp_s next;
};

tloc		comp_loc(comp_s c) { assert(c); return c->loc; }
const char *	comp_name(comp_s c) { assert(c); return c->name; }
compkind	comp_kind(comp_s c) { assert(c); return c->kind; }
scope_s		comp_scope(comp_s c) { assert(c); return c->scope; }
scope_s		comp_idsscope(comp_s c) { assert(c); return c->idsscope; }
idltype_s	comp_eventtype(comp_s c) { assert(c); return c->events; }
hash_s		comp_props(comp_s c) { assert(c); return c->props; }
hash_s		comp_tasks(comp_s c) { assert(c); return c->tasks; }
hash_s		comp_ports(comp_s c) { assert(c); return c->ports; }
hash_s		comp_services(comp_s c) { assert(c); return c->services; }
hash_s		comp_remotes(comp_s c) { assert(c); return c->remotes; }

static idltype_s	comp_ievcreate(tloc l, const char *name);

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

struct service_s {
  tloc loc;
  const char *name;
  svckind kind;
  comp_s component;

  hash_s props;
  hash_s params;
  hash_s fsm;
};

tloc		service_loc(service_s s) { assert(s); return s->loc; }
const char *	service_name(service_s s) { assert(s); return s->name; }
svckind		service_kind(service_s s) { assert(s); return s->kind; }
comp_s		service_comp(service_s s) { assert(s); return s->component; }
hash_s		service_props(service_s s) { assert(s); return s->props; }
hash_s		service_params(service_s s) { assert(s); return s->params; }
hash_s		service_fsm(service_s s) { assert(s); return s->fsm; }

struct remote_s {
  tloc loc;
  const char *name;
  comp_s component;

  hash_s params;
  idltype_s type;
};

tloc		remote_loc(remote_s r) { assert(r); return r->loc; }
const char *	remote_name(remote_s r) { assert(r); return r->name; }
comp_s		remote_comp(remote_s r) { assert(r); return r->component; }
idltype_s	remote_type(remote_s r) { assert(r); return r->type; }
hash_s		remote_params(remote_s r) { assert(r); return r->params; }


/** the components and interfaces of a dotgen file */
static comp_s clist = NULL;

/** the current active component */
static comp_s active = NULL;

comp_s	comp_active(void) { return active; }

comp_s
comp_first()
{
  if (clist && comp_kind(clist) != COMP_REGULAR) return comp_next(clist);
  return clist;
}

comp_s
comp_next(comp_s c) {
  if (c) for(c = c->next; c && comp_kind(c) != COMP_REGULAR; c = c->next);
  return c;
}


/* --- comp_get ------------------------------------------------------------ */

/** Return a component of that name
 */
static comp_s
comp_get(const char *name)
{
  comp_s c;
  for(c = clist; c; c = c->next) if (!strcmp(comp_name(c), name)) return c;
  return NULL;
}


/* --- comp_push ----------------------------------------------------------- */

/** create or reopen a component
 */
comp_s
comp_push(tloc l, const char *name, compkind kind)
{
  comp_s c, t;
  scope_s scomp, spop, sids;
  assert(name);

  /* push component scope */
  assert(scope_current() == scope_global());
  scomp = scope_push(l, name, SCOPE_MODULE);
  if (!scomp) return NULL;

  /* get an existing component, or NULL if not yet created */
  c = comp_get(name);
  if (c) {
    if (c->kind != kind) {
      parserror(l, "conflicting types for %s %s", comp_strkind(kind), name);
      parsenoerror(comp_loc(c), " %s %s declared here",
                   comp_strkind(c->kind), name);
      return NULL;
    }

    xwarnx("reopened %s %s", comp_strkind(kind), c->name);
    return active = c;
  }

  /* create ids scope */
  sids = scope_push(l, prop_strkind(PROP_IDS), SCOPE_STRUCT);
  if (!sids) return NULL;
  scope_detach(sids);
  scope_pop();

  /* create new component */
  c = malloc(sizeof(*c));
  if (!c) {
    warnx("memory exhausted, cannot create component");
    goto error;
  }

  c->loc = l;
  c->name = string(name);
  c->kind = kind;
  c->scope = scomp;
  c->idsscope = sids;
  c->events = comp_ievcreate(l, name);
  c->props = hash_create(strings(name, " property list", NULL), 2);
  c->tasks = hash_create(strings(name, " tasks", NULL), 1);
  c->ports = hash_create(strings(name, " ports", NULL), 1);
  c->services = hash_create(strings(name, " services", NULL), 2);
  c->remotes = hash_create(strings(name, " remotes", NULL), 1);
  if (!c->events || !c->tasks || !c->ports || !c->services || !c->remotes) {
    if (c->tasks) hash_destroy(c->tasks, 1);
    if (c->ports) hash_destroy(c->ports, 1);
    if (c->services) hash_destroy(c->services, 1);
    if (c->remotes) hash_destroy(c->remotes, 1);
    free(c);
    goto error;
  }

  /* link component to others */
  c->next = NULL;
  if (!clist) clist = c; else {
    for(t = clist; t->next; t = t->next) /* empty body */;
    t->next = c;
  }

  xwarnx("created %s %s", comp_strkind(kind), c->name);
  return active = c;

error:
  spop = scope_pop();
  assert(spop == scomp);
  return NULL;
}


/* --- comp_pop ------------------------------------------------------------ */

comp_s
comp_pop(void)
{
  scope_s s;
  comp_s c = active;
  assert(active);
  s = scope_pop();
  assert(s == c->scope);
  active = NULL;
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


/* --- comp_remote --------------------------------------------------------- */

/** return component's remote
 */
remote_s
comp_remote(comp_s c, const char *name)
{
  assert(c && name);
  return hash_find(c->remotes, name);
}


/* --- comp_addprop -------------------------------------------------------- */

/** add a property in a component
 */
int
comp_addprop(tloc l, prop_s p)
{
  comp_s c = comp_active();
  assert(c);

  /* check unwanted properties */
  switch(prop_kind(p)) {
    case PROP_DOC: case PROP_IDS: case PROP_VERSION: case PROP_LANG:
    case PROP_EMAIL: case PROP_REQUIRE: case PROP_THROWS:
    case PROP_CODELS_REQUIRE: case PROP_CLOCKRATE:
      break;

    case PROP_PERIOD: case PROP_DELAY: case PROP_PRIORITY:
    case PROP_SCHEDULING: case PROP_STACK: case PROP_VALIDATE:
    case PROP_SIMPLE_CODEL: case PROP_FSM_CODEL: case PROP_TASK:
    case PROP_INTERRUPTS: case PROP_BEFORE: case PROP_AFTER:
      parserror(l, "property %s may not be defined for components",
                prop_strkind(prop_kind(p)));
      return errno = EINVAL;
  }

  /* merge property */
  return prop_merge(comp_props(c), p);
}


/* --- comp_addids --------------------------------------------------------- */

/** add IDS members in a component
 */
idltype_s
comp_addids(tloc l, scope_s s)
{
  comp_s c = comp_active();
  idltype_s ids, t, m;
  hiter it;
  assert(c && s);

  /* create the ids type if needed */
  ids = comp_ids(c);
  if (!ids) {
    ids = type_newstruct(l, prop_strkind(PROP_IDS), c->idsscope);
    if (!ids) return NULL;

    hash_insert(c->props, prop_strkind(PROP_IDS), prop_newids(l, ids),
                (hrelease_f)prop_destroy);
  }

  /* create new ids members if needed */
  if (s != c->idsscope) {
    scope_set(c->idsscope);
    for(t = scope_firstype(s, &it); t; t = scope_nextype(&it)) {
      if (type_kind(t) != IDL_MEMBER) continue;
      m = scope_findtype(type_membersscope(ids), type_name(t));
      if (m && type_type(m) == type_type(t)) continue;

      m = type_newmember(type_loc(t), type_name(t), type_type(t));
      if (!m) return NULL;
    }
    scope_pop();
    assert(scope_current() == c->scope);
  }

  /* recreate ids type, so that it always appears to be declared at this
   * point. This is important if the ids declares other nested types,
   * because the later have to always appear before the main type. */
  type_renew(ids);
  return ids;
}


/* --- comp_addtask -------------------------------------------------------- */

/** create a task in component
 */
task_s
comp_addtask(tloc l, const char *name, hash_s props)
{
  comp_s comp;
  hiter i;
  task_s t;
  codel_s c;
  prop_s p, d;
  int e;
  assert(name);

  /* a component must exist */
  comp = comp_active();
  if (!comp) {
    parserror(l, "missing component declaration before task %s", name);
    return NULL;
  }
  e = 0;

  if (!props) {
    props = hash_create("property list", 0);
    if (!props) return NULL;
  }

  /* check consitency of some properties */
  p = hash_find(props, prop_strkind(PROP_PERIOD));
  if (p) {
    cval c = type_constvalue(prop_value(p));
    if (const_convert(&c, CST_FLOAT) ||	c.f <= 0.) {
      parserror(prop_loc(p),
		"invalid numeric value for %s", prop_strkind(PROP_PERIOD));
      e = 1;
    }
  }

  d = hash_find(props, prop_strkind(PROP_DELAY));
  if (d) {
    cval c = type_constvalue(prop_value(d));
    if (!p) {
      parserror(prop_loc(d), "delay without period is not allowed");
      e = 1;
    }
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
        if (comp_addievs(l, prop_hash(i.value), 1)) e = errno;
        break;

      case PROP_DOC: case PROP_PERIOD: case PROP_DELAY: case PROP_PRIORITY:
      case PROP_SCHEDULING: case PROP_STACK: case PROP_FSM_CODEL:
	break;

      case PROP_SIMPLE_CODEL:
	parserror(l, "simple codel not allowed in task %s", name);
	parsenoerror(prop_loc(i.value), " %s declared here",
                     prop_name(i.value));
        e = 1; break;

      case PROP_IDS: case PROP_VERSION: case PROP_LANG: case PROP_EMAIL:
      case PROP_REQUIRE: case PROP_CODELS_REQUIRE: case PROP_CLOCKRATE:
      case PROP_TASK: case PROP_VALIDATE: case PROP_INTERRUPTS:
      case PROP_BEFORE: case PROP_AFTER:
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

  e = hash_insert(comp->tasks, t->name, t, (hrelease_f)task_destroy);
  switch(e) {
    case 0: break;

    case EEXIST:
      parserror(l, "duplicate task %s", t->name);
      task_s u = hash_find(comp->tasks, name);
      if (u) parserror(u->loc, " task %s declared here", u->name);
      /*FALLTHROUGH*/
    default:
      free(t);
      return NULL;
  }

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

  xwarnx("created task %s", t->name);
  return t;
}


/* --- comp_addservice ----------------------------------------------------- */

/** create a service in component
 */
service_s
comp_addservice(tloc l, svckind kind, const char *name, hash_s params,
                hash_s props)
{
  comp_s comp = comp_active();
  port_s port;
  remote_s remote;
  hiter i, j;
  service_s s;
  task_s t;
  codel_s c;
  prop_s p;
  int e;
  assert(comp && name && params);

  /* check for reserved or already used names */
  if (!strcmp(name, ALL_SERVICE_NAME)) {
    parserror(l, "'%s' is a reserved %s name", service_strkind(kind), name);
    return NULL;
  }
  port = comp_port(comp, name);
  remote = comp_remote(comp, name);
  if (port || remote) {
    parserror(l, "redefinition of '%s'", name);
    if (port)
      parsenoerror(port_loc(port), " port '%s' declared here", port_name(port));
    if (remote)
      parsenoerror(remote_loc(remote),
                   " remote '%s' declared here", remote_name(remote));
    return NULL;
  }

  /* create empty property list if none has been defined */
  if (!props) {
    props = hash_create("property list", 0);
    if (!props) return NULL;
  }
  e = 0;

  /* remember task for later use (below) */
  p = hash_find(props, prop_strkind(PROP_TASK));
  t = p ? prop_task(p) : NULL;
  switch(kind) {
    case S_ATTRIBUTE:
    case S_FUNCTION:
      if (t) {
        parserror(l, "%s '%s' may not run in a task",
                  service_strkind(kind), name);
        parsenoerror(prop_loc(p), " extraneous task '%s' property",
                     task_name(t));
        e = 1;
      }
      break;

    case S_ACTIVITY:
      break;
  }

  /* check unwanted properties */
  for(hash_first(props, &i); i.current; hash_next(&i))
    switch(prop_kind(i.value)) {
      case PROP_THROWS:
        if (comp_addievs(l, prop_hash(i.value), 1)) e = errno;
        break;

      case PROP_SIMPLE_CODEL:
        switch(kind) {
          case S_ATTRIBUTE:
          case S_ACTIVITY:
            parserror(l, "%s %s cannot have simple codels",
                      service_strkind(kind), name);
            parsenoerror(prop_loc(i.value), " %s declared here",
                         prop_name(i.value));
            e = 1;
            break;

          case S_FUNCTION:
            break;
        }
        /*FALLTHROUGH*/
      case PROP_VALIDATE:
	c = prop_codel(i.value);
	for(hash_first(codel_params(c), &j); j.current; hash_next(&j)) {
	  switch(param_src(j.value)) {
            case P_NOSRC: assert(0);
            case P_IDS: case P_SERVICE: break;

            case P_PORT: case P_REMOTE:
	      parserror(prop_loc(j.value),
                        "%s %s is not allowed in validate codel %s",
                        param_strsrc(param_src(j.value)),
                        port_name(param_port(j.value)), codel_name(c));
	      e = 1; break;
	  }
	}
	break;

      case PROP_FSM_CODEL:
        switch(kind) {
          case S_ATTRIBUTE:
          case S_FUNCTION:
            parserror(l, "%s %s cannot have state machine codels",
                      service_strkind(kind), name);
            parsenoerror(prop_loc(i.value), " %s declared here",
                         prop_name(i.value));
            e = 1;
            break;

          case S_ACTIVITY:
            if (!t) {
              parserror(prop_loc(i.value),
                        "%s '%s' with codels must run in a task",
                        service_strkind(kind), name);
              parsenoerror(l, " missing task property");
              e = 1;
            }
            break;
        }
        break;

      case PROP_TASK: case PROP_DOC: case PROP_INTERRUPTS: case PROP_BEFORE:
      case PROP_AFTER:
        /* ok */
        break;

      case PROP_IDS: case PROP_VERSION: case PROP_LANG: case PROP_EMAIL:
      case PROP_REQUIRE: case PROP_CODELS_REQUIRE: case PROP_CLOCKRATE:
      case PROP_PERIOD: case PROP_DELAY: case PROP_PRIORITY:
      case PROP_SCHEDULING: case PROP_STACK:
	parserror(prop_loc(i.value), "property %s is not suitable for %ss",
                  service_strkind(kind), prop_strkind(prop_kind(i.value)));
	e = 1; break;
    }

  /* check parameters */
  for(hash_first(params, &i); i.current; hash_next(&i)) {
    switch(param_src(i.value)) {
      case P_NOSRC: case P_PORT: case P_REMOTE: assert(0);
      case P_IDS: if (kind == S_ATTRIBUTE) break; else assert(0);
      case P_SERVICE: if (kind != S_ATTRIBUTE) break; else assert(0);
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
  s->kind = kind;
  s->component = comp;
  s->props = props;
  s->params = params;

  e = hash_insert(comp->services, s->name, s, (hrelease_f)service_destroy);

  switch(e) {
    case 0: break;

    case EEXIST:
      parserror(l, "duplicate %s %s", service_strkind(kind), s->name);
      service_s u = hash_find(comp->services, name);
      if (u) parserror(u->loc, " %s %s declared here",
                       service_strkind(service_kind(u)), u->name);
      /*FALLTHROUGH*/
    default:
      free(s);
      return NULL;
  }

  /* build service's fsm */
  s->fsm = codel_fsmcreate(l, s->props);
  if (!s->fsm) {
    free(s);
    return NULL;
  }

  /* set codels parent task and service */
  for(hash_first(props, &i); i.current; hash_next(&i))
    switch (prop_kind(i.value)) {
      case PROP_FSM_CODEL:
	c = prop_codel(i.value);
	*codel_task(c) = t;
	*codel_service(c) = s;
	break;

      case PROP_VALIDATE:
      case PROP_SIMPLE_CODEL:
	c = prop_codel(i.value);
	*codel_task(c) = NULL;
	*codel_service(c) = s;
	break;

      default: break;
    }

  xwarnx("created %s %s", service_strkind(kind), s->name);
  return s;
}




/* --- comp_addremote ------------------------------------------------------ */

/** create a remote in component
 */
remote_s
comp_addremote(tloc l, const char *name, hash_s params)
{
  comp_s comp;
  port_s port;
  service_s service;
  remote_s r;
  int e;
  assert(name && params);

  /* a component must exist */
  comp = comp_active();
  if (!comp) {
    parserror(l, "missing component declaration before remote %s", name);
    return NULL;
  }

  /* check for already used names */
  port = comp_port(comp, name);
  service = comp_service(comp, name);
  if (port || service) {
    parserror(l, "redefinition of '%s'", name);
    if (port)
      parsenoerror(port_loc(port), " port '%s' declared here", port_name(port));
    if (service)
      parsenoerror(service_loc(service), " %s '%s' declared here",
                   service_strkind(service_kind(service)),
                   service_name(service));
    return NULL;
  }

  /* create */
  r = malloc(sizeof(*r));
  if (!r) {
    warnx("memory exhausted, cannot create remote");
    return NULL;
  }

  r->loc = l;
  r->name = string(name);
  r->component = comp;
  r->params = params;

  e = hash_insert(comp->remotes, r->name, r, (hrelease_f)remote_destroy);
  switch(e) {
    case 0: break;

    case EEXIST:
      parserror(l, "duplicate remote %s", r->name);
      remote_s u = hash_find(comp->remotes, name);
      if (u) parserror(u->loc, " remote %s declared here", u->name);
      /*FALLTHROUGH*/
    default:
      free(r);
      return NULL;
  }

  /* create underlying type */
  assert(scope_current() == comp_scope(comp));
  r->type = type_newremote(l, name, r);
  if (!r->type) { remote_destroy(r); return NULL; }

  xwarnx("created remote %s", r->name);
  return r;
}


/* --- comp_strkind -------------------------------------------------------- */

/** Return a component kind as a string
 */

const char *
comp_strkind(compkind k)
{
  switch(k) {
    case COMP_REGULAR:		return "component";
    case COMP_IFACE:		return "interface";
  }

  assert(0);
  return NULL;
}


/* --- service_strkind ----------------------------------------------------- */

/** Return a service kind as a string
 */

const char *
service_strkind(svckind k)
{
  switch(k) {
    case S_ATTRIBUTE:		return "attribute";
    case S_FUNCTION:		return "function";
    case S_ACTIVITY:		return "activity";
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
    hash_destroy(t->props, 1);
    free(t);
  }
}


/* --- service_destroy ----------------------------------------------------- */

/** destroy service
 */
void
service_destroy(service_s s)
{
  if (s) {
    hash_destroy(s->props, 1);
    hash_destroy(s->params, 1);
    free(s);
  }
}


/* --- remote_destroy ------------------------------------------------------ */

/** destroy remote
 */
void
remote_destroy(remote_s r)
{
  if (r) {
    hash_destroy(r->params, 1);
    type_destroy(r->type);
    free(r);
  }
}


/* --- comp_ievcreate ------------------------------------------------------ */

/** Create component internal event type
 */
static idltype_s
comp_ievcreate(tloc l, const char *name)
{
  const char *stdev[] = COMPONENT_EVENT_STD_NAMES;
  idltype_s t, e;
  hash_s h;
  int i;

  /* create standard events */
  h = hash_create("enumerator list", 10);
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

  xwarnx("created component internal event type %s %s",
	 type_strkind(type_kind(t)), type_fullname(t));
  return t;

error:
  parserror(l, "failed to create component internal event type '%s'",
	    strings(name, "::" COMPONENT_EVENTTYPE_NAME, NULL));
  return NULL;
}


/* --- comp_addiev --------------------------------------------------------- */


/** Add internal event to component
 */
int
comp_addievs(tloc l, hash_s h, int nostd)
{
  const char *stdev[] = COMPONENT_EVENT_STD_NAMES;
  comp_s c = comp_active();
  idltype_s e;
  hiter i;
  int r, v;
  assert(c && h);

  r = 0;
  for(hash_first(h, &i); i.current; hash_next(&i)) {

    if (nostd) {
      for(v=0; v<sizeof(stdev)/sizeof(stdev[0]); v++)
        if (!strcmp(i.key, stdev[v])) {
          parserror(l, "event '%s' is reserved and cannot be thrown", i.key);
          r = r?r:EINVAL;
          break;
        }
    }

    e = scope_findtype(c->scope, i.key);
    if (!e || type_kind(e) != IDL_ENUMERATOR) {
      e = type_addenumerator(l, c->events, i.key);
      if (e)
	xwarnx("added component internal event %s", type_name(e));
      else {
	parserror(l, "failed to create component internal event '%s'", i.key);
	r = r?r:EINVAL;
      }
    }
    hash_set(h, i.key, e);
  }

  return errno = r;
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


/* --- comp_applytmpl ------------------------------------------------------ */

/** Apply template declarations to all components.
 */
int
comp_applytmpl()
{
  comp_s c, t;
  task_s task;
  service_s svc;
  param_s param;
  hash_s h;
  hiter i, j;
  int e = 0;

  for(t = clist; t; t = t->next) {
    if (comp_kind(t) != COMP_IFACE) continue;

    for(c = comp_first(); c; c = comp_next(c)) {
      assert(scope_current() == scope_global());
      active = c;
      if (!scope_push(c->loc, c->name, SCOPE_MODULE)) continue;

      e = prop_merge_list(comp_props(c), comp_props(t));

      for(hash_first(comp_ports(t), &i); i.current; hash_next(&i)) {
	if (!port_new(port_loc(i.value), port_kind(i.value),
		      port_name(i.value), port_type(i.value))) e = 1;
      }

      for(hash_first(comp_tasks(t), &i); i.current; hash_next(&i)) {
	task = comp_addtask(task_loc(i.value), task_name(i.value), NULL);
	if (task) {
	  e |= prop_merge_list(task_props(task), task_props(i.value));
	  hash_destroy(task->fsm, 1);
	  task->fsm = codel_fsmcreate(task_loc(task), task_props(task));
	}
	else
	  e = 1;
      }

      for(hash_first(comp_services(t), &i); i.current; hash_next(&i)) {
	h = hash_create("parameter list", 0);
	for(hash_first(
	      service_params(i.value), &j); j.current; hash_next(&j)) {
	  param = param_clone(j.value);
	  if (!param) { e = 1; break; }
	  if (hash_insert(h, j.key, param, (hrelease_f)param_destroy)) {
	    e = 1; break;
	  }
	}
	if (e) break;

	svc = comp_addservice(
          service_loc(i.value), service_kind(i.value), service_name(i.value),
          h, NULL);
	if (svc) {
	  e |= prop_merge_list(service_props(svc), service_props(i.value));
	  hash_destroy(svc->fsm, 1);
	  svc->fsm = codel_fsmcreate(service_loc(svc), service_props(svc));
	}
	else
	  e = 1;
      }

      for(hash_first(comp_remotes(t), &i); i.current; hash_next(&i)) {
	h = hash_create("parameter list", 0);
	for(hash_first(remote_params(i.value), &j); j.current; hash_next(&j)) {
	  param = param_clone(j.value);
	  if (!param) { e = 1; break; }
	  if (hash_insert(h, j.key, param, (hrelease_f)param_destroy)) {
	    e = 1; break;
	  }
	}
	if (e) break;

        if (!comp_addremote(remote_loc(i.value), remote_name(i.value), h))
          e = 1;
      }

      active = NULL;
      scope_pop();

      if (!e)
	xwarnx("applied template %s to component %s",
	       comp_name(t), comp_name(c));
    }
  }

  /* unlink templates now they've been applied */
  while(clist && comp_kind(clist) == COMP_IFACE)
    clist = clist->next;
  if (clist)
    for(t = clist; t->next;)
      if (comp_kind(t->next) == COMP_IFACE)
        t->next = t->next->next;
      else
        t = t->next;

  return e;
}


/* --- comp_dumpall -------------------------------------------------------- */

/** Dump all components as a dotgen specification.
 */
int
comp_dumpall(FILE *out)
{
  comp_s c;
  for(c = clist; c; c = c->next) {
    comp_dump(c, stdout);
    if (c->next) fputs("\n", out);
  }

  return 0;
}


/* --- comp_dump ----------------------------------------------------------- */

/** Dump component description as a dotgen specification.
 */
int
comp_dump(comp_s c, FILE *out)
{
  const char *indent = "  ";
  const char *brace = " {\n";
  const char *colon = NULL;

  hash_s props;
  hiter i, j;
  citer k;
  cval v;

  fprintf(out, "%s %s", comp_strkind(comp_kind(c)), comp_name(c));

#define poptbrace                                                       \
  do { if (brace) { fputs(brace, out); brace = NULL; } } while(0)

  props = comp_props(c);
  if (props)
    for(hash_first(props, &i); i.current; hash_next(&i)) {
      switch(prop_kind(i.value)) {
	case PROP_DOC:
        case PROP_VERSION:
	case PROP_LANG:
        case PROP_EMAIL:
          poptbrace;
          fprintf(out, "%s%s\t\"%s\";\n", indent,
                  prop_strkind(prop_kind(i.value)), prop_text(i.value));
          break;

        case PROP_REQUIRE:
	case PROP_CODELS_REQUIRE:
          poptbrace;
          fprintf(out, "%s%s\n", indent, prop_strkind(prop_kind(i.value)));
          colon = NULL;
          for(clist_first(prop_list(i.value), &k); k.current; clist_next(&k)) {
            if (colon) fputs(colon, out);
            fprintf(out, "%1$s%1$s\"%2$s\"", indent, k.value->s);
            colon = ",\n";
          }
          fputs(";\n", out);
          break;

        case PROP_THROWS:
          poptbrace;
          fprintf(out, "%s%s\n", indent, prop_strkind(prop_kind(i.value)));
          colon = NULL;
          for(hash_first(prop_hash(i.value), &j); j.current; hash_next(&j)) {
            if (colon) fputs(colon, out);
            fprintf(out, "%1$s%1$s\"%2$s\"", indent, j.key);
            colon = ",\n";
          }
          fputs(";\n", out);
          break;

        case PROP_CLOCKRATE:
          poptbrace;
          v = type_constvalue(prop_value(i.value));
          if (const_convert(&v, CST_FLOAT)) break;
          fprintf(out, "%s%s\t%gs\n", indent,
                  prop_strkind(prop_kind(i.value)), v.f);
	  break;

        default: break;
      }
    }
#undef poptbrace

  fprintf(out, "%s;\n", brace?"":"}");
  return 0;
}
