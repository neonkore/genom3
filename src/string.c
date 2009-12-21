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
 *                                           Anthony Mallet on Mon Dec 21 2009
 */
#include "acgenom.h"

#include <stdlib.h>
#include <string.h>
#include <err.h>

#include "genom.h"


/* --- local data ---------------------------------------------------------- */

static hash_s strings;


/* --- string -------------------------------------------------------------- */

char *
string(const char *src)
{
  char *v;
  int s;

  if (!strings) {
    strings = hash_create("strings", 256);
    if (!strings) {
      warnx("memory exhausted, cannot store any string");
      return NULL;
    }
  }

  v = hash_find(strings, src);
  if (v) return v;

  v = strdup(src);
  if (!v) {
    warnx("memory exhausted, cannot store a string");
    return NULL;
  }
  s = hash_insert(strings, src, v, free);
  if (s) return NULL;

  return v;
}


/* --- string_usage -------------------------------------------------------- */

/** Print information regarding string usage
 */

void
string_usage()
{
  hash_pstat(strings);
}
