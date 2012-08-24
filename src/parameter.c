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
  psrc src;
  pdir dir;
  const char *name;
  idltype_s base;	/**< base type (ids, port or local type) */
  clist_s member;	/**< base type member */
  idltype_s type;	/**< member type */

  union {
    port_s port;	/**< in/out port (for port only) */
    remote_s remote;	/**< in/out remote (for remote only) */
  };

  initer_s init;	/**< initial value (or NULL) */
};

tloc		param_loc(param_s p) { assert(p); return p->loc; }
psrc		param_src(param_s p) { assert(p); return p->src; }
pdir		param_dir(param_s p) { assert(p); return p->dir; }
const char *	param_name(param_s p) { assert(p); return p->name; }
clist_s		param_member(param_s p) { assert(p); return p->member; }
idltype_s	param_base(param_s p) { assert(p); return p->base; }
idltype_s	param_type(param_s p) { assert(p); return p->type; }
port_s		param_port(param_s p) {
  assert(p && p->src == P_PORT); return p->port;
}
remote_s	param_remote(param_s p) {
  assert(p && p->src == P_REMOTE); return p->remote;
}
initer_s	param_initer(param_s p) { assert(p); return p->init; }


static param_s	param_new(tloc l, psrc src, pdir dir, const char *name,
			idltype_s base, clist_s member, initer_s initer);
static int	param_setmember(param_s p, cval m);


/** the active local parameters */
static hash_s locals = NULL;

hash_s	param_locals() { return locals; }
void	param_setlocals(hash_s h) { locals = h; }


/* --- param_new ----------------------------------------------------------- */

/** create new parameter
 */
static param_s
param_new(tloc l, psrc src, pdir dir, const char *name, idltype_s base,
          clist_s member, initer_s initer)
{
  citer i;
  param_s p;
  assert(name && base);

  p = malloc(sizeof(*p));
  if (!p) {
    warnx("memory exhausted, cannot store parameter '%s'", name);
    return NULL;
  }
  p->loc = l;
  p->src = src;
  p->dir = dir;
  p->name = string(name);
  p->base = base;
  p->member = member;
  p->type = base;

  p->port = NULL;
  p->init = initer;

  for(clist_first(member, &i); i.value; clist_next(&i))
    if (param_setmember(p, *i.value)) { free(p); return NULL; }

  if (initer)
    if (initer_matchtype(l, p->type, initer)) { free(p); return NULL; }

  return p;
}

param_s
param_newids(tloc l, pdir dir, const char *name, clist_s member,
             initer_s initer)
{
  param_s p;
  citer i;
  comp_s c;
  idltype_s ids;

  if (dir == P_NODIR) return NULL;

  /* unnamed parameters are named after the last string member */
  if (!name)
    for(clist_first(member, &i); i.value; clist_next(&i))
      if (i.value->k == CST_STRING)
        name = i.value->s;
  assert(name);

  /* sanity checks */
  c = comp_active(); assert(c);
  ids = comp_ids(c);
  if (!ids) {
    parserror(l, "component %s has no ids", comp_name(c));
    parsenoerror(l, "dropped parameter '%s'", name);
    errno = EINVAL; return NULL;
  }

  /* create param */
  p = param_new(l, P_IDS, dir, name, ids, member, initer);
  if (!p) {
    parserror(l, "dropped parameter '%s'", name);
    return NULL;
  }

  if (type_fullname(p->type))
    xwarnx("created ids parameter %s %s %s",
           type_strkind(type_kind(p->type)), type_fullname(p->type), p->name);
  else
    xwarnx("created ids parameter %s %s",
           type_strkind(type_kind(p->type)), p->name);
  return p;
}

param_s
param_newlocal(tloc l, pdir dir, const char *name, clist_s member,
               idltype_s type, initer_s initer)
{
  param_s p;
  comp_s c;
  citer i;

  if (dir == P_NODIR) return NULL;

  assert(member);
  c = comp_active(); assert(c);

  clist_first(member, &i);
  assert(i.value->k == CST_STRING);
  if (!name) name = i.value->s;
  assert(name);

  /* lookup type */
  if (!type && param_locals()) {
    hiter j;
    for(hash_first(param_locals(), &j); j.current; hash_next(&j))
      if (!strcmp(param_name(j.value), i.value->s)) {
        type = param_type(j.value);
        break;
      }
  }
  if (!type) {
    parserror(l, "no such service parameter '%s'", i.value->s);
    return NULL;
  }
  assert(type);

  /* create param */
  clist_next(&i);
  p = param_new(l, P_SERVICE, dir, name, type, i.current, initer);
  if (!p) {
    parserror(l, "dropped parameter '%s'", name);
    return NULL;
  }

  if (type_fullname(p->type))
    xwarnx("created service parameter %s %s %s",
           type_strkind(type_kind(p->type)), type_fullname(p->type), p->name);
  else
    xwarnx("created service parameter %s %s",
           type_strkind(type_kind(p->type)), p->name);

  return p;
}

param_s
param_newport(tloc l, pdir dir, const char *name, clist_s member)
{
  param_s p;
  comp_s c;
  port_s port;
  citer i;

  if (dir == P_NODIR) return NULL;

  clist_first(member, &i);
  assert(i.value->k == CST_STRING);

  if (!name) name = i.value->s;
  assert(name);

  c = comp_active(); assert(c);
  port = comp_port(c, i.value->s);
  if (!port) {
    parserror(l, "unknown port '%s'", i.value->s);
    errno = EINVAL; return NULL;
  }
  if (dir == P_OUT && port_dir(port) == PORT_IN) {
    parserror(l, "read-only port '%s'", i.value->s);
    parsenoerror(port_loc(port), " port %s declared here", port_name(port));
    errno = EINVAL; return NULL;
  }

  clist_next(&i);

  if (i.current) {
    parserror(l, "cannot access members of port %s", port_name(port));
    parsenoerror(port_loc(port), " port %s declared here", port_name(port));
    errno = EINVAL; return NULL;
  }

  p = param_new(l, P_PORT, dir, name, port_type(port), i.current, NULL);
  if (!p) {
    parserror(l, "dropped parameter '%s'", name);
    return NULL;
  }

  p->port = port;
  xwarnx("created port parameter %s", p->name);
  return p;
}

param_s
param_newremote(tloc l, pdir dir, const char *name, clist_s member)
{
  param_s p;
  comp_s c;
  remote_s remote;
  citer i;

  if (dir == P_NODIR) return NULL;

  clist_first(member, &i);
  assert(i.value->k == CST_STRING);

  if (!name) name = i.value->s;
  assert(name);

  c = comp_active(); assert(c);
  remote = comp_remote(c, i.value->s);
  if (!remote) {
    parserror(l, "unknown remote '%s'", i.value->s);
    errno = EINVAL; return NULL;
  }

  clist_next(&i);
  if (i.current) {
    parserror(l, "remote %s has no member", remote_name(remote));
    errno = EINVAL; return NULL;
  }

  if (dir != P_IN) {
    parserror(l, "remote %s is read-only and may not be an %s parameter",
              remote_name(remote), param_strdir(dir));
    errno = EINVAL; return NULL;
  }

  p = param_new(l, P_REMOTE, dir, name, remote_type(remote), NULL, NULL);
  if (!p) {
    parserror(l, "dropped parameter '%s'", name);
    return NULL;
  }

  p->remote = remote;
  xwarnx("created remote parameter %s", p->name);
  return p;
}

param_s
param_newcodel(tloc l, psrc src, pdir dir, const char *name, clist_s member)
{
  param_s p = NULL;

  if (dir == P_NODIR) return NULL;

  /* resolve implicit sources */
  while (src == P_NOSRC) {
    citer i;
    comp_s c;
    port_s port;
    param_s local;
    remote_s remote;
    idltype_s ids, idsmember;

    if (!member) { src = P_IDS; break; }

    /* lookup all kind of parameters */
    clist_first(member, &i);
    assert(i.value->k == CST_STRING);

    c = comp_active(); assert(c);
    idsmember = NULL;
    local = NULL;
    port = comp_port(c, i.value->s);
    remote = comp_remote(c, i.value->s);
    ids = comp_ids(c);
    if (ids) idsmember = type_member(ids, i.value->s);
    if (param_locals()) {
      hiter j;
      for(hash_first(param_locals(), &j); j.current; hash_next(&j))
        if (!strcmp(param_name(j.value), i.value->s)) {
          local = j.value;
          break;
        }
    }

    /* raise an error in case of ambiguity */
    if (!!idsmember + !!local + !!port + !!remote > 1) {
      parserror(l, "missing source qualifier for parameter '%s'", i.value->s);
      if (idsmember)
        parsenoerror(type_loc(idsmember),
                     " ids member %s declared here", i.value->s);
      if (local)
        parsenoerror(param_loc(local),
                     " service parameter %s declared here", i.value->s);
      if (port)
        parsenoerror(port_loc(port), " port %s %s declared here",
                     port_strkind(port_kind(port)), i.value->s);
      if (remote)
        parsenoerror(remote_loc(remote),
                     " remote %s declared here", i.value->s);
      return NULL;
    }

    if (idsmember) src = P_IDS;
    else if (local) src = P_SERVICE;
    else if (port) src = P_PORT;
    else if (remote) src = P_REMOTE;
    else {
      parserror(l, "unknown source for parameter '%s'", i.value->s);
      return NULL;
    }
  }

  switch(src) {
    case P_IDS:
      p = param_newids(l, dir, name, member, NULL);
      break;

    case P_SERVICE:
      if (!member) {
        parserror(l, "unnamed service parameter '%s'", name);
        return NULL;
      }
      p = param_newlocal(l, dir, name, member, NULL, NULL);
      break;

    case P_PORT:
      if (!member) {
        parserror(l, "unnamed port parameter '%s'", name);
        return NULL;
      }
      p = param_newport(l, dir, name, member);
      break;

    case P_REMOTE:
      if (!member) {
        parserror(l, "unnamed remote parameter '%s'", name);
        return NULL;
      }
      p = param_newremote(l, dir, name, member);
      break;

    case P_NOSRC: assert(0);
  }

  return p;
}


/* --- param_clone --------------------------------------------------------- */

/** clone a parameter
 */
param_s
param_clone(param_s param)
{
  param_s p = NULL;
  clist_s m;
  cval k;

  assert(param);
  switch (param_src(param)) {
    case P_IDS:
      p = param_newids(param_loc(param), param_dir(param), param_name(param),
                       param_member(param), param_initer(param));
      break;

    case P_SERVICE:
      m = param_member(param);
      k.k = CST_STRING; k.s = param_name(param);
      m = clist_prepend(m, k, 0);
      p = param_newlocal(param_loc(param), param_dir(param), param_name(param),
                         m, param_type(param), param_initer(param));
      break;

    case P_PORT:
      m = param_member(param);
      k.k = CST_STRING; k.s = port_name(param_port(param));
      m = clist_prepend(m, k, 0);
      p = param_newport(param_loc(param), param_dir(param), param_name(param),
                        m);
      break;

    case P_REMOTE:
      k.k = CST_STRING; k.s = remote_name(param_remote(param));
      m = clist_prepend(NULL, k, 0);
      p = param_newremote(param_loc(param), param_dir(param), param_name(param),
                        m);
      break;

    case P_NOSRC: assert(0);
  }

  return p;
}


/* --- param_equal --------------------------------------------------------- */

/** compare two parameters
 */
int
param_equal(param_s p, param_s q)
{
  if (!p || !q) return 0;

  if (param_dir(p) != param_dir(q)) return 0;
  return type_equal(param_type(p), param_type(q));
}


/** compare two lists of parameters
 */
int
param_list_equal(hash_s l, hash_s m)
{
  hiter i, j;

  for(hash_first(l, &i), hash_first(m, &j);
      i.value && j.value; hash_next(&i), hash_next(&j)) {
    if (!param_equal(i.value, j.value)) return 0;
  }
  if (i.value || j.value) return 0;
  return 1;
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
    if (p->init) initer_destroy(p->init);
    free(p);
  }
}


/* --- param_setmember ----------------------------------------------------- */

/** update parameter to become a member of its current type
 */
static int
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
  return 0;
}


/* --- param_strsrc -------------------------------------------------------- */

/** Return a parameter source as a string
 */

const char *
param_strsrc(psrc s)
{
  switch(s) {
    case P_NOSRC:		return "sourceless";

    case P_IDS:			return "ids";
    case P_SERVICE:		return "service";
    case P_PORT:		return "port";
    case P_REMOTE:		return "remote";
  }

  assert(0);
  return NULL;
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
  }

  assert(0);
  return NULL;
}
