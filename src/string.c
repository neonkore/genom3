/*
 * Copyright (c) 2009-2010 LAAS/CNRS
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
 *                                           Anthony Mallet on Mon Dec 21 2009
 */
#include "acgenom.h"

#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <err.h>

#include "genom.h"


/* --- local data ---------------------------------------------------------- */

static hash_s hstrings = NULL;


/* --- string -------------------------------------------------------------- */

char *
string(const char *src)
{
  char *v;
  int s;

  assert(src);

  if (!hstrings) {
    hstrings = hash_create("strings", 256);
    if (!hstrings) {
      warnx("memory exhausted, cannot store any string");
      return NULL;
    }
  }

  v = hash_find(hstrings, src);
  if (v) return v;

  v = strdup(src);
  if (!v) {
    warnx("memory exhausted, cannot store a string");
    return NULL;
  }
  s = hash_insert(hstrings, src, v, free);
  if (s) return NULL;

  return v;
}


/* --- strings ------------------------------------------------------------- */

/** Concatenate all strings and return the result
 */

char *
strings(const char *src, ...)
{
  va_list va;
  const char *p;
  char *s, *n;
  int l;

  assert(src);
  l = strlen(src);
  s = strdup(src);
  if (!s) {
    warnx("memory exhausted, cannot create string");
    return NULL;
  }
  va_start(va, src);
  while((p = va_arg(va, const char *))) {
    l += strlen(p);
    n = realloc(s, l+1);
    if (!n) {
      free(s);
      warnx("memory exhausted, cannot create string");
      return NULL;
    }
    strcat(s = n, p);
  }
  va_end(va);

  n = string(s);
  free(s);
  return n;
}


/* --- string_usage -------------------------------------------------------- */

/** Print information regarding string usage
 */

void
string_usage()
{
  if (hstrings) hash_pstat(hstrings);
}


/* --- bufcat -------------------------------------------------------------- */

/** Append string with format
 */
void
bufcat(char **buf, const char *fmt, ...)
{
  va_list ap;
  int s;
#if defined(HAVE_ASPRINTF) && defined(HAVE_VASPRINTF)
  char *r = NULL;

  va_start(ap, fmt);
  s = vasprintf(&r, fmt, ap);
  va_end(ap);
  if (s == -1) {
    warnx("memory exhausted");
    _exit(2);
  }
  if (*buf) {
    s = asprintf(buf, "%s%s", *buf, r);
    if (s == -1) {
      warnx("memory exhausted");
      _exit(2);
    }
    free(r);
  } else {
    *buf = r;
  }
#else
  char buf1[1];
  size_t l;
  va_start(ap, fmt);
  s = vsnprintf(buf1, sizeof(buf1), fmt, ap);
  va_end(ap);
  if (s < 0) {
    warnx("memory exhausted");
    _exit(2);
  }
  l = *buf ? strlen(*buf) : 0;
  s += l+1;

  *buf = realloc(*buf, s);
  if (!*buf) {
    warnx("memory exhausted");
    _exit(2);
  }

  va_start(ap, fmt);
  s = vsnprintf(*buf + l, s - l, fmt, ap);
  va_end(ap);
  if (s < 0) {
    warnx("memory exhausted");
    _exit(2);
  }
#endif /* ASPRINTF && VASPRINTF */
}


/* --- opt_append ---------------------------------------------------------- */

int
opt_append(char ***odb, int *nodb, const char *opt, int index)
{
  char *s;
  char **a;

  a = realloc(*odb, (*nodb+2)*sizeof(**odb));
  if (!a) return errno = ENOMEM;
  *odb = a;

  s = string(opt);
  if (!s) return errno = ENOMEM;

  if (index < 0 || index >= *nodb) {
    (*odb)[*nodb] = s;
    (*odb)[*nodb+1] = NULL;
  } else {
    /* rotate all options */
    memmove(*odb+index+1, *odb+index, (*nodb-index+1)*sizeof(**odb));
    (*odb)[index] = s;
  }

  (*nodb)++;
  return 0;
}


/* --- opt_rm -------------------------------------------------------------- */

int
opt_rm(char ***odb, int *nodb, int index)
{
  char **a;

  if (index >= 0 && index < *nodb)
    memmove(*odb+index, *odb+index+1, (*nodb-index)*sizeof(**odb));
  (*nodb)--;

  a = realloc(*odb, (*nodb+1)*sizeof(**odb));
  if (a) *odb = a;

  return 0;
}
