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

#include <tcl.h>

#include "genom.h"
#include "engine.h"


/* --- type command -------------------------------------------------------- */

/** Implements the command associated to a type object.
 */
int
type_cmd(ClientData v, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  enum typeidx {
    typeidx_kind, typeidx_name, typeidx_fullname, typeidx_scope, typeidx_final,
    typeidx_type, typeidx_length, typeidx_value, typeidx_valuekind,
    typeidx_members, typeidx_discriminator, typeidx_loc, typeidx_class
  };
  static const char *args[] = {
    [typeidx_kind] = "kind", [typeidx_name] = "name",
    [typeidx_fullname] = "fullname", [typeidx_scope] = "scope",
    [typeidx_final] = "final", [typeidx_type] = "type",
    [typeidx_length] = "length", [typeidx_value] = "value",
    [typeidx_valuekind] = "valuekind", [typeidx_members] = "members",
    [typeidx_discriminator] = "discriminator", [typeidx_loc] = "loc",
    [typeidx_class] = "class", NULL
  };
  idltype_s t = v;
  Tcl_Obj *r = NULL;
  int s;

  int i = typeidx_fullname; /* return full name by default */

  if (objc > 2) {
    Tcl_WrongNumArgs(interp, 0, objv, "$type subcommand");
    return TCL_ERROR;
  }
  if (objc > 1) {
    s = Tcl_GetIndexFromObj(interp, objv[1], args, "subcommand", 0, &i);
    if (s != TCL_OK) return s;
  }
  switch(i) {
    case typeidx_kind:
      r = Tcl_NewStringObj(type_strkind(type_kind(t)), -1);
      break;

    case typeidx_name:
      r = type_name(t) ? Tcl_NewStringObj(type_name(t), -1) : NULL;
      break;

    case typeidx_fullname:
      r = type_fullname(t) ? Tcl_NewStringObj(type_fullname(t), -1) : NULL;
      break;

    case typeidx_scope:
      r = NULL; /* XXX TBD */
      break;

    case typeidx_final:
      r = Tcl_NewStringObj(type_genref(type_final(t)), -1);
      break;

    case typeidx_type:
      switch(type_kind(t)) {
	case IDL_ENUMERATOR: case IDL_SEQUENCE: case IDL_ARRAY: case IDL_CONST:
	case IDL_TYPEDEF: case IDL_MEMBER: case IDL_CASE:
	case IDL_FORWARD_STRUCT: case IDL_FORWARD_UNION:
	  r = Tcl_NewStringObj(type_genref(type_type(t)), -1);
	  break;

	default: r = NULL; break;
      }
      break;

    case typeidx_length:
      switch(type_kind(t)) {
	case IDL_SEQUENCE: case IDL_ARRAY: case IDL_STRING:
	  if (type_length(t) != -1U) {
	    r = Tcl_NewIntObj(type_length(t));
	    break;
	  }

	default: break;
      }
      break;

    case typeidx_value:
      switch(type_kind(t)) {
	case IDL_CONST:
	  r = Tcl_NewStringObj(const_strval(type_constvalue(t)), -1);
	  break;

	default: r = NULL; break;
      }
      break;

    case typeidx_valuekind:
      switch(type_kind(t)) {
	case IDL_CONST:
	  r = Tcl_NewStringObj(const_strkind(type_constvalue(t).k), -1);
	  break;

	default: r = NULL; break;
      }
      break;

    case typeidx_members: {
      idltype_s e;
      hiter m;
      switch(type_kind(t)) {
	case IDL_STRUCT: case IDL_UNION: case IDL_ENUM:
	  r = Tcl_NewListObj(0, NULL);
	  for(e = type_first(t, &m); e; e = type_next(&m)) {
	    Tcl_ListObjAppendElement(
	      interp, r, Tcl_NewStringObj(type_genref(e), -1));
	  }
	  break;

	default: r = NULL; break;
      }
      break;

    case typeidx_discriminator:
      switch(type_kind(t)) {
	case IDL_UNION:
	  r = Tcl_NewStringObj(type_genref(type_discriminator(t)), -1);
	  break;

	default: r = NULL; break;
      }
      break;

    case typeidx_loc:
      if (!type_loc(t).file) { r = NULL; } else {
	Tcl_Obj *l[3] = {
	  Tcl_NewStringObj(type_loc(t).file, -1),
	  Tcl_NewIntObj(type_loc(t).line),
	  Tcl_NewIntObj(type_loc(t).col),
	};
	r = Tcl_NewListObj(3, l);
      }
      break;
    }

    case typeidx_class:
      r = Tcl_NewStringObj("type", -1);
      break;
  }

  if (!r) {
    Tcl_AppendResult(interp, "no such member \"", args[i], "\"", NULL);
    return TCL_ERROR;
  }

  Tcl_SetObjResult(interp, r);
  return TCL_OK;
}


/* --- component command --------------------------------------------------- */

/** Implements the command associated to a component object.
 */
int
comp_cmd(ClientData v, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  enum compidx {
    compidx_name, compidx_doc, compidx_ids, compidx_iev, compidx_version,
    compidx_lang, compidx_email, compidx_require, compidx_brequire,
    compidx_clockrate, compidx_tasks, compidx_ports, compidx_services,
    compidx_loc, compidx_class
  };
  static const char *args[] = {
    [compidx_name] = "name", [compidx_doc] = "doc", [compidx_ids] = "ids",
    [compidx_iev] = "event-type", [compidx_version] = "version",
    [compidx_lang] = "language", [compidx_email] = "email",
    [compidx_require] = "require", [compidx_brequire] = "build-require",
    [compidx_clockrate] = "clock-rate", [compidx_tasks] = "tasks",
    [compidx_ports] = "ports", [compidx_services] = "services",
    [compidx_loc] = "loc", [compidx_class] = "class", NULL
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

  if (objc > 2) {
    Tcl_WrongNumArgs(interp, 0, objv, "$component subcommand");
    return TCL_ERROR;
  }
  if (objc > 1) {
    s = Tcl_GetIndexFromObj(interp, objv[1], args, "subcommand", 0, &i);
    if (s != TCL_OK) return s;
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
      p = hash_find(comp_props(c), prop_strkind(PROP_IDS)); assert(p);
      r = Tcl_NewStringObj(type_genref(prop_type(p)), -1);
      break;

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
      hiter i;

      r = Tcl_NewListObj(0, NULL);
      for(hash_first(comp_ports(c), &i); i.current; hash_next(&i)) {
	Tcl_ListObjAppendElement(
	  interp, r, Tcl_NewStringObj(port_genref(i.value), -1));
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


/* --- task command -------------------------------------------------------- */

/** Implements the command associated to a task object.
 */
int
task_cmd(ClientData v, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  enum taskidx {
    taskidx_name, taskidx_doc, taskidx_period, taskidx_delay, taskidx_priority,
    taskidx_stack, taskidx_codels, taskidx_throws, taskidx_loc, taskidx_class
  };
  static const char *args[] = {
    [taskidx_name] = "name", [taskidx_doc] = "doc",
    [taskidx_period] = "period", [taskidx_delay] = "delay",
    [taskidx_priority] = "priority", [taskidx_stack] = "stack",
    [taskidx_codels] = "codels", [taskidx_throws] = "throws",
    [taskidx_loc] = "loc", [taskidx_class] = "class", NULL
  };
  static const propkind argkind[] = {
    [taskidx_period] = PROP_PERIOD, [taskidx_delay] = PROP_DELAY,
    [taskidx_priority] = PROP_PRIORITY, [taskidx_stack] = PROP_STACK
  };
  task_s t = v;
  Tcl_Obj *r = NULL;
  prop_s p;
  int s;

  int i = taskidx_name; /* return name by default */

  if (objc > 2) {
    Tcl_WrongNumArgs(interp, 0, objv, "$task subcommand");
    return TCL_ERROR;
  }
  if (objc > 1) {
    s = Tcl_GetIndexFromObj(interp, objv[1], args, "subcommand", 0, &i);
    if (s != TCL_OK) return s;
  }
  switch(i) {
    case taskidx_name:
      r = Tcl_NewStringObj(task_name(t), -1);
      break;

    case taskidx_doc:
      p = hash_find(task_props(t), prop_strkind(PROP_DOC));
      r = p ? Tcl_NewStringObj(prop_text(p), -1) : NULL;
      break;

    case taskidx_period: case taskidx_delay:
    case taskidx_priority: case taskidx_stack:
      p = hash_find(task_props(t), prop_strkind(argkind[i]));
      r = p ? Tcl_NewStringObj(type_genref(prop_value(p)), -1) : NULL;
      break;

    case taskidx_throws: {
      prop_s p = hash_find(task_props(t), prop_strkind(PROP_THROWS));
      hiter i;

      r = Tcl_NewListObj(0, NULL);
      if (p)
	for(hash_first(prop_identifiers(p), &i); i.current; hash_next(&i)) {
	  Tcl_ListObjAppendElement(
	    interp, r, Tcl_NewStringObj(type_genref(i.value), -1));
	}
      break;
    }

    case taskidx_codels: {
      hiter i;

      r = Tcl_NewListObj(0, NULL);
      for(hash_first(task_props(t), &i); i.current; hash_next(&i)) {
	if (prop_kind(i.value) != PROP_CODEL) continue;
	Tcl_ListObjAppendElement(
	  interp, r, Tcl_NewStringObj(codel_genref(prop_codel(i.value)), -1));
      }
      break;
    }

    case taskidx_loc: {
      Tcl_Obj *l[3] = {
	Tcl_NewStringObj(task_loc(t).file, -1),
	Tcl_NewIntObj(task_loc(t).line),
	Tcl_NewIntObj(task_loc(t).col),
      };
      r = Tcl_NewListObj(3, l);
      break;
    }

    case taskidx_class:
      r = Tcl_NewStringObj("task", -1);
      break;
  }

  if (!r) {
    Tcl_AppendResult(interp, "undefined member \"", args[i], "\"", NULL);
    return TCL_ERROR;
  }

  Tcl_SetObjResult(interp, r);
  return TCL_OK;
}


/* --- port command -------------------------------------------------------- */

/** Implements the command associated to a port object.
 */
int
port_cmd(ClientData v, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  enum portidx {
    portidx_name, portidx_kind, portidx_type, portidx_loc, portidx_class
  };
  static const char *args[] = {
    [portidx_name] = "name", [portidx_kind] = "kind",
    [portidx_type] = "type", [portidx_loc] = "loc", [portidx_class] = "class",
    NULL
  };
  port_s p = v;
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
    case portidx_name:
      r = Tcl_NewStringObj(port_name(p), -1);
      break;

    case portidx_kind:
      r = Tcl_NewStringObj(port_strkind(port_kind(p)), -1);
      break;

    case portidx_type:
      switch(port_kind(p)) {
	case PORT_IN: case PORT_OUT:
	  r = Tcl_NewStringObj(type_genref(port_type(p)), -1);
	  break;

	case PORT_EVENT: r = NULL; break;
      }
      break;

    case portidx_loc: {
      Tcl_Obj *l[3] = {
	Tcl_NewStringObj(port_loc(p).file, -1),
	Tcl_NewIntObj(port_loc(p).line),
	Tcl_NewIntObj(port_loc(p).col),
      };
      r = Tcl_NewListObj(3, l);
      break;
    }

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


/* --- service command ----------------------------------------------------- */

/** Implements the command associated to a service object.
 */
int
service_cmd(ClientData v, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  enum serviceidx {
    serviceidx_name, serviceidx_doc, serviceidx_task, serviceidx_validate,
    serviceidx_codels, serviceidx_params, serviceidx_throws,
    serviceidx_interrupts, serviceidx_before, serviceidx_after,
    serviceidx_loc, serviceidx_class
  };
  static const char *args[] = {
    [serviceidx_name] = "name", [serviceidx_doc] = "doc",
    [serviceidx_task] = "task", [serviceidx_validate] = "validate",
    [serviceidx_codels] = "codels", [serviceidx_params] = "parameters",
    [serviceidx_throws] = "throws", [serviceidx_interrupts] = "interrupts",
    [serviceidx_before] = "before", [serviceidx_after] = "after",
    [serviceidx_loc] = "loc", [serviceidx_class] = "class", NULL
  };
  static const propkind argkind[] = {
    [serviceidx_codels] = PROP_CODEL, [serviceidx_validate] = PROP_VALIDATE,
    [serviceidx_interrupts] = PROP_INTERRUPTS,
    [serviceidx_before] = PROP_BEFORE, [serviceidx_after] = PROP_AFTER
  };
  service_s s = v;
  Tcl_Obj *r = NULL;
  prop_s p;
  int e;

  enum serviceidx i = serviceidx_name; /* return name by default */

  if (objc > 2) {
    Tcl_WrongNumArgs(interp, 0, objv, "$service subcommand");
    return TCL_ERROR;
  }
  if (objc > 1) {
    e = Tcl_GetIndexFromObj(interp, objv[1], args, "subcommand", 0, (int *)&i);
    if (e != TCL_OK) return e;
  }
  switch(i) {
    case serviceidx_name:
      r = Tcl_NewStringObj(service_name(s), -1);
      break;

    case serviceidx_doc:
      p = hash_find(service_props(s), prop_strkind(PROP_DOC));
      r = p ? Tcl_NewStringObj(prop_text(p), -1) : NULL;
      break;

    case serviceidx_before: case serviceidx_after:
    case serviceidx_interrupts: {
      hiter j;

      r = Tcl_NewListObj(0, NULL);
      p = hash_find(service_props(s), prop_strkind(argkind[i]));
      if (p)
	for(hash_first(prop_identifiers(p), &j); j.current; hash_next(&j)) {
	  Tcl_ListObjAppendElement(
	    interp, r, Tcl_NewStringObj(service_genref(j.value), -1));
	}
      break;
    }

    case serviceidx_task:
      p = hash_find(service_props(s), prop_strkind(PROP_TASK)); assert(p);
      r = Tcl_NewStringObj(task_genref(prop_task(p)), -1);
      break;

    case serviceidx_throws: {
      hiter i;

      r = Tcl_NewListObj(0, NULL);
      p = hash_find(service_props(s), prop_strkind(PROP_THROWS));
      if (p)
	for(hash_first(prop_identifiers(p), &i); i.current; hash_next(&i)) {
	  Tcl_ListObjAppendElement(
	    interp, r, Tcl_NewStringObj(type_genref(i.value), -1));
	}
      break;
    }

    case serviceidx_validate:
    case serviceidx_codels: {
      hiter j;

      r = Tcl_NewListObj(0, NULL);
      for(hash_first(service_props(s), &j); j.current; hash_next(&j)) {
	if (prop_kind(j.value) != argkind[i]) continue;
	Tcl_ListObjAppendElement(
	  interp, r, Tcl_NewStringObj(codel_genref(prop_codel(j.value)), -1));
      }
      break;
    }

    case serviceidx_params: {
      hiter i;

      r = Tcl_NewListObj(0, NULL);
      for(hash_first(service_params(s), &i); i.current; hash_next(&i)) {
	Tcl_ListObjAppendElement(
	  interp, r, Tcl_NewStringObj(param_genref(i.value), -1));
      }
      break;
    }

    case serviceidx_loc: {
      Tcl_Obj *l[3] = {
	Tcl_NewStringObj(service_loc(s).file, -1),
	Tcl_NewIntObj(service_loc(s).line),
	Tcl_NewIntObj(service_loc(s).col),
      };
      r = Tcl_NewListObj(3, l);
      break;
    }

    case serviceidx_class:
      r = Tcl_NewStringObj("service", -1);
      break;
  }

  if (!r) {
    Tcl_AppendResult(interp, "undefined member \"", args[i], "\"", NULL);
    return TCL_ERROR;
  }

  Tcl_SetObjResult(interp, r);
  return TCL_OK;
}


/* --- codel command ------------------------------------------------------- */

/** Implements the command associated to a codel object.
 */
int
codel_cmd(ClientData v, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  enum codelidx {
    codelidx_name, codelidx_return, codelidx_params, codelidx_yields,
    codelidx_triggers, codelidx_task, codelidx_service, codelidx_loc,
    codelidx_class
  };
  static const char *args[] = {
    [codelidx_name] = "name", [codelidx_return] = "return",
    [codelidx_params] = "parameters", [codelidx_yields] = "yields",
    [codelidx_triggers] = "triggers", [codelidx_task] = "task",
    [codelidx_service] = "service", [codelidx_loc] = "loc",
    [codelidx_class] = "class", NULL
  };
  codel_s c = v;
  Tcl_Obj *r = NULL;
  int s;

  int i = codelidx_name; /* return name by default */

  if (objc > 2) {
    Tcl_WrongNumArgs(interp, 0, objv, "$codel subcommand");
    return TCL_ERROR;
  }
  if (objc > 1) {
    s = Tcl_GetIndexFromObj(interp, objv[1], args, "subcommand", 0, &i);
    if (s != TCL_OK) return s;
  }
  switch(i) {
    case codelidx_name:
      r = Tcl_NewStringObj(codel_name(c), -1);
      break;

    case codelidx_return:
      r = Tcl_NewStringObj(type_genref(codel_return(c)), -1);
      break;

    case codelidx_params: {
      hiter i;

      r = Tcl_NewListObj(0, NULL);
      for(hash_first(codel_params(c), &i); i.current; hash_next(&i)) {
	Tcl_ListObjAppendElement(
	  interp, r, Tcl_NewStringObj(param_genref(i.value), -1));
      }
      break;
    }

    case codelidx_yields: {
      hiter i;

      r = Tcl_NewListObj(0, NULL);
      for(hash_first(codel_yields(c), &i); i.current; hash_next(&i)) {
	Tcl_ListObjAppendElement(
	  interp, r, Tcl_NewStringObj(type_genref(i.value), -1));
      }
      break;
    }

    case codelidx_triggers: {
      hiter i;

      r = Tcl_NewListObj(0, NULL);
      for(hash_first(codel_triggers(c), &i); i.current; hash_next(&i)) {
	Tcl_ListObjAppendElement(
	  interp, r, Tcl_NewStringObj(type_genref(i.value), -1));
      }
      break;
    }

    case codelidx_task:
      if (*codel_task(c))
	r = Tcl_NewStringObj(task_genref(*codel_task(c)), -1);
      break;

    case codelidx_service:
      if (*codel_service(c))
	r = Tcl_NewStringObj(service_genref(*codel_service(c)), -1);
      break;

    case codelidx_loc: {
      Tcl_Obj *l[3] = {
	Tcl_NewStringObj(codel_loc(c).file, -1),
	Tcl_NewIntObj(codel_loc(c).line),
	Tcl_NewIntObj(codel_loc(c).col),
      };
      r = Tcl_NewListObj(3, l);
      break;
    }

    case codelidx_class:
      r = Tcl_NewStringObj("codel", -1);
      break;
  }

  if (!r) {
    Tcl_AppendResult(interp, "undefined member \"", args[i], "\"", NULL);
    return TCL_ERROR;
  }

  Tcl_SetObjResult(interp, r);
  return TCL_OK;
}


/* --- param command ------------------------------------------------------- */

/** Implements the command associated to a param object.
 */
int
param_cmd(ClientData v, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  enum paramidx {
    paramidx_name, paramidx_dir, paramidx_member, paramidx_type, paramidx_port,
    paramidx_initer, paramidx_loc, paramidx_class
  };
  static const char *args[] = {
    [paramidx_name] = "name", [paramidx_dir] = "direction",
    [paramidx_member] = "member", [paramidx_type] = "type",
    [paramidx_port] = "port", [paramidx_initer] = "initializer",
    [paramidx_loc] = "loc", [paramidx_class] = "class", NULL
  };
  param_s p = v;
  Tcl_Obj *r;
  int s;

  int i = paramidx_name; /* return name by default */

  if (objc > 2) {
    Tcl_WrongNumArgs(interp, 0, objv, "$param subcommand");
    return TCL_ERROR;
  }
  if (objc > 1) {
    s = Tcl_GetIndexFromObj(interp, objv[1], args, "subcommand", 0, &i);
    if (s != TCL_OK) return s;
  }
  switch(i) {
    case paramidx_name:
      r = Tcl_NewStringObj(param_name(p), -1);
      break;

    case paramidx_dir:
      r = Tcl_NewStringObj(param_strdir(param_dir(p)), -1);
      break;

    case paramidx_member: {
      clist_s l = param_member(p);
      citer i;

      r = Tcl_NewListObj(0, NULL);
      if (!l) break;
      for(clist_first(l, &i); i.current; clist_next(&i)) {
	Tcl_ListObjAppendElement(
	  interp, r, Tcl_NewStringObj(const_strval(*i.value), -1));
      }
      break;
    }

    case paramidx_type:
      r = Tcl_NewStringObj(type_genref(param_type(p)), -1);
      break;

    case paramidx_port:
      switch(param_dir(p)) {
	case P_INPORT: case P_OUTPORT:
	  r = Tcl_NewStringObj(port_genref(param_port(p)), -1);
	  break;

	default: r = NULL; break;
      }
      break;

    case paramidx_initer:
      if (param_initer(p))
	r = Tcl_NewStringObj(initer_genref(param_initer(p)), -1);
      else
	r = NULL;
      break;

    case paramidx_loc: {
      Tcl_Obj *l[3] = {
	Tcl_NewStringObj(param_loc(p).file, -1),
	Tcl_NewIntObj(param_loc(p).line),
	Tcl_NewIntObj(param_loc(p).col),
      };
      r = Tcl_NewListObj(3, l);
      break;
    }

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
    initeridx_class
  };
  static const char *args[] = {
    [initeridx_member] = "member", [initeridx_doc] = "doc",
    [initeridx_kind] = "kind", [initeridx_value] = "value",
    [initeridx_class] = "class", NULL
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
      else if (initer_element(i) != -1U)
	r = Tcl_NewIntObj(initer_element(i));
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
