/*
 * Copyright (c) 2009,2011 LAAS/CNRS
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
 *                                           Anthony Mallet on Wed Dec 23 2009
 */
#include "acgenom.h"

#include <assert.h>
#include <stdlib.h>
#include <stddef.h>
#include <errno.h>
#include <err.h>

#include "genom.h"


/* --- local data ---------------------------------------------------------- */

struct dcl_s {
  tloc l;
  const char *name;

  unsigned int dims;
  unsigned long dim[0];

  idltype_s type;
};

const char *	dcl_name(dcl_s d) { assert(d); return d->name; }
idltype_s	dcl_type(dcl_s d) { assert(d); return d->type; }
int		dcl_isarray(dcl_s d) { assert(d); return d->dims?1:0; }


/* --- dcl_create ---------------------------------------------------------- */

/** Create a type declarator
 */
dcl_s
dcl_create(tloc l, const char *name)
{
  dcl_s d;

  d = malloc(sizeof(*d));
  if (!d) {
    warnx("memory exhausted, cannot create declarator");
    return NULL;
  }

  d->l = l;
  d->name = string(name);
  d->dims = 0;
  d->type = NULL;

  xwarnx("created declarator %s", d->name);
  return d;
}


/* --- dcl_destroy --------------------------------------------------------- */

/** Destroy a type declarator
 */
void
dcl_destroy(dcl_s d)
{
  if (d) {
    xwarnx("destroyed declarator %s", d->name);
    free(d);
  }
}


/* --- dcl_adddim ---------------------------------------------------------- */

/** Add a new dimension to a declarator
 */
dcl_s
dcl_adddim(dcl_s dcl, unsigned long dim)
{
  dcl_s d;
  assert(dcl);

  d = realloc(dcl, offsetof(struct dcl_s, dim[dcl->dims+1]));
  if (!d) {
    warnx("memory exhausted, cannot create declarator");
    return dcl;
  }

  d->dim[d->dims] = dim;
  d->dims++;

  xwarnx("declarator %s dimension %d set to %d", d->name, d->dims, dim);
  return d;
}


/* --- dcl_settype --------------------------------------------------------- */

/** define type of declarator
 */
idltype_s
dcl_settype(dcl_s dcl, idltype_s t)
{
  dcliter i;
  assert(dcl && t);

  /* for arrays, create intermediate anon array types */
  for(dcl_inner(dcl, &i); i.value != -1UL; dcl_next(&i)) {
    t = type_newarray(dcl->l, NULL, t, i.value);
    if (!t) break;
  }

  dcl->type = t;
  xwarnx("set declarator %s as %s", dcl->name, type_strkind(type_kind(t)));
  return t;
}


/* --- dcl_inner/next ------------------------------------------------------ */

/** Return the innermost/next dimension in declarator
 */

int
dcl_inner(dcl_s d, dcliter *i)
{
  assert(d); assert(i);

  i->d = d;
  i->current = 0;
  i->value = i->current>=d->dims?-1UL:d->dim[i->current];

  return i->current>=d->dims?ENOENT:0;
}

int
dcl_next(dcliter *i)
{
  assert(i);

  i->current++;
  i->value = i->current>=i->d->dims?-1UL:i->d->dim[i->current];

  return i->current>=i->d->dims?ENOENT:0;
}
