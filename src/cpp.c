/*
 * Copyright (c) 2009 LAAS/CNRS
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
 *                                           Anthony Mallet on Sat Dec 19 2009
 */
#include "acgenom.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <libgen.h>
#include <err.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>

#include "genom.h"


/* --- local data ---------------------------------------------------------- */

static const char *	cpp_execpath(void);


/** options array to pass to cpp */
static char **cppopts = NULL;

/** number of options */
static int ncppopts = 0;

/** pid of lastest cpp process */
static pid_t cpppid = -1;


/* --- cpp_optappend ------------------------------------------------------- */

int
cpp_optappend(const char *opt, int index)
{
  char *s;
  char **a;

  a = realloc(cppopts, (ncppopts+2)*sizeof(*cppopts));
  if (!a) goto enomem;
  cppopts = a;

  s = strdup(opt);
  if (!s) goto enomem;

  if (index < 0 || index >= ncppopts) {
    cppopts[ncppopts] = s;
    cppopts[ncppopts+1] = NULL;
  } else {
    /* rotate all options */
    memmove(cppopts+index+1, cppopts+index,
	    (ncppopts-index+1)*sizeof(*cppopts));
    cppopts[index] = s;
  }

  ncppopts++;
  xwarnx("added cpp option `%s'", s);
  return 0;

enomem:
  warnx("memory exhausted, ignoring cpp option `%s'", opt);
  return ENOMEM;
}


/* --- cpp_optrm ----------------------------------------------------------- */

int
cpp_optrm(int index)
{
  char **a;

  if (index >= 0 && index < ncppopts) {
    xwarnx("removed cpp option `%s'", cppopts[index]);
    free(cppopts[index]);
    memmove(cppopts+index, cppopts+index+1,
	    (ncppopts-index)*sizeof(*cppopts));
  } else {
    xwarnx("removed cpp option `%s'", cppopts[ncppopts-1]);
    free(cppopts[ncppopts-1]);
  }
  ncppopts--;

  a = realloc(cppopts, (ncppopts+1)*sizeof(*cppopts));
  if (a) cppopts = a;

  return 0;
}


/* --- cpp_invoke ---------------------------------------------------------- */

/** Call cpp on input file
 */

int
cpp_invoke(const char *in, int out)
{
  char *cpp, *p;
  int noptexec;
  int s, n;

  /* get cpp executable */
  cpp = strdup(cpp_execpath());
  if (!cpp || cpp[0] == '\0') {
    warnx("memory exhausted, cannot run cpp");
    return ENOMEM;
  }

  /* split executable into options array */
  for(noptexec=0, p = cpp; *p != '\0'; noptexec++) {
    n = strcspn(p, " \t");
    if (p[n] != '\0') {
      p[n] = '\0';
      s = cpp_optappend(p, noptexec);
      p += n+1;
    } else {
      s = cpp_optappend(p, noptexec);
      p += n;
    }
    if (s) {
      warnx("memory exhausted, cannot run cpp");
      free(cpp);
      errno = ENOMEM; goto err;
    }
  }
  free(cpp);

  /* set input file. link to a .c file if needed */
  if (!runopt.cppdotgen) {
    char file[PATH_MAX];

    strlcpy(file, runopt.tmpdir, sizeof(file));
    strlcat(file, "/", sizeof(file));
    strlcat(file, basename((char *)in), sizeof(file));
    strlcat(file, ".c", sizeof(file));
    s = symlink(in, file);
    xwarnx("linked input file to `%s'", file);
    if (s) {
      warnx("cannot link input file to `%s'", file); warn(NULL);
      errno = EIO; goto err;
    }

    s = cpp_optappend(file, -1);
  } else
    s = cpp_optappend(in, -1);
  if (s) {
    warnx("memory exhausted, cannot run cpp");
    errno = ENOMEM; goto err;
  }

  /* execvp cpp */
  if (runopt.verbose) {
    char **o;
    xwarnx("spawning cpp with the following arguments:");
    for(o = cppopts; *o; o++) xwarnx("  + %s", *o);
  }

  switch((cpppid = fork())) {
    case -1:
      warnx("unable to fork cpp"); warn(NULL);
      cpp_optrm(-1);
      errno = EAGAIN; goto err;

    case 0: /* child */
      if (dup2(out, fileno(stdout)) < 0) {
	warnx("unable to set cpp stdout"); warn(NULL);
	_exit(2);
      }

      s = execvp(cppopts[0], cppopts);
      if (s) { warnx("unable to exec '%s'", cppopts[0]); warn(NULL); }
      _exit(127);

    default: /* parent */ break;
  }

  /* clean up */
  for(;noptexec>0; noptexec--) cpp_optrm(0);
  cpp_optrm(-1);
  return 0;
err:
  for(;noptexec>0; noptexec--) cpp_optrm(0);
  return errno;
}


/* --- cpp_wait ------------------------------------------------------------ */

/** Wait for cpp process
 */

int
cpp_wait()
{
  int status;

  waitpid(cpppid, &status, 0);
  if ((!WIFEXITED(status) || WEXITSTATUS(status))) {
    warnx("cpp process exited with code %d", WEXITSTATUS(status));
    return WEXITSTATUS(status);
  }

  xwarnx("cpp process exited with code %d", WEXITSTATUS(status));
  return 0;
}


/* --- cpp_execpath -------------------------------------------------------- */

static const char *
cpp_execpath()
{
  const char *p = getenv("CPP");
  if (p) return p;

  return CPP;
}
