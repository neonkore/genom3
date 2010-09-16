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
#include <errno.h>
#include <libgen.h>
#include <string.h>
#include <fcntl.h>

#include <tcl.h>

#include "genom.h"
#include "engine.h"

/* --- genom --------------------------------------------------------------- */

/* \section dotgen genom
 * \index dotgen genom
 *
 * Those commands implement access to genom program parameters or general
 * information.
 */

/* \proc dotgen genom program
 * \index dotgen genom program
 *
 * Returns the absolute path to the GenoM executable currently running.
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

/* \proc dotgen genom cmdline
 * \index dotgen genom cmdline
 *
 * Returns a string containing the options passed to the GenoM program.
 */
int
dg_genom_cmdline(ClientData d, Tcl_Interp *interp, int objc,
		 Tcl_Obj *const objv[])
{
  Tcl_SetObjResult(interp, Tcl_NewStringObj(runopt.cmdline, -1));
  return TCL_OK;
}

/* \proc dotgen genom version
 * \index dotgen genom version
 *
 * Returns the full version string of the GenoM program.
 */
int
dg_genom_version(ClientData d, Tcl_Interp *interp, int objc,
		 Tcl_Obj *const objv[])
{
  Tcl_SetObjResult(interp, Tcl_NewStringObj(PACKAGE_STRING, -1));
  return TCL_OK;
}

/* \proc dotgen genom debug
 * \index dotgen genom debug
 *
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

/* \proc dotgen genom stdout [on]
 * \index dotgen genom stdout
 *
 * With optional boolean argument {\em on}, turns on or off the standard output
 * channel of the template engine.
 *
 * Without argument, the procedure returns the current activation status of the
 * standard output channel.
 *
 * \arg		[on] Turn on/off standard output channel.
 * \return	When called without argument, returns a boolean indicating the
 *		current status (on/off) of the standard output channel.
 */
int
dg_genom_stdout(ClientData d, Tcl_Interp *interp, int objc,
		Tcl_Obj *const objv[])
{
  extern int eng_swapfd(int from, int to);
  extern int stdfd[3][2];
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

  eng_swapfd(stdfd[0][1], 1);

  verbose = v;
  return TCL_OK;
}


/* --- template ------------------------------------------------------------ */

/* \section dotgen template
 * \index dotgen template
 *
 * Those commands return information about the template currently being parsed.
 */

/* \proc dotgen template name
 * \index dotgen template name
 *
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

/* \proc dotgen template dir
 * \index dotgen template dir
 *
 * Return a path to the current template directory (the directory holding the
 * template.tcl file).
 */
int
dg_template_dir(ClientData v, Tcl_Interp *interp, int objc,
		Tcl_Obj *const objv[])
{
  Tcl_SetObjResult(interp, Tcl_NewStringObj(runopt.tmpl, -1));
  return TCL_OK;
}

/* \proc dotgen template sysdir
 * \index dotgen template sysdir
 *
 * Return a path to the genom system directory.
 */
int
dg_template_sysdir(ClientData v, Tcl_Interp *interp, int objc,
		   Tcl_Obj *const objv[])
{
  Tcl_SetObjResult(interp, Tcl_NewStringObj(runopt.sysdir, -1));
  return TCL_OK;
}

/* \proc dotgen template tmpdir
 * \index dotgen template tmpdir
 *
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

/* \section dotgen input
 * \index dotgen input
 *
 * Those commands return information about the current genom input file (.gen
 * file).
 */

/* \proc dotgen input file
 * \index dotgen input file
 *
 * Return the full path to the current .gen file.
 */
int
dg_input_file(ClientData v, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  Tcl_SetObjResult(interp, Tcl_NewStringObj(runopt.input, -1));
  return TCL_OK;
}

/* \proc dotgen input base
 * \index dotgen input base
 *
 * Return the base name of the current .gen file, i.e. the file name with all
 * directories stripped out.
 */
int
dg_input_base(ClientData v, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  char base[PATH_MAX];

  strlcpy(base, runopt.input, sizeof(base));
  Tcl_SetObjResult(interp, Tcl_NewStringObj(basename(base), -1));
  return TCL_OK;
}

/* \proc dotgen input dir
 * \index dotgen input dir
 *
 * Return the directory name of the current .gen file.
 */
int
dg_input_dir(ClientData v, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  char dir[PATH_MAX];

  strlcpy(dir, runopt.input, sizeof(dir));
  Tcl_SetObjResult(interp, Tcl_NewStringObj(dirname(dir), -1));
  return TCL_OK;
}

/* \proc dotgen input notice
 * \index dotgen input notice
 *
 * Return the copyright notice (as text) found in the .gen file. This notice
 * can actually be any text and is the content of the special comment starting
 * with the three caracters '/' '*' '/', near the beginning of the .gen file.
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


/* --- parse --------------------------------------------------------------- */

/* \section dotgen parse
 * \proc dotgen parse file\string|string data
 * \index dotgen parse
 *
 * Parse .gen data either from a file or from a string. When parsing is
 * successful, the corresponding objects are exported to the engine.
 *
 * \arg file\string|string	Specify if parsing from a file or from a
 *				string.
 * \arg data			When parsing from a file, data is the file
 *				name. When parsing from a string, data is the
 *				data to be parsed.
 */
int
dg_parse(ClientData v, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  enum parseidx { parseidx_file, parseidx_string };
  static const char *args[] = {
    [parseidx_file] = "file", [parseidx_string] = "string", NULL
  };
  int fd;
  int k, s;

  if (objc != 3) {
    Tcl_WrongNumArgs(interp, 1, objv, "file|string data");
    return TCL_ERROR;
  }

  s = Tcl_GetIndexFromObj(interp, objv[1], args, "subcommand", 0, &k);
  if (s != TCL_OK) return s;

  switch(k) {
    case parseidx_file: {
      extern tloc curloc;
      fd = open(Tcl_GetString(objv[2]), O_RDONLY);
      if (fd < 0) {
	Tcl_AppendResult(interp, "unable to open \"", Tcl_GetString(objv[2]),
			 "\": ", strerror(errno), NULL);
	return TCL_ERROR;
      }
      dotgen_input(DG_INPUT_FILE, fd);
      curloc.file = Tcl_GetString(objv[2]);
      break;
    }

    case parseidx_string:
      dotgen_input(DG_INPUT_BUFFER, Tcl_GetString(objv[2]));
      break;
  }

  s = dotgenparse();

  if (k == parseidx_file) close(fd);
  if (s) {
    Tcl_AppendResult(interp, "parse errors", NULL);
    return TCL_ERROR;
  }

  s = engine_export(interp);
  return s?TCL_ERROR:TCL_OK;
}


/* --- types --------------------------------------------------------------- */

/* \section dotgen types
 *
 * This command return information about the type definitions in the .gen
 * file.
 */

/* \proc dotgen types [pattern]
 * \index dotgen types
 *
 * This command returns the list of type objects that are defined in the
 * current .gen file. This list may be filtered with the optional {\em pattern}
 * argument. Each element of the returned list is a type command that can be
 * used to access detailed information about that particular type object.
 *
 * \arg		[pattern] Filter on the type names. The filter may contain a
 *		glob-like pattern (with * or ? wildcards). Only the types
 *		whose name match the pattern will be returned.
 * \return	A list of type objects.
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
      assert(type_fullname(i.value));
      if (p && !Tcl_StringMatch(type_fullname(i.value), p)) continue;

      Tcl_ListObjAppendElement(
	interp, l, Tcl_NewStringObj(type_genref(i.value), -1));
    }
  }

  Tcl_SetObjResult(interp, l);
  return TCL_OK;
}


/* --- components ----------------------------------------------------------- */

/* \section dotgen components
 *
 * This command return information about the components definitions in the .gen
 * file.
 */

/* \proc dotgen components [pattern]
 * \index dotgen components
 *
 * This command returns the list of components that are defined in the current
 * .gen file (normally just one). This list may be filtered with the optional
 * {\em pattern} argument. Each element of the returned list is a component
 * command that can be used to access detailed information about each
 * particular component object.
 *
 * \arg		[pattern] Filter on the component name. The filter may contain
 *		a glob-like pattern (with * or ? wildcards). Only the components
 *		whose name match the pattern will be returned.
 * \return	A list of component objects.
 */
int
dg_components(ClientData v, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
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
  for(c = comp_current(); c; c = comp_next(c)) {
    if (p && !Tcl_StringMatch(comp_name(c), p)) continue;

    Tcl_ListObjAppendElement(interp, l, Tcl_NewStringObj(comp_genref(c), -1));
  }

  Tcl_SetObjResult(interp, l);
  return TCL_OK;
}
