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
 *                                           Anthony Mallet on Wed Dec 23 2009
 */
#include "acgenom.h"

#include <assert.h>
#include <errno.h>

#include "genom.h"

/* --- const_unaryop ------------------------------------------------------- */

/** apply the unary operator (+, -, ~) to the cval
 */
int
const_unaryop(cval *value, char op)
{
  assert(value);
  switch(value->k) {
    case CST_CHAR: case CST_STRING: case CST_ENUM: return EDOM;
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
    case CST_CHAR: case CST_STRING: case CST_ENUM: return EDOM;
    default: break;
  }

  /* convert arg and value to the largest type, except for << and >> which
   * needs unsigned arg */
  if (op != '<' && op != '>') {
    switch(value->k) {
      case CST_BOOL:	s = const_convert(&arg, value->k); break;
      case CST_UINT:
	switch(arg.k) {
	  case CST_BOOL:s = const_convert(&arg, value->k); break;
	  default:	s = const_convert(value, arg.k); break;
	}
	break;
      case CST_INT:
	switch(arg.k) {
	  case CST_BOOL:
	  case CST_UINT:s = const_convert(&arg, value->k); break;
	  default:	s = const_convert(value, arg.k); break;
	}
	break;

      case CST_FLOAT:
	switch(arg.k) {
	  case CST_BOOL:
	  case CST_UINT:
	  case CST_INT:	s = const_convert(&arg, value->k); break;
	  default:	s = const_convert(value, arg.k); break;
	}
	break;

      default:		s = const_convert(value, arg.k); break;
    }
    if (s) return s;
    assert(value->k == arg.k);
  } else {
    s = const_convert(&arg, CST_UINT);
    if (s) return s;
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

    case CST_CHAR:
    case CST_STRING:
    case CST_ENUM:
      if (value->k != k) return EDOM;
      return 0;
  }

  value->k = k;
  return 0;
}


/* --- const_convert ------------------------------------------------------- */

/** convert the cval to the given type
 */
int
const_cast(cval *value, idltype_s t)
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

    case IDL_ANY: case IDL_ENUMERATOR: case IDL_ARRAY:
    case IDL_SEQUENCE: case IDL_STRUCT: case IDL_UNION:
      return EINVAL;

    case IDL_CONST: case IDL_TYPEDEF:
      /* not a valid return from type_final() */
      assert(0); break;
  }

  return s;
}
