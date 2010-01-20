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
 *                                           Anthony Mallet on Mon Jan 18 2010
 */
#include "acgenom.h"

#include <assert.h>
#include <string.h>
#include <libgen.h>
#include <errno.h>

#include <tcl.h>

#include "genom.h"


/* --- local data ---------------------------------------------------------- */

#define DOTGEN_NS	"dotgen"
#define OBJECT_NS	"object"
#define TYPE_NS		"type"
#define COMPONENT_NS	"component"
#define PROPERTY_NS	"property"

#define GENOM_CMD	"genom"
#define TEMPLATE_CMD	"template"
#define INPUT_CMD	"input"
#define TYPES_CMD	"types"
#define COMPONENTS_CMD	"components"


static int	engine_gentype(Tcl_Interp *interp, idltype_s t);
static int	engine_gencomponent(Tcl_Interp *interp, comp_s c);
static int	engine_genproperty(Tcl_Interp *interp, prop_s p);

static int	genom_version(ClientData d, Tcl_Interp *interp, int objc,
			Tcl_Obj *const objv[]);
static int	template_dir(ClientData d, Tcl_Interp *interp, int objc,
			Tcl_Obj *const objv[]);
static int	template_tmpdir(ClientData d, Tcl_Interp *interp, int objc,
			Tcl_Obj *const objv[]);
static int	input_file(ClientData d, Tcl_Interp *interp, int objc,
			Tcl_Obj *const objv[]);
static int	input_dir(ClientData d, Tcl_Interp *interp, int objc,
			Tcl_Obj *const objv[]);
static int	types(ClientData d, Tcl_Interp *interp, int objc,
			Tcl_Obj *const objv[]);
static int	components(ClientData d, Tcl_Interp *interp, int objc,
			Tcl_Obj *const objv[]);

static int	type_cmd(ClientData c, Tcl_Interp *interp, int objc,
			Tcl_Obj *const objv[]);
static int	comp_cmd(ClientData c, Tcl_Interp *interp, int objc,
			Tcl_Obj *const objv[]);
static int	prop_cmd(ClientData c, Tcl_Interp *interp, int objc,
			Tcl_Obj *const objv[]);

static char *	type_genref(idltype_s t);
static char *	prop_genref(prop_s p);
static Tcl_Obj *prop_lookup(Tcl_Interp *interp, hash_s h, const char *k);

/* list of commands in the dotgen namespace */
static const struct dgcmd {
  const char *cmd;
  void *fn;
} dgcmds[] = {
  { "::" DOTGEN_NS "::" GENOM_CMD "::version", genom_version },
  { "::" DOTGEN_NS "::" TEMPLATE_CMD "::dir", template_dir },
  { "::" DOTGEN_NS "::" TEMPLATE_CMD "::tmpdir", template_tmpdir },
  { "::" DOTGEN_NS "::" INPUT_CMD "::file", input_file },
  { "::" DOTGEN_NS "::" INPUT_CMD "::dir", input_dir },
  { "::" DOTGEN_NS "::" TYPES_CMD, types },
  { "::" DOTGEN_NS "::" COMPONENTS_CMD, components },
  { NULL, NULL }
};

/* list of namespaces that are transformed into 'ensemble' commands */
static const char *nslist[] = {
  "::" DOTGEN_NS "::" GENOM_CMD,
  "::" DOTGEN_NS "::" TEMPLATE_CMD,
  "::" DOTGEN_NS "::" INPUT_CMD,
  "::" DOTGEN_NS,
  NULL
};


/* --- engine_invoke ------------------------------------------------------- */

/** Execute a Tcl template
 */
int
engine_invoke(const char *tmpl, int argc, const char * const *argv)
{
  const struct dgcmd *i;
  char path[PATH_MAX];
  Tcl_Interp *interp;
  Tcl_Namespace *n;
  const char **j;
  Tcl_Obj *obj;
  char *args;
  hiter t;
  int s;

  /* create tcl interpreter */
  interp = Tcl_CreateInterp();
  if (!interp) {
    fprintf(stderr, "cannot create Tcl interpreter\n");
    return 127;
  }

  /* make template arguments available in argc/argv variables */
  obj = Tcl_NewIntObj(argc - 1);
  Tcl_SetVar2Ex(interp, "argc", NULL, obj, TCL_GLOBAL_ONLY);

  args = Tcl_Merge(argc-1, argv+1);
  Tcl_SetVar(interp, "argv", args, TCL_GLOBAL_ONLY);
  Tcl_Free(args);

  Tcl_SetVar(interp, "argv0", argv[0], TCL_GLOBAL_ONLY);

  /* set the "tcl_interactive" variable. */
  Tcl_SetVar(interp, "tcl_interactive", "0", TCL_GLOBAL_ONLY);

  /* initialize interpreter */
  if (Tcl_Init(interp) == TCL_ERROR) goto error;
  if (!Tcl_PkgRequire(interp, "Tcl", "8.5", 0))
    goto error;

  /* create type objects */
  for(hash_first(type_all(), &t); t.current; hash_next(&t)) {
    assert(type_fullname(t.value));
    if (engine_gentype(interp, t.value)) goto error;
  }

  /* create component objects */
  s = engine_gencomponent(interp, comp_dotgen());
  if (s) goto error;

  /* create dotgen commands */
  for(i = dgcmds; i->cmd; i++)
    if (!Tcl_CreateObjCommand(interp, i->cmd, i->fn, NULL, NULL)) goto error;

  for(j = nslist; *j; j++) {
    n = Tcl_FindNamespace(interp, *j, NULL, TCL_GLOBAL_ONLY);
    if (!n) goto error;
    if (Tcl_Export(interp, n, "*", 0) != TCL_OK) goto error;
    if (!Tcl_CreateEnsemble(interp, *j, n, TCL_ENSEMBLE_PREFIX)) goto error;
  }

  /* set namespace variables */
  Tcl_SetVar(interp, "::" DOTGEN_NS "::ns(type)",
	     "::" DOTGEN_NS "::" OBJECT_NS "::" TYPE_NS, TCL_GLOBAL_ONLY);
  Tcl_SetVar(interp, "::" DOTGEN_NS "::ns(component)",
	     "::" DOTGEN_NS "::" OBJECT_NS "::" COMPONENT_NS, TCL_GLOBAL_ONLY);

  /* require genom-engine package */
  obj = Tcl_GetVar2Ex(interp, "auto_path", NULL, TCL_GLOBAL_ONLY);
  s = Tcl_ListObjAppendElement(
    interp, obj, Tcl_NewStringObj(runopt.sysdir, -1));
  if (s != TCL_OK) goto error;
  Tcl_SetVar2Ex(interp, "auto_path", NULL, obj, TCL_GLOBAL_ONLY);

  if (!Tcl_PkgRequire(interp, PACKAGE_NAME "-engine", PACKAGE_VERSION, 1))
    goto error;

  /* invoke template */
  printf("invoking template %s\n", tmpl);
  strlcpy(path, tmpl, sizeof(path));
  strlcat(path, "/", sizeof(path));
  strlcat(path, TMPL_SPECIAL_FILE, sizeof(path));
  strlcat(path, "tcl", sizeof(path));
  s = Tcl_EvalFile(interp, path);
  if (s != TCL_OK) goto error;

  return 0;
error:
  if (runopt.verbose)
    fprintf(stderr, "%s\n", Tcl_GetVar(interp, "errorInfo", TCL_GLOBAL_ONLY));
  else
    fprintf(stderr, "%s\n", Tcl_GetStringResult(interp));
  return 2;
}


/* --- engine_gentype ------------------------------------------------------ */

/** Generate Tcl IDL type object.
 *
 * A command is created in the appropriate namespace with ClientData set to the
 * type.
 */
static int
engine_gentype(Tcl_Interp *interp, idltype_s t)
{
  Tcl_CmdInfo info;
  const char *key = type_genref(t);
  int s;
  assert(t);

  /* the type might already exist (because of recursion). In this case, just
   * do nothing. */
  if (Tcl_GetCommandInfo(interp, key, &info)) return 0;

  /* create the command for the type object */
  if (!Tcl_CreateObjCommand(interp, key, type_cmd, t, NULL))
    return EINVAL;
  if (type_fullname(t))
    printf("exported %s %s\n", type_strkind(type_kind(t)), type_fullname(t));

  /* generate type references recursively */
  switch(type_kind(t)) {
    case IDL_BOOL: case IDL_USHORT: case IDL_SHORT: case IDL_ULONG:
    case IDL_LONG: case IDL_FLOAT: case IDL_DOUBLE: case IDL_CHAR:
    case IDL_OCTET: case IDL_ANY: case IDL_ENUMERATOR: case IDL_STRING:
      /* no type reference */
      s = 0;
      break;

    case IDL_CONST:
    case IDL_ARRAY:
    case IDL_SEQUENCE:
    case IDL_MEMBER:
    case IDL_CASE:
    case IDL_TYPEDEF:
    case IDL_FORWARD_STRUCT:
    case IDL_FORWARD_UNION:
      s = engine_gentype(interp, type_type(t));
      break;

    case IDL_UNION:
      s = engine_gentype(interp, type_discriminator(t));
      if (s) break;
      /*FALLTHROUGH*/
    case IDL_STRUCT:
    case IDL_ENUM: {
      hash_s h;
      hiter i;

      h = type_members(t); assert(h);
      for(hash_first(h, &i); i.value; hash_next(&i)) {
	s = engine_gentype(interp, i.value);
	if (s) break;
      }
      break;
    }
  }

  return s;
}


/* --- engine_gencomponent ------------------------------------------------- */

/** Generate Tcl component object.
 *
 * A command is created in the appropriate namespace with ClientData set to the
 * component. Recursively generates component's properties.
 */
static int
engine_gencomponent(Tcl_Interp *interp, comp_s c)
{
  const char *name = strings(
    "::" DOTGEN_NS "::" OBJECT_NS "::" COMPONENT_NS "::", comp_name(c),
    NULL);
  hash_s h;
  hiter i;
  int s;

  if (!Tcl_CreateObjCommand(interp, name, comp_cmd, c, NULL))
    return EINVAL;
  printf("exported component %s\n", comp_name(c));

  /* properties */
  s = 0;
  h = comp_props(c); assert(h);
  for(hash_first(h, &i); i.current; hash_next(&i)) {
    s = engine_genproperty(interp, i.value);
    if (s) break;
  }

  return s;
}


/* --- engine_genproperty -------------------------------------------------- */

/** Generate Tcl property object.
 *
 * A command is created in the appropriate namespace with ClientData set to the
 * property.
 */
static int
engine_genproperty(Tcl_Interp *interp, prop_s p)
{
  if (!Tcl_CreateObjCommand(interp, prop_genref(p), prop_cmd, p, NULL))
    return EINVAL;
  printf("exported property %s\n", prop_name(p));

  return 0;
}


/* --- genom --------------------------------------------------------------- */

static int
genom_version(ClientData d, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  Tcl_SetObjResult(interp, Tcl_NewStringObj(PACKAGE_STRING, -1));
  return TCL_OK;
}


/* --- template ------------------------------------------------------------ */

static int
template_dir(ClientData v, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  Tcl_SetObjResult(interp, Tcl_NewStringObj(runopt.tmpl, -1));
  return TCL_OK;
}

static int
template_tmpdir(ClientData v, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  Tcl_SetObjResult(interp, Tcl_NewStringObj(runopt.tmpdir, -1));
  return TCL_OK;
}


/* --- input --------------------------------------------------------------- */

static int
input_file(ClientData v, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  Tcl_SetObjResult(interp, Tcl_NewStringObj(runopt.input, -1));
  return TCL_OK;
}

static int
input_dir(ClientData v, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  Tcl_SetObjResult(interp, Tcl_NewStringObj(dirname(runopt.input), -1));
  return TCL_OK;
}


/* --- types --------------------------------------------------------------- */

/** Return the list of types
 */
static int
types(ClientData v, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
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
static int
components(ClientData v, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
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

  Tcl_AppendResult(
    interp, "::" DOTGEN_NS "::" OBJECT_NS "::" COMPONENT_NS "::",
    comp_name(comp_dotgen()), NULL);
  return TCL_OK;
}


/* --- type command -------------------------------------------------------- */

/** Implements the command associated to a type object.
 */
static int
type_cmd(ClientData v, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  enum typeidx {
    typeidx_kind, typeidx_name, typeidx_fullname, typeidx_scope, typeidx_type,
    typeidx_length, typeidx_value, typeidx_valuekind, typeidx_members,
    typeidx_discriminator, typeidx_loc
  };
  static const char *args[] = {
    [typeidx_kind] = "kind", [typeidx_name] = "name",
    [typeidx_fullname] = "fullname", [typeidx_scope] = "scope",
    [typeidx_type] = "type", [typeidx_length] = "length",
    [typeidx_value] = "value", [typeidx_valuekind] = "valuekind",
    [typeidx_members] = "members", [typeidx_discriminator] = "discriminator",
    [typeidx_loc] = "loc", NULL
  };
  idltype_s t = v;
  Tcl_Obj *r;
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

	default: r = NULL; break;
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
static int
comp_cmd(ClientData v, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  enum compidx {
    compidx_name, compidx_lang, compidx_loc
  };
  static const char *args[] = {
    [compidx_name] = "name", [compidx_lang] = "language",
    [compidx_loc] = "loc", NULL
  };
  comp_s c = v;
  Tcl_Obj *r;
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

    case compidx_lang:
      r = prop_lookup(interp, comp_props(c), prop_strkind(PROP_LANG));
      break;

    case compidx_loc: {
      Tcl_Obj *l[3] = {
	Tcl_NewStringObj(comp_loc(c).file, -1),
	Tcl_NewIntObj(comp_loc(c).line),
	Tcl_NewIntObj(comp_loc(c).col),
      };
      r = Tcl_NewListObj(3, l);
      break;
    }
  }

  if (!r) {
    Tcl_AppendResult(
      interp, "in component \"", comp_name(c), "\"", NULL);
    return TCL_ERROR;
  }

  Tcl_SetObjResult(interp, r);
  return TCL_OK;
}


/* --- property command ---------------------------------------------------- */

/** Implements the command associated to a property object.
 */
static int
prop_cmd(ClientData v, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  enum propidx {
    propidx_name, propidx_kind, propidx_value, propidx_loc
  };
  static const char *args[] = {
    [propidx_name] = "name", [propidx_kind] = "kind",
    [propidx_value] = "value", [propidx_loc] = "loc", NULL
  };
  prop_s p = v;
  Tcl_Obj *r;
  int s;

  int i = propidx_value; /* return value by default */

  if (objc > 2) {
    Tcl_WrongNumArgs(interp, 0, objv, "$property subcommand");
    return TCL_ERROR;
  }
  if (objc > 1) {
    s = Tcl_GetIndexFromObj(interp, objv[1], args, "subcommand", 0, &i);
    if (s != TCL_OK) return s;
  }
  switch(i) {
    case propidx_name:
      r = Tcl_NewStringObj(prop_name(p), -1);
      break;

    case propidx_kind:
      r = Tcl_NewStringObj(prop_strkind(prop_kind(p)), -1);
      break;

    case propidx_value:
      switch(prop_kind(p)) {
	case PROP_IDS:
	  /* bufcat(&b, " %s", gentyperef(prop_type(p))); */
	  break;

	case PROP_DOC: case PROP_VERSION: case PROP_EMAIL: case PROP_LANG:
	  r = Tcl_NewStringObj(prop_text(p), -1);
	  break;

	case PROP_REQUIRE: case PROP_BUILD_REQUIRE: {
	  clist_s l = prop_list(p);
	  citer i;

	  r = Tcl_NewListObj(0, NULL);
	  for(clist_first(l, &i); i.current; clist_next(&i)) {
	    assert(i.value->k == CST_STRING);
	    Tcl_ListObjAppendElement(
	      interp, r, Tcl_NewStringObj(const_strval(*i.value), -1));
	  }
	  break;
	}

	case PROP_PERIOD: case PROP_DELAY: case PROP_PRIORITY: case PROP_STACK:
	  r = Tcl_NewStringObj(const_strval(*prop_value(p)), -1);
	  break;

	case PROP_TASK:
	  /* bufcat(&b, " {%s}", task_name(prop_task(p))); */
	  break;

	case PROP_CODEL:
	  /* bufcat(&b, " {%s}", codel_name(prop_codel(p))); */
	  break;

	case PROP_THROWS: case PROP_INTERRUPTS: case PROP_BEFORE:
	case PROP_AFTER: {
	  hash_s id = prop_identifiers(p);
	  hiter i;

	  r = Tcl_NewListObj(0, NULL);
	  for(hash_first(id, &i); i.current; hash_next(&i)) {
	    Tcl_ListObjAppendElement(interp, r, Tcl_NewStringObj(i.value, -1));
	  }
	  break;
	}
      }
      break;

    case propidx_loc: {
      Tcl_Obj *l[3] = {
	Tcl_NewStringObj(prop_loc(p).file, -1),
	Tcl_NewIntObj(prop_loc(p).line),
	Tcl_NewIntObj(prop_loc(p).col),
      };
      r = Tcl_NewListObj(3, l);
      break;
    }
  }

  Tcl_SetObjResult(interp, r);
  return TCL_OK;
}


/* --- type_genref --------------------------------------------------------- */

/** Return a string representing a reference to a type.
 *
 * The name is guaranteed to be unique. It is generated from the address
 * of the type, formatted with %p (no conversion is ever done in the other
 * direction...).
 */
static char *
type_genref(idltype_s t)
{
  char lname[32];
  snprintf(lname, sizeof(lname), "%p", t);

  return strings(
    "::" DOTGEN_NS "::" OBJECT_NS "::" TYPE_NS "::", lname, NULL);
}


/* --- prop_genref --------------------------------------------------------- */

/** Return a string representing a reference to a property.
 *
 * The name is guaranteed to be unique. It is generated from the address
 * of the property, formatted with %p (no conversion is ever done in the other
 * direction...).
 */
static char *
prop_genref(prop_s p)
{
  char lname[32];
  snprintf(lname, sizeof(lname), "%p", p);

  return strings(
    "::" DOTGEN_NS "::" OBJECT_NS "::" PROPERTY_NS "::", lname, NULL);
}


/* --- prop_lookup --------------------------------------------------------- */

/** Return a Tcl_Obj containing a reference to the property.
 */
static Tcl_Obj *
prop_lookup(Tcl_Interp *interp, hash_s h, const char *k)
{
  prop_s p;

  p = hash_find(h, k);
  if (!p) {
    if (interp) {
      Tcl_AppendResult(
	interp, "undefined property \"", k, "\"", NULL);
    }
    return NULL;
  }

  return Tcl_NewStringObj(prop_genref(p), -1);
}
