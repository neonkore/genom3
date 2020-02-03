/*
 * Copyright (c) 2010-2013,2017,2020 LAAS/CNRS
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


/* --- port command -------------------------------------------------------- */

/*/
 * == *$port* TCL engine command
 *
 * Those commands manipulate port objects and return information about
 * them. They all take a port object as their first argument, noted
 * `$port` in the following command descriptions. Such an object is
 * typically returned by other procedures, such as
 * link:cmd-component{outfilesuffix}#services[`$component ports`].
 */


/** Implements the command associated to a port object.
 */
int
port_cmd(ClientData v, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  enum portidx {
    portidx_name, portidx_dir, portidx_kind, portidx_comp, portidx_type,
    portidx_datatype, portidx_doc, portidx_throws, portidx_loc, portidx_class
  };
  static const char *args[] = {
    [portidx_name] = "name", [portidx_dir] = "dir", [portidx_kind] = "kind",
    [portidx_comp] = "component", [portidx_type] = "type",
    [portidx_datatype] = "datatype", [portidx_doc] = "doc",
    [portidx_throws] = "throws", [portidx_loc] = "loc",
    [portidx_class] = "class", NULL
  };
  port_s p = v;
  prop_s prop;
  Tcl_Obj *r;
  int s;

  int i = portidx_name; /* return name by default */

  if (objc > 2) {
    Tcl_WrongNumArgs(interp, 0, objv, "$port subcommand");
    return TCL_ERROR;
  }
  if (objc > 1) {
    s = Tcl_GetIndexFromObj(interp, objv[1], args, "subcommand", 0, &i);
    if (s != TCL_OK) return s;
  }
  switch(i) {
    /*/
     * [[name]]
     * === *$port name*
     *
     * Return the name of the port as a string.
     */
    case portidx_name:
      r = Tcl_NewStringObj(port_name(p), -1);
      break;

      /*/
       * [[dir]]
       * === *$port dir*
       *
       * Return `in` or `out` depending on the direction of the port.
       */
    case portidx_dir:
      r = Tcl_NewStringObj(port_strdir(port_dir(p)), -1);
      break;

      /*/
       * [[kind]]
       * === *$port kind*
       *
       * Return `simple` or `multiple` depending on the kind of the port.
       */
    case portidx_kind:
      r = Tcl_NewStringObj(port_strkind(port_kind(p)), -1);
      break;

      /*/
       * [[component]]
       * === *$port component*
       *
       * Return the link:cmd-component{outfilesuffix}[component object] in which
       * the port is defined.
       */
    case portidx_comp:
      r = Tcl_NewStringObj(comp_genref(port_comp(p)), -1);
      break;

      /*/
       * [[type]]
       * === *$port type*
       *
       * Return the link:cmd-type{outfilesuffix}[type object] defining the
       * port itself. (not the same as `datatype`, see below).
       */
    case portidx_type:
      r = port_type(p)? Tcl_NewStringObj(type_genref(port_type(p)), -1) : NULL;
      break;

      /*/
       * [[datatype]]
       * === *$port datatype*
       *
       * Return the link:cmd-type{outfilesuffix}[data type] to be published in
       * the port.
       */
    case portidx_datatype:
      r = port_datatype(p)?
        Tcl_NewStringObj(type_genref(port_datatype(p)), -1) : NULL;
      break;

      /*/
       * [[doc]]
       * === *$port doc*
       *
       * Return a string containing the documentation of the port defined
       * in the `doc` attributes of the `.gen` description.
       */
    case portidx_doc:
      prop = hash_find(port_props(p), prop_strkind(PROP_DOC));
      r = prop ? Tcl_NewStringObj(prop_text(prop), -1) : NULL;
      break;

      /*/
       * [[throws]]
       * === *$port throws*
       *
       * Return a list of link:cmd-type{outfilesuffix}[`exceptions`] possibly
       * raised by the port.
       */
    case portidx_throws: {
      prop_s t;
      hiter i;

      r = Tcl_NewListObj(0, NULL);

      t = hash_find(comp_props(port_comp(p)), prop_strkind(PROP_THROWS));
      if (t)
        for(hash_first(prop_hash(t), &i); i.current; hash_next(&i)) {
          Tcl_ListObjAppendElement(
            interp, r, Tcl_NewStringObj(type_genref(i.value), -1));
        }
      break;
    }

    /*/
     * [[loc]]
     * === *$port loc*
     *
     * Return a list describing the source location where that port is
     * defined. The list contains three elements: the file name, the line
     * number and the column number.
     */
    case portidx_loc: {
      Tcl_Obj *l[3] = {
	Tcl_NewStringObj(port_loc(p).file, -1),
	Tcl_NewIntObj(port_loc(p).line),
	Tcl_NewIntObj(port_loc(p).col),
      };
      r = Tcl_NewListObj(3, l);
      break;
    }

    /*/
     * [[class]]
     * === *$port class*
     *
     * Always returns the string "port". Useful to determine at runtime
     * that the object is a port object.
     */
    case portidx_class:
      r = Tcl_NewStringObj("port", -1);
      break;
  }

  if (!r) {
    Tcl_AppendResult(interp, "undefined member \"", args[i], "\"", NULL);
    return TCL_ERROR;
  }

  Tcl_SetObjResult(interp, r);
  return TCL_OK;
}
