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
 *                                           Anthony Mallet on Mon Jan 18 2010
 */
#include "acgenom.h"

#include <assert.h>
#include <string.h>
#include <libgen.h>
#include <err.h>
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
#define REMOTE_CMD	"remote"
#define CODEL_CMD	"codel"
#define PARAM_CMD	"param"
#define INITER_CMD	"initer"

#define GENOM_CMD	"genom"
#define TEMPLATE_CMD	"template"
#define INPUT_CMD	"input"
#define PARSE_CMD	"parse"
#define TYPES_CMD	"types"
#define COMPONENTS_CMD	"components"
#define INTERFACE_CMD	"interfaces"

/* list of extra package paths */
static const char *extra[] = TCL_EXTRAPKG;

/* exit status */
static int estatus = 0;

/* list of commands in the dotgen namespace */
static const struct dgcmd {
  const char *cmd;
  void *fn;
} dgcmds[] = {
  { "::" DOTGEN_NS "::" GENOM_CMD "::program", dg_genom_program },
  { "::" DOTGEN_NS "::" GENOM_CMD "::cmdline", dg_genom_cmdline },
  { "::" DOTGEN_NS "::" GENOM_CMD "::version", dg_genom_version },
  { "::" DOTGEN_NS "::" GENOM_CMD "::templates", dg_genom_templates },
  { "::" DOTGEN_NS "::" GENOM_CMD "::debug", dg_genom_debug },
  { "::" DOTGEN_NS "::" GENOM_CMD "::verbose", dg_genom_verbose },
  { "::" DOTGEN_NS "::" TEMPLATE_CMD "::name", dg_template_name },
  { "::" DOTGEN_NS "::" TEMPLATE_CMD "::dir", dg_template_dir },
  { "::" DOTGEN_NS "::" TEMPLATE_CMD "::builtindir", dg_template_builtindir },
  { "::" DOTGEN_NS "::" TEMPLATE_CMD "::tmpdir", dg_template_tmpdir },
  { "::" DOTGEN_NS "::" INPUT_CMD "::notice", dg_input_notice },
  { "::" DOTGEN_NS "::" INPUT_CMD "::deps", dg_input_deps },
  { "::" DOTGEN_NS "::" PARSE_CMD, dg_parse },
  { "::" DOTGEN_NS "::" TYPES_CMD, dg_types },
  { "::" DOTGEN_NS "::" COMPONENTS_CMD, dg_components },
  { "::" DOTGEN_NS "::" INTERFACE_CMD, dg_interfaces },
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


static int	engine_eline(ClientData dummy, Tcl_Interp *interp,
			int objc, Tcl_Obj *const objv[]);
static int	engine_exit(ClientData dummy, Tcl_Interp *interp,
			int objc, Tcl_Obj *const objv[]);

static int	engine_gentype(Tcl_Interp *interp, Tcl_Interp *slave,
			idltype_s t);
static int	engine_gencomponent(Tcl_Interp *interp,Tcl_Interp *slave,
			comp_s c);
static int	engine_gentask(Tcl_Interp *interp, Tcl_Interp *slave,
			task_s t);
static int	engine_genport(Tcl_Interp *interp, Tcl_Interp *slave,
			port_s p);
static int	engine_genservice(Tcl_Interp *interp, Tcl_Interp *slave,
			service_s s);
static int	engine_genremote(Tcl_Interp *interp, Tcl_Interp *slave,
			remote_s r);
static int	engine_gencodel(Tcl_Interp *interp, Tcl_Interp *slave,
			codel_s c);
static int	engine_genparam(Tcl_Interp *interp, Tcl_Interp *slave,
			param_s p);
static int	engine_geniniter(Tcl_Interp *interp, Tcl_Interp *slave,
			initer_s i);

static char *	genref(const char *prefix, void *o);


/* --- engine_invoke ------------------------------------------------------- */

/** Execute a Tcl template
 */
int
engine_invoke(const char *tmpl, int argc, const char * const *argv)
{
  const struct dgcmd *i;
  char path[PATH_MAX];
  Tcl_Interp *interp, *slave;
  Tcl_Namespace *n;
  const char **j;
  Tcl_Obj *obj;
  char *args;
  int p, s;

  /* create tcl interpreter */
  interp = Tcl_CreateInterp();
  if (!interp) {
    warnx("cannot create Tcl interpreter");
    return 127;
  }
  estatus = 2;

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

  /* redefine exit */
  if (!Tcl_CreateObjCommand(interp, "exit", engine_exit, NULL, NULL))
    goto error;

  /* create a safe slave interpreter for evaluating template files */
  slave = Tcl_CreateSlave(interp, "slave", 1/*safe*/);
  if (!slave) goto error;

  if (!Tcl_CreateObjCommand(slave, "slave-eline", engine_eline, NULL, NULL))
    goto error;

  /* create dotgen objects */
  if (!Tcl_CreateNamespace(interp, "::" DOTGEN_NS, NULL, NULL))
    goto error;

  if (engine_export(interp, slave)) goto error;

  if (!Tcl_SetVar(interp, "::" DOTGEN_NS "::ns(object)",
		 "::" DOTGEN_NS "::" OBJECT_NS,
		  TCL_GLOBAL_ONLY | TCL_LEAVE_ERR_MSG))
    goto error;


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
  xwarnx("invoking template %s", tmpl);
  strlcpy(path, tmpl, sizeof(path));
  strlcat(path, "/", sizeof(path));
  strlcat(path, TMPL_SPECIAL_FILE, sizeof(path));
  strlcat(path, "tcl", sizeof(path));
  s = Tcl_EvalFile(interp, path);
  if (s != TCL_OK) goto error;

  estatus = 0;
done:
  Tcl_DeleteInterp(interp);
  return estatus;

error:
  strlcpy(path, runopt.tmpl, sizeof(path));
  if (runopt.verbose)
    fprintf(stderr, "%s: %s\n",
            basename(path), Tcl_GetVar(interp, "errorInfo", TCL_GLOBAL_ONLY));
  else
    fprintf(stderr, "%s: %s\n",
            basename(path), Tcl_GetStringResult(interp));
  goto done;
}


/* --- engine_eline -------------------------------------------------------- */

/** Return the current errorLine number found in Tcl_Interp. Too bad that
 * there is no built-in command for that. */
static int
engine_eline(ClientData dummy, Tcl_Interp *interp,
             int objc, Tcl_Obj *const objv[])
{
  Tcl_SetObjResult(interp, Tcl_NewIntObj(interp->errorLine));
  return TCL_OK;
}


/* --- engine_exit --------------------------------------------------------- */

/** Redefine tcl "exit" command so that a template cannot terminate genom
 * by itself */
static int
engine_exit(ClientData dummy, Tcl_Interp *interp,
            int objc, Tcl_Obj *const objv[])
{
  char *value;

  if ((objc != 1) && (objc != 2)) {
    Tcl_WrongNumArgs(interp, 1, objv, "?returnCode?");
    return TCL_ERROR;
  }

  value = (objc == 1) ? "0" : Tcl_GetString(objv[1]);
  Tcl_GetInt(interp, value, &estatus);
  Tcl_AppendResult(interp, "exit ", value, NULL);
  return TCL_ERROR;
}


/* --- engine_createcmd ---------------------------------------------------- */

/** Create a tcl command and alias it into the slave interpreter
 */

int
engine_createcmd(Tcl_Interp *interp, Tcl_Interp *slave, const char *cmd,
		 Tcl_ObjCmdProc *proc, ClientData data)
{
  Tcl_CmdInfo info;

  /* do nothing if the command exists already */
  if (Tcl_GetCommandInfo(interp, cmd, &info))
    return errno = EEXIST;

  if (!Tcl_CreateObjCommand(interp, cmd, proc, data, NULL))
    return errno = EINVAL;
  if (slave) {
    if (!Tcl_CreateObjCommand(slave, cmd, proc, data, NULL)) {
      Tcl_DeleteCommand(interp, cmd);
      return errno = EINVAL;
    }
  }

  return 0;
}


/* --- engine_export ------------------------------------------------------- */

/** Generate Tcl procedures corresponding to dotgen objects.
 */
int
engine_export(Tcl_Interp *interp, Tcl_Interp *slave)
{
  hiter t;
  comp_s c;
  int s;

  if (type_all()) {
    for(hash_first(type_all(), &t); t.current; hash_next(&t)) {
      s = engine_gentype(interp, slave, t.value);
      if (s) return s;
    }
  }

  for(c = comp_first(); c; c = comp_next(c)) {
    s = engine_gencomponent(interp, slave, c);
    if (s) return s;
  }

  return 0;
}


/* --- engine_gentype ------------------------------------------------------ */

/** Generate Tcl IDL type object.
 */
static int
engine_gentype(Tcl_Interp *interp, Tcl_Interp *slave, idltype_s t)
{
  const char *key = type_genref(t);
  int s;
  assert(t);

  /* create the command for the type object if needed */
  if (engine_createcmd(interp, slave, key, type_cmd, t)) {
    if (errno != EEXIST) return errno;
  } else if (type_fullname(t))
    xwarnx("exported %s %s", type_strkind(type_kind(t)), type_fullname(t));

  /* generate type references recursively */
  switch(type_kind(t)) {
    case IDL_BOOL: case IDL_USHORT: case IDL_SHORT: case IDL_ULONG:
    case IDL_LONG: case IDL_ULONGLONG: case IDL_LONGLONG: case IDL_FLOAT:
    case IDL_DOUBLE: case IDL_CHAR: case IDL_OCTET: case IDL_ANY:
    case IDL_ENUMERATOR: case IDL_STRING: case IDL_EVENT: case IDL_PORT:
    case IDL_REMOTE: case IDL_NATIVE:
      /* no recursive type reference */
      s = 0;
      break;

    case IDL_FORWARD_STRUCT: case IDL_FORWARD_UNION:
      /* don't recurse infinitely, the real object takes care of it */
      s = 0;
      break;

    case IDL_CONST:
    case IDL_ARRAY:
    case IDL_SEQUENCE:
    case IDL_MEMBER:
    case IDL_CASE:
    case IDL_TYPEDEF:
      s = engine_gentype(interp, slave, type_type(t));
      break;

    case IDL_UNION:
      s = engine_gentype(interp, slave, type_discriminator(t));
      if (s) break;
      /*FALLTHROUGH*/
    case IDL_STRUCT: case IDL_ENUM: case IDL_EXCEPTION: {
      hash_s h;
      hiter i;

      s = 0;
      h = type_members(t); assert(h);
      for(hash_first(h, &i); i.value; hash_next(&i)) {
	s = engine_gentype(interp, slave, i.value);
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
engine_gencomponent(Tcl_Interp *interp, Tcl_Interp *slave, comp_s c)
{
  const char *key = comp_genref(c);
  hiter i;
  int s;

  /* create the command if needed */
  if (engine_createcmd(interp, slave, key, comp_cmd, c)) {
    if (errno != EEXIST) return errno;
  } else
    xwarnx("exported %s %s", comp_strkind(comp_kind(c)), comp_name(c));

  /* properties */
  for(hash_first(comp_props(c), &i); i.current; hash_next(&i)) {
    switch(prop_kind(i.value)) {
      case PROP_IDS:
	s = engine_gentype(interp, slave, prop_type(i.value));
	break;

      case PROP_CLOCKRATE:
	s = engine_gentype(interp, slave, prop_value(i.value));
	break;

      case PROP_PERIOD: case PROP_DELAY: case PROP_PRIORITY:
      case PROP_SCHEDULING: case PROP_STACK: case PROP_VALIDATE:
      case PROP_SIMPLE_CODEL: case PROP_FSM_CODEL: case PROP_DOC:
      case PROP_VERSION: case PROP_LANG: case PROP_EMAIL: case PROP_REQUIRE:
      case PROP_CODELS_REQUIRE: case PROP_TASK: case PROP_THROWS:
      case PROP_INTERRUPTS: case PROP_BEFORE: case PROP_AFTER:
      case PROP_EXTENDS: case PROP_PROVIDES: case PROP_USES:
        s = 0;
        break;
    }

    if (s) return s;
  }

  /* tasks */
  for(hash_first(comp_tasks(c), &i); i.current; hash_next(&i)) {
    s = engine_gentask(interp, slave, i.value);
    if (s) return s;
  }

  /* ports */
  for(hash_first(comp_ports(c), &i); i.current; hash_next(&i)) {
    s = engine_genport(interp, slave, i.value);
    if (s) return s;
  }

  /* services */
  for(hash_first(comp_services(c), &i); i.current; hash_next(&i)) {
    s = engine_genservice(interp, slave, i.value);
    if (s) return s;
  }

  /* remotes */
  for(hash_first(comp_remotes(c), &i); i.current; hash_next(&i)) {
    s = engine_genremote(interp, slave, i.value);
    if (s) return s;
  }

  return 0;
}


/* --- engine_gentask ------------------------------------------------------ */

/** Generate Tcl task object.
 */
static int
engine_gentask(Tcl_Interp *interp, Tcl_Interp *slave, task_s t)
{
  const char *key = task_genref(t);
  hash_s h;
  hiter i;
  int s;

  /* create the command if needed */
  if (engine_createcmd(interp, slave, key, task_cmd, t)) {
    if (errno != EEXIST) return errno;
  } else
    xwarnx("exported task %s", task_name(t));

  /* properties */
  s = 0;
  h = task_props(t); assert(h);
  for(hash_first(h, &i); i.current; hash_next(&i)) {
    switch(prop_kind(i.value)) {
      case PROP_PERIOD: case PROP_DELAY: case PROP_PRIORITY: case PROP_STACK:
	s = engine_gentype(interp, slave, prop_value(i.value));
	break;

      case PROP_VALIDATE: case PROP_SIMPLE_CODEL: case PROP_FSM_CODEL:
	s = engine_gencodel(interp, slave, prop_codel(i.value));
	break;

      case PROP_DOC: case PROP_IDS: case PROP_VERSION: case PROP_LANG:
      case PROP_EMAIL: case PROP_REQUIRE: case PROP_CODELS_REQUIRE:
      case PROP_CLOCKRATE: case PROP_SCHEDULING: case PROP_TASK:
      case PROP_THROWS: case PROP_INTERRUPTS: case PROP_BEFORE:
      case PROP_AFTER: case PROP_EXTENDS: case PROP_PROVIDES: case PROP_USES:
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
engine_genport(Tcl_Interp *interp, Tcl_Interp *slave, port_s p)
{
  const char *key = port_genref(p);
  int s;

  /* create the command if needed */
  if (engine_createcmd(interp, slave, key, port_cmd, p)) {
    if (errno != EEXIST) return errno;
  } else
    xwarnx("exported port %s", port_name(p));

  if (port_type(p)) {
    s = engine_gentype(interp, slave, port_type(p));
    if (s) return s;
  }
  if (port_datatype(p)) {
    s = engine_gentype(interp, slave, port_datatype(p));
    if (s) return s;
  }

  return 0;
}


/* --- engine_genservice --------------------------------------------------- */

/** Generate Tcl service object.
 */
static int
engine_genservice(Tcl_Interp *interp, Tcl_Interp *slave, service_s s)
{
  const char *key = service_genref(s);
  hiter i;
  int e = 0;

  /* create the command if needed */
  if (engine_createcmd(interp, slave, key, service_cmd, s)) {
    if (errno != EEXIST) return errno;
  } else
    xwarnx("exported service %s", service_name(s));

  /* parameters */
  for(hash_first(service_params(s), &i); i.current; hash_next(&i)) {
    e = engine_genparam(interp, slave, i.value);
    if (e) return e;
  }

  /* properties */
  for(hash_first(service_props(s), &i); i.current; hash_next(&i)) {
    switch(prop_kind(i.value)) {
      case PROP_VALIDATE: case PROP_SIMPLE_CODEL: case PROP_FSM_CODEL:
	e = engine_gencodel(interp, slave, prop_codel(i.value));
	break;

      case PROP_PERIOD: case PROP_DELAY: case PROP_PRIORITY:
      case PROP_SCHEDULING: case PROP_STACK: case PROP_DOC: case PROP_IDS:
      case PROP_VERSION: case PROP_LANG: case PROP_EMAIL: case PROP_REQUIRE:
      case PROP_CODELS_REQUIRE: case PROP_CLOCKRATE: case PROP_TASK:
      case PROP_THROWS: case PROP_INTERRUPTS: case PROP_BEFORE:
      case PROP_AFTER: case PROP_EXTENDS: case PROP_PROVIDES: case PROP_USES:
	break;
    }

    if (e) break;
  }

  return 0;
}


/* --- engine_genremote ---------------------------------------------------- */

/** Generate Tcl remote object.
 */
static int
engine_genremote(Tcl_Interp *interp, Tcl_Interp *slave, remote_s r)
{
  const char *key = remote_genref(r);
  hiter i;
  int e = 0;

  /* create the command if needed */
  if (engine_createcmd(interp, slave, key, remote_cmd, r)) {
    if (errno != EEXIST) return errno;
  } else
    xwarnx("exported remote %s", remote_name(r));

  /* parameters */
  for(hash_first(remote_params(r), &i); i.current; hash_next(&i)) {
    e = engine_genparam(interp, slave, i.value);
    if (e) return e;
  }

  /* type */
  e = engine_gentype(interp, slave, remote_type(r));
  if (e) return e;

  return 0;
}


/* --- engine_gencodel ----------------------------------------------------- */

/** Generate Tcl codel object.
 */
static int
engine_gencodel(Tcl_Interp *interp, Tcl_Interp *slave, codel_s c)
{
  const char *key = codel_genref(c);
  hiter i;
  int s;

  /* create the command if needed */
  if (engine_createcmd(interp, slave, key, codel_cmd, c)) {
    if (errno != EEXIST) return errno;
  } else
    xwarnx("exported codel %s", codel_name(c));

  /* parameters */
  for(hash_first(codel_params(c), &i); i.current; hash_next(&i)) {
    s = engine_genparam(interp, slave, i.value);
    if (s) return s;
  }

  /* events */
  for(hash_first(codel_triggers(c), &i); i.current; hash_next(&i)) {
    s = engine_gentype(interp, slave, i.value);
    if (s) return s;
  }
  for(hash_first(codel_yields(c), &i); i.current; hash_next(&i)) {
    s = engine_gentype(interp, slave, i.value);
    if (s) return s;
  }

  return 0;
}


/* --- engine_genparam ----------------------------------------------------- */

/** Generate Tcl param object.
 */
static int
engine_genparam(Tcl_Interp *interp, Tcl_Interp *slave, param_s p)
{
  const char *key = param_genref(p);
  initer_s i;
  int s;

  /* create the command if needed */
  if (engine_createcmd(interp, slave, key, param_cmd, p)) {
    if (errno != EEXIST) return errno;
  } else
    xwarnx("exported parameter %s", param_name(p));

  /* types */
  s = engine_gentype(interp, slave, param_type(p));
  if (s) return s;

  /* initializer */
  i = param_initer(p);
  if (i) {
    s = engine_geniniter(interp, slave, i);
    if (s) return s;
  }

  return 0;
}


/* --- engine_geniniter ---------------------------------------------------- */

/** Generate Tcl initializer object.
 */
static int
engine_geniniter(Tcl_Interp *interp, Tcl_Interp *slave, initer_s i)
{
  const char *key;
  int s;

  while(i) {
    if (initer_compound(i)) {
      s = engine_geniniter(interp, slave, initer_compound(i));
      if (s) return s;
    }

    key = initer_genref(i);

    /* create the command if needed */
    if (engine_createcmd(interp, slave, key, initer_cmd, i)) {
      if (errno != EEXIST) return errno;
    }

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
remote_genref(remote_s r)
{
  return genref("::" DOTGEN_NS "::" OBJECT_NS "::" REMOTE_CMD, r);
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
