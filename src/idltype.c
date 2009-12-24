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
    struct {
      idltype_s type;		/**< sequence, const, enumerator, typedef,
				 * member, case, union */
      union {
	unsigned long length;	/**< string, sequence */
	cval value;		/**< const */
	clist_s values;		/**< case */
	scope_s elems;		/**< struct, union */
      };
    };
    hash_s members;		/**< enum */
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
  t->type = NULL;
  t->members = NULL;
  t->length = 0;
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
type_newsequence(tloc l, const char *name, idltype_s t, unsigned long len)
{
  idltype_s c = type_new(l, IDL_SEQUENCE, name);
  if (!c) return NULL;
  assert(t);

  c->type = t;
  c->length = len;
  return c;
}

idltype_s
type_newconst(tloc l, const char *name, idltype_s t, cval v)
{
  idltype_s c;
  assert(t);

  if (const_cast(l, &v, t)) return NULL;

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
      assert(((idltype_s)i.value)->type == NULL);
      ((idltype_s)i.value)->type = t;
    }

  return t;
}

idltype_s
type_newenumerator(tloc l, const char *name)
{
  idltype_s t = type_new(l, IDL_ENUMERATOR, name);
  if (!t) return NULL;

  t->type = NULL;
  return t;
}

idltype_s
type_newarray(tloc l, const char *name, idltype_s t, unsigned long len)
{
  idltype_s c = type_new(l, IDL_ARRAY, name);
  if (!c) return NULL;
  assert(t);

  c->type = t;
  c->length = len;
  return c;
}

idltype_s
type_newstruct(tloc l, const char *name, scope_s s)
{
  idltype_s t = type_new(l, IDL_STRUCT, name);
  if (!t) return NULL;
  assert(s);

  t->elems = s;
  return t;
}

idltype_s
type_newmember(tloc l, const char *name, idltype_s t)
{
  idltype_s c = type_new(l, IDL_MEMBER, name);
  if (!c) return NULL;
  assert(t);

  c->type = t;
  return c;
}

idltype_s
type_newunion(tloc l, const char *name, idltype_s t, scope_s s)
{
  hiter i;
  citer j;
  idltype_s c;
  assert(t && s);

  /* check that all case labels match the switch type */
  for(scope_firstype(s, &i); i.current; scope_nextype(&i)) {
    c = i.value; if (c->kind != IDL_CASE) continue;
    for(clist_first(c->values, &j); j.value; clist_next(&j))
      if (j.value->k != CST_VOID && const_cast(type_loc(c), j.value, t)) {
	parserror(type_loc(c), "case label incompatible with switch");
	return NULL;
      }
  }

  /* switch type must not clash with members */
  if (type_name(t) && (c = scope_findtype(s, type_name(t)))) {
    parserror(type_loc(c), "%s %s clashes with switch type %s",
	      type_strkind(type_kind(c)), type_name(c), type_name(t));
    parsenoerror(l, "  %s used here", type_name(t));
    return NULL;
  }

  c = type_new(l, IDL_UNION, name);
  if (!c) return NULL;
  c->type = t;
  c->elems = s;
  return c;
}

idltype_s
type_newcase(tloc l, const char *name, idltype_s t, clist_s c)
{
  hiter i;
  citer j, k;
  idltype_s u;
  assert(t && c);

  /* check case values unicity in current scope - not a very efficient
   * algorithm but there won't usually be hundreds of elements in the lists. */
  for(scope_firstype(scope_current(), &i); i.current; scope_nextype(&i)) {
    u = i.value; if (u->kind != IDL_CASE) continue;
    for(clist_first(u->values, &j); j.value; clist_next(&j))
      for(clist_first(c, &k); k.value; clist_next(&k))
	if (const_equal(*k.value, *j.value)) {
	  parserror(l, "duplicate %s label",
		    k.value->k == CST_VOID?"default":"case");
	  parsenoerror(type_loc(u), "  %s label already defines %s %s",
		       k.value->k == CST_VOID?"default":"case",
		       type_strkind(IDL_CASE), u->name);
	  return NULL;
	}
  }

  u = type_new(l, IDL_CASE, name);
  if (!u) return NULL;
  u->type = t;
  u->values = c;
  return u;
}

idltype_s
type_newalias(tloc l, const char *name, idltype_s t)
{
  idltype_s c = type_new(l, IDL_TYPEDEF, name);
  if (!c) return NULL;
  assert(t);

  c->type = t;
  return c;
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

    case IDL_CASE: case IDL_MEMBER: case IDL_CONST: case IDL_TYPEDEF:
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

    case IDL_CASE: case IDL_MEMBER: case IDL_CONST: case IDL_TYPEDEF:
      return type_final(t->type);
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
  return t->type;
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
    case IDL_MEMBER:		return "struct member";
    case IDL_UNION:		return "union";
    case IDL_CASE:		return "union member";

    case IDL_TYPEDEF:		return "typedef";
  }

  assert(0);
  return NULL;
}
