/*
 * Copyright (c) 2010-2014,2020 LAAS/CNRS
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
 *                                           Anthony Mallet on Tue Jul  6 2010
 */
#include "acgenom.h"

#include <assert.h>

#include <tcl.h>

#include "genom.h"
#include "engine.h"


/* --- local data ---------------------------------------------------------- */

static Tcl_Obj *	port_list(Tcl_Interp *interp, comp_s c,
                                  Tcl_Obj *const dfilter[],
                                  unsigned int ndfilter);



/* --- component command --------------------------------------------------- */

/*/
 * == *$component* TCL engine command
 *
 * Those commands manipulate components objects and return information about
 * them. They all take a compoent object as their first argument, noted
 * `$component` in the following command descriptions. Such an object is
 * typically returned by other procedures, such as
 * link:cmd-dotgen{outfilesuffix}#dotgen_components[`dotgen components`].
 */

/** Implements the command associated to a component object.
 */
int
comp_cmd(ClientData v, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  enum compidx {
    compidx_name, compidx_doc, compidx_ids, compidx_version, compidx_lang,
    compidx_email, compidx_require, compidx_crequire, compidx_clockrate,
    compidx_throws, compidx_tasks, compidx_ports, compidx_services,
    compidx_remotes, compidx_codels, compidx_types, compidx_digest,
    compidx_loc, compidx_class
  };
  static const char *args[] = {
    [compidx_name] = "name", [compidx_doc] = "doc", [compidx_ids] = "ids",
    [compidx_version] = "version", [compidx_lang] = "language",
    [compidx_email] = "email", [compidx_require] = "require",
    [compidx_crequire] = "codels-require", [compidx_clockrate] = "clock-rate",
    [compidx_throws] = "throws", [compidx_tasks] = "tasks",
    [compidx_ports] = "ports", [compidx_services] = "services",
    [compidx_remotes] = "remotes", [compidx_codels] = "codels",
    [compidx_types] = "types", [compidx_digest] = "digest",
    [compidx_loc] = "loc", [compidx_class] = "class", NULL
  };
  static const propkind argkind[] = {
    [compidx_doc] = PROP_DOC, [compidx_version] = PROP_VERSION,
    [compidx_lang] = PROP_LANG, [compidx_email] = PROP_EMAIL,
    [compidx_require] = PROP_REQUIRE, [compidx_crequire] = PROP_CODELS_REQUIRE,
    [compidx_clockrate] = PROP_CLOCKRATE
  };
  comp_s c = v;
  Tcl_Obj *r = NULL;
  prop_s p;
  int s;

  int i = compidx_name; /* return name by default */

  if (objc > 1) {
    s = Tcl_GetIndexFromObj(interp, objv[1], args, "subcommand", 0, &i);
    if (s != TCL_OK) return s;
  }
  /* 'ports', 'types' and 'digest' subcommands can have additional parameters,
   * other subcommand don't have any. */
  if (i != compidx_ports && i != compidx_codels && i != compidx_types &&
      i != compidx_digest && objc > 2) {
      Tcl_WrongNumArgs(interp, 0, objv, "$component subcommand");
      return TCL_ERROR;
  }
  if (i == compidx_types && objc < 3) {
    Tcl_WrongNumArgs(interp, 0, objv, "$component types visibility ?filter?");
    return TCL_ERROR;
  }

  switch((enum compidx)i) {
    /*/
     * [[name]]
     * === *$component name*
     *
     * Return the name of the component as a string.
     */
    case compidx_name:
      r = Tcl_NewStringObj(comp_name(c), -1);
      break;

      /*/
       * [[doc]]
       * === *$component doc*
       *
       * Return a string containing the documentation of the component defined
       * in the `doc` attributes of the `.gen` description.
       *
       * [[version]]
       * === *$component version*
       *
       * Return a string containing the version of the component defined in
       * the `version` attribute of the `.gen` description.
       *
       * [[lang]]
       * === *$component lang*
       *
       * Return a string containing the programming language of the component
       * codels defined in the `lang` attribute of the `.gen` description.
       *
       * [[email]]
       * === *$component email*
       *
       * Return a string containing the maintainer e-mail of the component
       * defined in the `email` attribute of the `.gen` description.
       */
    case compidx_doc: case compidx_version: case compidx_lang:
    case compidx_email:
      p = hash_find(comp_props(c), prop_strkind(argkind[i]));
      r = p ? Tcl_NewStringObj(prop_text(p), -1) : NULL;
      break;

      /*/
       * [[clockrate]]
       * === *$component clockrate*
       *
       * Return a link:cmd-type{outfilesuffix}[numeric floating point constant]
       * representing the period of the internal component clock in
       * seconds. This is either the value defined in the `clock-rate`
       * attributes of the `.gen` description, or the greatest common divisor
       * of all the periods of periodic tasks if no `clock-rate` is defined. If
       * the component has no periodic tasks, this raises an error.
       */
    case compidx_clockrate:
      p = hash_find(comp_props(c), prop_strkind(argkind[i]));
      r = p ? Tcl_NewStringObj(type_genref(prop_value(p)), -1) : NULL;
      break;

      /*/
       * [[ids]]
       * === *$component ids*
       *
       * Return the `ids` link:cmd-type{outfilesuffix}[`struct` type] of the
       * component.
       */
    case compidx_ids:
      p = hash_find(comp_props(c), prop_strkind(PROP_IDS));
      if (p) r = Tcl_NewStringObj(type_genref(prop_type(p)), -1);
      break;

      /*/
       * [[require]]
       * === *$component require*
       *
       * Return a list of strings containing the requirement of the component
       * defined in the `require` attribute of the `.gen` description.
       *
       * [[crequire]]
       * === *$component codels-require*
       *
       * Return a list of strings containing the requirement of the component
       * defined in the `codels-require` attribute of the `.gen` description.
       */
    case compidx_require: case compidx_crequire:
      r = Tcl_NewListObj(0, NULL);
      p = hash_find(comp_props(c), prop_strkind(argkind[i]));
      if (p) {
	clist_s l = prop_list(p);
	citer i;

	for(clist_first(l, &i); i.current; clist_next(&i)) {
	  assert(i.value->k == CST_STRING);
	  Tcl_ListObjAppendElement(
	    interp, r, Tcl_NewStringObj(const_strval(*i.value), -1));
	}
      }
      break;

      /*/
       * [[throws]]
       * === *$component throws*
       *
       * Return a list of link:cmd-type{outfilesuffix}[`exceptions`] possibly
       * raised by the commponent. This is the concatenation of all exceptions
       * defined in the component itself and its tasks and services.
       */
    case compidx_throws: {
      Tcl_Obj *argv[] = {
        Tcl_NewStringObj("object", -1),
        Tcl_NewStringObj("unique", -1),
        NULL,
      };
      prop_s p;
      hiter i, j;

      r = Tcl_NewListObj(0, NULL);

      p = hash_find(comp_props(c), prop_strkind(PROP_THROWS));
      if (p)
        for(hash_first(prop_hash(p), &i); i.current; hash_next(&i)) {
          Tcl_ListObjAppendElement(
            interp, r, Tcl_NewStringObj(type_genref(i.value), -1));
        }

      for(hash_first(comp_tasks(c), &j); j.current; hash_next(&j)) {
        p = hash_find(task_props(j.value), prop_strkind(PROP_THROWS));
        if (p)
          for(hash_first(prop_hash(p), &i); i.current; hash_next(&i)) {
            Tcl_ListObjAppendElement(
              interp, r, Tcl_NewStringObj(type_genref(i.value), -1));
          }
      }

      for(hash_first(comp_services(c), &j); j.current; hash_next(&j)) {
        p = hash_find(service_props(j.value), prop_strkind(PROP_THROWS));
        if (p)
          for(hash_first(prop_hash(p), &i); i.current; hash_next(&i)) {
            Tcl_ListObjAppendElement(
              interp, r, Tcl_NewStringObj(type_genref(i.value), -1));
          }
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
       * [[tasks]]
       * === *$component tasks*
       *
       * Return the list of link:cmd-task{outfilesuffix}[tasks] defined in the
       * commponent.
       */
    case compidx_tasks: {
      hiter i;

      r = Tcl_NewListObj(0, NULL);
      for(hash_first(comp_tasks(c), &i); i.current; hash_next(&i)) {
	Tcl_ListObjAppendElement(
	  interp, r, Tcl_NewStringObj(task_genref(i.value), -1));
      }
      break;
    }

      /*/
       * [[ports]]
       * === *$component ports* [[in|out|simple|multiple] ...]
       *
       * Return the list of link:cmd-port{outfilesuffix}[ports] defined in the
       * commponent. The list may be filtered by a keyword described
       * below. Multiple keyword may be given: they are combined with an
       * implicit 'or'.
       *
       * .Arguments
       * [horizontal]
       * 'in':: Return the input ports.
       * 'out':: Return the output ports.
       * 'simple':: Return the ports that are not of kind `multiple`.
       * 'multiple':: Return the ports that are of kind `multiple`.
       */
    case compidx_ports:
      r = port_list(interp, c, &objv[2], objc-2);
      if (!r) return TCL_ERROR;
      break;

      /*/
       * [[services]]
       * === *$component services*
       *
       * Return the list of link:cmd-service{outfilesuffix}[services] defined
       * in the commponent.
       */
    case compidx_services: {
      hiter i;

      r = Tcl_NewListObj(0, NULL);
      for(hash_first(comp_services(c), &i); i.current; hash_next(&i)) {
	Tcl_ListObjAppendElement(
	  interp, r, Tcl_NewStringObj(service_genref(i.value), -1));
      }
      break;
    }

      /*/
       * [[remotes]]
       * === *$component remotes*
       *
       * Return the list of link:cmd-service{outfilesuffix}[remote services]
       * defined in the commponent.
       */
    case compidx_remotes: {
      hiter i;

      r = Tcl_NewListObj(0, NULL);
      for(hash_first(comp_remotes(c), &i); i.current; hash_next(&i)) {
        Tcl_ListObjAppendElement(
          interp, r, Tcl_NewStringObj(remote_genref(i.value), -1));
      }
      break;
    }

      /*/
       * [[codels]]
       * === *$component codels* [[validate|simple|fsm] ...]
       *
       * Return the list of link:cmd-codel{outfilesuffix}[codels] defined in
       * the commponent. The list may be filtered by a keyword described
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
    case compidx_codels: {
      enum ckindidx {
        ckindidx_validate, ckindidx_simple, ckindidx_fsm
      };
      static const char *ckindarg[] = {
        [ckindidx_validate] = "validate", [ckindidx_simple] = "simple",
        [ckindidx_fsm] = "fsm",
        NULL
      };
      int e, f, k, filter;
      hiter i, j;

      r = Tcl_NewListObj(0, NULL);

      /* iterate over optional filters */
      filter = 0;
      for(f = 2; f < objc; f++) {
        e = Tcl_GetIndexFromObj(interp, objv[f], ckindarg, "kind", 0, &k);
        if (e != TCL_OK) return e;

        switch(k) {
          case ckindidx_validate: filter |= 1; break;
          case ckindidx_simple: filter |= 2; break;
          case ckindidx_fsm: filter |= 4; break;
        }
      }
      if (!filter) filter = 7;

      /* tasks */
      for(hash_first(comp_tasks(c), &i); i.current; hash_next(&i)) {
        for(hash_first(task_props(i.value), &j); j.current; hash_next(&j)) {
          switch (prop_kind(j.value)) {
            case PROP_VALIDATE: k = 1; break;
            case PROP_SIMPLE_CODEL: k = 2; break;
            case PROP_FSM_CODEL: k = 4; break;
            default: k = 0; break;
          }
          if (k & filter)
            Tcl_ListObjAppendElement(
              interp, r,
              Tcl_NewStringObj(codel_genref(prop_codel(j.value)), -1));
        }
      }

      /* services */
      for(hash_first(comp_services(c), &i); i.current; hash_next(&i)) {
        for(hash_first(service_props(i.value), &j); j.current; hash_next(&j)) {
          switch (prop_kind(j.value)) {
            case PROP_VALIDATE: k = 1; break;
            case PROP_SIMPLE_CODEL: k = 2; break;
            case PROP_FSM_CODEL: k = 4; break;
            default: k = 0; break;
          }
          if (k & filter)
            Tcl_ListObjAppendElement(
              interp, r,
              Tcl_NewStringObj(codel_genref(prop_codel(j.value)), -1));
        }
      }

      break;
    }

      /*/
       * [[types]]
       * === *$component types* ['visibility'] ['filter']
       *
       * Return the list of link:cmd-type{outfilesuffix}[types] defined in the
       * commponent. The list may be filtered according to a particular
       * `visiblity` and a `filter` function.
       *
       * .Arguments
       * [horizontal]
       * 'visibility':: Must be one of `public`, `private` or `external`.
       * 'public'::: Return the types that are used in services or ports and
       * are thus exposed to clients.
       * 'private'::: Return the types that are only used internally in the
       * component and do not appear in any interface.
       * 'external'::: Return the types that are used by remote services.
       *
       * 'filter'::
       * The optional filter can be used to filter out some elements from the
       * type list. The filter must be a tcl anonymous function (see tcl
       * `[apply]` command) that accepts one argument that is a genom
       * object. It  must return a boolean to indicate whether the type should
       * be included (true) or excluded (false).
       *
       * [[digest]]
       * === *$component digest* ['filter']
       *
       * Return a string containing a MD5 hash of the commponent. The hash is
       * computed using relevant data of ports (name, kind, type) and services
       * (name, parameters direction and type). The list of objects used in the
       * hash may be filtered according to a particular `filter` function.
       *
       * .Arguments
       * [horizontal]
       * 'filter'::
       * The optional filter must be a tcl anonymous function (see tcl
       * `[apply]` command) that accepts one argument that is a genom
       * object. It  must return a boolean to indicate whether the object should
       * be included (true) or excluded (false) from the hash computation.
       */
    case compidx_types:
    case compidx_digest: {
      Tcl_Obj *argv[] = {
        Tcl_NewStringObj("object", -1),
        objv[1],
        objv[0],
        objc>2 ? objv[2]:NULL,
        objc>3 ? objv[3]:NULL
      };

      Tcl_IncrRefCount(argv[0]);
      s = Tcl_EvalObjv(interp, objc+1, argv, TCL_EVAL_GLOBAL);
      Tcl_DecrRefCount(argv[0]);
      if (s != TCL_OK) return TCL_ERROR;
      r = Tcl_GetObjResult(interp);
      break;
    }

    /*/
     * [[loc]]
     * === *$component loc*
     *
     * Return a list describing the source location where that component is
     * defined. The list contains three elements: the file name, the line
     * number and the column number.
     */
    case compidx_loc: {
      Tcl_Obj *l[3] = {
	Tcl_NewStringObj(comp_loc(c).file, -1),
	Tcl_NewIntObj(comp_loc(c).line),
	Tcl_NewIntObj(comp_loc(c).col),
      };
      r = Tcl_NewListObj(3, l);
      break;
    }

    /*/
     * [[class]]
     * === *$component class*
     *
     * Always returns the string "component". Useful to determine at runtime
     * that the object is a component object.
     */
    case compidx_class:
      r = Tcl_NewStringObj(comp_strkind(comp_kind(c)), -1);
      break;
  }

  if (!r) {
    Tcl_AppendResult(interp, "undefined member \"", args[i], "\"", NULL);
    return TCL_ERROR;
  }

  Tcl_SetObjResult(interp, r);
  return TCL_OK;
}


/* --- port_list ----------------------------------------------------------- */

/** return a Tcl list of ports
 */
static Tcl_Obj *
port_list(Tcl_Interp *interp, comp_s c, Tcl_Obj *const dfilter[],
          unsigned int ndfilter)
{
  static const struct { char *opt; int k; int dir; } parg[] = {
    { "in", PORT_IN, 1 }, { "out", PORT_OUT, 1 },
    { "simple", PORT_SIMPLE, 0 }, { "multiple", PORT_MULTIPLE, 0 },
    { NULL }
  };

  hiter i;
  int d = 0;
  int k, f, s;

  Tcl_Obj *r = Tcl_NewListObj(0, NULL);

  for(hash_first(comp_ports(c), &i); i.current; hash_next(&i)) {
    k = 1;
    for(f = 0; f < ndfilter; f++) {
      s = Tcl_GetIndexFromObjStruct(
        interp, dfilter[f], parg, sizeof(parg[0]), "filter", 0, &d);
      if (s != TCL_OK) return NULL;
      if (parg[d].dir) {
        if (port_dir(i.value) != parg[d].k) k = 0;
      } else {
        if (port_kind(i.value) != parg[d].k) k = 0;
      }
    }

    if (k) {
      Tcl_ListObjAppendElement(
        interp, r, Tcl_NewStringObj(port_genref(i.value), -1));
    }
  }

  return r;
}
