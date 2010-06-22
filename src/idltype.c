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
				 * member, case, union, array, forward dcls */
      union {
	unsigned long length;	/**< string, sequence, array */
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
const char *
type_fullname(idltype_s t) { assert(t); return t->fullname; }
idlkind
type_kind(idltype_s t) { assert(t); return t->kind; }
scope_s
type_scope(idltype_s t) { assert(t); return t->scope; }


/** hash of all declared types */
static hash_s htypes;
hash_s
type_all() { return htypes; }

static idltype_s	type_new(tloc l, idlkind k, const char *name);


/* --- type_new ------------------------------------------------------------ */

/** Create a new type and register in the current scope if name is not null
 */
static idltype_s
type_new(tloc l, idlkind k, const char *name)
{
  idltype_s t, f;
  scope_s s;
  char *c;
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
  t->scope = s;
  t->type = NULL;
  t->members = NULL;
  t->length = 0;
  t->value = (cval){ 0 };

  /* don't register anything for anon types */
  if (!name) return t;

  /* handle forward declarations */
  c = strings(scope_fullname(s), "::", name, NULL);
  f = hash_find(htypes, c);
  if (f &&
      ((type_kind(f) == IDL_FORWARD_STRUCT && k == IDL_STRUCT) ||
       (type_kind(f) == IDL_FORWARD_UNION && k == IDL_UNION))) {
    if (f->type) {
      parserror(l, "%s %s is already defined",
		type_strkind(type_kind(f)), f->fullname);
      parsenoerror(f->loc, " %s %s defined here",
		   type_strkind(type_kind(f->type)), f->type->fullname);
      free(t);
      return NULL;
    }
    if (strcmp(f->loc.file, l.file)) {
      parserror(l, "%s type %s defined in a different source file from"
		" its forward declaration", type_strkind(k), t->fullname);
      parsenoerror(f->loc, " %s %s declared here",
		   type_strkind(type_kind(f)), f->fullname);
      free(t);
      return NULL;
    }
    f->type = t;

    /* rename forwarded declaration key */
    scope_renametype(s, f, strings("&", f->name, NULL));
    hash_rename(htypes, f->fullname, strings("&", f->fullname, NULL));

    xwarnx("finalized %s type %s", type_strkind(type_kind(f)), f->fullname);
  }
  if (f &&
      ((type_kind(f) == IDL_FORWARD_STRUCT && k == IDL_FORWARD_STRUCT) ||
       (type_kind(f) == IDL_FORWARD_UNION && k == IDL_FORWARD_UNION))) {
    free(t);
    xwarnx("reusing already declared %s type %s",
	   type_strkind(type_kind(f)), f->fullname);
    return f;
  }

  /* register type */
  e = scope_addtype(s, t);
  if (e) { free(t); return NULL; }

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
type_newforward(tloc l, const char *name, idlkind k)
{
  idltype_s t = type_new(l, k, name);
  assert(k == IDL_FORWARD_STRUCT || k == IDL_FORWARD_UNION);

  return t;
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
type_addenumerator(tloc l, idltype_s e, const char *name)
{
  idltype_s t;
  assert(e && scope_current() == e->scope);

  t = type_newenumerator(l, name);
  if (!t) return NULL;

  if (!e->members) {
    e->members = hash_create("enumerator list", 3);
    if (!e->members) goto error;
  }
  if (hash_insert(e->members, type_name(t), t, NULL)) goto error;

  t->type = e;
  return t;
error:
  type_destroy(t);
  return NULL;
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
  idltype_s t = type_new(l, IDL_STRUCT, name);;
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

  /* ok */
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
    case IDL_LONG: case IDL_ULONGLONG: case IDL_LONGLONG: case IDL_FLOAT:
    case IDL_DOUBLE: case IDL_CHAR: case IDL_OCTET: case IDL_ANY:
      return 1;

    case IDL_STRING:
      return a->length == b->length;

    case IDL_ENUMERATOR: case IDL_ENUM:
    case IDL_FORWARD_STRUCT: case IDL_FORWARD_UNION:
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


/* --- type_member --------------------------------------------------------- */

/** Return member name of the given type
 */
idltype_s
type_member(idltype_s t, const char *name)
{
  assert(t && name);
  t = type_final(t);
  if (!t) return NULL;

  /* only structs, unions and enum have members */
  switch(type_kind(t)) {
    case IDL_STRUCT: case IDL_UNION:
      t = scope_findtype(t->elems, name);
      if (!t) return NULL;
      if (type_kind(t) != IDL_MEMBER && type_kind(t) != IDL_CASE) return NULL;
      break;

    case IDL_ENUM:
      t = hash_find(t->members, name);
      if (!t) return NULL;
      break;

    default: return NULL;
  }

  return t;
}


/* --- type_first/next ----------------------------------------------------- */

/** Return the first/next member of the given type
 */
idltype_s
type_first(idltype_s t, hiter *i)
{
  assert(t && i);
  t = type_final(t);
  if (!t) return NULL;

  /* only structs, unions and enum have members */
  switch(type_kind(t)) {
    case IDL_STRUCT: case IDL_UNION:
      do {
	if (scope_firstype(t->elems, i)) return NULL;
	t = i->value;
      } while (type_kind(t) != IDL_MEMBER && type_kind(t) != IDL_CASE);
      break;

    case IDL_ENUM:
      if (hash_first(t->members, i)) return NULL;
      t = i->value;
      break;

    default: i->current = NULL; return NULL;
  }

  return t;
}

idltype_s
type_after(idltype_s t, idltype_s p, hiter *i)
{
  idltype_s l; if (!t) { i->current = NULL; return NULL; }

  for(l = type_first(t, i); l; l = type_next(i))
    if (l == p) return type_next(i);

  return NULL;
}

idltype_s
type_next(hiter *i)
{
  idltype_s t;
  assert(i); if (!i->current) return NULL;

  switch(type_kind(i->value)) {
    case IDL_MEMBER: case IDL_CASE:
      do {
	if (scope_nextype(i)) return NULL;
	t = i->value;
      } while (type_kind(t) != IDL_MEMBER && type_kind(t) != IDL_CASE);
      break;

    case IDL_ENUMERATOR:
      if (hash_next(i)) return NULL;
      t = i->value;
      break;

    default: i->current = NULL; return NULL;
  }

  return t;
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
    case IDL_LONG: case IDL_ULONGLONG: case IDL_LONGLONG: case IDL_FLOAT:
    case IDL_DOUBLE: case IDL_CHAR: case IDL_OCTET: case IDL_STRING:
    case IDL_ANY: case IDL_ENUM: case IDL_ENUMERATOR: case IDL_ARRAY:
    case IDL_SEQUENCE: case IDL_STRUCT: case IDL_UNION:
      return t;

    case IDL_FORWARD_STRUCT: case IDL_FORWARD_UNION:
      if (!t->type) return t;
      /*FALLTHROUGH*/
    case IDL_CASE: case IDL_MEMBER: case IDL_CONST: case IDL_TYPEDEF:
      return type_final(t->type);
  }

  assert(0);
  return 0;
}


/* --- type_length --------------------------------------------------------- */

/** For strings, array, sequences, return the length (-1U if no length)
 */
unsigned long
type_length(idltype_s t)
{
  assert(t);
  t = type_final(t);
  assert(t && (t->kind == IDL_STRING ||
	       t->kind == IDL_ARRAY ||
	       t->kind == IDL_SEQUENCE));
  return t->length;
}


/* --- type_discriminator -------------------------------------------------- */

/** For unions, return the discriminator
 */
idltype_s
type_discriminator(idltype_s t)
{
  assert(t);
  t = type_final(t);
  assert(t && t->kind == IDL_UNION);
  return t->type;
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


/* --- type_casevalue ------------------------------------------------------ */

/** For cases, return the values
 */
clist_s
type_casevalues(idltype_s t)
{
  assert(t && t->kind == IDL_CASE);
  return t->values;
}


/* --- type_type ----------------------------------------------------------- */

/** For enumerators, return the enum parent type. For sequences, arrays, const,
 * typedefs, structs and union members and forward declarations, return the
 * actual type.
 */
idltype_s
type_type(idltype_s t)
{
  assert(t && (
	   t->kind == IDL_ENUMERATOR		||
	   t->kind == IDL_SEQUENCE		||
	   t->kind == IDL_ARRAY			||
	   t->kind == IDL_CONST			||
	   t->kind == IDL_TYPEDEF		||
	   t->kind == IDL_MEMBER		||
	   t->kind == IDL_CASE			||
	   t->kind == IDL_FORWARD_STRUCT	||
	   t->kind == IDL_FORWARD_UNION
	   ));

  return t->type;
}


/* --- type_members -------------------------------------------------------- */

/** Return hash of member types
 */
hash_s
type_members(idltype_s t)
{
  assert(t);

  switch(type_kind(t)) {
    case IDL_STRUCT: case IDL_UNION:
      return scope_types(t->elems);

    case IDL_ENUM:
      return t->members;

    default: assert(0);
  }

  return NULL;
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
    case IDL_ULONGLONG:		return "unsigned long long";
    case IDL_LONGLONG:		return "long long";
    case IDL_FLOAT:		return "float";
    case IDL_DOUBLE:		return "double";
    case IDL_CHAR:		return "char";
    case IDL_OCTET:		return "octet";
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

    case IDL_FORWARD_STRUCT:	return "forward struct";
    case IDL_FORWARD_UNION:	return "forward union";
  }

  assert(0);
  return NULL;
}


/* --- type_usage ---------------------------------------------------------- */

/** Print information regarding types usage
 */

void
type_usage()
{
  if (htypes) hash_pstat(htypes);
}
