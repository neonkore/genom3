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
 *                                           Anthony Mallet on Tue Mar 23 2010
 */

#include "acgenom.h"

#include <assert.h>
#include <errno.h>
#include <math.h>

#include "genom.h"


/* --- local data ---------------------------------------------------------- */

static int dotgen_clkratechk(comp_s c, prop_s p);
static int dotgen_clkratedefault(comp_s c);


/* --- dotgen_consolidate -------------------------------------------------- */

/** perform sanity checks on parsed file
 */
int
dotgen_consolidate()
{
  hash_s types = type_all();
  comp_s c = comp_dotgen();
  hash_s services = comp_services(c);
  prop_s p;
  hiter i, j;
  int e;

  xwarnx("consolidating parsed tree");

  /* a component must exist */
  if (!c) {
    extern tloc curloc;
    parserror(curloc, "missing component declaration");
    return EINVAL;
  }

  /* look for unresolved forward type declaration */
  for(hash_first(types, &i); i.current; hash_next(&i)) {
    assert(type_fullname(i.value));
    switch(type_kind(i.value)) {
      case IDL_FORWARD_STRUCT:
      case IDL_FORWARD_UNION:
	if (!type_type(i.value))
	  parserror(type_loc(i.value), "%s %s was never defined",
		    type_strkind(type_kind(i.value)), type_fullname(i.value));
	break;

      default: break;
    }
  }

  /* check clock-rate or compute default value if not specified */
  e = 0;
  p = hash_find(comp_props(c), prop_strkind(PROP_CLOCKRATE));
  if (p)
    e |= dotgen_clkratechk(c, p);
  else
    e |= dotgen_clkratedefault(c);


  /* resolve service names in interrupts, before and after properties */
  for(hash_first(services, &i); i.current; hash_next(&i)) {
    for(hash_first(service_props(i.value), &j); j.current; hash_next(&j)) {
      switch(prop_kind(j.value)) {
	case PROP_PERIOD: case PROP_DELAY: case PROP_PRIORITY: case PROP_STACK:
	case PROP_DOC: case PROP_IDS: case PROP_VERSION: case PROP_LANG:
	case PROP_EMAIL: case PROP_REQUIRE: case PROP_BUILD_REQUIRE:
	case PROP_CLOCKRATE: case PROP_TASK: case PROP_VALIDATE:
	case PROP_CODEL: case PROP_THROWS:
	  break;

	case PROP_INTERRUPTS: case PROP_BEFORE: case PROP_AFTER:
	  e |= comp_resolvesvc(
	    prop_loc(j.value), c, prop_identifiers(j.value));
	  break;
      }
    }
  }

  if (e) return 2;
  return 0;
}


/* --- dotgen_clkratechk --------------------------------------------------- */

/** Check clock-rate is compatible with all tasks periods
 *
 */
static int
dotgen_clkratechk(comp_s c, prop_s p)
{
  hiter i;
  prop_s tp;
  cval rate, trate;
  double ticks;

  /* get clock rate */
  rate = type_constvalue(prop_value(p));
  if (const_convert(&rate, CST_FLOAT)) assert(0);

  /* check consistency with all tasks */
  for(hash_first(comp_tasks(c), &i); i.current; hash_next(&i)) {
    tp = hash_find(task_props(i.value), prop_strkind(PROP_PERIOD));
    if (tp) {
      trate = type_constvalue(prop_value(tp));
      if (const_convert(&trate, CST_FLOAT)) assert(0);

      ticks = trate.f/rate.f;
      if (fabs(ticks - (int)ticks) > 1e-6) {
	parserror(prop_loc(tp),
		  "task %s period is not a multiple of %s",
		  task_name(i.value), prop_strkind(PROP_CLOCKRATE));
	parsenoerror(prop_loc(p), "%s declared here",
		     prop_strkind(PROP_CLOCKRATE));
      } else
	xwarnx("task %s period is %d ticks", task_name(i.value), (int)ticks);
    }
  }

  return 0;
}


/* --- dotgen_clkratedefault ----------------------------------------------- */

/** Compute default clock-rate as the GCD of all tasks period.
 *
 */
static int
dotgen_clkratedefault(comp_s c)
{
  hiter i;
  prop_s tp;
  cval trate;
  int r, a, b;

  /* the clock-rate must be a divisor of one second */
  unsigned int p = 1000000;

  /* compute the micro-seconds GCD of all tasks period  */
  for(hash_first(comp_tasks(c), &i); i.current; hash_next(&i)) {
    tp = hash_find(task_props(i.value), prop_strkind(PROP_PERIOD));
    if (tp) {
      trate = type_constvalue(prop_value(tp));
      if (const_convert(&trate, CST_FLOAT)) assert(0);

      a = p;	b = trate.f * 1e6;
      while(1) {
	r = a % b;
	if (r == 0) { p = b; break; }

	a = b;	b = r;
      }
    }
  }

  if (p < 1) {
    parserror(comp_loc(c), "unable to compute a default clock-rate");
    return 0;
  }

  if (p < 1000)
    xwarnx("default clock-rate set to %d microseconds", p);
  else
    xwarnx("default clock-rate set to %g miliseconds", p/1000.);

  /* add the property in the component */
  trate.k = CST_FLOAT;
  trate.f = p/1.e6;
  tp = prop_newvalue(comp_loc(c), PROP_CLOCKRATE, trate);

  if (hash_insert(comp_props(c), prop_name(tp), tp, (hrelease_f)prop_destroy)) {
    prop_destroy(tp);
    parserror(comp_loc(c), "unable to set a default clock-rate");
    return 0;
  }

  return 0;
}
