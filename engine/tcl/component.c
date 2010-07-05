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
 *                                           Anthony Mallet on Tue Jul  6 2010
 */
#include "acgenom.h"

#include <assert.h>

#include <tcl.h>

#include "genom.h"
#include "engine.h"


/* --- local data ---------------------------------------------------------- */


/* --- component command --------------------------------------------------- */

/** Implements the command associated to a component object.
 */
int
comp_cmd(ClientData v, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  enum compidx {
    compidx_name, compidx_doc, compidx_ids, compidx_attr, compidx_iev,
    compidx_version, compidx_lang, compidx_email, compidx_require,
    compidx_brequire, compidx_clockrate, compidx_tasks, compidx_ports,
    compidx_services, compidx_loc, compidx_class
  };
  static const char *args[] = {
    [compidx_name] = "name", [compidx_doc] = "doc", [compidx_ids] = "ids",
    [compidx_attr] = "attribute", [compidx_iev] = "event-type",
    [compidx_version] = "version", [compidx_lang] = "language",
    [compidx_email] = "email", [compidx_require] = "require",
    [compidx_brequire] = "build-require", [compidx_clockrate] = "clock-rate",
    [compidx_tasks] = "tasks", [compidx_ports] = "ports",
    [compidx_services] = "services", [compidx_loc] = "loc",
    [compidx_class] = "class", NULL
  };
  static const propkind argkind[] = {
    [compidx_doc] = PROP_DOC, [compidx_version] = PROP_VERSION,
    [compidx_lang] = PROP_LANG, [compidx_email] = PROP_EMAIL,
    [compidx_require] = PROP_REQUIRE, [compidx_brequire] = PROP_BUILD_REQUIRE,
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
  if (i != compidx_ports) {
    /* 'ports' subcommand can have unlimited additional parameters, other
     * subcommand don't have any. */
    if (objc > 2) {
      Tcl_WrongNumArgs(interp, 0, objv, "$component subcommand");
      return TCL_ERROR;
    }
  }
  switch(i) {
    case compidx_name:
      r = Tcl_NewStringObj(comp_name(c), -1);
      break;

    case compidx_doc: case compidx_version: case compidx_lang:
    case compidx_email:
      p = hash_find(comp_props(c), prop_strkind(argkind[i]));
      r = p ? Tcl_NewStringObj(prop_text(p), -1) : NULL;
      break;

    case compidx_clockrate:
      p = hash_find(comp_props(c), prop_strkind(argkind[i]));
      r = p ? Tcl_NewStringObj(type_genref(prop_value(p)), -1) : NULL;
      break;

    case compidx_ids:
      p = hash_find(comp_props(c), prop_strkind(PROP_IDS));
      if (p) r = Tcl_NewStringObj(type_genref(prop_type(p)), -1);
      break;

    case compidx_attr: {
      hiter h;

      p = hash_find(comp_props(c), prop_strkind(PROP_ATTRIBUTE));
      r = Tcl_NewListObj(0, NULL);
      if (p) for(hash_first(prop_hash(p), &h); h.current; hash_next(&h)) {
	  Tcl_ListObjAppendElement(
	    interp, r, Tcl_NewStringObj(param_genref(h.value), -1));
	}
      break;
    }

    case compidx_iev:
      r = Tcl_NewStringObj(type_genref(comp_eventtype(c)), -1);
      break;

    case compidx_require: case compidx_brequire:
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

    case compidx_tasks: {
      hiter i;

      r = Tcl_NewListObj(0, NULL);
      for(hash_first(comp_tasks(c), &i); i.current; hash_next(&i)) {
	Tcl_ListObjAppendElement(
	  interp, r, Tcl_NewStringObj(task_genref(i.value), -1));
      }
      break;
    }

    case compidx_ports: {
      static const char *dirarg[] = {
	[PORT_IN] = "in", [PORT_OUT] = "out", [PORT_EVENT] = "event",
	NULL
      };
      int d = -1, sc;
      hiter i;

      r = Tcl_NewListObj(0, NULL);
      for(hash_first(comp_ports(c), &i); i.current; hash_next(&i)) {
	if (objc < 3) {
	  Tcl_ListObjAppendElement(
	    interp, r, Tcl_NewStringObj(port_genref(i.value), -1));
	} else for(sc = 2; sc < objc; sc++) {
	    s = Tcl_GetIndexFromObj(interp, objv[sc], dirarg, "direction", 0, &d);
	    if (s != TCL_OK) return s;

	    if (d == port_kind(i.value)) {
	      Tcl_ListObjAppendElement(
		interp, r, Tcl_NewStringObj(port_genref(i.value), -1));
	      break;
	    }
	  }
      }
      break;
    }

    case compidx_services: {
      hiter i;

      r = Tcl_NewListObj(0, NULL);
      for(hash_first(comp_services(c), &i); i.current; hash_next(&i)) {
	Tcl_ListObjAppendElement(
	  interp, r, Tcl_NewStringObj(service_genref(i.value), -1));
      }
      break;
    }

    case compidx_loc: {
      Tcl_Obj *l[3] = {
	Tcl_NewStringObj(comp_loc(c).file, -1),
	Tcl_NewIntObj(comp_loc(c).line),
	Tcl_NewIntObj(comp_loc(c).col),
      };
      r = Tcl_NewListObj(3, l);
      break;
    }

    case compidx_class:
      r = Tcl_NewStringObj("component", -1);
      break;
  }

  if (!r) {
    Tcl_AppendResult(interp, "undefined member \"", args[i], "\"", NULL);
    return TCL_ERROR;
  }

  Tcl_SetObjResult(interp, r);
  return TCL_OK;
}
