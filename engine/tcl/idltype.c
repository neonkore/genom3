/*
 * Copyright (c) 2010-2012 LAAS/CNRS
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
 *                                           Anthony Mallet on Fri Jan 22 2010
 */
#include "acgenom.h"

#include <assert.h>
#include <libgen.h>
#include <string.h>

#include <tcl.h>

#include "genom.h"
#include "engine.h"


/* --- local data ---------------------------------------------------------- */

static Tcl_Obj *	member_list(Tcl_Interp *interp, idltype_s t,
				Tcl_Obj *pattern);


/* --- type command -------------------------------------------------------- */

/** Implements the command associated to a type object.
 */
int
type_cmd(ClientData v, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  enum typeidx {
    typeidx_kind, typeidx_name, typeidx_fullname, typeidx_scope, typeidx_fixed,
    typeidx_final, typeidx_type, typeidx_length, typeidx_value,
    typeidx_valuekind, typeidx_members, typeidx_discriminator, typeidx_port,
    typeidx_remote, typeidx_mapping, typeidx_declarator, typeidx_address,
    typeidx_deref, typeidx_argument, typeidx_pass, typeidx_digest,
    typeidx_native, typeidx_loc, typeidx_class
  };
  static const char *args[] = {
    [typeidx_kind] = "kind", [typeidx_name] = "name",
    [typeidx_fullname] = "fullname", [typeidx_scope] = "scope",
    [typeidx_fixed] = "fixed", [typeidx_final] = "final",
    [typeidx_type] = "type", [typeidx_length] = "length",
    [typeidx_value] = "value", [typeidx_valuekind] = "valuekind",
    [typeidx_members] = "members", [typeidx_discriminator] = "discriminator",
    [typeidx_port] = "port", [typeidx_remote] = "remote",
    [typeidx_mapping] = "mapping", [typeidx_declarator] = "declarator",
    [typeidx_address] = "address", [typeidx_deref] = "dereference",
    [typeidx_argument] = "argument", [typeidx_pass] = "pass",
    [typeidx_digest] = "digest", [typeidx_native] = "native",
    [typeidx_loc] = "loc", [typeidx_class] = "class", NULL
  };
  idltype_s t = v;
  Tcl_Obj *r = NULL;
  int s;

  int i = typeidx_fullname; /* return full name by default */

  if (objc > 1) {
    s = Tcl_GetIndexFromObj(interp, objv[1], args, "subcommand", 0, &i);
    if (s != TCL_OK) return s;
  }
  if (i == typeidx_members) {
    /* 'members' subcommand can have one additional parameter */
    if (objc > 3) {
      Tcl_WrongNumArgs(interp, 2, objv, "?pattern?");
      return TCL_ERROR;
    }
  } else if (i == typeidx_declarator || i == typeidx_address ||
             i == typeidx_deref) {
    /* 'declarator', 'address' and 'deref' subcommands can have one additional
     * parameter */
    if (objc > 3) {
      Tcl_WrongNumArgs(interp, 2, objv, "?var?");
      return TCL_ERROR;
    }
  } else if (i == typeidx_native) {
    /* 'native' subcommand can have one additional parameter */
    if (objc > 3) {
      Tcl_WrongNumArgs(interp, 2, objv, "?template?");
      return TCL_ERROR;
    }
  } else if (i == typeidx_argument || i == typeidx_pass) {
    /* 'argument' and 'pass' subcommands can have two additional parameters */
    if (objc < 3 || objc > 4) {
      Tcl_WrongNumArgs(interp, 2, objv, "kind ?var?");
      return TCL_ERROR;
    }
  } else {
    if (objc > 2) {
      Tcl_WrongNumArgs(interp, 1, objv, "subcommand");
      return TCL_ERROR;
    }
  }
  switch(i) {
    case typeidx_kind:
      r = Tcl_NewStringObj(type_strkind(type_kind(t)), -1);
      break;

    case typeidx_name:
      r = type_name(t) ? Tcl_NewStringObj(type_name(t), -1) : NULL;
      break;

    case typeidx_fullname:
      r = type_fullname(t) ? Tcl_NewStringObj(type_fullname(t), -1) : NULL;
      break;

    case typeidx_scope: {
      scope_s s = type_scope(t);
      Tcl_Obj *n[2], *l;

      r = Tcl_NewListObj(0, NULL);
      for (s = type_scope(t); scope_parent(s); s = scope_parent(s)) {
	n[0] = Tcl_NewStringObj(scope_strkind(scope_kind(s)), -1);
	n[1] = Tcl_NewStringObj(scope_name(s), -1);
	l = Tcl_NewListObj(2, n);
	Tcl_ListObjReplace(interp, r, 0, 0, 1, &l);
      }
      break;
    }

    case typeidx_fixed:
      r = Tcl_NewBooleanObj(type_fixed(t));
      break;

    case typeidx_final:
      r = Tcl_NewStringObj(type_genref(type_final(t)), -1);
      break;

    case typeidx_type:
      switch(type_kind(t)) {
	case IDL_ENUMERATOR: case IDL_SEQUENCE: case IDL_ARRAY: case IDL_CONST:
	case IDL_TYPEDEF: case IDL_MEMBER: case IDL_CASE:
	case IDL_FORWARD_STRUCT: case IDL_FORWARD_UNION:
	  r = Tcl_NewStringObj(type_genref(type_type(t)), -1);
	  break;

	default: r = NULL; break;
      }
      break;

    case typeidx_length:
      switch(type_kind(t)) {
	case IDL_SEQUENCE: case IDL_ARRAY: case IDL_STRING:
	  if (type_length(t) != -1U) {
	    r = Tcl_NewIntObj(type_length(t));
	    break;
	  }

	default: break;
      }
      break;

    case typeidx_value:
      switch(type_kind(t)) {
	case IDL_CONST:
	  r = Tcl_NewStringObj(const_strval(type_constvalue(t)), -1);
	  break;

	default: r = NULL; break;
      }
      break;

    case typeidx_valuekind:
      switch(type_kind(t)) {
	case IDL_CONST:
	  r = Tcl_NewStringObj(const_strkind(type_constvalue(t).k), -1);
	  break;

	default: r = NULL; break;
      }
      break;

    case typeidx_members:
      r = member_list(interp, t, objc > 2 ? objv[2] : NULL);
      break;

    case typeidx_discriminator:
      switch(type_kind(t)) {
	case IDL_UNION:
	  r = Tcl_NewStringObj(type_genref(type_discriminator(t)), -1);
	  break;

	default: r = NULL; break;
      }
      break;

    case typeidx_port:
      switch(type_kind(t)) {
        case IDL_PORT:
          r = Tcl_NewStringObj(port_genref(type_port(t)), -1);
          break;

        default: r = NULL; break;
      }
      break;

    case typeidx_remote:
      switch(type_kind(t)) {
        case IDL_REMOTE:
          r = Tcl_NewStringObj(remote_genref(type_remote(t)), -1);
          break;

        default: r = NULL; break;
      }
      break;

    case typeidx_mapping: {
      Tcl_Obj *argv[] = {
        Tcl_NewStringObj("language::mapping", -1),
        objv[0]
      };

      Tcl_IncrRefCount(argv[0]);
      s = Tcl_EvalObjv(interp, 2, argv, TCL_EVAL_GLOBAL);
      Tcl_DecrRefCount(argv[0]);
      if (s != TCL_OK) return TCL_ERROR;
      r = Tcl_GetObjResult(interp);
      break;
    }

    case typeidx_declarator:
    case typeidx_address:
    case typeidx_deref: {
      Tcl_Obj *argv[] = {
        Tcl_NewStringObj("language::", -1),
        objv[0],
        objv[2]
      };

      Tcl_IncrRefCount(argv[0]);
      Tcl_AppendStringsToObj(argv[0], args[i], NULL);
      s = Tcl_EvalObjv(interp, objc, argv, TCL_EVAL_GLOBAL);
      Tcl_DecrRefCount(argv[0]);
      if (s != TCL_OK) return TCL_ERROR;
      r = Tcl_GetObjResult(interp);
      break;
    }

    case typeidx_argument:
    case typeidx_pass: {
      Tcl_Obj *argv[] = {
        Tcl_NewStringObj("language::", -1),
        objv[0],
        objv[2],
        objv[3]
      };

      Tcl_IncrRefCount(argv[0]);
      Tcl_AppendStringsToObj(argv[0], args[i], NULL);
      s = Tcl_EvalObjv(interp, objc, argv, TCL_EVAL_GLOBAL);
      Tcl_DecrRefCount(argv[0]);
      if (s != TCL_OK) return TCL_ERROR;
      r = Tcl_GetObjResult(interp);
      break;
    }

    case typeidx_digest: {
      Tcl_Obj *argv[] = {
        Tcl_NewStringObj("object", -1),
        Tcl_NewStringObj("digest", -1),
        objv[0],
      };

      Tcl_IncrRefCount(argv[0]);
      Tcl_IncrRefCount(argv[1]);
      s = Tcl_EvalObjv(interp, 3, argv, TCL_EVAL_GLOBAL);
      Tcl_DecrRefCount(argv[1]);
      Tcl_DecrRefCount(argv[0]);
      if (s != TCL_OK) return TCL_ERROR;
      r = Tcl_GetObjResult(interp);
      break;
    }

    case typeidx_native: {
      hash_s h;
      char *tmpl;
      char *native;

      if (!type_fullname(t)) break;

      h = dotgen_hnative();
      if (!h) break;

      if (objc < 3) {
        char name[PATH_MAX];

        strlcpy(name, runopt.tmpl, sizeof(name));
        tmpl = basename(name);
      } else
        tmpl = Tcl_GetString(objv[2]);
      if (!tmpl) break;

      h = hash_find(h, tmpl);
      if (!h) break;

      native = hash_find(h, type_fullname(t));
      if (!native) break;

      r = Tcl_NewStringObj(native, -1);
      break;
    }

    case typeidx_loc:
      if (!type_loc(t).file) { r = NULL; } else {
	Tcl_Obj *l[3] = {
	  Tcl_NewStringObj(type_loc(t).file, -1),
	  Tcl_NewIntObj(type_loc(t).line),
	  Tcl_NewIntObj(type_loc(t).col),
	};
	r = Tcl_NewListObj(3, l);
      }
      break;

    case typeidx_class:
      r = Tcl_NewStringObj("type", -1);
      break;
  }

  if (!r) {
    Tcl_AppendResult(interp, "no such member \"", args[i], "\"", NULL);
    return TCL_ERROR;
  }

  Tcl_SetObjResult(interp, r);
  return TCL_OK;
}


/* --- member_list --------------------------------------------------------- */

/** return a Tcl list of type members, matching a pattern if not NULL.
*/
static Tcl_Obj *
member_list(Tcl_Interp *interp, idltype_s t, Tcl_Obj *pattern)
{
  const char *p;
  idltype_s e;
  hiter m;

  Tcl_Obj *r = Tcl_NewListObj(0, NULL);
  p =  pattern ? Tcl_GetString(pattern) : NULL;

  for(e = type_first(t, &m); e; e = type_next(&m)) {
    if (p && type_name(e) && !Tcl_StringMatch(type_name(e), p))
      continue;

    Tcl_ListObjAppendElement(
      interp, r, Tcl_NewStringObj(type_genref(e), -1));
  }

  return r;
}
