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
#include <libgen.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/wait.h>
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

/* interpreter pid */
static pid_t engpid;

/** template options */
static char **engopts = NULL;

/** number of template options */
static int nengopts;


/* --- eng_seteng ---------------------------------------------------------- */

/** set engine used for component generation
 */
int
eng_seteng(const char *tmpl)
{
  const engdescr *engs[] = {
#ifdef TCLSH
    &eng_tcl,
#endif
    NULL
  };
  const engdescr *e;
  char *name;

  struct dirent *de;
  DIR *d;

  /* look for template.xxx entry */
  d = opendir(tmpl);
  if (!d) {
    warnx("cannot open directory %s", tmpl);
    warn(NULL);
    return errno;
  }

  name = NULL;
  while((de = readdir(d))) {
    if (strncmp(de->d_name, TMPL_SPECIAL_FILE, strlen(TMPL_SPECIAL_FILE)))
      continue;
    if (de->d_type == DT_DIR) {
      warnx("template entry '%s' should not be a directory", de->d_name);
      break;
    }

    xwarnx("found template entry '%s'", de->d_name);
    name = de->d_name + strlen(TMPL_SPECIAL_FILE);
    break;
  }
  closedir(d);
  if (!name) {
    warnx("cannot find template entry '" TMPL_SPECIAL_FILE "<engine>'");
    return ENOENT;
  }

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

  /* set interpreter */
  if (!runopt.interp[0])
    strlcpy(runopt.interp, engine->interp, sizeof(runopt.interp));
  xwarnx("using %s interpreter", runopt.interp);

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

/** generate a description of the dotgen file in the engine language and invoke
 * interpreter to execute template.
 */
int
eng_invoke()
{
  char output[1024];
  char gen[PATH_MAX];
  char interp[PATH_MAX];
  char tmpl[PATH_MAX];

  int stdfd[4], m;
  FILE *g, *out, *err;
  fd_set fdset;

  int s;

  comp_s c = comp_dotgen();
  assert(c);
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
  strlcpy(interp, runopt.interp, sizeof(interp));
  strlcpy(interp, basename(interp), sizeof(interp));
  strlcpy(tmpl, runopt.tmpl, sizeof(tmpl));
  strlcpy(tmpl, basename(tmpl), sizeof(tmpl));

  if (pipe(&stdfd[0]) < 0 || pipe(&stdfd[2]) < 0) {
    warn("cannot create a pipe for template interpreter:");
    return errno;
  }

  s = eng_optappend(interp, 0);
  if (!s) s = eng_optappend(gen, 1);
  if (s) return s;

  if (runopt.verbose) {
    char **o;
    xwarnx("spawning engine with the following arguments:");
    for(o = engopts; *o; o++) xwarnx("  + %s", *o);
  }

  switch((engpid = fork())) {
    case -1:
      warnx("unable to fork"); warn(NULL);
      return errno = EAGAIN;

    case 0: { /* child */
      if (dup2(stdfd[1], fileno(stdout)) < 0 ||
	  dup2(stdfd[3], fileno(stderr)) < 0) {
	warnx("unable to set interpreter stdout"); warn(NULL);
	_exit(2);
      }
      close(stdfd[0]); close(stdfd[1]);
      close(stdfd[2]); close(stdfd[3]);

      s = execvp(runopt.interp, engopts);
      if (s) { warnx("unable to exec '%s'", runopt.interp); warn(NULL); }
      _exit(127);
    }

    default: /* parent */
      close(stdfd[1]);
      close(stdfd[3]);
      eng_optrm(0);
      eng_optrm(0);
      break;
  }

  /* wait for interpreter */
  out = fdopen(stdfd[0], "r"); assert(out);
  err = fdopen(stdfd[2], "r"); assert(err);

  m = stdfd[0]>stdfd[2] ? stdfd[0] : stdfd[2];
  FD_ZERO(&fdset);
  FD_SET(stdfd[0], &fdset);
  FD_SET(stdfd[2], &fdset);
  do {
    s = select(1 + m, &fdset, NULL, NULL, NULL);
    if (s > 0) {
      if (FD_ISSET(stdfd[0], &fdset)) {
	if (fgets(output, sizeof(output), out)) {
	  if (runopt.verbose) printf("%s: %s", tmpl, output);
	} else FD_CLR(stdfd[0], &fdset);
      } else FD_SET(stdfd[0], &fdset);

      if (FD_ISSET(stdfd[2], &fdset)) {
	if (fgets(output, sizeof(output), err)) {
	  fprintf(stderr, "%s: %s", tmpl, output);
	} else FD_CLR(stdfd[2], &fdset);
      } else FD_SET(stdfd[2], &fdset);
    }

    if (feof(out) && feof(err)) break;
  } while(s >= 0 || errno == EINTR);
  if (s < 0) warnx("unable to read interpreter output");

  fclose(out); fclose(err);

  waitpid(engpid, &s, 0);
  if ((!WIFEXITED(s) || WEXITSTATUS(s))) {
    warnx("generator engine exited with code %d", WEXITSTATUS(s));
    return WEXITSTATUS(s);
  }

  xwarnx("generator engine exited with code %d", WEXITSTATUS(s));

  return 0;
}
