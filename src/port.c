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
  portkind kind;
  const char *name;
  comp_s component;
  idltype_s type;
  idltype_s datatype;
};

tloc		port_loc(port_s p) { assert(p); return p->loc; }
const char *	port_name(port_s p) { assert(p); return p->name; }
portkind	port_kind(port_s p) { assert(p); return p->kind; }
comp_s		port_comp(port_s p) { assert(p); return p->component; }
idltype_s	port_type(port_s p) { assert(p); return p->type; }
idltype_s	port_datatype(port_s p) { assert(p); return p->datatype; }

static int		port_mkbuiltins(tloc l);
static idltype_s	port_handle_type(tloc l);
static idltype_s	port_handleset_type(tloc l);


/* --- port_new ------------------------------------------------------------ */

/** create a port in a component
 */
port_s
port_new(tloc l, portkind k, const char *name, idltype_s t)
{
  comp_s c;
  service_s service;
  remote_s remote;
  port_s p;
  idltype_s h;
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
    parserror(l, "anonymous types are not allowed for port %s", name);
    return NULL;
  }

  /* accept array type for handle ports only */
  if (k & PORT_ARRAY && (k & PORT_CATMSK) != PORT_HANDLE) {
    parserror(l, "%s port %s is not supported", port_strkind(k), name);
    return NULL;
  }

  /* create */
  p = malloc(sizeof(*p));
  if (!p) {
    warnx("memory exhausted, cannot create port");
    return NULL;
  }

  if (k & PORT_HANDLE) {
    if (k & PORT_STATIC)
      h = port_handle_type(l);
    else if (k & PORT_ARRAY)
      h = port_handleset_type(l);
    else assert(0);
  } else if (k & PORT_DATA) {
    h = t;
  } else assert(0);

  p->loc = l;
  p->kind = k;
  p->name = string(name);
  p->component = c;
  p->type = h;
  p->datatype = t;

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


/* --- port_mkbuiltins ----------------------------------------------------- */

/** create port builtin types
 */
static int
port_mkbuiltins(tloc l)
{
  idltype_s t, h;
  scope_s s, p;

  p = scope_current();
  scope_set(scope_global());

  s = scope_push(l, GENOM_NAMESPACE, SCOPE_MODULE);
  if (!s) return errno;

  /* port handle */
  s = scope_push(l, PORT_HANDLE_NAME, SCOPE_STRUCT);
  if (!s) return errno;
  t = type_newbasic(l, NULL, IDL_ULONG);
  if (!t) return errno;
  t = type_newmember(l, "id", t);
  if (!t) return errno;
  scope_detach(scope_pop());
  h = type_newstruct(l, PORT_HANDLE_NAME, s);
  if (!h) return errno;

  /* port handle sequence */
  t = type_newsequence(l, NULL, h, -1U);
  if (!t) return errno;
  t = type_newalias(l, PORT_HANDLE_SET_NAME, t);
  if (!t) return errno;

  scope_set(p);
  xwarnx("created builtin port handle types");
  return 0;
}


/* --- port_handle_type ---------------------------------------------------- */

/** get port handle builtin type
 */
static idltype_s
port_handle_type(tloc l)
{
  idltype_s h;
  int s;

  h = type_find("::" GENOM_NAMESPACE "::" PORT_HANDLE_NAME);
  if (!h) {
    s = port_mkbuiltins(l);
    assert(!s);
    h = type_find("::" GENOM_NAMESPACE "::" PORT_HANDLE_NAME);
  }

  assert(h);
  return h;
};


/* --- port_handleset_type ------------------------------------------------- */

/** get port handle set builtin type
 */
static idltype_s
port_handleset_type(tloc l)
{
  idltype_s h;
  int s;

  h = type_find("::" GENOM_NAMESPACE "::" PORT_HANDLE_SET_NAME);
  if (!h) {
    s = port_mkbuiltins(l);
    assert(!s);
    h = type_find("::" GENOM_NAMESPACE "::" PORT_HANDLE_SET_NAME);
  }

  assert(h);
  return h;
};


/* --- port_strkind -------------------------------------------------------- */

/** Return a port kind as a string
 */
const char *
port_strkind(portkind k)
{
  switch((unsigned int)k) {
    case PORT_IN|PORT_DATA|PORT_STATIC:		return "data in";
    case PORT_IN|PORT_HANDLE|PORT_STATIC:	return "handle in";
    case PORT_OUT|PORT_DATA|PORT_STATIC:	return "data out";
    case PORT_OUT|PORT_HANDLE|PORT_STATIC:	return "handle out";

    case PORT_IN|PORT_DATA|PORT_ARRAY:		return "data in array";
    case PORT_IN|PORT_HANDLE|PORT_ARRAY:	return "handle in array";
    case PORT_OUT|PORT_DATA|PORT_ARRAY:		return "data out array";
    case PORT_OUT|PORT_HANDLE|PORT_ARRAY:	return "handle out array";

    default: break;
  }

  assert(0);
  return NULL;
}
