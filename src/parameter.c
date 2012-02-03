/*
 * Copyright (c) 2010-2012 LAAS/CNRS
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
 *                                           Anthony Mallet on Tue Jun 29 2010
 */
#include "acgenom.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <err.h>

#include "genom.h"


/* --- local data ---------------------------------------------------------- */

struct param_s {
  tloc loc;
  pdir dir;
  const char *name;
  idltype_s base;	/**< base type (ids or port) */
  clist_s member;	/**< ids or port member */
  idltype_s type;	/**< member type */

  port_s port;		/**< in/out port (for port only) */

  initer_s init;	/**< initial value (or NULL) */
};

tloc		param_loc(param_s p) { assert(p); return p->loc; }
pdir		param_dir(param_s p) { assert(p); return p->dir; }
const char *	param_name(param_s p) { assert(p); return p->name; }
clist_s		param_member(param_s p) { assert(p); return p->member; }
idltype_s	param_base(param_s p) { assert(p); return p->base; }
idltype_s	param_type(param_s p) { assert(p); return p->type; }
port_s		param_port(param_s p) {
  assert(p && (p->dir == P_INPORT || p->dir == P_OUTPORT)); return p->port;
}
initer_s	param_initer(param_s p) { assert(p); return p->init; }


static param_s	param_new(tloc l, const char *name, idltype_s base);


/* --- param_new ----------------------------------------------------------- */

/** create new parameter
 */
static param_s
param_new(tloc l, const char *name, idltype_s base)
{
  param_s p;
  assert(name && base);

  p = malloc(sizeof(*p));
  if (!p) {
    warnx("memory exhausted, cannot store parameter '%s'", name);
    return NULL;
  }
  p->loc = l;
  p->dir = P_NODIR;
  p->name = string(name);
  p->base = base;
  p->member = NULL;
  p->type = base;

  p->port = NULL;
  p->init = NULL;

  return p;
}

param_s
param_newids(tloc l, const char *name, const char *member)
{
  param_s p;
  comp_s c;
  idltype_s ids;

  assert(name);
  c = comp_active(); assert(c);
  ids = comp_ids(c);
  if (!ids) {
    parserror(l, "component %s has no ids", comp_name(c));
    errno = EINVAL; return NULL;
  }

  p = param_new(l, name, ids);
  if (p) {
    xwarnx("created ids parameter %s", p->name);
    if (member) {
      cval m = { .k = CST_STRING, { .s = member } };
      if (param_setmember(p, m)) { free(p); return NULL; }
    }
  }
  return p;
}

param_s
param_newport(tloc l, const char *name)
{
  param_s p;
  comp_s c;
  port_s port;

  assert(name);
  c = comp_active(); assert(c);
  port = comp_port(c, name);
  if (!port) {
    parserror(l, "unknown port '%s'", name);
    errno = EINVAL; return NULL;
  }

  p = param_new(l, name, port_type(port));
  if (p) {
    p->port = port;
    xwarnx("created port parameter %s", p->name);
  }
  return p;
}


/* --- param_clone --------------------------------------------------------- */

/** clone a parameter
 */
param_s
param_clone(param_s param)
{
  param_s p;
  citer i;
  int s;
  assert(param);

  switch (param_dir(param)) {
    case P_IN: case P_OUT: case P_INOUT:
      p = param_newids(param_loc(param), param_name(param), NULL);
      break;

    case P_INPORT: case P_OUTPORT:
      p = param_newport(param_loc(param), param_name(param));
      break;

    default: assert(0);
  }
  if (!p) return NULL;

  for(clist_first(param_member(param), &i); i.value; clist_next(&i)) {
    s = param_setmember(p, *i.value);
    if (s) { free(p); return NULL; }
  }

  if (param_setdir(p, param_dir(param))) return NULL;
  if (param_setinitv(param_loc(param), p, param_initer(param))) return NULL;

  xwarnx("created parameter %s", p->name);
  return p;
}


/* --- param_setname ------------------------------------------------------- */

/** rename parameter.
 */
int
param_setname(param_s p, const char *name)
{
  assert(p && name);
  xwarnx("renamed parameter %s to %s", p->name, name);
  p->name = string(name);
  return 0;
}


/* --- param_setdir -------------------------------------------------------- */

/** set parameter direction.
 */
int
param_setdir(param_s p, pdir dir)
{
  assert(p && p->dir == P_NODIR);

  p->dir = dir;
  return 0;
}


/* --- param_setinitv ------------------------------------------------------ */

/** set parameter default value
 */
int
param_setinitv(tloc l, param_s p, initer_s i)
{
  int s;
  assert(p && !p->init);
  if (!i) return 0;

  s = initer_matchtype(l, p->type, i);
  if (s) return s;

  p->init = i;
  return 0;
}


/* --- param_typeiniter ---------------------------------------------------- */

/** return initializer for type, which must be a member of the type of the
 * parameter.
 */
initer_s
param_typeiniter(param_s p, idltype_s t)
{
  assert(p && t);
  if (!p->init) return NULL;
  return initer_typeiniter(p->init, p->type, t);
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


/* --- param_setmember ----------------------------------------------------- */

/** update parameter to become a member of its current type
 */
int
param_setmember(param_s p, cval m)
{
  uint32_t d;
  idltype_s t;

  assert(p);
  assert(p->type);

  switch(m.k) {
    case CST_UINT: /* array element */
      switch(type_kind(type_final(p->type))) {
	case IDL_ARRAY: case IDL_SEQUENCE:
	  t = type_type(type_final(p->type));
	  d = type_length(type_final(p->type));
	  if (t) break;
	default:
	  parserror(p->loc, "%s %s is scalar",
		    type_strkind(type_kind(p->type)), type_fullname(p->type));
	  parsenoerror(type_loc(p->type), " %s %s declared here",
		       type_strkind(type_kind(p->type)),
		       type_fullname(p->type));
	  return errno = EINVAL;
      }
      if (m.u >= d) {
	parserror(p->loc, "element %d out of bounds in %s %s", m.u,
		  type_strkind(type_kind(p->type)), type_fullname(p->type));
      }
      break;

    case CST_STRING:
      t = type_member(p->type, m.s);
      if (!t) {
	parserror(p->loc, "unknown member '%s' in %s %s",
		  m.s, type_strkind(type_kind(p->type)),
		  type_fullname(p->type));
	parsenoerror(type_loc(p->type), " %s %s declared here",
		     type_strkind(type_kind(p->type)), type_fullname(p->type));
	return errno = ENOENT;
      }
      if (type_kind(t) == IDL_MEMBER || type_kind(t) == IDL_CASE)
        t = type_type(t);
      break;

    default:
      assert(0);
  }

  p->type = t;
  p->member = clist_append(p->member, m, 0/*!unique*/);

  xwarnx("set parameter %s to %s %s", p->name,
	 type_strkind(type_kind(p->type)), type_fullname(p->type));
  return 0;
}


/* --- param_strdir ------------------------------------------------------- */

/** Return a parameter direction as a string
 */

const char *
param_strdir(pdir d)
{
  switch(d) {
    case P_NODIR:		return "directionless";
    case P_IN:			return "in";
    case P_OUT:			return "out";
    case P_INOUT:		return "inout";
    case P_INPORT:		return "inport";
    case P_OUTPORT:		return "outport";
  }

  assert(0);
  return NULL;
}
