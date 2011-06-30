/*
 * Copyright (c) 2011 LAAS/CNRS
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
 *                                           Anthony Mallet on Wed Feb 16 2011
 */
#include "acgenom.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <err.h>

#include "genom.h"


/* --- local data ---------------------------------------------------------- */

struct port_s {
  tloc loc;
  portkind kind;
  const char *name;
  comp_s component;
  idltype_s type;
};

tloc		port_loc(port_s p) { assert(p); return p->loc; }
const char *	port_name(port_s p) { assert(p); return p->name; }
portkind	port_kind(port_s p) { assert(p); return p->kind; }
comp_s		port_comp(port_s p) { assert(p); return p->component; }
idltype_s	port_type(port_s p) { assert(p); return p->type; }


/* --- port_new ------------------------------------------------------------ */

/** create a port in a component
 */
port_s
port_new(tloc l, portkind k, const char *name, idltype_s t)
{
  comp_s c;
  port_s p;
  int e;
  assert(name);

  /* kind must be well formed */
  assert((k & PORT_DIRMSK) && (k & PORT_CATMSK) && (k & PORT_FLGMSK));

  /* a component must exist */
  c = comp_active();
  if (!c) {
    parserror(l, "missing component declaration before port %s", name);
    return NULL;
  }

  /* create */
  p = malloc(sizeof(*p));
  if (!p) {
    warnx("memory exhausted, cannot create port");
    return NULL;
  }

  p->loc = l;
  p->kind = k;
  p->name = string(name);
  p->component = c;
  p->type = t;

  e = hash_insert(comp_ports(c), p->name, p, (hrelease_f)port_destroy);
  switch(e) {
    case 0: break;

    case EEXIST:
      parserror(l, "duplicate port %s", p->name);
      port_s u = comp_port(c, name);
      if (u) parserror(u->loc, " port %s declared here", u->name);
      /*FALLTHROUGH*/
    default:
      free(p);
      return NULL;
  }

  xwarnx("created %s port %s", port_strkind(k), p->name);
  return p;
}


/* --- port_destroy -------------------------------------------------------- */

/** destroy port
 */
void
port_destroy(port_s p)
{
  if (p) free(p);
}


/* --- port_strkind -------------------------------------------------------- */

/** Return a port kind as a string
 */
const char *
port_strkind(portkind k)
{
  switch((unsigned int)k) {
    case PORT_IN|PORT_DATA|PORT_STATIC:		return "data in";
    case PORT_IN|PORT_EVENT|PORT_STATIC:	return "event in";
    case PORT_OUT|PORT_DATA|PORT_STATIC:	return "data out";
    case PORT_OUT|PORT_EVENT|PORT_STATIC:	return "event out";

    case PORT_IN|PORT_DATA|PORT_ARRAY:		return "data in array";
    case PORT_IN|PORT_EVENT|PORT_ARRAY:		return "event in array";
    case PORT_OUT|PORT_DATA|PORT_ARRAY:		return "data out array";
    case PORT_OUT|PORT_EVENT|PORT_ARRAY:	return "event out array";

    default: break;
  }

  assert(0);
  return NULL;
}
