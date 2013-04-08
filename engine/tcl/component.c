/*
 * Copyright (c) 2010-2013 LAAS/CNRS
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

/** Implements the command associated to a component object.
 */
int
comp_cmd(ClientData v, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  enum compidx {
    compidx_name, compidx_doc, compidx_ids, compidx_version, compidx_lang,
    compidx_email, compidx_require, compidx_crequire, compidx_clockrate,
    compidx_throws, compidx_tasks, compidx_ports, compidx_services,
    compidx_remotes, compidx_digest, compidx_loc, compidx_class
  };
  static const char *args[] = {
    [compidx_name] = "name", [compidx_doc] = "doc", [compidx_ids] = "ids",
    [compidx_version] = "version", [compidx_lang] = "language",
    [compidx_email] = "email", [compidx_require] = "require",
    [compidx_crequire] = "codels-require", [compidx_clockrate] = "clock-rate",
    [compidx_throws] = "throws", [compidx_tasks] = "tasks",
    [compidx_ports] = "ports", [compidx_services] = "services",
    [compidx_remotes] = "remotes", [compidx_digest] = "digest",
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
  /* 'ports' and 'digest' subcommands can have additional parameters, other
   * subcommand don't have any. */
  if (i != compidx_ports && i != compidx_digest && objc > 2) {
      Tcl_WrongNumArgs(interp, 0, objv, "$component subcommand");
      return TCL_ERROR;
  }

  switch((enum compidx)i) {
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

    case compidx_throws: {
      Tcl_Obj *argv[] = {
        Tcl_NewStringObj("lsort", -1),
        Tcl_NewStringObj("-unique", -1),
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

    case compidx_tasks: {
      hiter i;

      r = Tcl_NewListObj(0, NULL);
      for(hash_first(comp_tasks(c), &i); i.current; hash_next(&i)) {
	Tcl_ListObjAppendElement(
	  interp, r, Tcl_NewStringObj(task_genref(i.value), -1));
      }
      break;
    }

    case compidx_ports:
      r = port_list(interp, c, &objv[2], objc-2);
      if (!r) return TCL_ERROR;
      break;

    case compidx_services: {
      hiter i;

      r = Tcl_NewListObj(0, NULL);
      for(hash_first(comp_services(c), &i); i.current; hash_next(&i)) {
	Tcl_ListObjAppendElement(
	  interp, r, Tcl_NewStringObj(service_genref(i.value), -1));
      }
      break;
    }

    case compidx_remotes: {
      hiter i;

      r = Tcl_NewListObj(0, NULL);
      for(hash_first(comp_remotes(c), &i); i.current; hash_next(&i)) {
        Tcl_ListObjAppendElement(
          interp, r, Tcl_NewStringObj(remote_genref(i.value), -1));
      }
      break;
    }

    case compidx_digest: {
      Tcl_Obj *argv[] = {
        Tcl_NewStringObj("object", -1),
        Tcl_NewStringObj("digest", -1),
        objv[0],
        objc>2 ? objv[2]:NULL
      };

      Tcl_IncrRefCount(argv[0]);
      Tcl_IncrRefCount(argv[1]);
      s = Tcl_EvalObjv(interp, objc>2 ? 4:3, argv, TCL_EVAL_GLOBAL);
      Tcl_DecrRefCount(argv[1]);
      Tcl_DecrRefCount(argv[0]);
      if (s != TCL_OK) return TCL_ERROR;
      r = Tcl_GetObjResult(interp);
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
