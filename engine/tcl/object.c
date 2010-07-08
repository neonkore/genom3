/*
 * Copyright (c) 2010 LAAS/CNRS
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
    typeidx_kind, typeidx_name, typeidx_fullname, typeidx_scope, typeidx_final,
    typeidx_type, typeidx_length, typeidx_value, typeidx_valuekind,
    typeidx_members, typeidx_discriminator, typeidx_loc, typeidx_class
  };
  static const char *args[] = {
    [typeidx_kind] = "kind", [typeidx_name] = "name",
    [typeidx_fullname] = "fullname", [typeidx_scope] = "scope",
    [typeidx_final] = "final", [typeidx_type] = "type",
    [typeidx_length] = "length", [typeidx_value] = "value",
    [typeidx_valuekind] = "valuekind", [typeidx_members] = "members",
    [typeidx_discriminator] = "discriminator", [typeidx_loc] = "loc",
    [typeidx_class] = "class", NULL
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
    /* 'members' subcommand can have one additional parameter, others don't
     * have any */
    if (objc > 3) {
      Tcl_WrongNumArgs(interp, 0, objv, "$type members ?pattern?");
      return TCL_ERROR;
    }
  } else {
    if (objc > 2) {
      Tcl_WrongNumArgs(interp, 0, objv, "$type subcommand");
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


/* --- codel command ------------------------------------------------------- */

/** Implements the command associated to a codel object.
 */
int
codel_cmd(ClientData v, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  enum codelidx {
    codelidx_name, codelidx_return, codelidx_params, codelidx_yields,
    codelidx_triggers, codelidx_task, codelidx_service, codelidx_loc,
    codelidx_class
  };
  static const char *args[] = {
    [codelidx_name] = "name", [codelidx_return] = "return",
    [codelidx_params] = "parameters", [codelidx_yields] = "yields",
    [codelidx_triggers] = "triggers", [codelidx_task] = "task",
    [codelidx_service] = "service", [codelidx_loc] = "loc",
    [codelidx_class] = "class", NULL
  };
  codel_s c = v;
  Tcl_Obj *r = NULL;
  int s;

  int i = codelidx_name; /* return name by default */

  if (objc > 2) {
    Tcl_WrongNumArgs(interp, 0, objv, "$codel subcommand");
    return TCL_ERROR;
  }
  if (objc > 1) {
    s = Tcl_GetIndexFromObj(interp, objv[1], args, "subcommand", 0, &i);
    if (s != TCL_OK) return s;
  }
  switch(i) {
    case codelidx_name:
      r = Tcl_NewStringObj(codel_name(c), -1);
      break;

    case codelidx_return:
      r = Tcl_NewStringObj(type_genref(codel_return(c)), -1);
      break;

    case codelidx_params: {
      hiter i;

      r = Tcl_NewListObj(0, NULL);
      for(hash_first(codel_params(c), &i); i.current; hash_next(&i)) {
	Tcl_ListObjAppendElement(
	  interp, r, Tcl_NewStringObj(param_genref(i.value), -1));
      }
      break;
    }

    case codelidx_yields: {
      hiter i;

      r = Tcl_NewListObj(0, NULL);
      for(hash_first(codel_yields(c), &i); i.current; hash_next(&i)) {
	Tcl_ListObjAppendElement(
	  interp, r, Tcl_NewStringObj(type_genref(i.value), -1));
      }
      break;
    }

    case codelidx_triggers: {
      hiter i;

      r = Tcl_NewListObj(0, NULL);
      for(hash_first(codel_triggers(c), &i); i.current; hash_next(&i)) {
	Tcl_ListObjAppendElement(
	  interp, r, Tcl_NewStringObj(type_genref(i.value), -1));
      }
      break;
    }

    case codelidx_task:
      if (*codel_task(c))
	r = Tcl_NewStringObj(task_genref(*codel_task(c)), -1);
      break;

    case codelidx_service:
      if (*codel_service(c))
	r = Tcl_NewStringObj(service_genref(*codel_service(c)), -1);
      break;

    case codelidx_loc: {
      Tcl_Obj *l[3] = {
	Tcl_NewStringObj(codel_loc(c).file, -1),
	Tcl_NewIntObj(codel_loc(c).line),
	Tcl_NewIntObj(codel_loc(c).col),
      };
      r = Tcl_NewListObj(3, l);
      break;
    }

    case codelidx_class:
      r = Tcl_NewStringObj("codel", -1);
      break;
  }

  if (!r) {
    Tcl_AppendResult(interp, "undefined member \"", args[i], "\"", NULL);
    return TCL_ERROR;
  }

  Tcl_SetObjResult(interp, r);
  return TCL_OK;
}
