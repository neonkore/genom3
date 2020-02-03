/*
 * Copyright (c) 2010,2012-2015,2020 LAAS/CNRS
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

/*/
 * == *$task* TCL engine command
 *
 * Those commands manipulate tasks objects and return information about
 * them. They all take a task object as their first argument, noted
 * `$task` in the following command descriptions. Such an object is
 * typically returned by other procedures, such as
 * link:cmd-component{outfilesuffix}#tasks[`$component tasks`].
 */

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
    /*/
     * [[name]]
     * === *$task name*
     *
     * Return the name of the task as a string.
     */
    case taskidx_name:
      r = Tcl_NewStringObj(task_name(t), -1);
      break;

      /*/
       * [[component]]
       * === *$task component*
       *
       * Return the link:cmd-component{outfilesuffix}[component object] in which
       * the task is defined.
       */
    case taskidx_comp:
      r = Tcl_NewStringObj(comp_genref(task_comp(t)), -1);
      break;

      /*/
       * [[doc]]
       * === *$task doc*
       *
       * Return a string containing the documentation of the component defined
       * in the `doc` attributes of the `.gen` description.
       */
    case taskidx_doc: case taskidx_scheduling:
      p = hash_find(task_props(t), prop_strkind(argkind[i]));
      r = p ? Tcl_NewStringObj(prop_text(p), -1) : NULL;
      break;

      /*/
       * [[period]]
       * === *$task period*
       *
       * Return a link:cmd-type{outfilesuffix}[numeric floating point constant]
       * representing the period of the task in seconds. If the task has no
       * period, this raises an error.
       *
       * [[delay]]
       * === *$task delay*
       *
       * Return a link:cmd-type{outfilesuffix}[numeric floating point constant]
       * representing the initial delay of a task in seconds.
       *
       * [[priority]]
       * === *$task priority*
       *
       * Return a link:cmd-type{outfilesuffix}[integer constant]
       * representing the task scheduling priority.
       *
       * [[stack]]
       * === *$task stack*
       *
       * Return a link:cmd-type{outfilesuffix}[integer constant]
       * representing the stack size of the task.
       */
    case taskidx_period: case taskidx_delay:
    case taskidx_priority: case taskidx_stack:
      p = hash_find(task_props(t), prop_strkind(argkind[i]));
      r = p ? Tcl_NewStringObj(type_genref(prop_value(p)), -1) : NULL;
      break;

      /*/
       * [[throws]]
       * === *$task throws*
       *
       * Return a list of link:cmd-type{outfilesuffix}[`exceptions`] possibly
       * raised by the task. This is the concatenation of all exceptions
       * defined in the task itself and its services.
       */
    case taskidx_throws: {
      Tcl_Obj *argv[] = {
        Tcl_NewStringObj("object", -1),
        Tcl_NewStringObj("unique", -1),
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

      /*/
       * [[services]]
       * === *$task services*
       *
       * Return the list of link:cmd-service{outfilesuffix}[services] defined
       * in the commponent.
       */
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

      /*/
       * [[codels]]
       * === *$task codels*
       *
       * Return the list of link:cmd-codel{outfilesuffix}[codels] defined
       * in the task.
       */
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

      /*/
       * [[fsm]]
       * === *$task fsm* ['state']
       *
       * When called without argument, `$task fsm` returns the list of
       * link:cmd-type{outfilesuffix}[states] defined in the finite state
       * machine of the task.
       *
       * When called with one of the states as argument, the procedure returns
       * the link:cmd-codel{outfilesuffix}[codel] object associated with that
       * state.
       */
    case taskidx_fsm: {
      idltype_s e;
      codel_s c;
      hiter i;

      if (objc < 3) {
        r = Tcl_NewListObj(0, NULL);
        for(hash_first(task_fsm(t), &i); i.current; hash_next(&i)) {
          e = scope_findtype(comp_scope(task_comp(t)), i.key);
          assert(e);
          Tcl_ListObjAppendElement(
            interp, r, Tcl_NewStringObj(type_genref(e), -1));
        }
      } else {
        c = hash_find(task_fsm(t), Tcl_GetString(objv[2]));
        if (!c) {
          Tcl_Obj *argv[] = { objv[2], Tcl_NewStringObj("name", -1) };

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

    /*/
     * [[loc]]
     * === *$task loc*
     *
     * Return a list describing the source location where that task is
     * defined. The list contains three elements: the file name, the line
     * number and the column number.
     */
    case taskidx_loc: {
      Tcl_Obj *l[3] = {
	Tcl_NewStringObj(task_loc(t).file, -1),
	Tcl_NewIntObj(task_loc(t).line),
	Tcl_NewIntObj(task_loc(t).col),
      };
      r = Tcl_NewListObj(3, l);
      break;
    }

    /*/
     * [[class]]
     * === *$task class*
     *
     * Always returns the string "task". Useful to determine at runtime
     * that the object is a task object.
     */
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
