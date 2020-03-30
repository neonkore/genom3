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
 *                                           Anthony Mallet on Thu Jul  8 2010
 */
#include "acgenom.h"

#include <assert.h>
#include <string.h>

#include <tcl.h>

#include "genom.h"
#include "engine.h"


/* --- service command ----------------------------------------------------- */

/*/
 * == *$service* TCL engine command
 *
 * Those commands manipulate service objects and return information about
 * them. They all take a service object as their first argument, noted
 * `$service` in the following command descriptions. Such an object is
 * typically returned by other procedures, such as
 * link:cmd-component{outfilesuffix}#services[`$component services`].
 */

/** Implements the command associated to a service object.
 */
int
service_cmd(ClientData v, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  enum serviceidx {
    serviceidx_name, serviceidx_kind, serviceidx_comp, serviceidx_doc,
    serviceidx_task, serviceidx_validate, serviceidx_codels, serviceidx_params,
    serviceidx_fsm, serviceidx_throws, serviceidx_interrupts,
    serviceidx_before, serviceidx_after, serviceidx_mutex, serviceidx_digest,
    serviceidx_loc, serviceidx_class
  };
  static const char *args[] = {
    [serviceidx_name] = "name", [serviceidx_kind] = "kind",
    [serviceidx_comp] = "component", [serviceidx_doc] = "doc",
    [serviceidx_task] = "task", [serviceidx_validate] = "validate",
    [serviceidx_codels] = "codels", [serviceidx_params] = "parameters",
    [serviceidx_fsm] = "fsm", [serviceidx_throws] = "throws",
    [serviceidx_interrupts] = "interrupts", [serviceidx_before] = "before",
    [serviceidx_after] = "after", [serviceidx_mutex] = "mutex",
    [serviceidx_digest] = "digest", [serviceidx_loc] = "loc",
    [serviceidx_class] = "class", NULL
  };
  static const propkind argkind[] = {
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
  } else if (i == serviceidx_codels) {
    if (objc > 3) {
      Tcl_WrongNumArgs(interp, 0, objv, "$service codel ?simple|fsm?");
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
    /*/
     * [[name]]
     * === *$service name*
     *
     * Return the name of the service as a string.
     */
    case serviceidx_name:
      r = Tcl_NewStringObj(service_name(s), -1);
      break;

      /*/
       * [[kind]]
       * === *$service kind*
       *
       * Return a string that is either `attribute`, `function` or `activity`
       * depending on the kind of service.
       */
    case serviceidx_kind:
      r = Tcl_NewStringObj(service_strkind(service_kind(s)), -1);
      break;

      /*/
       * [[component]]
       * === *$service component*
       *
       * Return the link:cmd-component{outfilesuffix}[component object] in which
       * the service is defined.
       */
    case serviceidx_comp:
      r = Tcl_NewStringObj(comp_genref(service_comp(s)), -1);
      break;

      /*/
       * [[doc]]
       * === *$service doc*
       *
       * Return a string containing the documentation of the service defined
       * in the `doc` attributes of the `.gen` description.
       */
    case serviceidx_doc:
      p = hash_find(service_props(s), prop_strkind(PROP_DOC));
      r = p ? Tcl_NewStringObj(prop_text(p), -1) : NULL;
      break;

      /*/
       * [[before]]
       * === *$service before*
       *
       * Return a list of services that cannot run until this service has
       * been executed sucessfully at least once.
       *
       * [[after]]
       * === *$service after*
       *
       * Return a list of services that must must have been successfully
       * executed at least once before this service can run.
       *
       * [[interrupts]]
       * === *$service interrupts*
       *
       * Return a list of services that will be interrupted whenever this
       * service is executed.
       */
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

      /*/
       * [[task]]
       * === *$service task*
       *
       * Return the link:cmd-task{outfilesuffix}[task object] in which
       * the service is defined, or an error if the service has no task context
       * (i.e. the service is an `attribute` or a `function).
       */
    case serviceidx_task:
      p = hash_find(service_props(s), prop_strkind(PROP_TASK));
      if (p)
	r = Tcl_NewStringObj(task_genref(prop_task(p)), -1);
      break;

      /*/
       * [[throws]]
       * === *$service throws*
       *
       * Return a list of link:cmd-type{outfilesuffix}[`exceptions`] possibly
       * raised by the service.
       */
    case serviceidx_throws: {
      Tcl_Obj *argv[] = {
        Tcl_NewStringObj("object", -1),
        Tcl_NewStringObj("unique", -1),
        NULL,
      };
      hiter i;

      r = Tcl_NewListObj(0, NULL);

      p = hash_find(comp_props(service_comp(s)), prop_strkind(PROP_THROWS));
      if (p)
        for(hash_first(prop_hash(p), &i); i.current; hash_next(&i)) {
          Tcl_ListObjAppendElement(
            interp, r, Tcl_NewStringObj(type_genref(i.value), -1));
        }

      p = hash_find(service_props(s), prop_strkind(PROP_TASK));
      if (p) {
        p = hash_find(task_props(prop_task(p)), prop_strkind(PROP_THROWS));
        if (p)
          for(hash_first(prop_hash(p), &i); i.current; hash_next(&i)) {
            Tcl_ListObjAppendElement(
              interp, r, Tcl_NewStringObj(type_genref(i.value), -1));
          }
      }

      p = hash_find(service_props(s), prop_strkind(PROP_THROWS));
      if (p)
        for(hash_first(prop_hash(p), &i); i.current; hash_next(&i)) {
          Tcl_ListObjAppendElement(
            interp, r, Tcl_NewStringObj(type_genref(i.value), -1));
        }

      argv[2] = r;
      Tcl_IncrRefCount(argv[0]);
      Tcl_IncrRefCount(argv[1]);
      Tcl_IncrRefCount(argv[2]);
      e = Tcl_EvalObjv(interp, 3, argv, TCL_EVAL_GLOBAL);
      Tcl_DecrRefCount(argv[2]);
      Tcl_DecrRefCount(argv[1]);
      Tcl_DecrRefCount(argv[0]);
      if (e != TCL_OK) return TCL_ERROR;
      r = Tcl_GetObjResult(interp);
      break;
    }

    case serviceidx_validate: {
      hiter j;

      r = Tcl_NewListObj(0, NULL);
      for(hash_first(service_props(s), &j); j.current; hash_next(&j)) {
	if (prop_kind(j.value) != PROP_VALIDATE) continue;
	Tcl_ListObjAppendElement(
	  interp, r, Tcl_NewStringObj(codel_genref(prop_codel(j.value)), -1));
      }
      break;
    }

      /*/
       * [[codels]]
       * === *$service codels* [[validate|simple|fsm] ...]
       *
       * Return the list of link:cmd-codel{outfilesuffix}[codels] defined in
       * the service. The list may be filtered by a keyword described
       * below. Multiple keyword may be given: they are combined with an
       * implicit 'or'.
       *
       * .Arguments
       * [horizontal]
       * 'validate':: Return the validation codels.
       * 'simple':: Return the simple codels of functions.
       * 'fsm':: Return the codels associated with activities and a state
       * machine.
       */
    case serviceidx_codels: {
      enum ckindidx {
        ckindidx_validate, ckindidx_simple, ckindidx_fsm
      };
      static const char *ckindarg[] = {
        [ckindidx_validate] = "validate", [ckindidx_simple] = "simple",
        [ckindidx_fsm] = "fsm", NULL
      };
      int f, d, sc;
      hiter j;

      r = Tcl_NewListObj(0, NULL);

      for(hash_first(service_props(s), &j); j.current; hash_next(&j)) {
        if (prop_kind(j.value) != PROP_VALIDATE &&
            prop_kind(j.value) != PROP_SIMPLE_CODEL &&
            prop_kind(j.value) != PROP_FSM_CODEL) continue;

        /* iterate over optional filters */
        f = (objc < 3);
        for(sc = 2; !f && sc < objc; sc++) {
          e = Tcl_GetIndexFromObj(interp, objv[sc], ckindarg, "kind", 0, &d);
          if (e != TCL_OK) return e;

          switch(d) {
            case ckindidx_validate:
              if (prop_kind(j.value) == PROP_VALIDATE) f = 1;
              break;
            case ckindidx_simple:
              if (prop_kind(j.value) == PROP_SIMPLE_CODEL) f = 1;
              break;
            case ckindidx_fsm:
              if (prop_kind(j.value) == PROP_FSM_CODEL) f = 1;
              break;
          }
        }
        if (f)
          Tcl_ListObjAppendElement(
            interp, r, Tcl_NewStringObj(codel_genref(prop_codel(j.value)), -1));
      }
      break;
    }

    /*/
     * [[params]]
     * === *$service params* [[local|in|out|inout] ...]
     *
     * Return the list of parameters of the service. One or several direction
     * filter may be specified to filter the list according to the or'ed
     * combination of the directions.
     *
     * .Arguments
     * [horizontal]
     * 'local':: The parameters that are private to the running instance of the
     * service.
     * 'in':: The parameters that input to the service.
     * 'out':: The parameters that output by the service.
     * 'inout':: The parameters that both input to and output by the service.
     */
    case serviceidx_params: {
      enum servicepdiridx {
        svcpdiridx_nodir, svcpdiridx_in, svcpdiridx_out, svcpdiridx_inout
      };
      static const char *dirarg[] = {
        [svcpdiridx_nodir] = "local", [svcpdiridx_in] = "in",
        [svcpdiridx_out] = "out", [svcpdiridx_inout] = "inout", NULL
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
            case svcpdiridx_nodir:
              if (param_dir(i.value) == P_NODIR) f = 1;
              break;
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

      /*/
       * [[fsm]]
       * === *$service fsm* ['state']
       *
       * When called without argument, `$service fsm` returns the list of
       * link:cmd-type{outfilesuffix}[states] defined in the finite state
       * machine of the service.
       *
       * When called with one of the states as argument, the procedure returns
       * the link:cmd-codel{outfilesuffix}[codel] object associated with that
       * state.
       */
    case serviceidx_fsm: {
      idltype_s t;
      codel_s c;
      hiter i;

      if (objc < 3) {
        r = Tcl_NewListObj(0, NULL);
        for(hash_first(service_fsm(s), &i); i.current; hash_next(&i)) {
          t = scope_findtype(comp_scope(service_comp(s)), i.key);
          assert(t);
          Tcl_ListObjAppendElement(
            interp, r, Tcl_NewStringObj(type_genref(t), -1));
        }
      } else {
        c = hash_find(service_fsm(s), Tcl_GetString(objv[2]));
        if (!c) {
          Tcl_Obj *argv[] = { objv[2], Tcl_NewStringObj("name", -1) };

          Tcl_IncrRefCount(argv[1]);
          e = Tcl_EvalObjv(interp, 2, argv, TCL_EVAL_GLOBAL);
          Tcl_DecrRefCount(argv[1]);
          if (e == TCL_OK)
            c = hash_find(service_fsm(s),
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
     * [[mutex]]
     * === *$service mutex*
     *
     * Return a list of link:cmd-codel{outfilesuffix}[`codels`] that may access
     * internal resources used by the service. All these codels cannot run
     * when the service is running.
     */
    case serviceidx_mutex: {
      hiter i;
      hash_s h = service_codel_mutex(s);
      if (!h) {
        Tcl_AppendResult(interp, "internal error", NULL);
        return TCL_ERROR;
      }

      r = Tcl_NewListObj(0, NULL);
      for(hash_first(h, &i); i.current; hash_next(&i)) {
        Tcl_ListObjAppendElement(
          interp, r, Tcl_NewStringObj(codel_genref(i.value), -1));
      }
      hash_destroy(h, 1);
      break;
    }

      /*/
       * [[digest]]
       * === *$service digest*
       *
       * Return a string containing a MD5 hash of the service. The hash is
       * computed using the service name and the ordered list of parameters
       * direction and type.
       */
    case serviceidx_digest: {
      Tcl_Obj *argv[] = {
        Tcl_NewStringObj("object", -1),
        Tcl_NewStringObj("digest", -1),
        objv[0]
      };

      Tcl_IncrRefCount(argv[0]);
      Tcl_IncrRefCount(argv[1]);
      e = Tcl_EvalObjv(interp, 3, argv, TCL_EVAL_GLOBAL);
      Tcl_DecrRefCount(argv[1]);
      Tcl_DecrRefCount(argv[0]);
      if (e != TCL_OK) return TCL_ERROR;
      r = Tcl_GetObjResult(interp);
      break;
    }

    /*/
     * [[loc]]
     * === *$service loc*
     *
     * Return a list describing the source location where that service is
     * defined. The list contains three elements: the file name, the line
     * number and the column number.
     */
    case serviceidx_loc: {
      Tcl_Obj *l[3] = {
	Tcl_NewStringObj(service_loc(s).file, -1),
	Tcl_NewIntObj(service_loc(s).line),
	Tcl_NewIntObj(service_loc(s).col),
      };
      r = Tcl_NewListObj(3, l);
      break;
    }

    /*/
     * [[class]]
     * === *$service class*
     *
     * Always returns the string "service". Useful to determine at runtime
     * that the object is a service object.
     */
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
