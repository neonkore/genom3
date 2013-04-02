/*
 * Copyright (c) 2012-2013 LAAS/CNRS
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
 *                                           Anthony Mallet on Thu Jul 26 2012
 */
#include "acgenom.h"

#include <assert.h>
#include <string.h>

#include <tcl.h>

#include "genom.h"
#include "engine.h"


/* --- remote command ------------------------------------------------------ */

/** Implements the command associated to a remote object.
 */
int
remote_cmd(ClientData v, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  enum remoteidx {
    remoteidx_name, remoteidx_kind, remoteidx_comp, remoteidx_type,
    remoteidx_params, remoteidx_digest, remoteidx_loc, remoteidx_class
  };
  static const char *args[] = {
    [remoteidx_name] = "name",  [remoteidx_kind] = "kind",
    [remoteidx_comp] = "component", [remoteidx_type] = "type",
    [remoteidx_params] = "parameters", [remoteidx_digest] = "digest",
    [remoteidx_loc] = "loc", [remoteidx_class] = "class", NULL
  };
  remote_s s = v;
  Tcl_Obj *r = NULL;
  int e;

  enum remoteidx i = remoteidx_name; /* return name by default */

  if (objc > 1) {
    e = Tcl_GetIndexFromObj(interp, objv[1], args, "subcommand", 0, (int *)&i);
    if (e != TCL_OK) return e;
  }
  if (i != remoteidx_params) {
    /* 'parameters' subcommand can have unlimited additional parameters, other
     * subcommand don't have any. */
    if (objc > 2) {
      Tcl_WrongNumArgs(interp, 0, objv, "$remote subcommand");
      return TCL_ERROR;
    }
  }
  switch(i) {
    case remoteidx_name:
      r = Tcl_NewStringObj(remote_name(s), -1);
      break;

    case remoteidx_kind:
      r = Tcl_NewStringObj(
        service_strkind(service_kind(remote_service(s))), -1);
      break;

    case remoteidx_comp:
      r = Tcl_NewStringObj(comp_genref(remote_comp(s)), -1);
      break;

    case remoteidx_type:
      r = Tcl_NewStringObj(type_genref(remote_type(s)), -1);
      break;

    case remoteidx_params: {
      enum remotepdiridx {
        rmtpdiridx_in, rmtpdiridx_out, rmtpdiridx_inout
      };
      static const char *dirarg[] = {
        [rmtpdiridx_in] = "in", [rmtpdiridx_out] = "out",
        [rmtpdiridx_inout] = "inout", NULL
      };
      int f, d = -1, sc;
      hiter i;

      /* initialize output list to the empty list */
      r = Tcl_NewListObj(0, NULL);

      /* build filtered list of parameters */
      for(hash_first(remote_params(s), &i); i.current; hash_next(&i)) {

	/* iterate over optional direction filters */
        f = (objc < 3);
	for(sc = 2; !f && sc < objc; sc++) {
          e = Tcl_GetIndexFromObj(interp, objv[sc], dirarg, "direction", 0, &d);
          if (e != TCL_OK) return e;

          switch(d) {
            case rmtpdiridx_in:
              if (param_dir(i.value) == P_IN) f = 1;
              break;
            case rmtpdiridx_out:
              if (param_dir(i.value) == P_OUT) f = 1;
              break;
            case rmtpdiridx_inout:
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

    case remoteidx_digest: {
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

    case remoteidx_loc: {
      Tcl_Obj *l[3] = {
	Tcl_NewStringObj(remote_loc(s).file, -1),
	Tcl_NewIntObj(remote_loc(s).line),
	Tcl_NewIntObj(remote_loc(s).col),
      };
      r = Tcl_NewListObj(3, l);
      break;
    }

    case remoteidx_class:
      r = Tcl_NewStringObj("remote", -1);
      break;
  }

  if (!r) {
    Tcl_AppendResult(interp, "undefined member \"", args[i], "\"", NULL);
    return TCL_ERROR;
  }

  Tcl_SetObjResult(interp, r);
  return TCL_OK;
}
