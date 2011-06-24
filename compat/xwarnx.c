/*
 * Copyright (c) 2011 LAAS/CNRS
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
 *					Anthony Mallet on Fri Jun 24 2011
 */
#include "acgenom.h"

#include <stdarg.h>
#include <err.h>

static int verbose;

/* --- xwarnx -------------------------------------------------------------- */

/** Same a warnx() if verbose flag is true. Silent otherwise
 */

void
xwarnx(const char *fmt, ...)
{
  va_list va;

  if (!verbose) return;

  va_start(va, fmt);
  vwarnx(fmt, va);
  va_end(va);
}


/* --- xwarnx_verbosity ---------------------------------------------------- */

/** Configure xwarnx verbosity
 */

void
xwarnx_verbosity(int _verbose)
{
  verbose = _verbose;
}
