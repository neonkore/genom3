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
#ifdef WITH_TCL

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <dlfcn.h>
#include <sys/time.h>
#include <errno.h>

#include "genom.h"


/* --- local data ---------------------------------------------------------- */

static int	tcl_invoke(const char *tmpl, int argc, char **argv);

const engdescr eng_tcl = {
  .name =	"tcl",
  .invoke =	tcl_invoke
};


/* --- setup --------------------------------------------------------------- */

/** Setup Tcl engine
 */
static int
tcl_invoke(const char *tmpl, int argc, char **argv)
{
  char module[PATH_MAX];
  int (*invoke)(const char *, int, char **);
  void *e;
  int s;

  /* open engine module - it's not statically linked to avoid a hardcoded
   * dependency on tcl. ltdl should probably be used instead dlopen() et al.
   */
  strlcpy(module, runopt.sysdir, sizeof(module));
  strlcat(module, "/tcl/engine.so", sizeof(module));
  printf("loading %s engine from '%s'\n", eng_tcl.name, module);
  e = dlopen(module, RTLD_LAZY);
  if (!e) {
    fprintf(stderr, "cannot load '%s'\n", module);
    fprintf(stderr, "%s\n", dlerror());
    return ENOENT;
  }

  /* invoke engine init function */
  printf("initializing %s engine\n", eng_tcl.name);
  invoke = dlsym(e, "engine_invoke");
  if (!invoke) {
    fprintf(stderr, "cannot execute template\n");
    fprintf(stderr, "%s\n", dlerror());
    return EINVAL;
  }

  s = invoke(tmpl, argc, argv);
  if (s) return s;

  return 0;
}

#endif /* WITH_TCL */
