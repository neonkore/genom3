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

  idltype_s rettype;	/**< return type */
  hash_s params;	/**< parameter list */
  hash_s yields;	/**< transitions */
  hash_s triggers;	/**< start conditions */

  task_s task;		/**< codel's task */
  service_s service;	/**< codel's service */
};

tloc		codel_loc(codel_s c) { assert(c); return c->loc; }
const char *	codel_name(codel_s c) { assert(c); return c->name; }
idltype_s	codel_return(codel_s c) { assert(c); return c->rettype; }
hash_s		codel_params(codel_s c) { assert(c); return c->params; }
hash_s		codel_triggers(codel_s c) { assert(c); return c->triggers; }
hash_s		codel_yields(codel_s c) { assert(c); return c->yields; }
task_s *	codel_task(codel_s c) { assert(c); return &c->task; }
service_s *	codel_service(codel_s c) { assert(c); return &c->service; }


struct param_s {
  tloc loc;
  pdir dir;
  const char *name;

  clist_s member;	/**< ids or port member */
  idltype_s type;	/**< member type */
  port_s port;		/**< in/out port (for port only) */

  initer_s init;	/**< initial value (or NULL) */
};

tloc		param_loc(param_s p) { assert(p); return p->loc; }
pdir		param_dir(param_s p) { assert(p); return p->dir; }
const char *	param_name(param_s p) { assert(p); return p->name; }
clist_s		param_member(param_s p) { assert(p); return p->member; }
idltype_s	param_type(param_s p) { assert(p); return p->type; }
port_s		param_port(param_s p) {
  assert(p && (p->dir == P_INPORT || p->dir == P_OUTPORT)); return p->port;
}
initer_s	param_initer(param_s p) { assert(p); return p->init; }


/* --- codel_create -------------------------------------------------------- */

/** create new codel
 */
codel_s
codel_create(tloc l, const char *name, hash_s triggers, hash_s yields,
	     hash_s params)
{
  codel_s c;
  assert(name && params);

  c = malloc(sizeof(*c));
  if (!c) {
    warnx("memory exhausted, cannot create codel '%s'", name);
    return NULL;
  }
  c->loc = l;
  c->name = string(name);

  c->rettype = comp_eventtype(comp_dotgen());

  c->params = params;
  c->yields = yields ? yields : hash_create("yields list", 0);
  c->triggers = triggers ? triggers : hash_create("triggers list", 0);

  c->task = NULL;
  c->service = NULL;

  if (!yields) {
    /* validation codels return OK, or one of the exceptions of the service */
    if (!hash_insert(c->yields, "ok", "ok", NULL))
      comp_addievs(l, c->yields);
    else
      parserror(l, "failed to create component internal event '%s'", "ok");
  }

  xwarnx("created codel %s", c->name);
  return c;
}


/* --- param_new ----------------------------------------------------------- */

/** create new parameter
 */
static param_s
param_new(tloc l, pdir d, const char *name)
{
  param_s p;
  assert(name);

  p = malloc(sizeof(*p));
  if (!p) {
    warnx("memory exhausted, cannot store parameter '%s'", name);
    return NULL;
  }
  p->loc = l;
  p->dir = d;
  p->name = string(name);

  p->member = NULL;
  p->type = NULL;
  p->port = NULL;
  p->init = NULL;

  if (name[0])
    xwarnx("created %s parameter %s", param_strdir(p->dir), p->name);
  else
    xwarnx("created %s unnamed parameter", param_strdir(p->dir));
  return p;
}

param_s
param_newids(tloc l, pdir d, const char *name)
{
  comp_s c;
  idltype_s ids;
  param_s p;

  c = comp_dotgen(); if (!c) {
    parserror(l, "missing component declaration before parameter");
    return NULL;
  }
  ids = comp_ids(c); if (!ids) return NULL;

  p = param_new(l, d, name?name:"");
  if (!p) return NULL;

  p->type = ids;

  if (name)
    if (param_setmember(l, p, name)) { param_destroy(p); p = NULL; }
  return p;
}

param_s
param_newport(tloc l, pdir d, const char *name, port_s port)
{
  param_s p;
  assert(port);
  if (!name) name = port_name(port);
  assert(name);

  if ((port_kind(port) != PORT_IN && d == P_INPORT) ||
      (port_kind(port) != PORT_OUT && d == P_OUTPORT)) {
    parserror(l, "port '%s' is not an %s", port_name(port), param_strdir(d));
    parsenoerror(port_loc(port), " port '%s' declared here", port_name(port));
    return NULL;
  }

  p = param_new(l, d, name);
  if (!p) return NULL;

  p->type = port_type(port);
  p->port = port;
  return p;
}


/* --- param_setname ------------------------------------------------------- */

/** rename parameter
 */
int
param_setname(tloc l, param_s p, const char *name)
{
  assert(p);
  if (p->name[0])
    xwarnx("renamed parameter %s to %s", p->name, name);
  else
    xwarnx("renamed unnamed parameter to %s", name);

  p->name = string(name);
  return 0;
}


/* --- param_setmember ----------------------------------------------------- */

/** update parameter to become a member of its current type
 */
int
param_setmember(tloc l, param_s p, const char *name)
{
  idltype_s t;
  cval m;

  assert(p && name);
  assert(p->type);

  t = type_member(p->type, name);
  if (!t) {
    parserror(l, "unknown member '%s' in %s %s",
	      name, type_strkind(type_kind(p->type)), type_fullname(p->type));
    parsenoerror(type_loc(p->type), " %s %s declared here",
		 type_strkind(type_kind(p->type)), type_fullname(p->type));
    return errno = ENOENT;
  }

  m.k = CST_STRING;
  m.s = string(name);

  p->member = clist_append(p->member, m, 0/*!unique*/);
  if (!p->member) return errno;
  p->type = t;

  xwarnx("set parameter %s to %s %s", p->name,
	 type_strkind(type_kind(p->type)), type_fullname(p->type));
  return strcmp(p->name, m.s) ? param_setname(l, p, m.s) : 0;
}


/* --- param_setelement ---------------------------------------------------- */

/** update parameter to become an element of its current (array, sequence) type
 */
int
param_setelement(tloc l, param_s p, unsigned int e)
{
  unsigned long d;
  idltype_s t;
  cval m;

  assert(p);
  assert(p->type);

  switch(type_kind(type_final(p->type))) {
    case IDL_ARRAY: case IDL_SEQUENCE:
      t = type_type(type_final(p->type));
      d = type_length(type_final(p->type));
      if (t) break;
    default:
      parserror(l, "%s %s is scalar",
		type_strkind(type_kind(p->type)), type_fullname(p->type));
      parsenoerror(type_loc(p->type), " %s %s declared here",
		   type_strkind(type_kind(p->type)), type_fullname(p->type));
      return errno = EINVAL;
  }

  if (e >= d) {
    parserror(l, "element %d out of bounds in %s %s", e,
	      type_strkind(type_kind(p->type)), type_fullname(p->type));
  }

  m.k = CST_UINT;
  m.u = e;

  p->member = clist_append(p->member, m, 0/*!unique*/);
  if (!p->member) return errno;
  p->type = t;

  xwarnx("set parameter %s to %s %s", p->name,
	 type_strkind(type_kind(p->type)), type_fullname(p->type));
  return 0;
}


/* --- param_setinitv ------------------------------------------------------ */

/** set parameter default value
 */
int
param_setinitv(tloc l, param_s p, initer_s i)
{
  int s;
  assert(p && i);

  s = initer_matchtype(l, p->type, i);
  if (s) return s;

  p->init = i;
  return 0;
}


/* --- param_destroy ------------------------------------------------------- */

/** destroy parameter
 */
void
param_destroy(param_s p)
{
  if (p) {
    xwarnx("destroyed parameter %s", p->name);
    if (p->init) initer_destroy(p->init);
    free(p);
  }
}


/* --- param_strdir ------------------------------------------------------- */

/** Return a parameter direction as a string
 */

const char *
param_strdir(pdir d)
{
  switch(d) {
    case P_IN:			return "in";
    case P_OUT:			return "out";
    case P_INOUT:		return "inout";
    case P_INPORT:		return "inport";
    case P_OUTPORT:		return "outport";
  }

  assert(0);
  return NULL;
}
