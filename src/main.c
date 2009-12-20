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
 *                                           Cédric Pasteur on Thu May 14 2009
 */
#include "acgenom.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <getopt.h>
#include <libgen.h>
#include <fcntl.h>
#include <errno.h>
#include <err.h>
#include <sys/stat.h>

#include "genom.h"


/* --- local data ---------------------------------------------------------- */

static void	usage(FILE *channel, char *argv0);
static int	rmrfdir(const char *path);


/** runtime options */
struct runopt_s runopt;


/* --- main ---------------------------------------------------------------- */

int
main(int argc, char *argv[])
{
  /* options descriptor */
  static struct option opts[] = {
    { "verbose",	no_argument,		NULL,			'v' },
    { "tmpdir",		required_argument,	NULL,			'T' },
    { "rename",		no_argument,		NULL,			'r' },
    { "no-rename",	no_argument,		&runopt.cppdotgen,	1 },
    { "help",		no_argument,		NULL,			'h' },
    { NULL,		0,			NULL,			0 }
  };

  extern int dotgenparse(void);
  extern FILE *dotgenin;
  extern char *optarg;
  extern int optind;
  char *argv0 = argv[0];
  int pipefd[2];
  int status;
  int c, s;

  /* set default options */
  optarg = getenv("TMPDIR");
  strlcpy(runopt.tmpdir, optarg?optarg:TMPDIR, sizeof(runopt.tmpdir));

  runopt.verbose = 0;
  runopt.preproc = 0;

#ifdef CPP_DOTGEN
  runopt.cppdotgen = getenv("CPP")?0:1;
#else
  runopt.cppdotgen = 0;
#endif

  /* parse command line options */
  while ((c = getopt_long(argc, argv, "EvT:rh", opts, NULL)) != -1)
    switch (c) {
      case 0: break;

      case 'T':
	strlcpy(runopt.tmpdir, optarg, sizeof(runopt.tmpdir));
	break;

      case 'r': runopt.cppdotgen = 0; break;
      case 'v': runopt.verbose = 1; break;
      case 'E': runopt.preproc = 1; break;

      case 'h':
	usage(stdout, argv0);
	exit(0);
	break;

      case '?':
      default:
	usage(stderr, argv0);
        exit(1);
        break;
    }
  argc -= optind;
  argv += optind;

  /* parse template arguments */
  if (argc < 2) {
    usage(stderr, argv0);
    exit(1);
  }

  strlcpy(runopt.tmpl, argv[0], sizeof(runopt.tmpl));

  /* parse input files */
  s = open(argv[1], O_RDONLY, 0);
  if (s < 0) {
    warnx("cannot open input file `%s'", argv[1]); warn(NULL);
    exit(2);
  }
  close(s);

  if (argv[1][0] != '/') {
    getcwd(runopt.input, sizeof(runopt.input));
    strlcat(runopt.input, "/", sizeof(runopt.input));
    strlcat(runopt.input, argv[1], sizeof(runopt.input));
    xwarnx("absolute path to input file `%s'", runopt.input);
  } else
    strlcpy(runopt.input, argv[1], sizeof(runopt.input));

  /* create a temporary directory */
  strlcat(runopt.tmpdir, "/genomXXXXXX", sizeof(runopt.tmpdir));
  strlcpy(runopt.tmpdir, mkdtemp(runopt.tmpdir), sizeof(runopt.tmpdir));
  xwarnx("created directory `%s'", runopt.tmpdir);

  /* process input file */
  if (runopt.preproc) {
    cpp_invoke(runopt.input, 1);
    status = cpp_wait();
    goto done;
  }

  if (pipe(pipefd) < 0) {
    warn("cannot create a pipe to cpp:");
    status = 2; goto done;
  }
  dotgenin = fdopen(pipefd[0], "r");
  cpp_invoke(runopt.input, pipefd[1]);
  s = dotgenparse();
  status = cpp_wait();
  if (s) {
    warnx("there were parse errors");
    if (!status) status = s;
  }

  /* clean up */
done:
  rmrfdir(runopt.tmpdir);
  return status;
}


/* --- xwarnx -------------------------------------------------------------- */

/** warnx() if verbose option was given
 */

void
xwarnx(const char *fmt, ...)
{
  va_list va;

  if (!runopt.verbose) return;

  va_start(va, fmt);
  vwarnx(fmt, va);
  va_end(va);
}


/* --- usage --------------------------------------------------------------- */

/** Print usage on a channel.
 *
 * \param[in] channel	Output channel
 * \param[in] argv0	Program invocation name
 */

static void
usage(FILE *channel, char *argv0)
{
  fprintf(
    channel,
    "Usage: %s [options] template [template options] file\n"
    "Parses a GenoM component and invokes template for code generation.\n"
    "\n"
    "General options:\n"
    "  -E\t\t\tstop after preprocessing stage\n"
    "  -T,--tmpdir=dir\tuse dir as the directory for temporary files\n"
    "  -r,--rename\t\talways invoke cpp with a .c file linked to input file\n"
    "     --no-rename\tpass input file directly to cpp (opposite of -r)\n"
    "\n"
    "Environment variables:\n"
    "  CPP\t\tC preprocessor program\n"
    "  TMPDIR\tdirectory for temporary files\n",
	  basename(argv0));
}


/* --- rmrfdir ------------------------------------------------------------- */

/** Remove a directory entry recursively, with a best effort approach (no error
 * checks)
 */

static int
rmrfdir(const char *path)
{
  struct dirent *e;
  DIR *d;
  int s, cwd;

  cwd = open(".", O_RDONLY, 0);
  if (cwd < 0) return errno;

  d = opendir(path);
  if (!d) { s = errno; close(cwd); return s; }

  if (chdir(path)) { s = errno; close(cwd); return s; }

  while((e = readdir(d))) {
    if (!strcmp(e->d_name, ".") || !strcmp(e->d_name, ".."))
      continue;

    switch (e->d_type) {
      case DT_DIR: rmrfdir(e->d_name); break;
      default: unlink(e->d_name); break;
    }

    xwarnx("removed path `%s/%s'", path, e->d_name);
  }

  fchdir(cwd);
  close(cwd);

  rmdir(path);
  xwarnx("removed path `%s'", path);
  return 0;
}
