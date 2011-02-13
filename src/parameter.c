/*
 * Copyright (c) 2010 LAAS/CNRS
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

  clist_s member;	/**< ids or port member */
  idltype_s base;	/**< base type (ids or port) */
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


static int	param_setelement(param_s p, unsigned int e);
static int	param_setmember(param_s p, const char *name);


/* --- param_new ----------------------------------------------------------- */

/** create new parameter
 */
param_s
param_new(tloc l, const char *name, clist_s member)
{
  param_s p;
  assert(name);

  p = malloc(sizeof(*p));
  if (!p) {
    warnx("memory exhausted, cannot store parameter '%s'", name);
    return NULL;
  }
  p->loc = l;
  p->dir = P_NODIR;
  p->name = string(name);
  p->member = member;

  p->base = NULL;
  p->type = NULL;
  p->port = NULL;
  p->init = NULL;

  xwarnx("created parameter %s", p->name);
  return p;
}


/* --- param_clone --------------------------------------------------------- */

/** clone a parameter
 */
param_s
param_clone(param_s param)
{
  param_s p;
  assert(param);

  p = malloc(sizeof(*p));
  if (!p) {
    warnx("memory exhausted, cannot store parameter '%s'", param_name(param));
    return NULL;
  }
  p->loc = param_loc(param);
  p->dir = P_NODIR;
  p->name = param_name(param);
  p->member = param_member(param);

  p->base = NULL;
  p->type = NULL;
  p->port = NULL;
  p->init = NULL;

  if (param_dir(param) != P_NODIR &&
      param_setdir(p, param_dir(param))) return NULL;
  if (param_setinitv(param_loc(param), p, param_initer(param)))

  xwarnx("created parameter %s", p->name);
  return p;
}


/* --- param_setdir -------------------------------------------------------- */

/** set parameter direction. This function is a bit more complex than it seems,
 * because the direction implies either an IDS member, or a port. The function
 * retrieves the correct type and performs consistency checks.
 */
int
param_setdir(param_s p, pdir dir)
{
  comp_s c;
  citer i;
  int s;
  assert(p && p->dir == P_NODIR && !p->base && !p->port);

  c = comp_current(); if (!c) {
    parserror(p->loc, "missing component declaration before parameter '%s'",
	      p->name);
    return EINVAL;
  }

  /* compute base type */
  switch(dir) {
    case P_NODIR: /* must not set a direction to NODIR */ assert(0); break;

    case P_IN: case P_OUT: case P_INOUT:
      p->base = comp_ids(c); if (!p->base) {
	parserror(p->loc, "component %s has no ids", comp_name(c));
	return errno = EINVAL;
      }
      break;

    case P_INPORT: case P_OUTPORT: {
      /* pop first member for port name */
      cval k = clist_pop(&p->member);
      if (k.k == CST_VOID) {
	parserror(p->loc, "missing port name in parameter '%s' definition",
		  p->name);
	return errno = EINVAL;
      }
      assert(k.k == CST_STRING);
      p->port = comp_port(c, k.s); if (!p->port) {
	parserror(p->loc, "unknown port '%s'", p->name);
	return errno = EINVAL;
      }
      p->base = port_type(p->port);
      break;
    }
  }

  /* set member type */
  p->type = p->base;
  for(clist_first(p->member, &i); i.value; clist_next(&i)) {
    switch(i.value->k) {
      case CST_UINT:	s = param_setelement(p, i.value->u); break;
      case CST_STRING:	s = param_setmember(p, i.value->s); break;
      default:		assert(0); break;
    }
    if (s) return s;
  }

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


/* --- param_setelement ---------------------------------------------------- */

/** update parameter to become an element of its current (array, sequence) type
 */
static int
param_setelement(param_s p, unsigned int e)
{
  unsigned long d;
  idltype_s t;

  assert(p);
  assert(p->type);

  switch(type_kind(type_final(p->type))) {
    case IDL_ARRAY: case IDL_SEQUENCE:
      t = type_type(type_final(p->type));
      d = type_length(type_final(p->type));
      if (t) break;
    default:
      parserror(p->loc, "%s %s is scalar",
		type_strkind(type_kind(p->type)), type_fullname(p->type));
      parsenoerror(type_loc(p->type), " %s %s declared here",
		   type_strkind(type_kind(p->type)), type_fullname(p->type));
      return errno = EINVAL;
  }

  if (e >= d) {
    parserror(p->loc, "element %d out of bounds in %s %s", e,
	      type_strkind(type_kind(p->type)), type_fullname(p->type));
  }

  p->type = t;
  xwarnx("set parameter %s to %s %s", p->name,
	 type_strkind(type_kind(p->type)), type_fullname(p->type));
  return 0;
}


/* --- param_setmember ----------------------------------------------------- */

/** update parameter to become a member of its current type
 */
static int
param_setmember(param_s p, const char *name)
{
  idltype_s t;

  assert(p && name);
  assert(p->type);

  t = type_member(p->type, name);
  if (!t) {
    parserror(p->loc, "unknown member '%s' in %s %s",
	      name, type_strkind(type_kind(p->type)), type_fullname(p->type));
    parsenoerror(type_loc(p->type), " %s %s declared here",
		 type_strkind(type_kind(p->type)), type_fullname(p->type));
    return errno = ENOENT;
  }

  p->type = t;
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
