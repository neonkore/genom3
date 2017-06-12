/*
 * Copyright (c) 2009-2013,2017 LAAS/CNRS
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

static void		usage(FILE *channel, char *argv0);
static const char *	abspath(const char *path);
static const char *	findexec(const char *prog);
static int		rmrfdir(const char *path);

/* options strings and usage message are generated from options.txt */
#include "options.c"

/** runtime options */
struct runopt_s runopt;

/** number of errors and warnings */
int nerrors, nwarnings;


/* --- main ---------------------------------------------------------------- */

int
main(int argc, char *argv[])
{
  extern char *optarg;
  extern int optind;
  char *argv0 = argv[0];
  int pipefd[2];
  int status;
  int i, c, s;

  /* set default options */
  runopt.tmpl[0] = '\0';
  runopt.engine[0] = '\0';
  strlcpy(runopt.sysdir, SYSDIR, sizeof(runopt.sysdir));
  strlcpy(runopt.tmpldir, TMPLDIR, sizeof(runopt.tmpldir));

  optarg = getenv("GENOM_TMPL_PATH");
  runopt.sitetmplpath = optarg?optarg:SITETMPLPATH;

  optarg = getenv("TMPDIR");
  strlcpy(runopt.tmpdir, optarg?optarg:TMPDIR, sizeof(runopt.tmpdir));

  optarg = getenv("GENOM_CPP");
  strlcpy(runopt.cpppath, optarg?optarg:CPPPATH, sizeof(runopt.cpppath));

  xwarnx_verbosity(0);
  runopt.verbose = 0;
  runopt.debug = 0;
  runopt.preproc = 0;
  runopt.parse = 0;
  runopt.list = 0;
  runopt.genom = string(findexec(argv[0]));
  runopt.cmdline = string("");
  runopt.notice = NULL;

  runopt.cppdotgen = 1;
  s = cpp_optappend("-D__GENOM__=" PACKAGE_VERSION, -1);
  if (s < 0) {
    warnx("cannot set default cpp options");
    exit(2);
  }

  /* parse command line options */
  while (
    (c = getopt_long(argc, argv, shortopts_string, longopts_list, NULL)) != -1)
    switch (c) {
      case 0: break;

      case 'I':
      case 'D': {
	char opt[PATH_MAX];

	opt[0] = '-'; opt[1] = c; opt[2] = 0;
	strlcat(opt, optarg, sizeof(opt));
	s = cpp_optappend(opt, -1);
	if (s < 0) {
	  warnx("cannot set cpp option `%s'", opt);
	  exit(2);
	}

	if (c == 'I') strlcpy(opt+2, abspath(optarg), sizeof(opt)-2);
	runopt.cmdline = strings(runopt.cmdline, " ", opt, NULL);
	break;
      }

      case 'T':
	strlcpy(runopt.tmpdir, optarg, sizeof(runopt.tmpdir));
	runopt.cmdline = strings(runopt.cmdline, " -T ", abspath(optarg), NULL);
	break;

      case 't':
	runopt.sitetmplpath = string(optarg);
	runopt.cmdline = strings(runopt.cmdline, " -t ", abspath(optarg), NULL);
	break;

      case 's':
	strlcpy(runopt.sysdir, optarg, sizeof(runopt.sysdir));
	runopt.cmdline = strings(runopt.cmdline, " -s ", abspath(optarg), NULL);
	break;

      case 'r':
	runopt.cppdotgen = 0;
	runopt.cmdline = strings(runopt.cmdline, " -r", NULL);
	break;

      case 'l':
	if (runopt.preproc || runopt.parse)
	  errx(2, "mutually exclusive options near '%s'", argv[optind-1]);
	runopt.list = 1;
	break;

      case 'n':
	if (runopt.list || runopt.preproc || runopt.parse > 1)
	  errx(2, "mutually exclusive options near '%s'", argv[optind-1]);
	runopt.parse = 1;
	break;

      case 'N':
	if (runopt.list || runopt.preproc || runopt.parse == 1)
	  errx(2, "mutually exclusive options near '%s'", argv[optind-1]);
	runopt.parse = 2;
	break;

      case 'E':
	if (runopt.list || runopt.parse)
	  errx(2, "mutually exclusive options near '%s'", argv[optind-1]);
	runopt.preproc = 1;
	break;

      case 'v': runopt.verbose = 1; xwarnx_verbosity(1); break;
      case 'd': {
#ifndef NDEBUG
	extern int dotgendebug;
	dotgendebug = 1;
#endif
	runopt.debug = 1;
	break;
      }

      case -'v': puts(PACKAGE_VERSION); exit(0); break;
      case 'h':  usage(stdout, argv0);  exit(0); break;

      case '?':
      default:
	usage(stderr, argv0);
        exit(1);
        break;
    }
  argc -= optind;
  argv += optind;

  /* just list templates */
  if (runopt.list) {
    char **list, **t;

    if (argc != 0) errx(2, "wrong number of arguments");
    s = eng_listtmpl(&list);
    if (s) exit(s);

    for(t = list; t && *t; t++) { printf("%s\n", *t); free(*t); }
    free(list);
    exit(0);
  }

  /* create a temporary directory */
  strlcat(runopt.tmpdir, "/genomXXXXXX", sizeof(runopt.tmpdir));
  if (!mkdtemp(runopt.tmpdir))
    err(2, "cannot create directory `%s'", runopt.tmpdir);
  xwarnx("created directory `%s'", runopt.tmpdir);

  /* if a template is required (not -E or -n), configure template arguments */
  if (!runopt.parse && !runopt.preproc) {
    if (argc < 1) {
      usage(stderr, argv0);
      status = 1;
      goto done;
    }

    const char *p = eng_findtmpl(argv[0]);
    if (!p) { status = ENOENT; goto done; }
    strlcpy(runopt.tmpl, abspath(p), sizeof(runopt.tmpl));
    xwarnx("template path `%s'", runopt.tmpl);

    status = eng_seteng(runopt.tmpl);
    if (status) goto done;

    argc--;
    argv++;

    for(i=0; i<argc; i++) eng_optappend(argv[i], -1);
  }

  /* check for an input file if just parsing/preprocessing */
  if ((runopt.parse || runopt.preproc) && argc < 1) {
    usage(stderr, argv0);
    status = 1;
    goto done;
  }

  /* just preprocess input file */
  if (runopt.preproc) {
    cpp_invoke(argv[0], 1);
    status = cpp_wait();
    goto done;
  }

  /* create parsing context */
  status = scope_pushglobal();
  if (status) goto done;

  /* just parse input file */
  if (runopt.parse) {
    if (pipe(pipefd) < 0) {
      warn("cannot create a pipe to cpp:");
      status = 2; goto done;
    }
    dotgen_input(DG_INPUT_FILE, pipefd[0]);
    cpp_invoke(argv[0], pipefd[1]);

    s = dotgenparse();
    status = cpp_wait();
    if (!status) status = s;
    if (!status) status = dotgen_consolidate();
    if (s || nerrors) {
      warnx(s?"fatal errors":"%d error%s", nerrors, nerrors>1?"s":"");
      if (!status) status = s?s:nerrors;
    }
    if (runopt.parse > 1) status = comp_dumpall(stdout);
    goto done;
  }

  /* invoke template */
  status = eng_invoke();
  if (status) goto done;

done:
  if (runopt.debug) {
    string_usage();
    type_usage();
  }

  /* clean up */
  if (runopt.debug)
    warnx("left out temporary files in %s", runopt.tmpdir);
  else
    rmrfdir(runopt.tmpdir);
  return status;
}


/* --- parserror ----------------------------------------------------------- */

/** Print a parsing error or warning
 */

static void
parsemsg(tloc l, const char *pfix, const char *fmt, va_list va)
{
  char *f = basename(l.file);

  fprintf(stderr, "%s:%d:%s%s ", f, l.line, pfix?pfix:"", pfix?":":"");
  vfprintf(stderr, fmt, va);
  fprintf(stderr, "\n");
}

void
parsenoerror(tloc l, const char *fmt, ...)
{
  va_list va;

  va_start(va, fmt);
  parsemsg(l, NULL, fmt, va);
  va_end(va);
}

void
parserror(tloc l, const char *fmt, ...)
{
  va_list va;

  va_start(va, fmt);
  parsemsg(l, NULL, fmt, va);
  va_end(va);
  nerrors++;
}

void
parsewarning(tloc l, const char *fmt, ...)
{
  va_list va;

  va_start(va, fmt);
  parsemsg(l, " warning", fmt, va);
  va_end(va);
  nwarnings++;
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
  fprintf(channel,
	  "GenoM " PACKAGE_VERSION " component generator\n\n"
	  "Usage:\n  %1$s [-l] [-h] [--version]\n"
	  "  %1$s [-I dir] [-D macro[=value]] [-E|-n] [-v] [-d] file.gen\n"
	  "  %1$s [general options] template [template options] file.gen\n"
	  "\n%2$s",
	  basename(argv0), usage_string);
}


/* --- abspath ------------------------------------------------------------- */

/** Return absolute path
 */
static const char *
abspath(const char *path)
{
  const char *p;
  char *r;
  if (path[0] != '/') {
    r = getcwd(NULL, 0);
    p = strings(r, "/", path, NULL);
    free(r);
  } else
    p = string(path);

  return p;
}


/* --- findexec ------------------------------------------------------------ */

/** Return absolute path to the program. Search in PATH.
 */
static const char *
findexec(const char *prog)
{
  char f[PATH_MAX];
  struct stat sb;
  char *p, *t;
  int len;

  char *path = getenv("PATH");
  if (!path) path = "/usr/bin:/bin";

  if (!stat(prog, &sb) && S_ISREG(sb.st_mode) && !access(prog, X_OK))
    return abspath(prog);

  /* create a writable copy of PATH */
  path = string(path);

  for (p = path; p; ) {
      /* for each path... */
      t = p;
      if ((p = strchr(p, ':')) != NULL) {
	*p = '\0';
	if (t == p) t = ".";
      } else
	if (strlen(t) == 0) t = ".";

      len = snprintf(f, sizeof(f), "%s/%s", t, prog);
      if (p) p++;

      if (len >= sizeof(f)) continue;
      if (stat(f, &sb) == -1) continue;
      if (!S_ISREG(sb.st_mode)) continue;
      if (access(f, X_OK) == -1) continue;

      return abspath(f);
  }

  return prog;
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

    xwarnx("removed file `%s/%s'", path, e->d_name);
  }

  if (fchdir(cwd)) {}
  close(cwd);
  closedir(d);

  rmdir(path);
  xwarnx("removed directory `%s'", path);
  return 0;
}
