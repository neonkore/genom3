/*
 * Copyright (c) 2010-2012 LAAS/CNRS
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

/*/ @nodebeproc{dotgen genom, Genom program path and command line}
 * @findex dotgen genom
 *
 * Those commands implement access to genom program parameters or general
 * information.
 */

/*/ @deffn {TCL Backend} {dotgen genom program}
 *
 * Return the absolute path to the GenoM executable currently running.
 * @end deffn
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

/*/ @deffn {TCL Backend} {dotgen genom cmdline}
 *
 * Returns a string containing the options passed to the @genom{} program.
 * @end deffn
 */
int
dg_genom_cmdline(ClientData d, Tcl_Interp *interp, int objc,
		 Tcl_Obj *const objv[])
{
  Tcl_SetObjResult(interp, Tcl_NewStringObj(runopt.cmdline, -1));
  return TCL_OK;
}

/*/ @deffn {TCL Backend} {dotgen genom version}
 *
 * Returns the full version string of the @genom{} program.
 * @end deffn
 */
int
dg_genom_version(ClientData d, Tcl_Interp *interp, int objc,
		 Tcl_Obj *const objv[])
{
  Tcl_SetObjResult(interp, Tcl_NewStringObj(PACKAGE_STRING, -1));
  return TCL_OK;
}

/*/ @deffn {TCL Backend} {dotgen genom templates}
 *
 * Return the list of all currently available templates name.
 * @end deffn
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

/*/ @deffn {TCL Backend} {dotgen genom debug}
 *
 * Returns a boolean indicating whether genom was invoked in debugging mode or
 * not.
 * @end deffn
 */
int
dg_genom_debug(ClientData d, Tcl_Interp *interp, int objc,
	       Tcl_Obj *const objv[])
{
  Tcl_SetObjResult(interp, Tcl_NewBooleanObj(runopt.debug));
  return TCL_OK;
}

/*/ @deffn {TCL Backend} {dotgen genom stdout} [@var{onoff}]
 *
 * With optional boolean argument @var{onoff}, turns on or off the standard
 * output channel of the template engine.
 *
 * Without argument, the procedure returns the current activation status of the
 * standard output channel.
 *
 * @@args
 * @item @var{onoff}
 * Optional argument to turn on or off the standard output channel.
 * @@end args
 *
 * @@returns
 * When called without argument, returns a boolean indicating the current
 * status (on/off) of the standard output channel.
 * @@end returns
 * @end deffn
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

  eng_swapfd(stdfd[1][1], 1);
  eng_swapfd(stdfd[2][1], 2);

  verbose = v;
  return TCL_OK;
}


/* --- template ------------------------------------------------------------ */

/*/ @nodebeproc{dotgen template, Template path and directories}
 * @findex dotgen template
 *
 * Those commands return information about the template currently being parsed.
 */

/*/ @deffn {TCL Backend} {dotgen template name}
 *
 * Return the current template name.
 * @end deffn
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

/*/ @deffn {TCL Backend} {dotgen template dir}
 *
 * Return a path to the template source directory (the directory holding the
 * template.tcl file).
 * @end deffn
 */
int
dg_template_dir(ClientData v, Tcl_Interp *interp, int objc,
		Tcl_Obj *const objv[])
{
  Tcl_SetObjResult(interp, Tcl_NewStringObj(runopt.tmpl, -1));
  return TCL_OK;
}

/*/ @deffn {TCL Backend} {dotgen template builtindir}
 *
 * Return a path to the genom builtin templates source directory.
 * @end deffn
 */
int
dg_template_builtindir(ClientData v, Tcl_Interp *interp, int objc,
                       Tcl_Obj *const objv[])
{
  Tcl_SetObjResult(interp, Tcl_NewStringObj(TMPLDIR, -1));
  return TCL_OK;
}

/*/ @deffn {TCL Backend} {dotgen template tmpdir}
 *
 * Return a path to the temporary directory where the template engine writes
 * its temporary files.
 * @end deffn
 */
int
dg_template_tmpdir(ClientData v, Tcl_Interp *interp, int objc,
		   Tcl_Obj *const objv[])
{
  Tcl_SetObjResult(interp, Tcl_NewStringObj(runopt.tmpdir, -1));
  return TCL_OK;
}


/* --- input --------------------------------------------------------------- */

/*/ @nodebeproc{dotgen input, Input file name and path}
 * @findex dotgen input
 *
 * Those commands return information about the current genom input file (.gen
 * file).
 */

/*/ @deffn {TCL Backend} {dotgen input file}
 *
 * Return the full path to the current .gen input file.
 * @end deffn
 */
int
dg_input_file(ClientData v, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  Tcl_SetObjResult(interp, Tcl_NewStringObj(runopt.input, -1));
  return TCL_OK;
}

/*/ @deffn {TCL Backend} {dotgen input base}
 *
 * Return the base name of the current .gen file, i.e. the file name with all
 * directories stripped out.
 * @end deffn
 */
int
dg_input_base(ClientData v, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  char base[PATH_MAX];

  strlcpy(base, runopt.input, sizeof(base));
  Tcl_SetObjResult(interp, Tcl_NewStringObj(basename(base), -1));
  return TCL_OK;
}

/*/ @deffn {TCL Backend} {dotgen input dir}
 *
 * Return the directory part of the current .gen file.
 * @end deffn
 */
int
dg_input_dir(ClientData v, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  char dir[PATH_MAX];

  strlcpy(dir, runopt.input, sizeof(dir));
  Tcl_SetObjResult(interp, Tcl_NewStringObj(dirname(dir), -1));
  return TCL_OK;
}

/*/ @deffn {TCL Backend} {dotgen input notice}
 *
 * Return the copyright notice (as text) found in the .gen file. This notice
 * can actually be any text and is the content of the special comment starting
 * with the three caracters @code{/} @code{*} @code{/}, near the beginning of
 * the .gen file.
 * @end deffn
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

/*/ @deffn {TCL Backend} {dotgen input deps}
 *
 * Return the comprehensive list of input files processed so far. This includes
 * the input @code{.gen} file itself, plus any other file required, directly or
 * indirectly, via a @code{#include} directive. This list is typically used to
 * generate dependency information in a Makefile.
 * @end deffn
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

/*/ @nodebeproc{dotgen parse, Process additional input}
 * @deffn {TCL Backend} {dotgen parse} @b{file|string} @var{data}
 *
 * Parse additional @code{.gen} data either from a file or from a string. When
 * parsing is successful, the corresponding objects are exported to the
 * backend.
 *
 * @@args
 * @item @b{file|string}
 * Specify if parsing from a file or from a string.
 * @item @var{data}
 * When parsing from a file, data is the file name. When parsing from a string,
 * data is the string to be parsed.
 * @@end args
 * @end deffn
 */
int
dg_parse(ClientData v, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  enum parseidx { parseidx_file, parseidx_string };
  static const char *args[] = {
    [parseidx_file] = "file", [parseidx_string] = "string", NULL
  };
  int pipefd[2];
  int k, s;
  Tcl_Obj *path;

  if (objc != 3) {
    Tcl_WrongNumArgs(interp, 1, objv, "file|string data");
    return TCL_ERROR;
  }

  s = Tcl_GetIndexFromObj(interp, objv[1], args, "subcommand", 0, &k);
  if (s != TCL_OK) return s;

  switch(k) {
    case parseidx_file: {
      extern tloc curloc;

      path = Tcl_DuplicateObj(objv[2]);
      Tcl_IncrRefCount(path);
      if (Tcl_FSGetPathType(path) != TCL_PATH_ABSOLUTE) {
        Tcl_Obj *l = Tcl_NewListObj(0, NULL);
        Tcl_IncrRefCount(l);
        Tcl_ListObjAppendElement(interp, l, Tcl_NewStringObj(runopt.tmpl, -1));
        Tcl_ListObjAppendElement(interp, l, path);
        Tcl_DecrRefCount(path);
        path = Tcl_FSJoinPath(l, 2);
        Tcl_IncrRefCount(path);
        Tcl_DecrRefCount(l);
      }

      if (pipe(pipefd) < 0) {
        Tcl_AppendResult(interp,
                         "cannot create a pipe to cpp:", strerror(errno), NULL);
        Tcl_DecrRefCount(path);
        return TCL_ERROR;
      }
      dotgen_input(DG_INPUT_FILE, pipefd[0]);
      curloc.file = string(Tcl_GetString(path));
      cpp_invoke(Tcl_GetString(path), pipefd[1]);
      Tcl_DecrRefCount(path);
      break;
    }

    case parseidx_string:
      dotgen_input(DG_INPUT_BUFFER, Tcl_GetString(objv[2]));
      break;
  }

  nerrors = nwarnings = 0;
  s = dotgenparse();
  if (k == parseidx_file) {
    if (cpp_wait()) s = 2;
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

/*/ @nodebeproc{dotgen types, Data type definitions from the specification}
 * @deffn {TCL Backend} {dotgen types} [@var{pattern}]
 *
 * This command returns the list of type objects that are defined in the
 * current @code{.gen} file. This list may be filtered with the optional
 * @var{pattern} argument. Each element of the returned list is a type command
 * that can be used to access detailed information about that particular type
 * object.
 *
 * @@args
 * @item @var{pattern}
 * Filter the type names with @var{pattern}. The filter may contain a glob-like
 * pattern (with @code{*} or @code{?} wildcards). Only the types whose name
 * match the pattern will be returned.
 * @@end args
 * @@returns
 * A list of type objects of class @code{type}.
 * @@end returns
 * @end deffn
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

/*/ @nodebeproc{dotgen components,Components definitions from the specification}
 * @deffn {TCL Backend} {dotgen components} [@var{pattern}]
 *
 * This command returns the list of components that are defined in the current
 * @code{.gen} file. This list may be filtered with the optional @var{pattern}
 * argument. Each element of the returned list is a component command that can
 * be used to access detailed information about each particular component
 * object.
 *
 * @@args
 * @item @var{pattern}
 * Filter the component name. The filter may contain a glob-like pattern (with
 * @code{*} or @code{?} wildcards). Only the components whose name match the
 * pattern will be returned.
 * @@end args
 * @@returns
 * A list of component objects of class @code{component}.
 * @@end returns
 * @end deffn
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
  for(c = comp_first(); c; c = comp_next(c)) {
    if (p && !Tcl_StringMatch(comp_name(c), p)) continue;

    Tcl_ListObjAppendElement(interp, l, Tcl_NewStringObj(comp_genref(c), -1));
  }

  Tcl_SetObjResult(interp, l);
  return TCL_OK;
}
