/*
 * Copyright (c) 2010,2012-2013 LAAS/CNRS
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
    taskidx_name, taskidx_comp, taskidx_doc, taskidx_period, taskidx_delay,
    taskidx_priority, taskidx_scheduling, taskidx_stack, taskidx_services,
    taskidx_codels, taskidx_fsm, taskidx_throws, taskidx_loc, taskidx_class
  };
  static const char *args[] = {
    [taskidx_name] = "name", [taskidx_comp] = "component",
    [taskidx_doc] = "doc", [taskidx_period] = "period",
    [taskidx_delay] = "delay", [taskidx_priority] = "priority",
    [taskidx_scheduling] = "scheduling", [taskidx_stack] = "stack",
    [taskidx_services] = "services", [taskidx_codels] = "codels",
    [taskidx_fsm] = "fsm", [taskidx_throws] = "throws", [taskidx_loc] = "loc",
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

  enum taskidx i = taskidx_name; /* return name by default */

  if (objc > 1) {
    s = Tcl_GetIndexFromObj(interp, objv[1], args, "subcommand", 0, (int *)&i);
    if (s != TCL_OK) return s;
  }
  if (i == taskidx_fsm) {
    if (objc > 3) {
      Tcl_WrongNumArgs(interp, 0, objv, "$task fsm event");
      return TCL_ERROR;
    }
  } else {
    if (objc > 2) {
      Tcl_WrongNumArgs(interp, 0, objv, "$task subcommand");
      return TCL_ERROR;
    }
  }
  switch(i) {
    case taskidx_name:
      r = Tcl_NewStringObj(task_name(t), -1);
      break;

    case taskidx_comp:
      r = Tcl_NewStringObj(comp_genref(task_comp(t)), -1);
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
      Tcl_Obj *argv[] = {
        Tcl_NewStringObj("lsort", -1),
        Tcl_NewStringObj("-unique", -1),
        NULL,
      };
      prop_s p;
      hiter i;

      r = Tcl_NewListObj(0, NULL);

      p = hash_find(comp_props(task_comp(t)), prop_strkind(PROP_THROWS));
      if (p)
        for(hash_first(prop_hash(p), &i); i.current; hash_next(&i)) {
          Tcl_ListObjAppendElement(
            interp, r, Tcl_NewStringObj(type_genref(i.value), -1));
        }

      p = hash_find(task_props(t), prop_strkind(PROP_THROWS));
      if (p)
        for(hash_first(prop_hash(p), &i); i.current; hash_next(&i)) {
          Tcl_ListObjAppendElement(
            interp, r, Tcl_NewStringObj(type_genref(i.value), -1));
        }

      argv[2] = r;
      Tcl_IncrRefCount(argv[0]);
      Tcl_IncrRefCount(argv[1]);
      Tcl_IncrRefCount(argv[2]);
      s = Tcl_EvalObjv(interp, 3, argv, TCL_EVAL_GLOBAL);
      Tcl_DecrRefCount(argv[2]);
      Tcl_DecrRefCount(argv[1]);
      Tcl_DecrRefCount(argv[0]);
      if (s != TCL_OK) return TCL_ERROR;
      r = Tcl_GetObjResult(interp);
      break;
    }

    case taskidx_services: {
      hiter i;
      prop_s p;

      r = Tcl_NewListObj(0, NULL);
      for(hash_first(
	    comp_services(task_comp(t)), &i); i.current; hash_next(&i)) {
	p = hash_find(service_props(i.value), prop_strkind(PROP_TASK));
	if (p && prop_task(p) == t)
	  Tcl_ListObjAppendElement(
	    interp, r, Tcl_NewStringObj(service_genref(i.value), -1));
      }
      break;
    }

    case taskidx_codels: {
      hiter i;

      r = Tcl_NewListObj(0, NULL);
      for(hash_first(task_props(t), &i); i.current; hash_next(&i)) {
	if (prop_kind(i.value) != PROP_SIMPLE_CODEL &&
            prop_kind(i.value) != PROP_FSM_CODEL) continue;
	Tcl_ListObjAppendElement(
	  interp, r, Tcl_NewStringObj(codel_genref(prop_codel(i.value)), -1));
      }
      break;
    }

    case taskidx_fsm: {
      idltype_s e;
      codel_s c;
      hiter i;

      if (objc < 3) {
        r = Tcl_NewListObj(0, NULL);
        for(hash_first(task_fsm(t), &i); i.current; hash_next(&i)) {
          e = type_find(i.key); assert(e);
          Tcl_ListObjAppendElement(
            interp, r, Tcl_NewStringObj(type_genref(e), -1));
        }
      } else {
        c = hash_find(task_fsm(t), Tcl_GetString(objv[2]));
        if (!c) {
          Tcl_Obj *argv[] = { objv[2], Tcl_NewStringObj("fullname", -1) };

          Tcl_IncrRefCount(argv[1]);
          s = Tcl_EvalObjv(interp, 2, argv, TCL_EVAL_GLOBAL);
          Tcl_DecrRefCount(argv[1]);
          if (s == TCL_OK)
            c = hash_find(task_fsm(t),
                          Tcl_GetString(Tcl_GetObjResult(interp)));
        }

        if (c)
          r = Tcl_NewStringObj(codel_genref(c), -1);
        else
          r = Tcl_NewListObj(0, NULL);
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
