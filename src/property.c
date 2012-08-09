/*
 * Copyright (c) 2009-2012 LAAS/CNRS
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
 *                                           Anthony Mallet on Tue Dec 29 2009
 */
#include "acgenom.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <err.h>

#include "genom.h"


/* --- local data ---------------------------------------------------------- */

struct prop_s {
  tloc loc;
  propkind kind;
  const char *name;

  union {
    idltype_s type;	/**< IDS */
    const char *text;	/**< doc, version, e-mail, lang */
    clist_s clist;	/**< require, codels-require */
    idltype_s value;	/**< clock-rate, period, delay, priority, stack */
    codel_s codel;	/**< codel */
    hash_s hash;	/**< extends, throws */
    task_s task;	/**< task */
  };
};

tloc		prop_loc(prop_s p) { assert(p); return p->loc; }
const char *	prop_name(prop_s p) { assert(p); return p->name; }
propkind	prop_kind(prop_s p) { assert(p); return p->kind; }


/* --- prop_create --------------------------------------------------------- */

/** Create a property
 */
static prop_s
prop_new(tloc l, propkind k, const char *name)
{
  prop_s p;
  assert(name);

  p = malloc(sizeof(*p));
  if (!p) {
    warnx("memory exhausted, cannot create property");
    return NULL;
  }

  p->loc = l;
  p->kind = k;
  p->name = string(name);
  p->type = NULL;

  xwarnx("created property %s", p->name);
  return p;
}

prop_s
prop_newids(tloc l, idltype_s t)
{
  assert(t);
  if (type_kind(t) == IDL_CONST) {
    parserror(l, "invalid %s type for ids",
	      type_strkind(type_kind(type_final(t))));
    return NULL;
  }
  switch(type_kind(type_final(t))) {
    case IDL_BOOL: case IDL_USHORT: case IDL_SHORT: case IDL_ULONG:
    case IDL_LONG: case IDL_ULONGLONG: case IDL_LONGLONG: case IDL_FLOAT:
    case IDL_DOUBLE: case IDL_CHAR: case IDL_OCTET: case IDL_STRING:
    case IDL_ENUM: case IDL_ENUMERATOR: case IDL_ARRAY: case IDL_SEQUENCE:
    case IDL_STRUCT: case IDL_UNION:
      break;

    case IDL_ANY: case IDL_FORWARD_STRUCT: case IDL_FORWARD_UNION:
    case IDL_PORT: case IDL_REMOTE:
      parserror(l, "invalid %s type for ids",
		type_strkind(type_kind(type_final(t))));
      return NULL;

    case IDL_CONST: case IDL_CASE: case IDL_MEMBER: case IDL_TYPEDEF:
      /* not a valid return from type_final() */
      assert(0); break;
  }

  prop_s p = prop_new(l, PROP_IDS, prop_strkind(PROP_IDS));
  if (!p) return NULL;

  p->type = t;
  return p;
}

prop_s
prop_newstring(tloc l, propkind k, const char *s)
{
  prop_s p = prop_new(l, k, prop_strkind(k));
  if (!p) return NULL;
  assert(s);

  p->text = string(s);
  return p;
}

prop_s
prop_newhash(tloc l, propkind k, hash_s h)
{
  prop_s p = prop_new(l, k, prop_strkind(k));
  if (!p) return NULL;
  assert(h);

  p->hash = h;
  return p;
}

prop_s
prop_newrequire(tloc l, propkind k, clist_s c)
{
  prop_s p = prop_new(l, k, prop_strkind(k));
  if (!p) return NULL;
  assert(c);

  p->clist = c;
  return p;
}

prop_s
prop_newvalue(tloc l, propkind k, cval c)
{
  idltype_s t, v;
  idlkind tk;
  prop_s p;

  /* check consitency */
  if (k == PROP_CLOCKRATE || k == PROP_PERIOD || k == PROP_DELAY) {
    if (const_convert(&c, CST_FLOAT) || c.f < 0.) {
      parserror(l, "invalid numeric value for %s", prop_strkind(k));
      return NULL;
    }
  }

  switch(c.k) {
    case CST_VOID:
      parserror(l, "invalid %s value", const_strkind(c.k));
      return NULL;

    case CST_BOOL:	tk = IDL_BOOL; break;
    case CST_UINT:	tk = IDL_ULONG; break;
    case CST_INT:	tk = IDL_LONG; break;
    case CST_FLOAT:	tk = IDL_DOUBLE; break;
    case CST_CHAR:	tk = IDL_CHAR; break;
    case CST_STRING:	tk = IDL_STRING; break;
    case CST_ENUM:	tk = IDL_ENUM; break;
  }

  /* create anon const type */
  t = type_newbasic(l, NULL, tk);
  if (!t) return NULL;
  v = type_newconst(l, NULL, t, c);
  if (!v) return NULL;

  p = prop_new(l, k, prop_strkind(k));
  if (!p) return NULL;

  p->value = v;
  return p;
}

prop_s
prop_newcodel(tloc l, propkind k, codel_s c)
{
  prop_s p;
  assert(k == PROP_VALIDATE || k == PROP_SIMPLE_CODEL || k == PROP_FSM_CODEL);

  /* always build the property name as "codel xxx" for validation/bare
   * codels/fsm codels to avoid name clashes */
  p = prop_new(
    l, k, strings(prop_strkind(PROP_SIMPLE_CODEL), " ", codel_name(c), NULL));
  if (!p) return NULL;

  p->codel = c;
  return p;
}

prop_s
prop_newtask(tloc l, const char *name)
{
  task_s t;
  prop_s p;

  t = comp_task(comp_active(), name);
  if (!t) {
    parserror(l, "unknown task '%s'", name);
    return NULL;
  }

  p = prop_new(l, PROP_TASK, prop_strkind(PROP_TASK));
  if (!p) return NULL;
  p->task = t;
  return p;
}


/* --- prop_type ----------------------------------------------------------- */

idltype_s
prop_type(prop_s p)
{
  assert(p);
  assert(p->kind == PROP_IDS);
  return p->type;
}


/* --- prop_text ----------------------------------------------------------- */

const char *
prop_text(prop_s p)
{
  assert(p);
  assert(p->kind == PROP_DOC || p->kind == PROP_VERSION ||
	 p->kind == PROP_EMAIL || p->kind == PROP_LANG ||
	 p->kind == PROP_SCHEDULING);
  return p->text;
}


/* --- prop_list ----------------------------------------------------------- */

clist_s
prop_list(prop_s p)
{
  assert(p);
  assert(p->kind == PROP_REQUIRE || p->kind == PROP_CODELS_REQUIRE);
  return p->clist;
}


/* --- prop_value ---------------------------------------------------------- */

idltype_s
prop_value(prop_s p)
{
  assert(p);
  assert(
    p->kind == PROP_CLOCKRATE || p->kind == PROP_PERIOD ||
    p->kind == PROP_DELAY || p->kind == PROP_PRIORITY ||
    p->kind == PROP_STACK);
  return p->value;
}


/* --- prop_task ----------------------------------------------------------- */

task_s
prop_task(prop_s p)
{
  assert(p);
  assert(p->kind == PROP_TASK);
  return p->task;
}


/* --- prop_codel ---------------------------------------------------------- */

codel_s
prop_codel(prop_s p)
{
  assert(p);
  assert(p->kind == PROP_VALIDATE || p->kind == PROP_SIMPLE_CODEL ||
         p->kind == PROP_FSM_CODEL);
  return p->codel;
}


/* --- prop_hash ----------------------------------------------------------- */

hash_s
prop_hash(prop_s p)
{
  assert(p);
  assert(p->kind == PROP_THROWS || p->kind == PROP_INTERRUPTS ||
         p->kind == PROP_BEFORE || p->kind == PROP_AFTER ||
         p->kind == PROP_EXTENDS || p->kind == PROP_PROVIDES ||
         p->kind == PROP_USES);
  return p->hash;
}


/* --- prop_destroy -------------------------------------------------------- */

/** Destroy a property
 */
void
prop_destroy(prop_s p)
{
  if (p) {
    xwarnx("destroyed property %s", p->name);
    free(p);
  }
}


/* --- prop_merge_list ----------------------------------------------------- */

/** Merge two lists of properties into the first one.
 */
int
prop_merge_list(hash_s p, hash_s m, int ignore_dup)
{
  hiter i;
  int e;

  if (!m) return 0;

  for(hash_first(m, &i); i.current; hash_next(&i)) {
    e = prop_merge(p, i.value, ignore_dup);
    if (e) return e;
  }

  return 0;
}


/* --- prop_merge ---------------------------------------------------------- */

/** Merge two lists of properties into the first one.
 */
int
prop_merge(hash_s p, prop_s i, int ignore_dup)
{
  hiter j;
  hash_s iev;
  prop_s q;
  int e = 0;

  assert(p && i);

  /* some properties must be recreated for current component */
  switch (prop_kind(i)) {
    case PROP_THROWS:
      iev = hash_create("enumerator list", 0);
      if (!iev) { e = errno; break; }

      for(hash_first(prop_hash(i), &j); j.current; hash_next(&j)) {
        e = hash_insert(iev, j.key, string(j.key), NULL);
        if (e) break;
      }
      if (comp_addievs(prop_loc(i), iev, 1/*nostd*/))
        e = errno;
      else
        i->hash = iev;
      break;

    case PROP_IDS:
      if (!comp_addids(type_loc(prop_type(i)),
                       type_membersscope(prop_type(i))))
        e |= errno;
      break;

    case PROP_VALIDATE:
    case PROP_SIMPLE_CODEL:
    case PROP_FSM_CODEL:
      i->codel = codel_clone(prop_codel(i));
      if (!i->codel) e = errno;
      break;

    default: break;
  }
  if (e) return e;

  e = hash_insert(p, prop_name(i), i, NULL);
  /* property might already exist */
  if (e == EEXIST) {
    q = hash_find(p, prop_name(i)); assert(q);
    /* some properties can be merged */
    switch(prop_kind(i)) {
      case PROP_THROWS: {
        for(hash_first(iev, &j); j.current; hash_next(&j)) {
          e = hash_insert(prop_hash(q), j.key, j.value, NULL);
          if (e && e != EEXIST) break; else e = 0;
        }
        break;
      }

      case PROP_REQUIRE: {
        clist_s n, l = prop_list(q); assert(l);
        citer k;
        for(clist_first(prop_list(i), &k); k.current; clist_next(&k)) {
          n = clist_append(l, *k.value, 1);
          if (n) l = n;
        }
        q->clist = l;
        e = 0;
        break;
      }

      case PROP_IDS: /* handled above */
        e = 0; break;

      default:
        if (ignore_dup) break;
        parserror(prop_loc(i), "duplicate %s declaration", prop_name(i));
        parsenoerror(prop_loc(q), " %s declared here", prop_name(q));
        break;
    }
  }

  return e;
}


/* --- prop_strkind -------------------------------------------------------- */

/** Return a property kind as a string
 */

const char *
prop_strkind(propkind k)
{
  switch(k) {
    case PROP_DOC:		return "doc";
    case PROP_IDS:		return "ids";
    case PROP_VERSION:		return "version";
    case PROP_LANG:		return "lang";
    case PROP_EMAIL:		return "e-mail";
    case PROP_REQUIRE:		return "requires";
    case PROP_CODELS_REQUIRE:	return "codels-require";
    case PROP_EXTENDS:		return "extends";
    case PROP_PROVIDES:		return "provides";
    case PROP_USES:		return "uses";
    case PROP_CLOCKRATE:	return "clock-rate";
    case PROP_PERIOD:		return "period";
    case PROP_DELAY:		return "delay";
    case PROP_PRIORITY:		return "priority";
    case PROP_SCHEDULING:	return "scheduling";
    case PROP_STACK:		return "stack";
    case PROP_TASK:		return "task";
    case PROP_VALIDATE:		return "validate";
    case PROP_SIMPLE_CODEL:	return "codel";
    case PROP_FSM_CODEL:	return "codel";
    case PROP_THROWS:		return "throws";
    case PROP_INTERRUPTS:	return "interrupts";
    case PROP_BEFORE:		return "before";
    case PROP_AFTER:		return "after";
  }

  assert(0);
  return NULL;
}
