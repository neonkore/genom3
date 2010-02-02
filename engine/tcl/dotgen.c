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
#include <libgen.h>

#include <tcl.h>

#include "genom.h"
#include "engine.h"


/* --- genom --------------------------------------------------------------- */

int
dg_genom_program(ClientData d, Tcl_Interp *interp, int objc,
		 Tcl_Obj *const objv[])
{
  Tcl_Obj *path = Tcl_NewStringObj(runopt.genom, -1);
  Tcl_IncrRefCount(path);
  Tcl_SetObjResult(interp, Tcl_FSGetNormalizedPath(interp, path));
  Tcl_DecrRefCount(path);
  return TCL_OK;
}

int
dg_genom_cmdline(ClientData d, Tcl_Interp *interp, int objc,
		 Tcl_Obj *const objv[])
{
  Tcl_SetObjResult(interp, Tcl_NewStringObj(runopt.cmdline, -1));
  return TCL_OK;
}

int
dg_genom_version(ClientData d, Tcl_Interp *interp, int objc,
		 Tcl_Obj *const objv[])
{
  Tcl_SetObjResult(interp, Tcl_NewStringObj(PACKAGE_STRING, -1));
  return TCL_OK;
}

int
dg_genom_debug(ClientData d, Tcl_Interp *interp, int objc,
	       Tcl_Obj *const objv[])
{
  Tcl_SetObjResult(interp, Tcl_NewBooleanObj(runopt.debug));
  return TCL_OK;
}

int
dg_genom_stdout(ClientData d, Tcl_Interp *interp, int objc,
		Tcl_Obj *const objv[])
{
  extern int eng_swapfd(int from, int to);
  extern int stdfd[4];
  static int verbose = 0;
  int v;

  if (objc == 1) {
    Tcl_SetObjResult(interp, Tcl_NewBooleanObj(runopt.verbose));
    return TCL_OK;
  }

  if (Tcl_GetBooleanFromObj(interp, objv[1], &v) != TCL_OK)
    return TCL_ERROR;

  if ((v && verbose) || (!v && !verbose))
    return TCL_OK;

  eng_swapfd(stdfd[1], 1);
  verbose = v;
  return TCL_OK;
}


/* --- template ------------------------------------------------------------ */

int
dg_template_name(ClientData v, Tcl_Interp *interp, int objc,
		 Tcl_Obj *const objv[])
{
  char name[PATH_MAX];

  strlcpy(name, runopt.tmpl, sizeof(name));
  Tcl_SetObjResult(interp, Tcl_NewStringObj(basename(name), -1));
  return TCL_OK;
}

int
dg_template_dir(ClientData v, Tcl_Interp *interp, int objc,
		Tcl_Obj *const objv[])
{
  Tcl_SetObjResult(interp, Tcl_NewStringObj(runopt.tmpl, -1));
  return TCL_OK;
}

int
dg_template_sysdir(ClientData v, Tcl_Interp *interp, int objc,
		   Tcl_Obj *const objv[])
{
  Tcl_SetObjResult(interp, Tcl_NewStringObj(runopt.sysdir, -1));
  return TCL_OK;
}

int
dg_template_tmpdir(ClientData v, Tcl_Interp *interp, int objc,
		   Tcl_Obj *const objv[])
{
  Tcl_SetObjResult(interp, Tcl_NewStringObj(runopt.tmpdir, -1));
  return TCL_OK;
}


/* --- input --------------------------------------------------------------- */

int
dg_input_file(ClientData v, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  Tcl_SetObjResult(interp, Tcl_NewStringObj(runopt.input, -1));
  return TCL_OK;
}

int
dg_input_base(ClientData v, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  char base[PATH_MAX];

  strlcpy(base, runopt.input, sizeof(base));
  Tcl_SetObjResult(interp, Tcl_NewStringObj(basename(base), -1));
  return TCL_OK;
}

int
dg_input_dir(ClientData v, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  char dir[PATH_MAX];

  strlcpy(dir, runopt.input, sizeof(dir));
  Tcl_SetObjResult(interp, Tcl_NewStringObj(dirname(dir), -1));
  return TCL_OK;
}

int
dg_input_notice(ClientData v, Tcl_Interp *interp, int objc,
		Tcl_Obj *const objv[])
{
  if (!runopt.notice) {
    Tcl_SetObjResult(interp, Tcl_NewStringObj("no notice found", -1));
    return TCL_ERROR;
  }

  Tcl_SetObjResult(interp, Tcl_NewStringObj(runopt.notice, -1));
  return TCL_OK;
}


/* --- types --------------------------------------------------------------- */

/** Return the list of types
 */
int
dg_types(ClientData v, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  const char *p;
  Tcl_Obj *l;
  hiter i;

  if (objc > 2) {
    Tcl_WrongNumArgs(interp, 1, objv, "?pattern?");
    return TCL_ERROR;
  }
  if (objc > 1) { p = Tcl_GetString(objv[1]); } else { p = NULL; }

  l = Tcl_NewListObj(0, NULL);
  for(hash_first(type_all(), &i); i.current; hash_next(&i)) {
    assert(type_fullname(i.value));
    if (p && !Tcl_StringMatch(type_fullname(i.value), p)) continue;

    Tcl_ListObjAppendElement(
      interp, l, Tcl_NewStringObj(type_genref(i.value), -1));
  }

  Tcl_SetObjResult(interp, l);
  return TCL_OK;
}


/* --- components ----------------------------------------------------------- */

/** Return the list of components
 */
int
dg_components(ClientData v, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  const char *p;

  if (objc > 2) {
    Tcl_WrongNumArgs(interp, 1, objv, "?pattern?");
    return TCL_ERROR;
  }
  if (objc > 1) { p = Tcl_GetString(objv[1]); } else { p = NULL; }

  if (p && !Tcl_StringMatch(comp_name(comp_dotgen()), p)) {
    Tcl_ResetResult(interp);
    return TCL_OK;
  }

  Tcl_AppendResult(interp, comp_genref(comp_dotgen()), NULL);
  return TCL_OK;
}
