/*
 * Copyright (c) 2011 LAAS/CNRS
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
 *                                           Anthony Mallet on Wed Mar  9 2011
 */
#include "acgenom.h"

#include <assert.h>

#include <tcl.h>

#include "genom.h"
#include "engine.h"


/* --- local data ---------------------------------------------------------- */

static Tcl_Obj *	param_list(Tcl_Interp *interp, codel_s c,
				Tcl_Obj *dfilter);


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

  if (objc > 1) {
    s = Tcl_GetIndexFromObj(interp, objv[1], args, "subcommand", 0, &i);
    if (s != TCL_OK) return s;
  }
  if (i == codelidx_params) {
    if (objc > 3) {
      Tcl_WrongNumArgs(interp, 0, objv, "$codel parameters ?direction?");
      return TCL_ERROR;
    }
  } else {
    if (objc > 2) {
      Tcl_WrongNumArgs(interp, 0, objv, "$codel subcommand");
      return TCL_ERROR;
    }
  }
  switch(i) {
    case codelidx_name:
      r = Tcl_NewStringObj(codel_name(c), -1);
      break;

    case codelidx_return:
      if (*codel_task(c))
	r = Tcl_NewStringObj(
	  type_genref(comp_eventtype(task_comp(*codel_task(c)))), -1);
      else if (*codel_service(c))
	r = Tcl_NewStringObj(
	  type_genref(comp_eventtype(service_comp(*codel_service(c)))), -1);
      break;

    case codelidx_params:
      r = param_list(interp, c, objc>2?objv[2]:NULL);
      if (!r) return TCL_ERROR;
      break;

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



/* --- param_list ---------------------------------------------------------- */

/** return a Tcl list of ports
 */
static Tcl_Obj *
param_list(Tcl_Interp *interp, codel_s c, Tcl_Obj *dfilter)
{
  static const struct { char *opt; pdir d; } dirarg[] = {
    { "in", P_IN }, { "out", P_OUT }, { "inout", P_INOUT },
    { "inport", P_INPORT }, { "outport", P_OUTPORT },
    { NULL }
  };

  hiter i;
  pdir d;
  int s, f;

  Tcl_Obj *r = Tcl_NewListObj(0, NULL);

  if (dfilter) {
    s = Tcl_GetIndexFromObjStruct(
      interp, dfilter, dirarg, sizeof(dirarg[0]), "filter", 0, &f);
    if (s != TCL_OK) return NULL;
    d = dirarg[f].d;
  } else
    d = P_NODIR;

  for(hash_first(codel_params(c), &i); i.current; hash_next(&i)) {
    if (d == P_NODIR ||
	param_dir(i.value) == d ||
	(param_dir(i.value) == P_INOUT && (d == P_IN || d == P_OUT))) {
      Tcl_ListObjAppendElement(
	interp, r, Tcl_NewStringObj(param_genref(i.value), -1));
    }
  }

  return r;
}
