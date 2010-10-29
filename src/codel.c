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
 *                                           Anthony Mallet on Wed Dec 30 2009
 */
#include "acgenom.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <err.h>

#include "genom.h"


/* --- local data ---------------------------------------------------------- */

struct codel_s {
  tloc loc;
  const char *name;

  idltype_s rettype;	/**< return type */
  hash_s params;	/**< parameter list */
  hash_s yields;	/**< transitions */
  hash_s triggers;	/**< start conditions */

  task_s task;		/**< codel's task */
  service_s service;	/**< codel's service */
};

tloc		codel_loc(codel_s c) { assert(c); return c->loc; }
const char *	codel_name(codel_s c) { assert(c); return c->name; }
idltype_s	codel_return(codel_s c) { assert(c); return c->rettype; }
hash_s		codel_params(codel_s c) { assert(c); return c->params; }
hash_s		codel_triggers(codel_s c) { assert(c); return c->triggers; }
hash_s		codel_yields(codel_s c) { assert(c); return c->yields; }
task_s *	codel_task(codel_s c) { assert(c); return &c->task; }
service_s *	codel_service(codel_s c) { assert(c); return &c->service; }


/* --- codel_create -------------------------------------------------------- */

/** create new codel
 */
codel_s
codel_create(tloc l, const char *name, hash_s triggers, hash_s yields,
	     hash_s params)
{
  codel_s c;
  assert(name && params);

  c = malloc(sizeof(*c));
  if (!c) {
    warnx("memory exhausted, cannot create codel '%s'", name);
    return NULL;
  }
  c->loc = l;
  c->name = string(name);

  c->rettype = comp_eventtype(comp_current());

  c->params = params;
  c->yields = yields ? yields : hash_create("yields list", 0);
  c->triggers = triggers ? triggers : hash_create("triggers list", 0);

  c->task = NULL;
  c->service = NULL;

  if (!yields) {
    /* validation codels return OK, or one of the exceptions of the service */
    if (hash_insert(c->yields, "ok", "ok", NULL))
      parserror(l, "failed to create component internal event '%s'", "ok");
  }

  /* register internal events */
  if (comp_addievs(l, c->yields) || comp_addievs(l, c->triggers)) {
    free(c);
    return NULL;
  }

  xwarnx("created codel %s", c->name);
  return c;
}


/* --- codel_fsmcreate ----------------------------------------------------- */

/** Build a fsm hash from existing codels in a list of properties
 */
hash_s
codel_fsmcreate(tloc l, hash_s props)
{
  const char *e;
  hash_s h;
  codel_s c, u;
  hiter i, j;
  int s;

  h = hash_create("fsm", 1);
  if (!h) {
    parserror(l, "failed to create fsm");
    return NULL;
  }

  for(hash_first(props, &i); i.current; hash_next(&i))
    switch(prop_kind(i.value)) {
      case PROP_CODEL:
	c = prop_codel(i.value);
	for(hash_first(codel_triggers(c), &j); j.current; hash_next(&j)) {
	  assert(type_kind(j.value) == IDL_ENUMERATOR);
	  e = type_name(j.value);
	  s = hash_insert(h, e, c, NULL);
	  switch(s) {
	    case 0: break;

	    case EEXIST:
	      /* only one codel per event is allowed */
	      parserror(codel_loc(c), "duplicate event %s", e);
	      u = hash_find(h, e);
	      if (u) parserror(codel_loc(u), " event %s declared here", e);
	      break;

	    default:
	      hash_destroy(h);
	      return NULL;
	  }
	}
	break;

      default: break;
    }

  return h;
}
