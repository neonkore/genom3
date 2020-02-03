/*
 * Copyright (c) 2010-2013,2020 LAAS/CNRS
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


/* --- param command ------------------------------------------------------- */

/*/
 * == *$param* TCL engine command
 *
 * Those commands manipulate parameter objects and return information about
 * them. They all take a parameter object as their first argument, noted
 * `$param` in the following command descriptions. Such an object is
 * typically returned by other procedures, such as
 * link:cmd-codel{outfilesuffix}#params[`$codel params`].
 */

/** Implements the command associated to a param object.
 */
int
param_cmd(ClientData v, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  enum paramidx {
    paramidx_name, paramidx_src, paramidx_dir, paramidx_member, paramidx_base,
    paramidx_type, paramidx_port, paramidx_remote, paramidx_param,
    paramidx_initer, paramidx_loc, paramidx_class
  };
  static const char *args[] = {
    [paramidx_name] = "name", [paramidx_src] = "src",
    [paramidx_dir] = "direction", [paramidx_member] = "member",
    [paramidx_base] = "base", [paramidx_type] = "type",
    [paramidx_port] = "port", [paramidx_remote] = "remote",
    [paramidx_param] = "param", [paramidx_initer] = "initializer",
    [paramidx_loc] = "loc", [paramidx_class] = "class", NULL
  };
  param_s p = v;
  Tcl_Obj *r;
  int s;

  int i = paramidx_name; /* return name by default */

  if (objc > 1) {
    s = Tcl_GetIndexFromObj(interp, objv[1], args, "subcommand", 0, &i);
    if (s != TCL_OK) return s;
  }

  /* 'initializer' subcommand can have one additional parameters, other
   * subcommand don't have any. */
  if (i == paramidx_initer) {
    if (objc > 3) {
      Tcl_WrongNumArgs(interp, 0, objv, "$param initializer ?type?");
      return TCL_ERROR;
    }
  } else {
    if (objc > 2) {
      Tcl_WrongNumArgs(interp, 0, objv, "$param subcommand");
      return TCL_ERROR;
    }
  }

  switch(i) {
    /*/
     * [[name]]
     * === *$param name*
     *
     * Return the name of the parameter as a string.
     */
    case paramidx_name:
      r = Tcl_NewStringObj(param_name(p), -1);
      break;

    /*/
     * [[src]]
     * === *$param src*
     *
     * Return a string indicating the origin of the parameter:
     * [horizontal]
     * `ids`:: defined in the IDS.
     * `local`:: defined locally for a running instance of a service.
     * `port`:: for port objects passed as a parameter.
     * `remote`:: for remote objects passed as a parameter.
     */
    case paramidx_src:
      r = Tcl_NewStringObj(param_strsrc(param_src(p)), -1);
      break;

    /*/
     * [[dir]]
     * === *$param dir*
     *
     * Return a string indicating the direction of the parameter:
     * [horizontal]
     * `local`:: defined locally for a running instance of a service.
     * `in`:: passed as input.
     * `out`:: passed as output.
     * `inout`:: passed as input and output.
     */
    case paramidx_dir:
      r = Tcl_NewStringObj(param_strdir(param_dir(p)), -1);
      break;

    /*/
     * [[member]]
     * === *$param member*
     *
     * For `ids` parameters, this returns a valid string in the current
     * programming language to access the parameter inside the IDS.
     * For other kinds of parameters, this raises an error.
     */
    case paramidx_member: {
      Tcl_Obj *argv[] = {
        Tcl_NewStringObj("language::member", -1),
        Tcl_NewStringObj(type_genref(param_base(p)), -1),
        Tcl_NewListObj(0, NULL)
      };
      clist_s l = param_member(p);
      citer i;

      if (!l) {
        r = Tcl_NewStringObj("", -1);
        break;
      }
      Tcl_IncrRefCount(argv[0]);
      Tcl_IncrRefCount(argv[1]);
      Tcl_IncrRefCount(argv[2]);

      for(clist_first(l, &i); i.current; clist_next(&i)) {
	Tcl_ListObjAppendElement(
	  interp, argv[2], Tcl_NewStringObj(const_strval(*i.value), -1));
      }

      s = Tcl_EvalObjv(interp, 3, argv, TCL_EVAL_GLOBAL);
      Tcl_DecrRefCount(argv[0]);
      Tcl_DecrRefCount(argv[1]);
      Tcl_DecrRefCount(argv[2]);
      if (s != TCL_OK) return TCL_ERROR;
      r = Tcl_GetObjResult(interp);
      break;
    }

    /*/
     * [[base]]
     * === *$param base*
     *
     * This returns the type of the source of the parameter. For `local`
     * parameters, this is the type of the local parameter in the service
     * definition. For IDS parameters, this is the `ids` type. For `port` or
     * `remote` kinds, this is the port or remote object.
     */
    case paramidx_base:
      r = Tcl_NewStringObj(type_genref(param_base(p)), -1);
      break;

    /*/
     * [[type]]
     * === *$param type*
     *
     * This returns the type object of the parameter.
     */
    case paramidx_type:
      r = Tcl_NewStringObj(type_genref(param_type(p)), -1);
      break;

    /*/
     * [[port]]
     * === *$param port*
     *
     * This returns the port object of `port` parameters.
     */
    case paramidx_port:
      switch(param_src(p)) {
	case P_PORT:
	  r = Tcl_NewStringObj(port_genref(param_port(p)), -1);
	  break;

	default: r = NULL; break;
      }
      break;

    /*/
     * [[remote]]
     * === *$param remote*
     *
     * This returns the remote object of `remote` parameters.
     */
    case paramidx_remote:
      if (param_src(p) == P_REMOTE)
        r = Tcl_NewStringObj(remote_genref(param_remote(p)), -1);
      else
        r = NULL;
      break;

    /*/
     * [[param]]
     * === *$param param*
     *
     * This returns the service parameter of `local` parameters passed to a
     * codel.
     */
    case paramidx_param:
      if (param_src(p) == P_LOCAL && param_param(p))
        r = Tcl_NewStringObj(param_genref(param_param(p)), -1);
      else
        r = NULL;
      break;

    /*/
     * [[initializer]]
     * === *$param initializer*
     *
     * This returns the initializer object associated to the parameter, or an
     * error if there is no initializer.
     *
     * An initializer object defines the following methods:
     *
     * [horizontal]
     * `member`:: A valid string in the current programming language to access
     * the initialized member of the parameter.
     * `doc`:: The documentation of this initializer.
     * `kind`:: The data type of the default value of the parameter, or the
     * string `compound` for recursive initializers.
     * `value`:: This is either a link:cmd-type{outfilesuffix}[constant value
     * object] of the type `kind`, or a recursively defined initializer object
     * if `kind` is equal to `compound`.
     * `loc`:: Returns the source location as a triplet.
     * `class`:: Always returns "initializer".
     */
    case paramidx_initer:
      r = NULL;
      if (objc == 2) {
	if (param_initer(p))
	  r = Tcl_NewStringObj(initer_genref(param_initer(p)), -1);
      } else {
	Tcl_CmdInfo info;
	Tcl_Command c = Tcl_GetCommandFromObj(interp, objv[2]);
	if (c && Tcl_GetCommandInfoFromToken(c, &info)) {
	  initer_s initer = param_typeiniter(p, info.objClientData);
	  if (initer)
	    r = Tcl_NewStringObj(initer_genref(initer), -1);
	  else {
	    Tcl_AppendResult(interp, "no initializer for type", NULL);
	    return TCL_ERROR;
	  }
	} else {
	  Tcl_AppendResult(interp, "no such type \"", Tcl_GetString(objv[2]),
			   "\"", NULL);
	  return TCL_ERROR;
	}
      }
      break;

    /*/
     * [[loc]]
     * === *$param loc*
     *
     * Return a list describing the source location where that parameter is
     * defined. The list contains three elements: the file name, the line
     * number and the column number.
     */
    case paramidx_loc: {
      Tcl_Obj *l[3] = {
	Tcl_NewStringObj(param_loc(p).file, -1),
	Tcl_NewIntObj(param_loc(p).line),
	Tcl_NewIntObj(param_loc(p).col),
      };
      r = Tcl_NewListObj(3, l);
      break;
    }

    /*/
     * [[class]]
     * === *$param class*
     *
     * Always returns the string "parameter". Useful to determine at runtime
     * that the object is a parameter object.
     */
    case paramidx_class:
      r = Tcl_NewStringObj("parameter", -1);
      break;
  }

  if (!r) {
    Tcl_AppendResult(interp, "undefined member \"", args[i], "\"", NULL);
    return TCL_ERROR;
  }

  Tcl_SetObjResult(interp, r);
  return TCL_OK;
}


/* --- initer command ------------------------------------------------------ */

/** Implements the command associated to a initializer object.
 */
int
initer_cmd(ClientData v, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  enum initeridx {
    initeridx_member, initeridx_doc, initeridx_kind, initeridx_value,
    initeridx_loc, initeridx_class
  };
  static const char *args[] = {
    [initeridx_member] = "member", [initeridx_doc] = "doc",
    [initeridx_kind] = "kind", [initeridx_value] = "value",
    [initeridx_loc] = "loc", [initeridx_class] = "class", NULL
  };
  initer_s i = v;
  Tcl_Obj *r = NULL;
  int s;

  int c = initeridx_kind; /* return kind by default */

  if (objc > 2) {
    Tcl_WrongNumArgs(interp, 0, objv, "$initializer subcommand");
    return TCL_ERROR;
  }
  if (objc > 1) {
    s = Tcl_GetIndexFromObj(interp, objv[1], args, "subcommand", 0, &c);
    if (s != TCL_OK) return s;
  }
  switch(c) {
    case initeridx_member:
      if (initer_member(i))
	r = Tcl_NewStringObj(initer_member(i), -1);
      else if (initer_index(i) != -1U)
	r = Tcl_NewIntObj(initer_index(i));
      break;

    case initeridx_doc:
      if (initer_doc(i))
	r = Tcl_NewStringObj(initer_doc(i), -1);
      break;

    case initeridx_kind:
      if (initer_compound(i))
	r = Tcl_NewStringObj("compound", -1);
      else
	r = Tcl_NewStringObj(const_strkind(initer_value(i).k), -1);
      break;

    case initeridx_value:
      if (initer_compound(i)) {
	r = Tcl_NewListObj(0, NULL);
	for(i = initer_compound(i); i; i = initer_next(i)) {
	  Tcl_ListObjAppendElement(
	    interp, r, Tcl_NewStringObj(initer_genref(i), -1));
	}
      } else
	r = Tcl_NewStringObj(const_strval(initer_value(i)), -1);
      break;

    case initeridx_loc: {
      Tcl_Obj *l[3] = {
        Tcl_NewStringObj(initer_loc(i).file, -1),
        Tcl_NewIntObj(initer_loc(i).line),
        Tcl_NewIntObj(initer_loc(i).col),
      };
      r = Tcl_NewListObj(3, l);
      break;
    }

    case initeridx_class:
      r = Tcl_NewStringObj("initializer", -1);
      break;
  }

  if (!r) {
    Tcl_AppendResult(interp, "undefined member \"", args[c], "\"", NULL);
    return TCL_ERROR;
  }

  Tcl_SetObjResult(interp, r);
  return TCL_OK;
}
