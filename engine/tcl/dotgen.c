/*
 * Copyright (c) 2010-2014,2017,2020 LAAS/CNRS
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
#include <errno.h>
#include <libgen.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include <tcl.h>

#include "genom.h"
#include "engine.h"

/* --- genom --------------------------------------------------------------- */

/*/
 * == *dotgen* TCL engine command
 */

/*/
 * [[dotgen_genom]]
 * === *dotgen genom*
 *
 * Those commands implement access to genom program parameters or general
 * information.
 */

/*/ `dotgen genom program`::
 * Return the absolute path to the GenoM executable currently running.
 */
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

/*/ `dotgen genom cmdline`::
 * Returns a string containing the options passed to the `genom3` program.
 */
int
dg_genom_cmdline(ClientData d, Tcl_Interp *interp, int objc,
		 Tcl_Obj *const objv[])
{
  Tcl_SetObjResult(interp, Tcl_NewStringObj(runopt.cmdline, -1));
  return TCL_OK;
}

/*/ `dotgen genom version`::
 * Returns the full version string of the `genom3` program.
 */
int
dg_genom_version(ClientData d, Tcl_Interp *interp, int objc,
		 Tcl_Obj *const objv[])
{
  Tcl_SetObjResult(interp, Tcl_NewStringObj(PACKAGE_STRING, -1));
  return TCL_OK;
}

/*/ `dotgen genom templates`::
 * Return the list of all currently available templates name.
 */
int
dg_genom_templates(ClientData v, Tcl_Interp *interp, int objc,
                   Tcl_Obj *const objv[])
{
  Tcl_Obj *l;
  char **list, **t;
  int s;

  s = eng_listtmpl(&list);
  if (s) {
    Tcl_AppendResult(interp, strerror(s), NULL);
    return TCL_ERROR;
  }

  l = Tcl_NewListObj(0, NULL);
  for(t = list; t && *t; t++) {
    Tcl_ListObjAppendElement(interp, l, Tcl_NewStringObj(*t, -1));
    free(*t);
  }
  free(list);

  Tcl_SetObjResult(interp, l);
  return TCL_OK;
}

/*/ `dotgen genom debug`::
 * Returns a boolean indicating whether genom was invoked in debugging mode or
 * not.
 */
int
dg_genom_debug(ClientData d, Tcl_Interp *interp, int objc,
	       Tcl_Obj *const objv[])
{
  Tcl_SetObjResult(interp, Tcl_NewBooleanObj(runopt.debug));
  return TCL_OK;
}

/*/ `dotgen genom verbose`::
 * Returns a boolean indicating whether genom was invoked in verbose mode or
 * not.
 */
int
dg_genom_verbose(ClientData d, Tcl_Interp *interp, int objc,
		Tcl_Obj *const objv[])
{
  Tcl_SetObjResult(interp, Tcl_NewBooleanObj(runopt.verbose));
  return TCL_OK;
}


/* --- template ------------------------------------------------------------ */

/*/
 * [[dotgen_template]]
 * === *dotgen template*
 *
 * Those commands return information about the template currently being parsed.
 */

/*/ `dotgen template name`::
 * Return the current template name.
 */
int
dg_template_name(ClientData v, Tcl_Interp *interp, int objc,
		 Tcl_Obj *const objv[])
{
  char name[PATH_MAX];

  strlcpy(name, runopt.tmpl, sizeof(name));
  Tcl_SetObjResult(interp, Tcl_NewStringObj(basename(name), -1));
  return TCL_OK;
}

/*/ `dotgen template dir`::
 * Return a path to the template source directory (the directory holding the
 * template.tcl file).
 */
int
dg_template_dir(ClientData v, Tcl_Interp *interp, int objc,
		Tcl_Obj *const objv[])
{
  Tcl_SetObjResult(interp, Tcl_NewStringObj(runopt.tmpl, -1));
  return TCL_OK;
}

/*/ `dotgen template builtindir`::
 * Return a path to the genom builtin templates source directory.
 */
int
dg_template_builtindir(ClientData v, Tcl_Interp *interp, int objc,
                       Tcl_Obj *const objv[])
{
  Tcl_SetObjResult(interp, Tcl_NewStringObj(TMPLDIR, -1));
  return TCL_OK;
}

/*/ `dotgen template tmpdir`::
 * Return a path to the temporary directory where the template engine writes
 * its temporary files.
 */
int
dg_template_tmpdir(ClientData v, Tcl_Interp *interp, int objc,
		   Tcl_Obj *const objv[])
{
  Tcl_SetObjResult(interp, Tcl_NewStringObj(runopt.tmpdir, -1));
  return TCL_OK;
}


/* --- input --------------------------------------------------------------- */

/*/
 * [[dotgen_input]]
 * === *dotgen input*
 *
 * Those commands return information about the current genom input file (.gen
 * file).
 */

/*/ `dotgen input notice`::
 * Return the copyright notice (as text) found in the .gen file. This notice
 * can actually be any text and is the content of the special comment starting
 * with the three caracters `/` `*` `/`, near the beginning of
 * the .gen file.
 */
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

/*/ `dotgen input deps`::
 * Return the comprehensive list of input files processed so far. This includes
 * the input `.gen` file itself, plus any other file required, directly or
 * indirectly, via a `#include` directive. This list is typically used to
 * generate dependency information in a Makefile.
 */
int
dg_input_deps(ClientData v, Tcl_Interp *interp, int objc,
              Tcl_Obj *const objv[])
{
  Tcl_Obj *d;
  hiter i;

  d = Tcl_NewListObj(0, NULL);
  if (dotgen_hdeps()) {
    for(hash_first(dotgen_hdeps(), &i); i.current; hash_next(&i)) {
      Tcl_ListObjAppendElement(interp, d, Tcl_NewStringObj(i.value, -1));
    }
  }
  Tcl_SetObjResult(interp, d);
  return TCL_OK;
}


/* --- parse --------------------------------------------------------------- */

/*/
 * [[dotgen_parse]]
 * === *dotgen parse* {file|string} 'data'
 *
 * Parse additional `.gen` data either from a file or from a string. When
 * parsing is successful, the corresponding objects are exported to the
 * backend.
 *
 * .Arguments
 * [horizontal]
 * 'file|string':: Specify if parsing from a file or from a string.
 *
 * 'data':: When parsing from a file, data is the file name. When parsing from
 * a string, data is the string to be parsed.
 */
int
dg_parse(ClientData v, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  enum parseidx { parseidx_file, parseidx_string };
  static const char *args[] = {
    [parseidx_file] = "file", [parseidx_string] = "string", NULL
  };
  int pipefd[2];
  int k, s, i;

  if (objc < 3) {
    Tcl_WrongNumArgs(interp, 1, objv, "file|string data ?...?");
    return TCL_ERROR;
  }

  s = Tcl_GetIndexFromObj(interp, objv[1], args, "subcommand", 0, &k);
  if (s != TCL_OK) return s;

  nerrors = nwarnings = 0;

  switch(k) {
    case parseidx_file: {
      const char *notice;
      char *path[objc-1];

      for(i = 0; i < objc-2; i++) {
        path[i] = Tcl_GetString(objv[i+2]);

        notice = cpp_getnotice(path[i]);
        if (notice) bufcat((char **)&runopt.notice, notice);
      }
      path[i] = NULL;

      if (pipe(pipefd) < 0) {
        Tcl_AppendResult(interp,
                         "cannot create a pipe to cpp:", strerror(errno), NULL);
        return TCL_ERROR;
      }
      dotgen_input(DG_INPUT_FILE, pipefd[0]);
      cpp_invoke(path, pipefd[1]);
      s = dotgenparse();
      if (cpp_wait()) s = 2;
      break;
    }

    case parseidx_string:
      for(i = 2; i < objc; i++) {
        dotgen_input(DG_INPUT_BUFFER, Tcl_GetString(objv[i]));
        s = dotgenparse();
        if (s) break;
      }
      break;
  }

  if (!s) s = dotgen_consolidate();
  if (s || nerrors) {
    char msg[128];
    snprintf(msg, sizeof(msg),
             s?"fatal errors":"%d error%s", nerrors, nerrors>1?"s":"");
    Tcl_AppendResult(interp, msg, NULL);
    return TCL_ERROR;
  }

  s = engine_export(interp, Tcl_GetSlave(interp, "slave"));
  return s?TCL_ERROR:TCL_OK;
}


/* --- types --------------------------------------------------------------- */

/*/
 * [[dotgen_types]]
 * === *dotgen types* ['pattern']
 *
 * This command returns the list of link:cmd-type{outfilesuffix}[`type
 * objects`] that are defined in the current `.gen` file. This list may be
 * filtered with the optional 'pattern' argument. Each element of the returned
 * list is a type command that can be used to access detailed information about
 * that particular type object.
 *
 * .Arguments
 * [horizontal]
 * 'pattern'::
 * Filter the type names with 'pattern'. The filter may contain a glob-like
 * pattern (with `*` or `?` wildcards). Only the types whose name
 * match the pattern will be returned.

 * .Return value
 * A list of type objects of class `type`.
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
  if (type_all()) {
    for(hash_first(type_all(), &i); i.current; hash_next(&i)) {
      if (p && !type_fullname(i.value)) continue;
      if (p && !Tcl_StringMatch(type_fullname(i.value), p)) continue;

      Tcl_ListObjAppendElement(
	interp, l, Tcl_NewStringObj(type_genref(i.value), -1));
    }
  }

  Tcl_SetObjResult(interp, l);
  return TCL_OK;
}


/* --- components ----------------------------------------------------------- */

static int
dg_clist(compkind k, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  const char *p;
  Tcl_Obj *l;
  comp_s c;

  if (objc > 2) {
    Tcl_WrongNumArgs(interp, 1, objv, "?pattern?");
    return TCL_ERROR;
  }
  if (objc > 1) { p = Tcl_GetString(objv[1]); } else { p = NULL; }

  l = Tcl_NewListObj(0, NULL);
  for(c = comp_first(); c; c = comp_next(c)) {
    if (comp_kind(c) != k) continue;
    if (p && !Tcl_StringMatch(comp_name(c), p)) continue;

    Tcl_ListObjAppendElement(interp, l, Tcl_NewStringObj(comp_genref(c), -1));
  }

  Tcl_SetObjResult(interp, l);
  return TCL_OK;
}


/*/
 * [[dotgen_components]]
 * === *dotgen components* ['pattern']
 *
 * This command returns the list of
 * link:cmd-component{outfilesuffix}[`components`] that are defined in the
 * current `.gen` file. This list may be filtered with the optional 'pattern'
 * argument. Each element of the returned list is a component command that can
 * be used to access detailed information about each particular component
 * object.
 *
 * .Arguments
 * [horizontal]
 * 'pattern'::
 * Filter the component name. The filter may contain a glob-like pattern (with
 * `*` or `?` wildcards). Only the components whose name match the
 * pattern will be returned.
 *
 * .Return value
 * A list of component objects of class `component`.
 */
int
dg_components(ClientData v, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  return dg_clist(COMP_REGULAR, interp, objc, objv);
}


/*/
 * [[dotgen_interfaces]]
 * === *dotgen interfaces* ['pattern']
 *
 * This command returns the list of interfaces that are defined in the current
 * `.gen` file. This list may be filtered with the optional 'pattern'
 * argument. Each element of the returned list is an interface command that can
 * be used to access detailed information about each particular interface
 * object.
 *
 * .Arguments
 * 'pattern'::
 * Filter the interface name. The filter may contain a glob-like pattern (with
 * `*` or `?` wildcards). Only the components whose name match the
 * pattern will be returned.
 *
 * .Return value
 * A list of interface objects of class `interface`.
 */
int
dg_interfaces(ClientData v, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  return dg_clist(COMP_IFACE, interp, objc, objv);
}
