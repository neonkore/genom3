/*
 * Copyright (c) 2010,2012 LAAS/CNRS
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
#include <errno.h>
#include <err.h>

#include "genom.h"


/* --- local data ---------------------------------------------------------- */

struct initer_s {
  unsigned int index;	/**< array element number (or -1U) */
  const char *member;	/**< member name (or NULL) */

  const char *doc;	/**< documentation string, or NULL */

  cval value;		/**< value, may be CST_VOID */
  initer_s sub;		/**< elements of a compound initializer */

  initer_s next;	/**< next initializer for this type */
};


unsigned int	initer_index(initer_s i) { assert(i); return i->index; }
const char *	initer_member(initer_s i) { assert(i); return i->member; }
const char *	initer_doc(initer_s i) { assert(i); return i->doc; }
cval		initer_value(initer_s i) { assert(i); return i->value; }
initer_s	initer_compound(initer_s i) { assert(i); return i->sub; }
initer_s	initer_next(initer_s i) { assert(i); return i->next; }

static int	initer_matcharray(tloc l, idltype_s t, initer_s i);
static int	initer_matchcomp(tloc l, idltype_s t, initer_s i);


/* --- initer_create ------------------------------------------------------- */

/** Create an initializer element, for element index or member name, with
 * initial value v or compound value sub.
 */

initer_s
initer_create(unsigned int index, const char *member, initer_s sub, cval v)
{
  initer_s i = malloc(sizeof(*i));
  if (!i) {
    warnx("memory exhausted, cannot create initializer element");
    return NULL;
  }

  /* member and index cannot be both defined */
  assert(!(member && index != -1U));
  /* same for sub and v */
  assert(!(v.k != CST_VOID && sub));

  i->index = index;
  i->member = member ? string(member) : NULL;
  i->doc = NULL;
  i->value = v;
  i->sub = sub;
  i->next = NULL;
  return i;
}


/* --- initer_setdoc ------------------------------------------------------- */

/** Set initializer documentation
 */
int
initer_setdoc(initer_s i, const char *d)
{
  assert(i && d && !i->doc);
  i->doc = string(d);
  return 0;
}


/* --- initer_append ------------------------------------------------------- */

/** Chain initializer elements
 */

initer_s
initer_append(initer_s l, initer_s m)
{
  initer_s i;
  if (!l) return m;

  for(i = l; i->next; i = i->next) /*empty body*/;
  i->next = m;

  return l;
}


/* --- initer_matchtype ---------------------------------------------------- */

/** Check that an initializer matches a type.
 */
int
initer_matchtype(tloc l, idltype_s t, initer_s i)
{
  /* constructed initializer */
  if (i->sub) {
    switch(type_kind(type_final(t))) {
      case IDL_ARRAY: case IDL_SEQUENCE:
	errno = initer_matcharray(l, t, i->sub);
	break;
      case IDL_STRUCT: case IDL_UNION:
	errno = initer_matchcomp(l, t, i->sub);
	break;
      default:
	parserror(l, "compound initializer cannot initialize %s%s%s",
		  type_strkind(type_kind(t)), type_name(t)?" ":"",
		  type_name(t)?type_fullname(t):"");
	return errno = EINVAL;
    }
    if (errno) return errno;
  }

  /* simple initializer */
  if (i->value.k != CST_VOID && !i->sub) {
    switch(type_kind(type_final(t))) {
      case IDL_ARRAY: case IDL_SEQUENCE: case IDL_STRUCT: case IDL_UNION:
	parserror(l, "cannot initialize %s%s%s with a scalar",
		  type_strkind(type_kind(t)), type_name(t)?" ":"",
		  type_name(t)?type_fullname(t):"");
	return errno = EINVAL;

      default: break;
    }

    if (const_cast(l, &i->value, t)) {
      if (type_fullname(t))
	parserror(l, "cannot set default value of member %s",
		  type_fullname(t));
      return errno;
    }

    if (type_fullname(t))
      xwarnx("set default value of member %s", type_fullname(t));
  }

  return 0;
}

static int
initer_matcharray(tloc l, idltype_s t, initer_s i)
{
  unsigned int d;
  idltype_s e;
  initer_s j;
  unsigned int k;
  int s;
  assert(t && i);

  e = type_type(type_final(t));
  d = type_length(type_final(t));

  /* loop on initializer elements */
  for(k = 0, j = i; j; k++, j = j->next) {
    if (j->member) {
      parserror(l, "unknown member '%s' in %s %s", j->member,
		type_strkind(type_kind(t)), type_fullname(t));
      return errno = EINVAL;
    }
    if (j->index != -1U) k = j->index; else j->index = k;

    if (k >= d) {
      parserror(l, "element %d in %s %s is out of bound",
		k, type_strkind(type_kind(t)), type_fullname(t));
      return errno = EINVAL;
    }

    s = initer_matchtype(l, e, j);
    if (s) return s;
  }

  if (j) {
    parserror(l, "too many initializers for %s %s",
	      type_strkind(type_kind(t)), type_fullname(t));
    return errno = EINVAL;
  }

  return 0;
}


static int
initer_matchcomp(tloc l, idltype_s t, initer_s i)
{
  idltype_s e, f;
  initer_s j;
  hiter k;
  int s;
  assert(t && i);

  /* loop on initializer elements */
  e = type_first(type_final(t), &k);
  if (!e) return errno = EINVAL;

  for(j = i; j; e = type_after(t, e, &k), j = j->next) {
    if (j->index != -1U) {
      parserror(l, "unknown array index '%d' in %s %s", j->index,
		type_strkind(type_kind(t)), type_fullname(t));
      return errno = EINVAL;
    }
    if (j->member) {
      f = type_member(t, j->member);
      if (!f) {
	parserror(l, "unknown member '%s' in %s %s", j->member,
		  type_strkind(type_kind(t)), type_fullname(t));
	return errno = EINVAL;
      }
      e = f;
    }
    if (!e) break;

    if (!j->member) j->member = type_name(e);

    s = initer_matchtype(l, e, j);
    if (s) return s;
  }

  if (j && !e) {
    parserror(l, "too many initializers for %s %s",
	      type_strkind(type_kind(t)), type_fullname(t));
    return errno = EINVAL;
  }

  return 0;
}


/* --- initer_typeiniter --------------------------------------------------- */

/** Return the initializer for needle, considering i is the initializer for
 * haystack.
 */
initer_s
initer_typeiniter(initer_s i, idltype_s haystack, idltype_s needle)
{
  if (!i || needle == haystack) return i;

  switch(type_kind(haystack)) {
    case IDL_BOOL: case IDL_USHORT: case IDL_SHORT: case IDL_ULONG:
    case IDL_LONG: case IDL_ULONGLONG: case IDL_LONGLONG: case IDL_FLOAT:
    case IDL_DOUBLE: case IDL_CHAR: case IDL_OCTET: case IDL_STRING:
    case IDL_ANY: case IDL_CONST: case IDL_ENUM: case IDL_ENUMERATOR:
    case IDL_PORT: case IDL_REMOTE:
      break;

    case IDL_ARRAY: case IDL_SEQUENCE:
      return initer_typeiniter(i->sub, type_type(haystack), needle);

    case IDL_STRUCT: case IDL_UNION: {
      idltype_s t;
      initer_s j, f;

      for(j=i->sub; j; j = j->next) {
	t = type_member(haystack, j->member);
	if (t) {
	  f = initer_typeiniter(j, t, needle);
	  if (f) return f;
	}
      }
      break;
    }

    case IDL_MEMBER: case IDL_TYPEDEF:
      return initer_typeiniter(i, type_type(haystack), needle);

    case IDL_FORWARD_STRUCT: case IDL_FORWARD_UNION:
      /* note: recursion should normally be avoided here (this would lead to
       * endless loop). But since the initializer itself will not be infinite,
       * the recursion will eventually stop. OTOH, it is important to
       * recurse so that initializer for such structures can be used. */
      return initer_typeiniter(i, type_type(haystack), needle);

    case IDL_CASE:
      break;
  }

  return NULL;
}


/* --- initer_destroy ------------------------------------------------------ */

/** Destroy a list
 */

void
initer_destroy(initer_s l)
{
  initer_s i;

  while(l) {
    i = l; l = l->next; free(i);
  }
}
