/*
 * Copyright (c) 2010-2013,2018 LAAS/CNRS
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
 *                                           Anthony Mallet on Thu Jan  7 2010
 */
#include "acgenom.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <libgen.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <err.h>

#include "genom.h"


/* --- local data ---------------------------------------------------------- */

/* engines */
#ifdef WITH_TCL
extern const engdescr eng_tcl;
#endif

/* default engine */
static const engdescr *engine = NULL;

/** template options */
static char **engopts = NULL;

/** number of template options */
static int nengopts;


static char *	eng_findentry(const char *dir);
static void	eng_listtmpldir(const char *base, const char *dir,
			char ***list, int *n);


/* --- eng_findtmpl -------------------------------------------------------- */

/** Find a template
 */
const char *
eng_findtmpl(const char *tmpl)
{
  char path[PATH_MAX];
  char *dirs, *dir;
  struct stat sb;
  char *name;

  /* if template is a regular file, this is probably a command line error */
  if (!stat(tmpl, &sb) && !(sb.st_mode & S_IFDIR)) {
    warnx("missing template argument before '%s'", tmpl);
    return NULL;
  }

  /* if template is an absolute path, return that */
  if (tmpl[0] == '/') return tmpl;

  /* iterate over components of tmplpath */
  dirs = strdup(runopt.sitetmplpath);
  if (!dirs) return tmpl;
  for (dir = strtok(dirs, ":"); dir; dir = strtok(NULL, ":")) {

    /* look for template.xxx entries */
    xwarnx("searching template directory '%s'", dir);

    strlcpy(path, dir, sizeof(path));
    strlcat(path, "/", sizeof(path));
    strlcat(path, tmpl, sizeof(path));
    if (stat(path, &sb)) continue;
    if (!(sb.st_mode & S_IFDIR)) continue;

    xwarnx("looking for template in '%s'", path);
    name = eng_findentry(path);
    if (name) {
      free(dirs);
      return string(path);
    }
  }
  free(dirs);

  /* check genom tmpldir */
  xwarnx("searching template directory '%s'", runopt.tmpldir);

  strlcpy(path, runopt.tmpldir, sizeof(path));
  strlcat(path, "/", sizeof(path));
  strlcat(path, tmpl, sizeof(path));
  if (!stat(path, &sb) && (sb.st_mode & S_IFDIR)) {
    xwarnx("looking for template in '%s'", path);
    name = eng_findentry(path);
    if (name) return string(path);
  }

  warnx("cannot find template '%s'", tmpl);
  return NULL;
}


/* --- eng_listtmpl -------------------------------------------------------- */

/** Return a null terminated array of strings containing all available
 * templates.
 */
int
eng_listtmpl(char ***list)
{
  char *dirs, *dir;
  int n = 0;
  assert(list);
  *list = NULL;

  /* iterate over components of sitetmplpath */
  dirs = strdup(runopt.sitetmplpath);
  if (!dirs) return ENOMEM;
  for (dir = strtok(dirs, ":"); dir; dir = strtok(NULL, ":"))
    eng_listtmpldir("", dir, list, &n);
  free(dirs);

  /* list tmpldir */
  eng_listtmpldir("", runopt.tmpldir, list, &n);

  if (!n) { warnx("no template found!"); return ENOENT; }
  return 0;
}

static void
eng_listtmpldir(const char *base, const char *dir, char ***list, int *n)
{
  char path[PATH_MAX];
  char ent[PATH_MAX];
  struct dirent *de;
  struct stat sb;
  DIR *d;
  char *name, *prefix;

  /* look for template.xxx entries */
  xwarnx("searching template directory '%s'", dir);

  prefix = ent + strlcpy(ent, base, sizeof(ent));
  if (base[0]) prefix = ent + strlcat(ent, "/", sizeof(ent));

  d = opendir(dir);
  if (!d) return;

  while((de = readdir(d))) {
    if (!(de->d_type & DT_DIR) && de->d_type != DT_UNKNOWN) continue;

    strlcpy(path, dir, sizeof(path));
    strlcat(path, "/", sizeof(path));
    strlcat(path, de->d_name, sizeof(path));
    if (stat(path, &sb)) continue;
    if (!(sb.st_mode & S_IFDIR)) continue;
    if (!strcmp(de->d_name, ".") || !strcmp(de->d_name, "..")) continue;

    *prefix = 0;
    strlcat(ent, de->d_name, sizeof(ent));
    eng_listtmpldir(ent, path, list, n);

    xwarnx("looking for template in '%s'", de->d_name);
    name = eng_findentry(path);
    if (name) {
      char **r;

      /* avoid duplicates */
      for(r = *list; r && *r; r++)
        if (!strcmp(ent, *r)) { name = NULL; break; }
      if (!name) continue;

      /* push into the list */
      r = realloc(*list, (*n+2)*sizeof(char *));
      if (r) {
        r[*n] = strdup(ent);
        r[*n+1] = NULL;
        *list = r;
        if (r[*n]) (*n)++;
      }
    }
  }
  closedir(d);
}


/* --- eng_seteng ---------------------------------------------------------- */

/** set engine used for component generation
 */
int
eng_seteng(const char *tmpl)
{
  const engdescr *engs[] = {
#ifdef WITH_TCL
    &eng_tcl,
#endif
    NULL
  };
  const engdescr *e;
  char *name;

  /* look for template.xxx entry */
  name = eng_findentry(tmpl);
  if (!name) {
    warnx("cannot find template entry '" TMPL_SPECIAL_FILE "<engine>'");
    return ENOENT;
  }
  name += strlen(TMPL_SPECIAL_FILE);

  /* select engine based on file extension */
  for(e = engs[0]; e; e++)
    if (!strncmp(e->name, name, strlen(name))) {
      engine = e;
      break;
    }

  if (!e) {
    if (name[0])
      warnx("unknown generator engine '%s'", name);
    else
      warnx("no generator engine");
    return errno = ENOENT;
  }
  xwarnx("using %s generator engine", name);
  return 0;
}


/* --- eng_optappend ------------------------------------------------------- */

int
eng_optappend(const char *opt, int index)
{
  int s = opt_append(&engopts, &nengopts, opt, index);
  if (s) {
    warnx("failed to add template option `%s'", opt);
    return s;
  }

  xwarnx("added template option `%s'", opt);
  return 0;
}


/* --- eng_optrm ----------------------------------------------------------- */

int
eng_optrm(int index)
{
  return opt_rm(&engopts, &nengopts, index);
}


/* --- eng_invoke ---------------------------------------------------------- */

/** invoke template engine.
 */
int
eng_invoke()
{
  char tmpl[PATH_MAX];
  int s;

  if (!engine) { warnx("no template engine"); return EINVAL; }

  /* invoke engine */
  strlcpy(tmpl, runopt.tmpl, sizeof(tmpl));
  strlcpy(tmpl, basename(tmpl), sizeof(tmpl));
  s = eng_optappend(tmpl, 0);
  if (s) return s;
  if (runopt.verbose) {
    char **o;
    xwarnx("invoking template engine with the following arguments:");
    for(o = engopts; *o; o++) xwarnx("  + %s", *o);
  }

  s = engine->invoke(runopt.tmpl, nengopts, engopts);
  xwarnx("generator engine exited with code %d", s);
  return s;
}


/* --- eng_findentry ------------------------------------------------------- */

/** return the name of the first template.xxx file in directory
 */
static char *
eng_findentry(const char *dir)
{
  struct dirent *de;
  char *name;
  DIR *d;

  /* look for template.xxx entry */
  d = opendir(dir);
  if (!d) {
    warnx("cannot open directory %s", dir);
    warn(NULL);
    return NULL;
  }

  name = NULL;
  while((de = readdir(d))) {
    if (strncmp(de->d_name, TMPL_SPECIAL_FILE, strlen(TMPL_SPECIAL_FILE)))
      continue;
    if (de->d_type & DT_DIR) {
      warnx("template entry '%s' should not be a directory", de->d_name);
      continue;
    }

    xwarnx("found template entry '%s'", de->d_name);
    name = string(de->d_name);
    break;
  }
  closedir(d);

  return name;
}
