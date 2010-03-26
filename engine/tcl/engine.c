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
#include "engine.h"


/* --- local data ---------------------------------------------------------- */

#define DOTGEN_NS	"dotgen"
#define OBJECT_NS	"object"
#define TYPE_CMD	"type"
#define COMPONENT_CMD	"component"
#define TASK_CMD	"task"
#define PORT_CMD	"port"
#define SERVICE_CMD	"service"
#define CODEL_CMD	"codel"
#define PARAM_CMD	"param"
#define INITER_CMD	"initer"

#define GENOM_CMD	"genom"
#define TEMPLATE_CMD	"template"
#define INPUT_CMD	"input"
#define TYPES_CMD	"types"
#define COMPONENTS_CMD	"components"

/* list of extra package paths */
static const char *extra[] = TCL_EXTRAPKG;

/* list of commands in the dotgen namespace */
static const struct dgcmd {
  const char *cmd;
  void *fn;
} dgcmds[] = {
  { "::" DOTGEN_NS "::" GENOM_CMD "::program", dg_genom_program },
  { "::" DOTGEN_NS "::" GENOM_CMD "::cmdline", dg_genom_cmdline },
  { "::" DOTGEN_NS "::" GENOM_CMD "::version", dg_genom_version },
  { "::" DOTGEN_NS "::" GENOM_CMD "::debug", dg_genom_debug },
  { "::" DOTGEN_NS "::" GENOM_CMD "::stdout", dg_genom_stdout },
  { "::" DOTGEN_NS "::" TEMPLATE_CMD "::name", dg_template_name },
  { "::" DOTGEN_NS "::" TEMPLATE_CMD "::dir", dg_template_dir },
  { "::" DOTGEN_NS "::" TEMPLATE_CMD "::sysdir", dg_template_sysdir },
  { "::" DOTGEN_NS "::" TEMPLATE_CMD "::tmpdir", dg_template_tmpdir },
  { "::" DOTGEN_NS "::" INPUT_CMD "::file", dg_input_file },
  { "::" DOTGEN_NS "::" INPUT_CMD "::base", dg_input_base },
  { "::" DOTGEN_NS "::" INPUT_CMD "::dir", dg_input_dir },
  { "::" DOTGEN_NS "::" INPUT_CMD "::notice", dg_input_notice },
  { "::" DOTGEN_NS "::" TYPES_CMD, dg_types },
  { "::" DOTGEN_NS "::" COMPONENTS_CMD, dg_components },
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


static int	engine_gentype(Tcl_Interp *interp, idltype_s t);
static int	engine_gencomponent(Tcl_Interp *interp, comp_s c);
static int	engine_gentask(Tcl_Interp *interp, task_s t);
static int	engine_genport(Tcl_Interp *interp, port_s p);
static int	engine_genservice(Tcl_Interp *interp, service_s s);
static int	engine_gencodel(Tcl_Interp *interp, codel_s c);
static int	engine_genparam(Tcl_Interp *interp, param_s p);
static int	engine_geniniter(Tcl_Interp *interp, initer_s i);

static char *	genref(const char *prefix, void *o);


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
  int p, s;

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

  /* create dotgen objects */
  for(hash_first(type_all(), &t); t.current; hash_next(&t)) {
    assert(type_fullname(t.value));
    if (engine_gentype(interp, t.value)) goto error;
  }

  s = engine_gencomponent(interp, comp_dotgen());
  if (s) goto error;

  Tcl_SetVar(interp, "::" DOTGEN_NS "::ns(object)",
	     "::" DOTGEN_NS "::" OBJECT_NS, TCL_GLOBAL_ONLY);

  /* create dotgen commands */
  for(i = dgcmds; i->cmd; i++)
    if (!Tcl_CreateObjCommand(interp, i->cmd, i->fn, NULL, NULL)) goto error;

  /* create ensembles */
  for(j = nslist; *j; j++) {
    n = Tcl_FindNamespace(interp, *j, NULL, TCL_GLOBAL_ONLY);
    if (!n) goto error;
    if (Tcl_Export(interp, n, "*", 0) != TCL_OK) goto error;
    if (!Tcl_CreateEnsemble(interp, *j, n, TCL_ENSEMBLE_PREFIX)) goto error;
  }

  /* set path to packages */
  obj = Tcl_GetVar2Ex(interp, "auto_path", NULL, TCL_GLOBAL_ONLY);
  s = Tcl_ListObjAppendElement(
    interp, obj, Tcl_NewStringObj(runopt.sysdir, -1));
  if (s != TCL_OK) goto error;

  for(p=0; p<sizeof(extra)/sizeof(extra[0]); p++) {
    s = Tcl_ListObjAppendElement(interp, obj, Tcl_NewStringObj(extra[p], -1));
    if (s != TCL_OK) goto error;
  }

  Tcl_SetVar2Ex(interp, "auto_path", NULL, obj, TCL_GLOBAL_ONLY);


  /* require genom-engine package */
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
 */
static int
engine_gencomponent(Tcl_Interp *interp, comp_s c)
{
  idltype_s iev;
  hiter i;
  int s;

  if (!Tcl_CreateObjCommand(interp, comp_genref(c), comp_cmd, c, NULL))
    return EINVAL;
  printf("exported component %s\n", comp_name(c));

  /* internal event type */
  iev = comp_eventtype(c);
  s = engine_gentype(interp, iev);
  if (s) return s;

  /* properties */
  for(hash_first(comp_props(c), &i); i.current; hash_next(&i)) {
    switch(prop_kind(i.value)) {
      case PROP_IDS:
	s = engine_gentype(interp, prop_type(i.value));
	break;

      case PROP_CLOCKRATE:
	s = engine_gentype(interp, prop_value(i.value));
	break;

      case PROP_PERIOD: case PROP_DELAY: case PROP_PRIORITY: case PROP_STACK:
      case PROP_VALIDATE: case PROP_CODEL: case PROP_DOC: case PROP_VERSION:
      case PROP_LANG: case PROP_EMAIL: case PROP_REQUIRE:
      case PROP_BUILD_REQUIRE: case PROP_TASK: case PROP_THROWS:
      case PROP_INTERRUPTS: case PROP_BEFORE: case PROP_AFTER:
	break;
    }

    if (s) return s;
  }

  /* tasks */
  for(hash_first(comp_tasks(c), &i); i.current; hash_next(&i)) {
    s = engine_gentask(interp, i.value);
    if (s) return s;
  }

  /* ports */
  for(hash_first(comp_ports(c), &i); i.current; hash_next(&i)) {
    s = engine_genport(interp, i.value);
    if (s) return s;
  }

  /* services */
  for(hash_first(comp_services(c), &i); i.current; hash_next(&i)) {
    s = engine_genservice(interp, i.value);
    if (s) return s;
  }

  return 0;
}


/* --- engine_gentask ------------------------------------------------------ */

/** Generate Tcl task object.
 */
static int
engine_gentask(Tcl_Interp *interp, task_s t)
{
  hash_s h;
  hiter i;
  int s;

  if (!Tcl_CreateObjCommand(interp, task_genref(t), task_cmd, t, NULL))
    return EINVAL;
  printf("exported task %s\n", task_name(t));

  /* properties */
  s = 0;
  h = task_props(t); assert(h);
  for(hash_first(h, &i); i.current; hash_next(&i)) {
    switch(prop_kind(i.value)) {
      case PROP_PERIOD: case PROP_DELAY: case PROP_PRIORITY: case PROP_STACK:
	s = engine_gentype(interp, prop_value(i.value));
	break;

      case PROP_VALIDATE: case PROP_CODEL:
	s = engine_gencodel(interp, prop_codel(i.value));
	break;

      case PROP_DOC: case PROP_IDS: case PROP_VERSION: case PROP_LANG:
      case PROP_EMAIL: case PROP_REQUIRE: case PROP_BUILD_REQUIRE:
      case PROP_CLOCKRATE: case PROP_TASK: case PROP_THROWS:
      case PROP_INTERRUPTS: case PROP_BEFORE: case PROP_AFTER:
	break;
    }

    if (s) break;
  }

  return s;
}


/* --- engine_genport ------------------------------------------------------ */

/** Generate Tcl port object.
 */
static int
engine_genport(Tcl_Interp *interp, port_s p)
{
  int s;

  if (!Tcl_CreateObjCommand(interp, port_genref(p), port_cmd, p, NULL))
    return EINVAL;
  printf("exported port %s\n", port_name(p));

  switch(port_kind(p)) {
    case PORT_IN: case PORT_OUT:
      s = engine_gentype(interp, port_type(p));
      if (s) return s;
      break;

    case PORT_EVENT: break;
  }

  return 0;
}


/* --- engine_genservice --------------------------------------------------- */

/** Generate Tcl service object.
 */
static int
engine_genservice(Tcl_Interp *interp, service_s s)
{
  hiter i;
  int e;

  if (!Tcl_CreateObjCommand(interp, service_genref(s), service_cmd, s, NULL))
    return EINVAL;
  printf("exported service %s\n", service_name(s));

  /* parameters */
  for(hash_first(service_params(s), &i); i.current; hash_next(&i)) {
    e = engine_genparam(interp, i.value);
    if (e) return e;
  }

  /* properties */
  for(hash_first(service_props(s), &i); i.current; hash_next(&i)) {
    switch(prop_kind(i.value)) {
      case PROP_VALIDATE: case PROP_CODEL:
	e = engine_gencodel(interp, prop_codel(i.value));
	break;

      case PROP_PERIOD: case PROP_DELAY: case PROP_PRIORITY: case PROP_STACK:
      case PROP_DOC: case PROP_IDS: case PROP_VERSION: case PROP_LANG:
      case PROP_EMAIL: case PROP_REQUIRE: case PROP_BUILD_REQUIRE:
      case PROP_CLOCKRATE: case PROP_TASK: case PROP_THROWS:
      case PROP_INTERRUPTS: case PROP_BEFORE: case PROP_AFTER:
	break;
    }

    if (e) break;
  }

  return 0;
}


/* --- engine_gencodel ----------------------------------------------------- */

/** Generate Tcl codel object.
 */
static int
engine_gencodel(Tcl_Interp *interp, codel_s c)
{
  hiter i;
  int s;

  if (!Tcl_CreateObjCommand(interp, codel_genref(c), codel_cmd, c, NULL))
    return EINVAL;
  printf("exported codel %s\n", codel_name(c));

  /* parameters */
  for(hash_first(codel_params(c), &i); i.current; hash_next(&i)) {
    s = engine_genparam(interp, i.value);
    if (s) return s;
  }

  return 0;
}


/* --- engine_genparam ----------------------------------------------------- */

/** Generate Tcl param object.
 */
static int
engine_genparam(Tcl_Interp *interp, param_s p)
{
  initer_s i;
  int s;

  if (!Tcl_CreateObjCommand(interp, param_genref(p), param_cmd, p, NULL))
    return EINVAL;
  printf("exported parameter %s\n", param_name(p));

  /* initializer */
  i = param_initer(p);
  if (i) {
    s = engine_geniniter(interp, i);
    if (s) return s;
  }

  return 0;
}


/* --- engine_geniniter ---------------------------------------------------- */

/** Generate Tcl initializer object.
 */
static int
engine_geniniter(Tcl_Interp *interp, initer_s i)
{
  int s;

  while(i) {
    if (initer_compound(i)) {
      s = engine_geniniter(interp, initer_compound(i));
      if (s) return s;
    }

    if (!Tcl_CreateObjCommand(interp, initer_genref(i), initer_cmd, i, NULL))
      return EINVAL;

    i = initer_next(i);
  }

  return 0;
}


/* --- genref -------------------------------------------------------------- */

/** Return a string representing a reference to an object.
 *
 * The name is guaranteed to be unique. It is generated from the address
 * of the type, formatted with %p (no conversion is ever done in the other
 * direction...).
 */
static char *
genref(const char *prefix, void *o)
{
  char lname[32];
  snprintf(lname, sizeof(lname), "@%p", o);

  return strings(prefix, lname, NULL);
}

char *
type_genref(idltype_s t)
{
  return genref("::" DOTGEN_NS "::" OBJECT_NS "::" TYPE_CMD, t);
}

char *
comp_genref(comp_s c)
{
  return genref("::" DOTGEN_NS "::" OBJECT_NS "::" COMPONENT_CMD, c);
}

char *
task_genref(task_s t)
{
  return genref("::" DOTGEN_NS "::" OBJECT_NS "::" TASK_CMD, t);
}

char *
port_genref(port_s p)
{
  return genref("::" DOTGEN_NS "::" OBJECT_NS "::" PORT_CMD, p);
}

char *
service_genref(service_s s)
{
  return genref("::" DOTGEN_NS "::" OBJECT_NS "::" SERVICE_CMD, s);
}

char *
codel_genref(codel_s c)
{
  return genref("::" DOTGEN_NS "::" OBJECT_NS "::" CODEL_CMD, c);
}

char *
param_genref(param_s p)
{
  return genref("::" DOTGEN_NS "::" OBJECT_NS "::" PARAM_CMD, p);
}

char *
initer_genref(initer_s i)
{
  return genref("::" DOTGEN_NS "::" OBJECT_NS "::" INITER_CMD, i);
}
