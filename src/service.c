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
 *                                           Anthony Mallet on Thu Aug  9 2012
 */
#include "acgenom.h"

#include <assert.h>
#include <string.h>
#include <err.h>
#include <errno.h>
#include <stdlib.h>

#include "genom.h"


/* --- local data ---------------------------------------------------------- */

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
  svckind kind;
  comp_s component;

  hash_s params;
  idltype_s type;
};

tloc		remote_loc(remote_s r) { assert(r); return r->loc; }
const char *	remote_name(remote_s r) { assert(r); return r->name; }
svckind		remote_kind(remote_s r) { assert(r); return r->kind; }
comp_s		remote_comp(remote_s r) { assert(r); return r->component; }
idltype_s	remote_type(remote_s r) { assert(r); return r->type; }
hash_s		remote_params(remote_s r) { assert(r); return r->params; }


/* --- service_create ------------------------------------------------------ */

/** create a service in component
 */
service_s
service_create(tloc l, svckind kind, const char *name, hash_s params,
               hash_s props)
{
  comp_s comp = comp_active();
  remote_s remote;
  port_s port;
  hiter i, j;
  service_s s;
  task_s t;
  codel_s c;
  prop_s p;
  int e;
  assert(comp && name && params);

  /* check for reserved names */
  if (!strcmp(name, ALL_SERVICE_NAME)) {
    parserror(l, "'%s' is a reserved %s name", service_strkind(kind), name);
    return NULL;
  }

  /* check for already used names */
  port = comp_port(comp, name);
  remote = comp_remote(comp, name);
  if (port || remote) {
    parserror(l, "redefinition of '%s'", name);
    if (port)
      parsenoerror(port_loc(port), " port '%s' declared here", port_name(port));
    if (remote)
      parsenoerror(remote_loc(remote),
                   " remote '%s' declared here", remote_name(remote));
    errno = EEXIST;
    return NULL;
  }

  /* reopen existing service */
  s = comp_service(comp, name);
  if (s) {
    if (s->kind != kind) {
      parserror(l, "%s %s conflicts with previous declaration",
                service_strkind(kind), name);
      parsenoerror(s->loc, " %s %s first declared here",
                   service_strkind(s->kind), name);
      return NULL;
    }

    /* check parameters */
    e = 0;
    for(hash_first(params, &i), hash_first(s->params, &j);
        i.current && j.current;
        hash_next(&i), hash_next(&j)) {
      if (strcmp(param_name(i.value), param_name(j.value))) {
        parserror(param_loc(i.value), "parameter %s previously declared as %s",
                  param_name(i.value), param_name(j.value));
        parsenoerror(param_loc(j.value), " parameter %s first declared here",
                     param_name(j.value));
        e = 1;
        continue;
      }
      if (!param_equal(i.value, j.value)) {
        parserror(param_loc(i.value),
                  "parameter %s conflicts with previous declaration",
                  param_name(i.value));
        parsenoerror(param_loc(j.value), " parameter %s first declared here",
                     param_name(j.value));
        e = 1;
        continue;
      }
    }
    for(;i.current; hash_next(&i)) {
      parserror(param_loc(i.value), "extraneous parameter %s",
                param_name(i.value));
      parsenoerror(service_loc(s), " service %s first declared here",
                   s->name);
      e = 1;
    }
    for(;j.current; hash_next(&j)) {
      parserror(param_loc(i.value), "missing parameter %s",
                param_name(j.value));
      parsenoerror(service_loc(s), " service %s first declared here",
                   s->name);
      e = 1;
    }
    if (e) return NULL;

    /* merge properties */
    if (props) {
      if (prop_merge_list(s->props, props, 0/*ignore dups*/))
        return NULL;
    }
    props = s->props;
    hash_destroy(s->fsm, 1);
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
            case P_IDS: case P_LOCAL: break;

            case P_PORT: case P_REMOTE:
	      parserror(
                prop_loc(j.value), "%s %s is not allowed in %s codel %s",
                param_strsrc(param_src(j.value)),
                port_name(param_port(j.value)),
                prop_kind(i.value) == PROP_VALIDATE ? "validate" : "simple",
                codel_name(c));
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
      case PROP_SCHEDULING: case PROP_STACK: case PROP_EXTENDS:
      case PROP_PROVIDES: case PROP_USES:
	parserror(prop_loc(i.value), "property %s is not suitable for %ss",
                  service_strkind(kind), prop_strkind(prop_kind(i.value)));
	e = 1; break;
    }

  /* check parameters */
  for(hash_first(params, &i); i.current; hash_next(&i)) {
    switch(param_src(i.value)) {
      case P_NOSRC: case P_PORT: case P_REMOTE: assert(0);
      case P_IDS: if (kind == S_ATTRIBUTE) break; else assert(0);
      case P_LOCAL: if (kind != S_ATTRIBUTE) break; else assert(0);
    }
  }
  if (e) return NULL;

  /* create */
  if (!s) {
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

  /* register */
  e = hash_insert(comp_services(comp), name, s, (hrelease_f)service_destroy);
  switch(e) {
    case 0:
    case EEXIST:
      xwarnx("%s %s %s in %s %s",
             e?"updated":"created", service_strkind(s->kind), s->name,
             comp_strkind(comp_kind(comp)), comp_name(comp));
      break;

    default:
      free(s);
      return NULL;
  }

  return s;
}


/* --- service_check ------------------------------------------------------- */

/** sanity checks for a service
 */
int
service_check(service_s service)
{
  hiter i, j;
  int e = 0;

  /* check service names in interrupts, before and after properties */
  for(hash_first(service->props, &i); i.current; hash_next(&i)) {
    switch(prop_kind(i.value)) {
      case PROP_PERIOD: case PROP_DELAY: case PROP_PRIORITY:
      case PROP_SCHEDULING: case PROP_STACK: case PROP_DOC: case PROP_IDS:
      case PROP_VERSION: case PROP_LANG: case PROP_EMAIL:
      case PROP_REQUIRE: case PROP_CODELS_REQUIRE: case PROP_CLOCKRATE:
      case PROP_TASK: case PROP_VALIDATE: case PROP_SIMPLE_CODEL:
      case PROP_FSM_CODEL: case PROP_THROWS: case PROP_EXTENDS:
      case PROP_PROVIDES: case PROP_USES:
        break;

      case PROP_INTERRUPTS: case PROP_BEFORE: case PROP_AFTER:
        for(hash_first(prop_hash(i.value), &j); j.current; hash_next(&j)) {
          if (strcmp(j.value, ALL_SERVICE_NAME) &&
              !comp_service(service->component, j.value)) {
            parserror(prop_loc(j.value), "no such service '%s'", j.value);
            e = 1;
          }
        }
        break;
    }
  }
  return e;
}


/* --- service_clone ------------------------------------------------------- */

/** clone a service
 */
service_s
service_clone(service_s service)
{
  service_s s;
  hash_s prop, param;
  param_s p;
  hiter i;
  int e;

  /* clone parameters */
  e = 0;
  param = hash_create("parameter list", 0);
  for(hash_first(service_params(service), &i); i.current; hash_next(&i)) {
    p = param_clone(i.value);
    if (!p || hash_insert(param, i.key, p, (hrelease_f)param_destroy)) {
      e = 1; continue;
    }
  }
  if (e) return NULL;

  /* clone properties */
  prop = hash_create("property list", 0);
  if (!prop || prop_merge_list(prop, service_props(service), 0/*ignore_dup*/))
    return NULL;

  /* create */
  s = service_create(service->loc, service->kind, service->name, param, prop);
  if (!s) {
    hash_destroy(prop, 1);
    hash_destroy(param, 1);
  }
  return s;
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


/* --- comp_addremote ------------------------------------------------------ */

/** create a remote in component
 */
remote_s
remote_create(tloc l, svckind kind, const char *name, hash_s params)
{
  comp_s comp = comp_active();
  service_s service;
  port_s port;
  remote_s r;
  int e;
  assert(comp && name && params);

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
    errno = EEXIST;
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
  r->kind = kind;
  r->component = comp;
  r->params = params;

  /* create underlying type */
  assert(scope_current() == comp_scope(comp));
  r->type = type_newremote(l, name, r);
  if (!r->type) { remote_destroy(r); return NULL; }

  /* register */
  e = hash_insert(comp_remotes(comp), name, r, (hrelease_f)remote_destroy);
  switch(e) {
    case 0: break;

    case EEXIST:
      parserror(l, "duplicate remote %s", name);
      remote_s u = comp_remote(comp, name);
      if (u) parserror(u->loc, " remote %s declared here", u->name);
      /*FALLTHROUGH*/
    default:
      free(r);
      return NULL;
  }

  xwarnx("created remote %s in %s %s",
         r->name, comp_strkind(comp_kind(comp)), comp_name(comp));
  return r;
}


/* --- remote_clone -------------------------------------------------------- */

/** clone a remote
 */
remote_s
remote_clone(remote_s remote)
{
  remote_s r;
  hash_s param;
  param_s p;
  hiter i;
  int e;

  /* clone parameters */
  e = 0;
  param = hash_create("parameter list", 0);
  for(hash_first(remote_params(remote), &i); i.current; hash_next(&i)) {
    p = param_clone(i.value);
    if (!p || hash_insert(param, i.key, p, (hrelease_f)param_destroy)) {
      e = 1; continue;
    }
  }
  if (e) return NULL;

  /* create */
  r = remote_create(remote->loc, remote->kind, remote->name, param);
  if (!r) hash_destroy(param, 1);
  return r;
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
