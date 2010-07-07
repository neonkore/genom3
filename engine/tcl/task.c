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
 *                                           Anthony Mallet on Wed Jul  7 2010
 */
#include "acgenom.h"

#include <assert.h>

#include <tcl.h>

#include "genom.h"
#include "engine.h"


/* --- task command -------------------------------------------------------- */

/** Implements the command associated to a task object.
 */
int
task_cmd(ClientData v, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  enum taskidx {
    taskidx_name, taskidx_doc, taskidx_period, taskidx_delay, taskidx_priority,
    taskidx_scheduling, taskidx_stack, taskidx_codels, taskidx_throws,
    taskidx_loc, taskidx_class
  };
  static const char *args[] = {
    [taskidx_name] = "name", [taskidx_doc] = "doc",
    [taskidx_period] = "period", [taskidx_delay] = "delay",
    [taskidx_priority] = "priority", [taskidx_scheduling] = "scheduling",
    [taskidx_stack] = "stack", [taskidx_codels] = "codels",
    [taskidx_throws] = "throws", [taskidx_loc] = "loc",
    [taskidx_class] = "class", NULL
  };
  static const propkind argkind[] = {
    [taskidx_doc] = PROP_DOC, [taskidx_period] = PROP_PERIOD,
    [taskidx_delay] = PROP_DELAY, [taskidx_priority] = PROP_PRIORITY,
    [taskidx_scheduling] = PROP_SCHEDULING, [taskidx_stack] = PROP_STACK
  };
  task_s t = v;
  Tcl_Obj *r = NULL;
  prop_s p;
  int s;

  int i = taskidx_name; /* return name by default */

  if (objc > 2) {
    Tcl_WrongNumArgs(interp, 0, objv, "$task subcommand");
    return TCL_ERROR;
  }
  if (objc > 1) {
    s = Tcl_GetIndexFromObj(interp, objv[1], args, "subcommand", 0, &i);
    if (s != TCL_OK) return s;
  }
  switch(i) {
    case taskidx_name:
      r = Tcl_NewStringObj(task_name(t), -1);
      break;

    case taskidx_doc: case taskidx_scheduling:
      p = hash_find(task_props(t), prop_strkind(argkind[i]));
      r = p ? Tcl_NewStringObj(prop_text(p), -1) : NULL;
      break;

    case taskidx_period: case taskidx_delay:
    case taskidx_priority: case taskidx_stack:
      p = hash_find(task_props(t), prop_strkind(argkind[i]));
      r = p ? Tcl_NewStringObj(type_genref(prop_value(p)), -1) : NULL;
      break;

    case taskidx_throws: {
      prop_s p = hash_find(task_props(t), prop_strkind(PROP_THROWS));
      hiter i;

      r = Tcl_NewListObj(0, NULL);
      if (p)
	for(hash_first(prop_hash(p), &i); i.current; hash_next(&i)) {
	  Tcl_ListObjAppendElement(
	    interp, r, Tcl_NewStringObj(type_genref(i.value), -1));
	}
      break;
    }

    case taskidx_codels: {
      hiter i;

      r = Tcl_NewListObj(0, NULL);
      for(hash_first(task_props(t), &i); i.current; hash_next(&i)) {
	if (prop_kind(i.value) != PROP_CODEL) continue;
	Tcl_ListObjAppendElement(
	  interp, r, Tcl_NewStringObj(codel_genref(prop_codel(i.value)), -1));
      }
      break;
    }

    case taskidx_loc: {
      Tcl_Obj *l[3] = {
	Tcl_NewStringObj(task_loc(t).file, -1),
	Tcl_NewIntObj(task_loc(t).line),
	Tcl_NewIntObj(task_loc(t).col),
      };
      r = Tcl_NewListObj(3, l);
      break;
    }

    case taskidx_class:
      r = Tcl_NewStringObj("task", -1);
      break;
  }

  if (!r) {
    Tcl_AppendResult(interp, "undefined member \"", args[i], "\"", NULL);
    return TCL_ERROR;
  }

  Tcl_SetObjResult(interp, r);
  return TCL_OK;
}
