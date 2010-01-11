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
 *                                           Anthony Mallet on Wed Dec 23 2009
 */
#include "acgenom.h"

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <err.h>

#include "genom.h"

/* --- local data ---------------------------------------------------------- */

struct clist_s {
  cval v;
  clist_s next;
};


static cvalkind	const_maxkind(cvalkind a, cvalkind b);


/* --- const_unaryop ------------------------------------------------------- */

/** apply the unary operator (+, -, ~) to the cval
 */
int
const_unaryop(cval *value, char op)
{
  assert(value);
  switch(value->k) {
    case CST_VOID: case CST_CHAR: case CST_STRING: case CST_ENUM: return EDOM;
    default: break;
  }

  switch(op) {
    case '+': switch(value->k) {
      case CST_BOOL:	return EDOM;
      default:	return 0;
    }

    case '-': switch(value->k) {
      case CST_UINT:	value->k = CST_INT; value->i = -value->u; return 0;
      case CST_INT:	value->i = -value->i; return 0;
      case CST_FLOAT:	value->f = -value->f; return 0;
      default:		return EDOM;
    }

    case '~': switch(value->k) {
      case CST_BOOL:	value->b = value->b?0:1; return 0;
      case CST_UINT:	value->u = ~value->u; return 0;
      case CST_INT:	value->i = ~value->i; return 0;
      default:		return EDOM;
    }
  }

  assert(0);
  return 0;
}


/* --- const_binaryop ------------------------------------------------------ */

/** apply the binary operator (+, -, *, /, %, |, &, ^, '<', '>') to the cval
 */
int
const_binaryop(cval *value, char op, cval arg)
{
  int s;
  assert(value);
  switch(value->k) {
    case CST_VOID: case CST_CHAR: case CST_STRING: case CST_ENUM: return EDOM;
    default: break;
  }

  /* convert arg and value to the largest type, except for << and >> which
   * needs unsigned arg */
  switch(op) {
    case '<': case '>':
      s = const_convert(&arg, CST_UINT);
      if (s) return s;
      break;

    default: {
      cvalkind k = const_maxkind(value->k, arg.k);
      s = const_convert(value, k);
      if (s) return s;
      s = const_convert(&arg, k);
      if (s) return s;
      assert(value->k == arg.k);
    }
  }

  /* compute result */
  switch(op) {
    case '+': switch(value->k) {
      case CST_UINT:	value->u += arg.u; return 0;
      case CST_INT:	value->i += arg.i; return 0;
      case CST_FLOAT:	value->f += arg.f; return 0;
      default:		return EDOM;
    }

    case '-': switch(value->k) {
      case CST_UINT:	value->u -= arg.u; return 0;
      case CST_INT:	value->i -= arg.i; return 0;
      case CST_FLOAT:	value->f -= arg.f; return 0;
      default:		return EDOM;
    }

    case '*': switch(value->k) {
      case CST_UINT:	value->u *= arg.u; return 0;
      case CST_INT:	value->i *= arg.i; return 0;
      case CST_FLOAT:	value->f *= arg.f; return 0;
      default:		return EDOM;
    }

    case '/': switch(value->k) {
      case CST_UINT:	value->u /= arg.u; return 0;
      case CST_INT:	value->i /= arg.i; return 0;
      case CST_FLOAT:	value->f /= arg.f; return 0;
      default:		return EDOM;
    }

    case '%': switch(value->k) {
      case CST_UINT:	value->u %= arg.u; return 0;
      case CST_INT:	value->i %= arg.i; return 0;
      default:		return EDOM;
    }

    case '|': switch(value->k) {
      case CST_UINT:	value->u |= arg.u; return 0;
      case CST_INT:	value->i |= arg.i; return 0;
      default:		return EDOM;
    }

    case '&': switch(value->k) {
      case CST_UINT:	value->u &= arg.u; return 0;
      case CST_INT:	value->i &= arg.i; return 0;
      default:		return EDOM;
    }

    case '^': switch(value->k) {
      case CST_UINT:	value->u ^= arg.u; return 0;
      case CST_INT:	value->i ^= arg.i; return 0;
      default:		return EDOM;
    }

    case '<': switch(value->k) {
      case CST_UINT:	value->u <<= arg.u; return 0;
      case CST_INT:	value->i <<= arg.u; return 0;
      default:		return EDOM;
    }

    case '>': switch(value->k) {
      case CST_UINT:	value->u >>= arg.u; return 0;
      case CST_INT:	value->i >>= arg.u; return 0;
      default:		return EDOM;
    }
  }

  assert(0);
  return 0;
}


/* --- const_equal --------------------------------------------------------- */

/** Return true if the two const values are equal
 */
int
const_equal(cval a, cval b)
{
  cvalkind k = const_maxkind(a.k, b.k);

  if (const_convert(&a, k)) return 0;
  if (const_convert(&b, k)) return 0;
  assert(a.k == b.k);

  switch(a.k) {
    case CST_VOID:	return 1;
    case CST_BOOL:	return a.b == b.b;
    case CST_UINT:	return a.u == b.u;
    case CST_INT:	return a.i == b.i;
    case CST_FLOAT:	return a.f == b.f;
    case CST_CHAR:	return a.c == b.c;
    case CST_STRING:	return strcmp(a.s, b.s)?0:1;
    case CST_ENUM:	return type_equal(a.e, b.e);
  }

  assert(0);
  return 0;
}


/* --- const_convert ------------------------------------------------------- */

/** convert the cval to the given type
 */
int
const_convert(cval *value, cvalkind k)
{
  assert(value);
  switch(value->k) {
    case CST_BOOL:
      switch(k) {
	case CST_BOOL:	return 0;
	default:	return EDOM;
      }
      break;

    case CST_UINT:
      switch(k) {
	case CST_UINT:	return 0;
	case CST_INT:	value->i = value->u; break;
	case CST_FLOAT:	value->f = value->u; break;
	default:	return EDOM;
      }
      break;

    case CST_INT:
      switch(k) {
	case CST_UINT:
	  if (value->i < 0) return EDOM;
	  value->u = value->i;
	  break;
	case CST_INT:	return 0;
	case CST_FLOAT:	value->f = value->i; break;
	default:	return EDOM;
      }
      break;

    case CST_FLOAT:
      switch(k) {
	case CST_UINT:
	  if ((unsigned long)value->f != value->f) return EDOM;
	  value->u = value->f;
	  break;
	case CST_INT:
	  if ((long)value->f != value->f) return EDOM;
	  value->i = value->f;
	  break;
	case CST_FLOAT:	return 0;
	default:	return EDOM;
      }
      break;

    case CST_VOID:
    case CST_CHAR:
    case CST_STRING:
    case CST_ENUM:
      if (value->k != k) return EDOM;
      return 0;
  }

  value->k = k;
  return 0;
}


/* --- const_cast ---------------------------------------------------------- */

/** convert the cval to the given type
 */
int
const_cast(tloc l, cval *value, idltype_s t)
{
  int s;
  assert(value && t);

  switch(type_kind(type_final(t))) {
    case IDL_BOOL:      s = const_convert(value, CST_BOOL);	break;
    case IDL_USHORT:
    case IDL_ULONG:	s = const_convert(value, CST_UINT);	break;
    case IDL_SHORT:
    case IDL_LONG:	s = const_convert(value, CST_INT);	break;
    case IDL_FLOAT:
    case IDL_DOUBLE:	s = const_convert(value, CST_FLOAT);	break;
    case IDL_CHAR:
    case IDL_OCTET:	s = const_convert(value, CST_CHAR);	break;
    case IDL_STRING:	s = const_convert(value, CST_STRING);	break;
    case IDL_ENUM:
      if (value->k == CST_ENUM) {
	assert(value->e && type_kind(value->e) == IDL_ENUMERATOR);
	s = type_equal(type_enumeratorenum(value->e), t) ? 0 : EDOM;
      } else s = EDOM;
      break;

    case IDL_ANY: case IDL_ENUMERATOR: case IDL_ARRAY: case IDL_SEQUENCE:
    case IDL_STRUCT: case IDL_UNION: case IDL_FORWARD_STRUCT:
    case IDL_FORWARD_UNION:
      parserror(l, "%s%s%s is not a valid constant type",
		type_strkind(type_kind(t)),
		type_name(t)?" ":"", type_name(t)?type_name(t):"");
      parsenoerror(type_loc(t), "  %s%s%s declared here",
		   type_strkind(type_kind(t)),
		   type_name(t)?" ":"", type_name(t)?type_name(t):"");
      return errno = EINVAL;

    case IDL_CASE: case IDL_MEMBER: case IDL_CONST: case IDL_TYPEDEF:
      /* not a valid return from type_final() */
      assert(0); break;
  }

  if (s)
    parserror(l, "cannot convert constant expression to %s%s%s",
	      type_strkind(type_kind(t)),
	      type_name(t)?" ":"", type_name(t)?type_name(t):"");
  return errno = s;
}


/* --- const_maxkind ------------------------------------------------------- */

/** Return the max kind (that can contain both given kind values)
 */
static cvalkind
const_maxkind(cvalkind a, cvalkind b)
{
  switch(a) {
    case CST_BOOL:		return a;
    case CST_UINT:
      switch(b) {
	case CST_BOOL:		return a;
	default:		return b;
      }
    case CST_INT:
      switch(b) {
	case CST_BOOL:
	case CST_UINT:		return a;
	default:		return b;
      }

    case CST_FLOAT:
      switch(b) {
	case CST_BOOL:
	case CST_UINT:
	case CST_INT:		return a;
	default:		return b;
      }

    default:			return b;
  }
}


/* --- const_strkind ------------------------------------------------------- */

/** Return a const kind as a string
 */

const char *
const_strkind(cvalkind k)
{
  switch(k) {
    case CST_VOID:	return "void";
    case CST_BOOL:	return "bool";
    case CST_UINT:	return "uint";
    case CST_INT:	return "int";
    case CST_FLOAT:	return "float";
    case CST_CHAR:	return "char";
    case CST_STRING:	return "string";
    case CST_ENUM:	return "enum";
  }

  assert(0);
  return NULL;
}


/* --- const_strval -------------------------------------------------------- */

/** Return a const value as a string. Returned string will be overriden by
 * subsequent calls.
 */

const char *
const_strval(cval v)
{
  static char buf[32];

  switch(v.k) {
    case CST_VOID:	buf[0] = '\0'; break;
    case CST_BOOL:	snprintf(buf, sizeof(buf), "%d", v.b); break;
    case CST_UINT:	snprintf(buf, sizeof(buf), "%lu", v.u); break;
    case CST_INT:	snprintf(buf, sizeof(buf), "%ld", v.i); break;
    case CST_FLOAT:	snprintf(buf, sizeof(buf), "%g", v.f); break;
    case CST_CHAR:	snprintf(buf, sizeof(buf), "%c", v.c); break;
    case CST_STRING:	return v.s;
    case CST_ENUM:	return type_fullname(v.e);
  }

  return buf;
}


/* --- clist_append -------------------------------------------------------- */

/** Create a list element or append to existing
 */

clist_s
clist_append(clist_s l, cval v, int unique)
{
  clist_s i;
  clist_s e = malloc(sizeof(*e));
  if (!e) {
    warnx("memory exhausted, cannot create list");
    return l;
  }
  e->v = v;
  e->next = NULL;
  if (!l) return e;

  /* chain to the tail and filter duplicates */
  for(i = l; i->next; i = i->next)
    if (unique && const_equal(i->v, v)) { free(e); return NULL; }
  if (unique && const_equal(i->v, v)) { free(e); return NULL; }

  i->next = e;
  return l;
}


/* --- clist_destroy ------------------------------------------------------- */

/** Destroy a list
 */

void
clist_destroy(clist_s l)
{
  clist_s i;

  while(l) {
    i = l; l = l->next; free(i);
  }
}


/* --- clist_first/next ---------------------------------------------------- */

/** Iterate over list's elements
 */
int
clist_first(clist_s l, citer *i)
{
  assert(i);

  i->current = l;
  i->value = l?&l->v:NULL;

  return l?0:ENOENT;
}

int
clist_next(citer *i)
{
  assert(i);

  i->current = i->current?i->current->next:NULL;
  i->value = i->current?&i->current->v:NULL;

  return i->current?0:ENOENT;
}
