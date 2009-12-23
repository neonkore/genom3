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
 *                                           Cédric Pasteur on Mon Apr 20 2009
 */
#include "acgenom.h"

#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <err.h>

#include "genom.h"


/* --- local data ---------------------------------------------------------- */

struct idltype_s {
  tloc loc;
  idlkind kind;
  const char *name;
  const char *fullname;

  scope_s scope;
  union {
    hash_s members;
    struct idltype_s *alias;
  };
};

tloc
type_loc(idltype_s t) { assert(t); return t->loc; }
const char *
type_name(idltype_s t) { assert(t); return t->name; }
idlkind
type_kind(idltype_s t) { assert(t); return t->kind; }
void
type_setscope(idltype_s t, scope_s s) { assert(t); t->scope = s; }


/** hash of all declared types */
static hash_s htypes;


static idltype_s	type_new(tloc l, idlkind k, const char *name);


/* --- type_new ------------------------------------------------------------ */

/** Create a new type in the current scope
 */
static idltype_s
type_new(tloc l, idlkind k, const char *name)
{
  idltype_s t;
  scope_s s;
  int e;

  if (!htypes) {
    htypes = hash_create("types", 32);
    if (!htypes) {
      warnx("memory exhausted, cannot store any types");
      return NULL;
    }
  }

  /* add type in the current scope */
  s = scope_current();

  t = malloc(sizeof(*t));
  if (!t) {
    warnx("memory exhausted, cannot store type '%s'", name);
    return NULL;
  }

  t->loc = l;
  t->kind = k;
  t->name = string(name);
  t->fullname = strings(scope_fullname(s), "::", name, NULL);
  t->members = NULL;
  t->alias = NULL;

  e = scope_addtype(s, t);
  if (e) { free(t); return NULL; }

  /* register type */
  e = hash_insert(htypes, t->fullname, t, (hrelease_f)type_destroy);
  if (e) {
    scope_deltype(s, t);
    free(t);
    errno = e;
    return NULL;
  }

  xwarnx("added %s type %s", type_strkind(k), t->fullname);
  return t;
}

idltype_s
type_newenum(tloc l, const char *name, hash_s enumerators)
{
  idltype_s t = type_new(l, IDL_ENUM, name);
  if (!t) return NULL;

  t->members = enumerators;
  return t;
}

idltype_s
type_newenumerator(tloc l, const char *name)
{
  return type_new(l, IDL_ENUMERATOR, name);
}


/* --- type_destroy -------------------------------------------------------- */

/** Destroy type
 */
void
type_destroy(idltype_s t)
{
  if (!t) return;

  hash_remove(htypes, t->fullname, 0);
  (void)scope_deltype(t->scope, t);

  xwarnx("destroyed %s type %s", type_strkind(t->kind), t->fullname);
  free(t);
}


/* --- type_strkind -------------------------------------------------------- */

/** Return a type kind as a string
 */

const char *
type_strkind(idlkind k)
{
  switch(k) {
    case IDL_ENUM:		return "enum";
    case IDL_ENUMERATOR:	return "enumerator";
  }

  assert(0);
  return NULL;
}
