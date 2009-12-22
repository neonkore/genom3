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
    parserror(l, "declaration of '%s' clashes with existing type name",
	      name);
    parsenoerror(type_loc(t), "  type '%s' declared here", type_name(t));
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
  e = hash_insert(current->children, name, s, scope_destroy);
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
  hash_s h;
  if (!s) return;

  if (s->parent) {
    h = s->parent->children;
    s->parent = NULL;
    hash_remove(h, s->name);
    return;
  }

  hash_destroy(s->idltypes);
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
