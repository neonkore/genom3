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
#include <string.h>
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
    unsigned long length;	/**< string */
    hash_s members;		/**< enum */
    struct idltype_s *parent;	/**< enumerator */
    struct idltype_s *alias;
    struct {
      struct idltype_s *type;
      cval value;
    };				/**< const */
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

/** Create a new type and register in the current scope if name is not null
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
  t->name = name ? string(name) : NULL;
  t->fullname = name ? strings(scope_fullname(s), "::", name, NULL) : NULL;
  t->members = NULL;
  t->parent = NULL;
  t->alias = NULL;
  t->value = (cval){ 0 };

  /* don't register anything for anon types */
  if (!name) return t;

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
type_newbasic(tloc l, const char *name, idlkind k)
{
  return type_new(l, k, name);
}

idltype_s
type_newstring(tloc l, const char *name, unsigned long len)
{
  idltype_s t = type_new(l, IDL_STRING, name);
  if (!t) return NULL;

  t->length = len;
  return t;
}

idltype_s
type_newconst(tloc l, const char *name, idltype_s t, cval v)
{
  idltype_s c;
  int s;
  assert(t);

  s = const_cast(&v, t);
  switch(s) {
    case EINVAL:
      parserror(l, "%s%s%s is not a valid constant type",
		type_strkind(type_kind(t)),
		type_name(t)?" ":"", type_name(t)?type_name(t):"");
      parsenoerror(type_loc(t), "  %s%s%s declared here",
		   type_strkind(type_kind(t)),
		   type_name(t)?" ":"", type_name(t)?type_name(t):"");
      break;

    case EDOM:
      parserror(l, "cannot convert constant expression to %s%s%s",
		type_strkind(type_kind(t)),
		type_name(t)?" ":"", type_name(t)?type_name(t):"");
      break;
  }
  if (s) return NULL;

  c = type_new(l, IDL_CONST, name);
  if (!c) return NULL;

  c->type = t;
  c->value = v;
  return c;
}

idltype_s
type_newenum(tloc l, const char *name, hash_s enumerators)
{
  idltype_s t = type_new(l, IDL_ENUM, name);
  hiter i;
  if (!t) return NULL;

  if ((t->members = enumerators))
    for(hash_first(t->members, &i); i.current; hash_next(&i)) {
      assert(((idltype_s)i.value)->kind == IDL_ENUMERATOR);
      assert(((idltype_s)i.value)->parent == NULL);
      ((idltype_s)i.value)->parent = t;
    }

  return t;
}

idltype_s
type_newenumerator(tloc l, const char *name)
{
  idltype_s t = type_new(l, IDL_ENUMERATOR, name);
  if (!t) return NULL;

  t->parent = NULL;
  return t;
}


/* --- type_destroy -------------------------------------------------------- */

/** Destroy type
 */
void
type_destroy(idltype_s t)
{
  if (!t) return;

  if (t->name) {
    hash_remove(htypes, t->fullname, 0);
    (void)scope_deltype(t->scope, t);

    xwarnx("destroyed %s type %s", type_strkind(t->kind), t->fullname);
  }
  free(t);
}


/* --- type_equal ---------------------------------------------------------- */

/** Return true if types represent the same final type
 */
int
type_equal(idltype_s a, idltype_s b)
{
  assert(a && b);
  a = type_final(a);
  b = type_final(b);
  if (type_kind(a) != type_kind(b)) return 0;

  switch(type_kind(a)) {
    case IDL_BOOL: case IDL_USHORT: case IDL_SHORT: case IDL_ULONG:
    case IDL_LONG: case IDL_FLOAT: case IDL_DOUBLE: case IDL_CHAR:
    case IDL_OCTET: case IDL_ANY:
      return 1;

    case IDL_STRING:
      return a->length == b->length;

    case IDL_ENUMERATOR:
    case IDL_ENUM:
      if (!a->fullname || !b->fullname) return 0;
      return strcmp(a->fullname, b->fullname)?0:1;

    case IDL_ARRAY: case IDL_SEQUENCE:
    case IDL_STRUCT: case IDL_UNION:

    case IDL_CONST:
    case IDL_TYPEDEF:
      /* not a valid return from type_final() */
      break;
  }

  assert(0);
  return 0;
}


/* --- type_find ----------------------------------------------------------- */

/** Return IDL object associated to name (may be a qualified name)
 */
idltype_s
type_find(const char *name)
{
  char *c;
  scope_s s;
  idltype_s t;
  assert(name);

  if (!htypes) return NULL;

  /* if the name starts with ::, direct look */
  if (!strncmp(name, "::", 2))
    return hash_find(htypes, name);

  /* otherwise, look in the current hierarchy */
  for(s = scope_current(); s; s = scope_parent(s)) {
    c = strings(scope_fullname(s), "::", name, NULL);
    t = hash_find(htypes, c);
    if (t) return t;
  }

  errno = ENOENT;
  return NULL;
}


/* --- type_final ---------------------------------------------------------- */

/** Return the actual basic type of a type (resolve typedefs and consts)
 */
idltype_s
type_final(idltype_s t)
{
  assert(t);
  switch(type_kind(t)) {
    case IDL_BOOL: case IDL_USHORT: case IDL_SHORT: case IDL_ULONG:
    case IDL_LONG: case IDL_FLOAT: case IDL_DOUBLE: case IDL_CHAR:
    case IDL_OCTET: case IDL_STRING: case IDL_ANY: case IDL_ENUM:
    case IDL_ENUMERATOR: case IDL_ARRAY: case IDL_SEQUENCE:
    case IDL_STRUCT: case IDL_UNION:
      return t;

    case IDL_CONST:
      return type_final(t->type);

    case IDL_TYPEDEF:
      return type_final(t->alias);
  }

  assert(0);
  return 0;
}


/* --- type_constvalue ----------------------------------------------------- */

/** For consts, return the value
 */
cval
type_constvalue(idltype_s t)
{
  assert(t && t->kind == IDL_CONST);
  return t->value;
}


/* --- type_type_enumeratorenum -------------------------------------------- */

/** For enumerators, return the enum parent type
 */
idltype_s
type_enumeratorenum(idltype_s t)
{
  assert(t && t->kind == IDL_ENUMERATOR);
  return t->parent;
}


/* --- type_strkind -------------------------------------------------------- */

/** Return a type kind as a string
 */

const char *
type_strkind(idlkind k)
{
  switch(k) {
    case IDL_BOOL:		return "boolean";
    case IDL_USHORT:		return "unsigned short";
    case IDL_SHORT:		return "short";
    case IDL_ULONG:		return "unsigned long";
    case IDL_LONG:		return "long";
    case IDL_FLOAT:		return "float";
    case IDL_DOUBLE:		return "double";
    case IDL_CHAR:		return "char";
    case IDL_OCTET:		return "octect";
    case IDL_STRING:		return "string";
    case IDL_ANY:		return "any";

    case IDL_CONST:		return "const";
    case IDL_ENUM:		return "enum";
    case IDL_ENUMERATOR:	return "enumerator";
    case IDL_ARRAY:		return "array";
    case IDL_SEQUENCE:		return "sequence";
    case IDL_STRUCT:		return "struct";
    case IDL_UNION:		return "union";

    case IDL_TYPEDEF:		return "typedef";
  }

  assert(0);
  return NULL;
}
