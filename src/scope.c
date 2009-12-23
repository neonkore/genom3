/*
 * Copyright (c) 2009 LAAS/CNRS
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
 *                                           Anthony Mallet on Tue Dec 22 2009
 */
#include "acgenom.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <err.h>

#include "genom.h"


/* --- local data ---------------------------------------------------------- */

/** IDL scope definition */
struct scope_s {
  tloc loc;
  const char *name;
  const char *fullname;

  hash_s idltypes;

  struct scope_s *parent;
  hash_s children;
};

const char *
scope_name(scope_s s) { assert(s); return s->name; }
const char *
scope_fullname(scope_s s) { assert(s); return s->fullname; }
scope_s
scope_parent(scope_s s) { assert(s); return s->parent; }

/** the global IDL scope */
static scope_s global = NULL;

/** the current IDL scope */
static scope_s current = NULL;


static scope_s	scope_new(tloc l, const char *name, const char *pname);


/* --- scope_current ------------------------------------------------------- */

/** Get current scope
 */
scope_s
scope_current()
{
  assert(current);
  return current;
}

scope_s
scope_global()
{
  assert(global);
  return global;
}


/* --- scope_addtype ------------------------------------------------------- */

/** Register type in the given scope
 */
int
scope_addtype(scope_s s, idltype_s t)
{
  scope_s c;
  idltype_s o;
  int e;
  assert(s); assert(t);

  /* check name does not clash with either parent/children scopes or types */
  if (!strcmp(type_name(t), s->name)) {
    parserror(type_loc(t),
	      "declaration of '%s' clashes with name of enclosing module",
	      type_name(t));
    parsenoerror(s->loc, "  module '%s' declared here", s->name);
    return errno = EEXIST;
  }

  o = hash_find(s->idltypes, type_name(t));
  if (o) {
    parserror(type_loc(t),
	      "declaration of '%s' clashes with existing %s",
	      type_name(t), type_strkind(type_kind(o)));
    parsenoerror(type_loc(o), "  %s '%s' declared here",
		 type_strkind(type_kind(o)), type_name(o));
    return errno = EEXIST;
  }

  c = hash_find(s->children, type_name(t));
  if (c) {
    parserror(type_loc(t),
	      "declaration of '%s' clashes with existing module name",
	      type_name(t));
    parsenoerror(c->loc, "  module '%s' declared here", c->name);
    return errno = EEXIST;
  }

  /* register type (don't pass a 'release' function as we're not owner) */
  e = hash_insert(s->idltypes, type_name(t), t, NULL);
  if (e) return errno;

  type_setscope(t, s);
  xwarnx("registered type %s in %s scope", type_name(t),
	 s->fullname[0]?s->fullname:"global");
  return 0;
}


/* --- scope_deltype ------------------------------------------------------- */

/** Delete type from the given scope
 */
int
scope_deltype(scope_s s, idltype_s t)
{
  int e;

  if (!t) return 0;
  e = hash_remove(s->idltypes, type_name(t), 0);
  type_setscope(t, NULL);

  xwarnx("removed type %s from %s scope", type_name(t),
	 s->fullname[0]?s->fullname:"global");
  return e;
}


/* --- scope_push ---------------------------------------------------------- */

/** Create new scope in the current scope
 */
scope_s
scope_push(tloc l, const char *name)
{
  int e;
  scope_s s;
  idltype_s t;

  assert(name);
  assert(current);

  /* check name does not clash with either parent scope or types */
  if (!strcmp(name, current->name)) {
    parserror(l, "declaration of '%s' clashes with name of enclosing module",
	      name);
    parsenoerror(current->loc, "  module '%s' declared here", current->name);
    return NULL;
  }

  t = hash_find(current->idltypes, name);
  if (t) {
    parserror(l, "declaration of '%s' clashes with existing %s",
	      name, type_strkind(type_kind(t)));
    parsenoerror(type_loc(t), "  %s '%s' declared here",
		 type_strkind(type_kind(t)), type_name(t));
    return NULL;
  }

  /* scope might already exist in children */
  s = hash_find(current->children, name);
  if (s) {
    xwarnx("reopened %s scope", s->fullname);
    return current = s;
  }

  /* create */
  s = scope_new(l, name, current->fullname);
  if (!s) return NULL;
  s->parent = current;
  e = hash_insert(current->children, name, s, (hrelease_f)scope_destroy);
  if (e) { scope_destroy(s); return NULL; }

  xwarnx("pushed %s scope", s->fullname);
  return current = s;
}


/* --- scope_pop ----------------------------------------------------------- */

/** Pop current scope (and return it)
 */
scope_s
scope_pop()
{
  scope_s s;

  /* global scope cannot be popped */
  if (!current->parent) return current;

  s = current;
  current = current->parent;
  xwarnx("popped %s scope", s->fullname);
  return s;
}


/* --- scope_pushglobal ---------------------------------------------------- */

/** Create global scope
 */
int
scope_pushglobal()
{
  assert(!global);
  global = scope_new((tloc){ .file=NULL, .line = 1, .col = 1 }, "", NULL);
  if (!global) return ENOMEM;

  global->parent = NULL;
  current = global;

  xwarnx("pushed global scope");
  return 0;
}


/* --- scope_destroy ------------------------------------------------------- */

void
scope_destroy(scope_s s)
{
  hiter i;
  if (!s) return;

  /* remove from parent table */
  if (s->parent) hash_remove(s->parent->children, s->name, 0);

  /* destroy registered types */
  for(hash_first(s->idltypes, &i); i.current; hash_next(&i))
    type_destroy(i.value);
  hash_destroy(s->idltypes);

  /* remove from children */
  for(hash_first(s->children, &i); i.current; hash_next(&i))
    ((scope_s)i.value)->parent = NULL;
  hash_destroy(s->children);

  xwarnx("destroyed %s scope", s->fullname[0]?s->fullname:"global");
  free(s);
}


/* --- scope_new ----------------------------------------------------------- */

static scope_s
scope_new(tloc l, const char *name, const char *pname)
{
  scope_s s;

  assert(name);

  s = malloc(sizeof(*s));
  if (!s) {
    warnx("memory exhausted, cannot create %s scope", name[0]?name:"global");
    return NULL;
  }

  s->loc = l;
  s->name = string(name);
  s->fullname = pname?strings(pname, "::", name, NULL):s->name;
  s->idltypes = hash_create(
    strings(s->fullname[0]?s->fullname:"global", " types", NULL), 15);
  if (!s->idltypes) {
    free(s);
    warnx("memory exhausted, cannot create %s scope", name[0]?name:"global");
    return NULL;
  }

  s->parent = NULL;
  s->children = hash_create(
    strings("scope children of ",
	    s->fullname[0]?s->fullname:"global", " scope", NULL),
    2);
  if (!s->children) {
    hash_destroy(s->idltypes);
    free(s);
    warnx("memory exhausted, cannot create %s scope", name[0]?name:"global");
    return NULL;
  }

  return s;
}
