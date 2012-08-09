/*
 * Copyright (c) 2011-2012 LAAS/CNRS
 * All rights reserved.
 *
 * Redistribution and use  in source  and binary  forms,  with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *   1. Redistributions of  source  code must retain the  above copyright
 *      notice and this list of conditions.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice and  this list of  conditions in the  documentation and/or
 *      other materials provided with the distribution.
 *
 *					Anthony Mallet on Mon Jun 27 2011
 */
#include "acgenom.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <sys/types.h>
#ifdef HAVE_SYS_WAIT_H
#include <sys/wait.h>
#endif

#include "genom.h"
#include "cpp.h"

static void	get_pkgconfig_cflags(char **argv);


void
pragmaimpl(usch *p)
{
  if (!strncasecmp((char *)p, "require", 7)) {
#   define PRAGMA_MAXARGS	10
    char *argv[PRAGMA_MAXARGS];
    const char *delim;
    unsigned int n;

    p += strcspn((char *)p, " \t");
    /* fill args, starting from index 2 */
    for (n=2; *p;) {
      for(; *p == ' ' || *p == '\t'; p++) /* empty body */;
      if (*p == 0) break;

      if (*p == 'L' && p[1] == '\"') p++;
      if (*p == '\"') { p++; delim = "\""; } else delim = " \t";
      argv[n] = (char *)p;
      do {
        p += strspn((char *)p, delim);
        p += strcspn((char *)p, delim);
      } while (p[-1] == '\\');
      if (++n > PRAGMA_MAXARGS) {
        error("pragma require has too many arguments");
        return;
      }
      if (*p == 0) break;
      *p++ = 0;
    }
    if (n <= 2) {
      error("pragma require without arguments");
      return;
    }
    argv[n] = NULL;
    get_pkgconfig_cflags(argv);
  }
}


static void
get_pkgconfig_cflags(char **argv)
{
  const char *pkgconfig = getenv("PKG_CONFIG");
  char buffer[1024];
  int outpipe[2];
  pid_t pid;
  char *cflags = NULL;
  size_t r, n;
  int status, next;
  char *p;

  if (pkgconfig == NULL) pkgconfig = "pkg-config";
  argv[0] = "pkg-config";
  argv[1] = "--cflags";

  if (pipe(outpipe) == -1) {
    error("error piping: %s", strerror(errno));
    return;
  }

  pid = fork();
  if (pid == 0) {
    /* fork pkg-config */
    close(outpipe[0]);
    close(fileno(stdout));
    dup2(outpipe[1], fileno(stdout));

    execvp(pkgconfig, argv);
    /* not reached */
    fprintf(stderr, "error running %s: ", pkgconfig);
    perror("");
    _exit(1);
  }

  /* parse pkg-config */
  close(outpipe[1]);

  if (waitpid(pid, &status, 0) == -1) {
    error("error running %s: %s", pkgconfig, strerror(errno));
    return;
  }
  if (!WIFEXITED(status) || WEXITSTATUS(status)) {
    exit(WEXITSTATUS(status));
  }

  n = 0;
  while((r = read(outpipe[0], buffer, 1023)) > 0) {
    buffer[r] = 0;
    n += r;
    p = realloc(cflags, n+1);
    if (!p) {
      error("error: %s", strerror(errno));
      if (cflags) free(cflags);
      return;
    }
    cflags = p;
    strcpy(cflags+n-r, buffer);
  }
  close(outpipe[0]);
  if (!cflags) return;

  /* split cflags at whitespace */
  next = 0;
  for(p = strtok(cflags, " \t"); p; p = strtok(NULL, " \t")) {
    if (next == 'I') {
      next = 0;
      addpkgidir(p);
    } else if (next == 'D') {
      next = 0;
      addpkgdef(p);
    } else if (!strncmp(p, "-I", 2)) {
      if (p[2] == 0) { next = 'I'; continue; }
      addpkgidir(p+2);
    } else if (!strncmp(p, "-D", 2)) {
      if (p[2] == 0) { next = 'D'; continue; }
      addpkgdef(p+2);
    }
  }

  free(cflags);
}
