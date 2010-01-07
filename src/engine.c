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
 *                                           Anthony Mallet on Thu Jan  7 2010
 */
#include "acgenom.h"

#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <err.h>

#include "genom.h"


/* --- local data ---------------------------------------------------------- */

/* engines */
#ifdef TCLSH
extern const engdescr eng_tcl;
#endif

/* default engine */
static const engdescr *engine = NULL;


/* --- eng_seteng ---------------------------------------------------------- */

/** set engine used for component generation
 */
int
eng_seteng(const char *tmpl)
{
  engdescr engs[] = {
#ifdef TCLSH
    eng_tcl,
#endif
    { .name = "" }
  };
  engdescr *e;
  char *name;
#if 0
  struct dirent *de;
  DIR *d;
  int s;

  /* look for template.xxx entry */
  d = opendir(tmpl);
  if (!d) { warnx("cannot open directory %s", tmpl); return errno; }

  name = NULL;
  while((de = readdir(d))) {
    if (strncmp(de->d_name, TMPL_SPECIAL_FILE)) continue;
    if (e->d_type == DT_DIR) continue;

    xwarnx("found template entry '%s'", e->d_name);
    name = de->d_name + strlen(TMPL_SPECIAL_FILE);
  }
  closedir(d);
  if (!name) {
    warnx("cannot find template entry '" TMPL_SPECIAL_FILE "'");
    return ENOENT;
  }
#else
  name = "tcl";
#endif

  /* select engine based on file extension */
  for(e = &engs[0]; e->name[0]; e++)
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

  /* set interpreter */
  if (!runopt.interp[0])
    strlcpy(runopt.interp, engine->interp, sizeof(runopt.interp));
  xwarnx("using %s interpreter", runopt.interp);

  return 0;
}


/* --- eng_invoke ---------------------------------------------------------- */

/** generate a description of the dotgen file in the engine language and invoke
 * interpreter to execute template.
 */
int
eng_invoke()
{
  char gen[PATH_MAX];
  FILE *g;
  int pipefd[2];
  int s;

  comp_s c = comp_dotgen();
  if (!c) return errno = ENOENT;
  if (!engine) { warnx("no generator engine"); return EINVAL; }

  /* create output for processed dotgen data */
  strlcpy(gen, runopt.tmpdir, sizeof(gen));
  strlcat(gen, "/", sizeof(gen));
  strlcat(gen, comp_name(c), sizeof(gen));
  g = fopen(gen, "w");
  if (!g) {
    warnx("cannot open temporary '%s' file", gen); warn(NULL);
    return errno;
  }
  xwarnx("creating '%s' %s component description", gen, engine->name);

  s = engine->gendotgen(c, g);
  fclose(g);
  if (s) return errno;

  /* invoke interpreter */
  if (pipe(pipefd) < 0) {
    warn("cannot create a pipe to template interpreter:");
    return errno;
  }

  return 0;
}
