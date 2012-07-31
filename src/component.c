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

typedef enum compkind {
  COMP_REG,		/* regular component */
  COMP_TMPL,		/* template */
} compkind;

struct comp_s {
  tloc loc;
  const char *name;
  compkind kind;

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
idltype_s	comp_eventtype(comp_s c) { assert(c); return c->events; }
hash_s		comp_props(comp_s c) { assert(c); return c->props; }
hash_s		comp_tasks(comp_s c) { assert(c); return c->tasks; }
hash_s		comp_ports(comp_s c) { assert(c); return c->ports; }
hash_s		comp_services(comp_s c) { assert(c); return c->services; }
hash_s		comp_remotes(comp_s c) { assert(c); return c->remotes; }

static comp_s		comp_get_kind(const char *name, compkind kind);
static comp_s		comp_create_kind(tloc l, const char *name,
				hash_s props, compkind kind);
static idltype_s	comp_ievcreate(tloc l, const char *name);
static const char *	comp_strkind(compkind k);

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


/** the components of a dotgen file */
static comp_s clist = NULL;

/** the current active component */
static comp_s active = NULL;

comp_s	comp_active() { return active; }
void	comp_setactive(comp_s c) { if (c) active = c; }

comp_s
comp_first()
{
  if (clist && comp_kind(clist) != COMP_REG) return comp_next(clist);
  return clist;
}

comp_s
comp_next(comp_s c) {
  if (c) for(c = c->next; c && comp_kind(c) != COMP_REG; c = c->next);
  return c;
}


/* --- comp_get ------------------------------------------------------------ */

/** Return a component of that name
 */
comp_s
comp_get(const char *name)
{
  return comp_get_kind(name, COMP_REG);
}

static comp_s
comp_get_kind(const char *name, compkind kind)
{
  comp_s c;

  for(c = comp_first(); c; c = comp_next(c))
    if (!strcmp(comp_name(c), name) && c->kind == kind)
      return c;

  return NULL;
}


/* --- comp_create --------------------------------------------------------- */

/** create a component
 */
comp_s
comp_create(tloc l, const char *name, hash_s props)
{
  return comp_create_kind(l, name, props, COMP_REG);
}

/** create a template.
 * Templates are stored as regular components with a special kind. Tasks,
 * services, ids and ports created for this template are applied to all
 * existing components.
 */
comp_s
tmpl_create(tloc l, const char *name, hash_s props)
{
  return comp_create_kind(l, name, props, COMP_TMPL);
}

static comp_s
comp_create_kind(tloc l, const char *name, hash_s props, compkind kind)
{
  comp_s c, t;
  prop_s p;
  hiter i;
  int e;
  assert(name);

  /* get an existing component, or NULL if not yet created */
  c = comp_get_kind(name, kind);
  if (c) {
    xwarnx("reopened %s %s", comp_strkind(kind), c->name);
    comp_setactive(c);
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
	case PROP_DOC: case PROP_IDS: case PROP_VERSION: case PROP_LANG:
        case PROP_EMAIL: case PROP_REQUIRE: case PROP_THROWS:
        case PROP_CODELS_REQUIRE: case PROP_CLOCKRATE:
	  break;

	case PROP_PERIOD: case PROP_DELAY: case PROP_PRIORITY:
	case PROP_SCHEDULING: case PROP_STACK: case PROP_VALIDATE:
	case PROP_SIMPLE_CODEL: case PROP_FSM_CODEL: case PROP_TASK:
        case PROP_INTERRUPTS: case PROP_BEFORE: case PROP_AFTER:
	  parserror(prop_loc(i.value),
		    "property %s is not suitable for components",
		    prop_strkind(prop_kind(i.value)));
	  e = 1; break;
      }
  if (e) return NULL;

  /* merge properties when component already exists */
  if (c) {
    e = prop_merge_list(comp_props(c), props);
    hash_destroy(props, 0/*do not destroy properties (now referenced)*/);
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
  c->kind = kind;
  c->events = NULL;
  c->props = props ? props : hash_create("property list", 0);
  c->tasks = hash_create(strings(name, " tasks", NULL), 2);
  c->ports = hash_create(strings(name, " ports", NULL), 2);
  c->services = hash_create(strings(name, " services", NULL), 2);
  c->remotes = hash_create(strings(name, " remotes", NULL), 2);
  if (!c->tasks || !c->ports || !c->services || !c->remotes) {
    if (c->tasks) hash_destroy(c->tasks, 1);
    if (c->ports) hash_destroy(c->ports, 1);
    if (c->services) hash_destroy(c->services, 1);
    if (c->remotes) hash_destroy(c->remotes, 1);
    free(c);
    return NULL;
  }

  /* link component to others */
  c->next = NULL;
  if (!clist) clist = c; else {
    for(t = clist; t->next; t = t->next) /* empty body */;
    t->next = c;
  }
  comp_setactive(c);

  /* define internal event type */
  c->events = comp_ievcreate(l, name);
  if (!c->events) {
    comp_setactive(clist = c->next);
    return NULL;
  }
  p = hash_find(c->props, prop_strkind(PROP_THROWS));
  if (p) {
    /* register internal events */
    if (comp_addievs(l, prop_hash(p), 1)) {
      comp_setactive(clist = c->next);
      return NULL;
    }
  }

  xwarnx("created %s %s", comp_strkind(kind), c->name);
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


/* --- comp_addids --------------------------------------------------------- */

/** add IDS members in a component
 */
idltype_s
comp_addids(tloc l, scope_s s)
{
  comp_s c = comp_active();
  idltype_s ids, t, m;
  scope_s p, cs;
  hiter it;
  prop_s idsp;
  assert(s);

  /* a component must exist */
  if (!c) {
    scope_destroy(s);
    parserror(l, "missing component declaration before ids");
    errno = EINVAL;
    return NULL;
  }

  /* get the ids type */
  ids = comp_ids(c);

  /* if the ids scope was not created in the component, create it. This happens
   * when the ids is declared in a template context */
  p = scope_push(l, comp_name(c), SCOPE_MODULE);
  if (!p) return NULL;
  if (scope_parent(s) != p) {
    if (!ids) {
      cs = scope_push(l, "ids", SCOPE_STRUCT);
      if (!cs) { scope_set(scope_global()); return NULL; }
      scope_detach(cs);
      scope_pop();
    } else
      cs = type_membersscope(ids);
  } else
    cs = s;

  /* create the ids type */
  if (!ids) {
    ids = type_newstruct(l, "ids", cs);
    if (!ids) { scope_set(scope_global()); return NULL; }

    idsp = prop_newids(l, ids);
    hash_insert(c->props, prop_name(idsp), idsp, (hrelease_f)prop_destroy);
  }

  /* create new ids members */
  if (cs != s) {
    scope_set(cs);
    for(t = scope_firstype(s, &it); t; t = scope_nextype(&it)) {
      if (type_kind(t) != IDL_MEMBER) continue;
      m = scope_findtype(cs, type_name(t));
      if (m && type_type(m) == type_type(t)) continue;

      m = type_newmember(type_loc(t), type_name(t), type_type(t));
      if (!m) { scope_set(scope_global()); return NULL; }
    }
  }

  /* recreate ids type, so that it always appears to be declared at this
   * point. This is important if the ids declares other nested types,
   * because the later have to always appear before the main type. */
  type_renew(ids);
  scope_set(scope_global());

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
  comp_s comp;
  port_s port;
  remote_s remote;
  hiter i, j;
  service_s s;
  task_s t;
  codel_s c;
  prop_s p;
  int e;
  assert(name && params);

  /* a component must exist */
  comp = comp_active();
  if (!comp) {
    parserror(l, "missing component declaration before %s %s",
              service_strkind(kind), name);
    return NULL;
  }

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
  scope_s s, p;
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
  assert(scope_current() == scope_global());
  s = scope_push(l, comp_name(comp), SCOPE_MODULE);
  if (!s) {
    parserror(l, "the scope '::%1$s' is reserved for component '%1$s'", name);
    remote_destroy(r);
    return NULL;
  }

  r->type = type_newremote(l, name, r);

  p = scope_pop();
  assert(p == s);
  if (!r->type) { remote_destroy(r); return NULL; }

  xwarnx("created remote %s", r->name);
  return r;
}


/* --- comp_strkind -------------------------------------------------------- */

/** Return a component kind as a string
 */

static const char *
comp_strkind(compkind k)
{
  switch(k) {
    case COMP_REG:		return "component";
    case COMP_TMPL:		return "template";
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
comp_addievs(tloc l, hash_s h, int nostd)
{
  const char *stdev[] = COMPONENT_EVENT_STD_NAMES;
  idltype_s iev, e;
  scope_s s, p;
  hiter i;
  int r, v;
  assert(h);

  if (!comp_active()) return 0;
  iev = comp_eventtype(comp_active()); assert(iev);

  s = scope_push(l, comp_name(comp_active()), SCOPE_MODULE);
  if (!s) return errno;

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

    e = scope_findtype(s, i.key);
    if (!e || type_kind(e) != IDL_ENUMERATOR) {
      e = type_addenumerator(l, iev, i.key);
      if (e)
	xwarnx("added component internal event %s", type_name(e));
      else {
	parserror(l, "failed to create component internal event '%s'", i.key);
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
    if (comp_kind(t) != COMP_TMPL) continue;

    for(c = comp_first(); c; c = comp_next(c)) {
      comp_setactive(c);
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

      if (!e)
	xwarnx("applied template %s to component %s",
	       comp_name(t), comp_name(c));
    }
  }

  /* unlink templates now they've been applied */
  while(clist && comp_kind(clist) == COMP_TMPL)
    clist = clist->next;
  if (clist)
    for(t = clist; t->next;)
      if (comp_kind(t->next) == COMP_TMPL)
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

  fprintf(out, "%s %s",
          comp_kind(c) == COMP_REG ? "component":"template", comp_name(c));

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
          fprintf(out, "%s%s:\t\"%s\";\n", indent,
                  prop_strkind(prop_kind(i.value)), prop_text(i.value));
          break;

        case PROP_REQUIRE:
	case PROP_CODELS_REQUIRE:
          poptbrace;
          fprintf(out, "%s%s:\n", indent, prop_strkind(prop_kind(i.value)));
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
          fprintf(out, "%s%s:\n", indent, prop_strkind(prop_kind(i.value)));
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
          fprintf(out, "%s%s:\t%gs\n", indent,
                  prop_strkind(prop_kind(i.value)), v.f);
	  break;

        default: break;
      }
    }
#undef poptbrace

  if (!brace) fputs("}", out);
  fprintf(out, ";\n");
  return 0;
}
