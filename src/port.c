/*
 * Copyright (c) 2011-2012 LAAS/CNRS
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
  portdir dir;
  portkind kind;
  const char *name;
  comp_s component;
  idltype_s type;
  idltype_s datatype;
};

tloc		port_loc(port_s p) { assert(p); return p->loc; }
const char *	port_name(port_s p) { assert(p); return p->name; }
portdir		port_dir(port_s p) { assert(p); return p->dir; }
portkind	port_kind(port_s p) { assert(p); return p->kind; }
comp_s		port_comp(port_s p) { assert(p); return p->component; }
idltype_s	port_type(port_s p) { assert(p); return p->type; }
idltype_s	port_datatype(port_s p) { assert(p); return p->datatype; }


/* --- port_new ------------------------------------------------------------ */

/** create a port in a component
 */
port_s
port_create(tloc l, portdir d, portkind k, const char *name, idltype_s t)
{
  comp_s c;
  service_s service;
  remote_s remote;
  port_s p;
  int e;
  assert(name);

  /* check for already used names */
  service = comp_service(c, name);
  remote = comp_remote(c, name);
  if (service || remote) {
    parserror(l, "redefinition of '%s'", name);
    if (service)
      parsenoerror(service_loc(service), " %s '%s' declared here",
                   service_strkind(service_kind(service)),
                   service_name(service));
    if (remote)
      parsenoerror(remote_loc(remote), " remote '%s' declared here",
                   remote_name(remote));
    return NULL;
  }

  /* refuse anonymous types */
  if (t && !type_name(t)) {
    parserror(l, "anonymous type is not allowed for port %s", name);
    return NULL;
  }

  /* create */
  p = malloc(sizeof(*p));
  if (!p) {
    warnx("memory exhausted, cannot create port");
    return NULL;
  }

  p->loc = l;
  p->dir = d;
  p->kind = k;
  p->name = string(name);
  p->component = c;
  p->datatype = t;
  p->type = type_newport(l, name, p);

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

  xwarnx("created %s %s port %s", port_strdir(d), port_strkind(k), p->name);
  return p;
}


/* --- port_clone ---------------------------------------------------------- */

/** clone a port
 */
port_s
port_clone(port_s port)
{
  return port_create(port->loc, port->dir, port->kind, port->name,
                     port->datatype);
}


/* --- port_destroy -------------------------------------------------------- */

/** destroy port
 */
void
port_destroy(port_s p)
{
  if (p) free(p);
}


/* --- port_strdir --------------------------------------------------------- */

/** Return a port dir as a string
 */
const char *
port_strdir(portdir d)
{
  switch(d) {
    case PORT_IN:	return "in";
    case PORT_OUT:	return "out";
    default: break;
  }

  assert(0);
  return NULL;
}

/* --- port_strkind -------------------------------------------------------- */

/** Return a port kind as a string
 */
const char *
port_strkind(portkind k)
{
  switch(k) {
    case PORT_SIMPLE:	return "simple";
    case PORT_MULTIPLE:	return "multiple";
    default: break;
  }

  assert(0);
  return NULL;
}
