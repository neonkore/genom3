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
 *                                           Anthony Mallet on Thu Jul  8 2010
 */
#include "acgenom.h"

#include <assert.h>
#include <string.h>

#include <tcl.h>

#include "genom.h"
#include "engine.h"


/* --- service command ----------------------------------------------------- */

/** Implements the command associated to a service object.
 */
int
service_cmd(ClientData v, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  enum serviceidx {
    serviceidx_name, serviceidx_kind, serviceidx_comp, serviceidx_doc,
    serviceidx_task, serviceidx_validate, serviceidx_codels, serviceidx_params,
    serviceidx_fsm, serviceidx_throws, serviceidx_interrupts,
    serviceidx_before, serviceidx_after, serviceidx_loc, serviceidx_class
  };
  static const char *args[] = {
    [serviceidx_name] = "name", [serviceidx_kind] = "kind",
    [serviceidx_comp] = "component", [serviceidx_doc] = "doc",
    [serviceidx_task] = "task", [serviceidx_validate] = "validate",
    [serviceidx_codels] = "codels", [serviceidx_params] = "parameters",
    [serviceidx_fsm] = "fsm", [serviceidx_throws] = "throws",
    [serviceidx_interrupts] = "interrupts", [serviceidx_before] = "before",
    [serviceidx_after] = "after", [serviceidx_loc] = "loc",
    [serviceidx_class] = "class", NULL
  };
  static const propkind argkind[] = {
    [serviceidx_codels] = PROP_CODEL, [serviceidx_validate] = PROP_VALIDATE,
    [serviceidx_interrupts] = PROP_INTERRUPTS,
    [serviceidx_before] = PROP_BEFORE, [serviceidx_after] = PROP_AFTER
  };
  service_s s = v;
  Tcl_Obj *r = NULL;
  prop_s p;
  int e;

  enum serviceidx i = serviceidx_name; /* return name by default */

  if (objc > 1) {
    e = Tcl_GetIndexFromObj(interp, objv[1], args, "subcommand", 0, (int *)&i);
    if (e != TCL_OK) return e;
  }
  if (i == serviceidx_fsm) {
    if (objc > 3) {
      Tcl_WrongNumArgs(interp, 0, objv, "$service fsm ?event?");
      return TCL_ERROR;
    }
  } else if (i != serviceidx_params) {
    /* 'parameters' subcommand can have unlimited additional parameters, other
     * subcommand don't have any. */
    if (objc > 2) {
      Tcl_WrongNumArgs(interp, 0, objv, "$service subcommand");
      return TCL_ERROR;
    }
  }
  switch(i) {
    case serviceidx_name:
      r = Tcl_NewStringObj(service_name(s), -1);
      break;

    case serviceidx_kind:
      r = Tcl_NewStringObj(service_strkind(service_kind(s)), -1);
      break;

    case serviceidx_comp:
      r = Tcl_NewStringObj(comp_genref(service_comp(s)), -1);
      break;

    case serviceidx_doc:
      p = hash_find(service_props(s), prop_strkind(PROP_DOC));
      r = p ? Tcl_NewStringObj(prop_text(p), -1) : NULL;
      break;

    case serviceidx_before: case serviceidx_after:
    case serviceidx_interrupts: {
      comp_s c = service_comp(s);
      hiter j, k;

      r = Tcl_NewListObj(0, NULL);
      p = hash_find(service_props(s), prop_strkind(argkind[i]));
      if (p)
	for(hash_first(prop_hash(p), &j); j.current; hash_next(&j)) {
	  if (!strcmp(j.value, ALL_SERVICE_NAME)) {
	    Tcl_SetListObj(r, 0, NULL);
	    for(hash_first(comp_services(c), &k); k.current; hash_next(&k)) {
	      Tcl_ListObjAppendElement(
		interp, r, Tcl_NewStringObj(service_genref(k.value), -1));
	    }
	    break;
	  }

	  Tcl_ListObjAppendElement(
	    interp, r,
	    Tcl_NewStringObj(service_genref(comp_service(c, j.value)), -1));
	}
      break;
    }

    case serviceidx_task:
      p = hash_find(service_props(s), prop_strkind(PROP_TASK));
      if (p)
	r = Tcl_NewStringObj(task_genref(prop_task(p)), -1);
      break;

    case serviceidx_throws: {
      hiter i;

      r = Tcl_NewListObj(0, NULL);
      p = hash_find(service_props(s), prop_strkind(PROP_THROWS));
      if (p)
	for(hash_first(prop_hash(p), &i); i.current; hash_next(&i)) {
	  Tcl_ListObjAppendElement(
	    interp, r, Tcl_NewStringObj(type_genref(i.value), -1));
	}
      break;
    }

    case serviceidx_validate:
    case serviceidx_codels: {
      hiter j;

      r = Tcl_NewListObj(0, NULL);
      for(hash_first(service_props(s), &j); j.current; hash_next(&j)) {
	if (prop_kind(j.value) != argkind[i]) continue;
	Tcl_ListObjAppendElement(
	  interp, r, Tcl_NewStringObj(codel_genref(prop_codel(j.value)), -1));
      }
      break;
    }

    case serviceidx_params: {
      enum servicepdiridx {
        svcpdiridx_in, svcpdiridx_out, svcpdiridx_inout
      };
      static const char *dirarg[] = {
        [svcpdiridx_in] = "in", [svcpdiridx_out] = "out",
        [svcpdiridx_inout] = "inout", NULL
      };
      int f, d = -1, sc;
      hiter i;

      /* initialize output list to the empty list */
      r = Tcl_NewListObj(0, NULL);

      /* build filtered list of parameters */
      for(hash_first(service_params(s), &i); i.current; hash_next(&i)) {

	/* iterate over optional direction filters */
        f = (objc < 3);
	for(sc = 2; !f && sc < objc; sc++) {
          e = Tcl_GetIndexFromObj(interp, objv[sc], dirarg, "direction", 0, &d);
          if (e != TCL_OK) return e;

          switch(d) {
            case svcpdiridx_in:
              if (param_dir(i.value) == P_IN) f = 1;
              break;
            case svcpdiridx_out:
              if (param_dir(i.value) == P_OUT) f = 1;
              break;
            case svcpdiridx_inout:
              if (param_dir(i.value) == P_INOUT) f = 1;
              break;
          }
        }
        if (f)
          Tcl_ListObjAppendElement(
            interp, r, Tcl_NewStringObj(param_genref(i.value), -1));
      }
      break;
    }

    case serviceidx_fsm: {
      codel_s c;
      hiter i;

      if (objc < 3) {
	r = Tcl_NewListObj(0, NULL);
	for(hash_first(service_fsm(s), &i); i.current; hash_next(&i)) {
	  Tcl_ListObjAppendElement(
	    interp, r, Tcl_NewStringObj(i.key, -1));
	}
      } else {
	c = hash_find(service_fsm(s), Tcl_GetString(objv[2]));
	if (c)
	  r = Tcl_NewStringObj(codel_genref(c), -1);
	else
	  r = Tcl_NewListObj(0, NULL);
      }
      break;
    }

    case serviceidx_loc: {
      Tcl_Obj *l[3] = {
	Tcl_NewStringObj(service_loc(s).file, -1),
	Tcl_NewIntObj(service_loc(s).line),
	Tcl_NewIntObj(service_loc(s).col),
      };
      r = Tcl_NewListObj(3, l);
      break;
    }

    case serviceidx_class:
      r = Tcl_NewStringObj("service", -1);
      break;
  }

  if (!r) {
    Tcl_AppendResult(interp, "undefined member \"", args[i], "\"", NULL);
    return TCL_ERROR;
  }

  Tcl_SetObjResult(interp, r);
  return TCL_OK;
}
