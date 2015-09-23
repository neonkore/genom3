/*
 * Copyright (c) 2009-2013,2015 LAAS/CNRS
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
 *                                           Anthony Mallet on Tue Dec 29 2009
 */
#include "acgenom.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <err.h>

#include "genom.h"


/* --- local data ---------------------------------------------------------- */

struct comp_s {
  tloc loc;
  const char *name;
  compkind kind;
  scope_s scope;
  scope_s idsscope;

  hash_s props;
  hash_s tasks;
  hash_s ports;
  hash_s services;
  hash_s remotes;

  comp_s next;
};

tloc		comp_loc(comp_s c) { assert(c); return c->loc; }
const char *	comp_name(comp_s c) { assert(c); return c->name; }
compkind	comp_kind(comp_s c) { assert(c); return c->kind; }
scope_s		comp_scope(comp_s c) { assert(c); return c->scope; }
scope_s		comp_idsscope(comp_s c) { assert(c); return c->idsscope; }
hash_s		comp_props(comp_s c) { assert(c); return c->props; }
hash_s		comp_tasks(comp_s c) { assert(c); return c->tasks; }
hash_s		comp_ports(comp_s c) { assert(c); return c->ports; }
hash_s		comp_services(comp_s c) { assert(c); return c->services; }
hash_s		comp_remotes(comp_s c) { assert(c); return c->remotes; }

static void		comp_dopragma(comp_s c);
static int		comp_extends(tloc l, comp_s c, hash_s e, propkind k);
static int		comp_merge(comp_s c, comp_s m, propkind k);

/** the components and interfaces of a dotgen file */
static comp_s clist = NULL;

/** the current active component */
static comp_s active = NULL;

comp_s		comp_active(void) { return active; }
comp_s		comp_first() { return clist; }
comp_s		comp_next(comp_s c) { assert(c); return c->next; }


/* --- comp_find ----------------------------------------------------------- */

/** Return a component of that name
 */
comp_s
comp_find(const char *name)
{
  comp_s c;
  for(c = clist; c; c = c->next) if (!strcmp(comp_name(c), name)) return c;
  return NULL;
}


/* --- comp_push ----------------------------------------------------------- */

/** create or reopen a component
 */
comp_s
comp_push(tloc l, const char *name, compkind kind)
{
  static const char *stdev[] = COMPONENT_STD_EVENTS;
  comp_s c, t;
  scope_s scomp, spop, sids;
  int i;
  assert(name);

  /* push component scope */
  assert(scope_current() == scope_global());
  scomp = scope_push(l, name, SCOPE_MODULE);
  if (!scomp) return NULL;

  /* get an existing component, or NULL if not yet created */
  c = comp_find(name);
  if (c) {
    if (c->kind != kind) {
      parserror(l, "conflicting types for %s %s", comp_strkind(kind), name);
      parsenoerror(comp_loc(c), " %s %s declared here",
                   comp_strkind(c->kind), name);
      return NULL;
    }

    active = c;
    comp_dopragma(c);
    xwarnx("reopened %s %s", comp_strkind(kind), c->name);
    return c;
  }

  /* create ids scope */
  sids = scope_push(l, prop_strkind(PROP_IDS), SCOPE_STRUCT);
  if (!sids) goto error;
  scope_detach(sids);
  scope_pop();

  /* create new component */
  c = malloc(sizeof(*c));
  if (!c) {
    warnx("memory exhausted, cannot create component");
    goto error;
  }

  c->loc = l;
  c->name = string(name);
  c->kind = kind;
  c->scope = scomp;
  c->idsscope = sids;
  c->props = hash_create(strings(name, " property list", NULL), 2);
  c->tasks = hash_create(strings(name, " tasks", NULL), 1);
  c->ports = hash_create(strings(name, " ports", NULL), 1);
  c->services = hash_create(strings(name, " services", NULL), 2);
  c->remotes = hash_create(strings(name, " remotes", NULL), 1);
  if (!c->tasks || !c->ports || !c->services || !c->remotes) {
    if (c->tasks) hash_destroy(c->tasks, 1);
    if (c->ports) hash_destroy(c->ports, 1);
    if (c->services) hash_destroy(c->services, 1);
    if (c->remotes) hash_destroy(c->remotes, 1);
    free(c);
    goto error;
  }

  /* link component to others */
  c->next = NULL;
  if (!clist) clist = c; else {
    for(t = clist; t->next; t = t->next) /* empty body */;
    t->next = c;
  }

  active = c;
  xwarnx("created %s %s", comp_strkind(kind), c->name);

  /* create std events for regular components (not interfaces) */
  if (kind == COMP_REGULAR)
    for(i=0; i<sizeof(stdev)/sizeof(stdev[0]); i++)
      if (!comp_addevent(l, stdev[i])) goto error;

  /* apply #pragma directives */
  comp_dopragma(c);

  return c;

error:
  spop = scope_pop();
  assert(spop == scomp);
  return NULL;
}


/* --- comp_pop ------------------------------------------------------------ */

comp_s
comp_pop(void)
{
  comp_s c = active;
  scope_s s;
  hiter i;
  int e = 0;
  assert(active);

  /* check tasks */
  for(hash_first(c->tasks, &i); i.current; hash_next(&i))
    e |= task_check(i.value);

  /* check services */
  for(hash_first(c->services, &i); i.current; hash_next(&i))
    e |= service_check(i.value);

  /* pop context */
  s = scope_pop();
  assert(s == c->scope);
  active = NULL;
  return c;
}


/* --- comp_ids ------------------------------------------------------------ */

/** return component's ids
 */
idltype_s
comp_ids(comp_s c)
{
  prop_s p;
  assert(c);
  p = hash_find(c->props, prop_strkind(PROP_IDS));
  return p ? prop_type(p) : NULL;
}


/* --- comp_port ----------------------------------------------------------- */

/** return component's port
 */
port_s
comp_port(comp_s c, const char *name)
{
  assert(c && name);
  return hash_find(c->ports, name);
}


/* --- comp_task ----------------------------------------------------------- */

/** return component's task
 */
task_s
comp_task(comp_s c, const char *name)
{
  assert(c && name);
  return hash_find(c->tasks, name);
}


/* --- comp_service -------------------------------------------------------- */

/** return component's service
 */
service_s
comp_service(comp_s c, const char *name)
{
  assert(c && name);
  return hash_find(c->services, name);
}


/* --- comp_remote --------------------------------------------------------- */

/** return component's remote
 */
remote_s
comp_remote(comp_s c, const char *name)
{
  assert(c && name);
  return hash_find(c->remotes, name);
}


/* --- comp_addprop -------------------------------------------------------- */

/** add a property in a component
 */
int
comp_addprop(tloc l, prop_s p)
{
  comp_s c = comp_active();
  int s;
  assert(c);

  /* check unwanted properties */
  switch(prop_kind(p)) {
    case PROP_DOC: case PROP_IDS: case PROP_VERSION: case PROP_LANG:
    case PROP_EMAIL: case PROP_REQUIRE: case PROP_CODELS_REQUIRE:
    case PROP_CLOCKRATE: case PROP_THROWS:
      /* merge property */
      return prop_merge(comp_props(c), p, 0/*ignore_dup*/);

    case PROP_EXTENDS:
    case PROP_PROVIDES:
    case PROP_USES:
      s = prop_merge(comp_props(c), p, 0/*ignore_dup*/);
      if (!s) s = comp_extends(l, c, prop_hash(p), prop_kind(p));
      return s;
      break;

    case PROP_PERIOD: case PROP_DELAY: case PROP_PRIORITY:
    case PROP_SCHEDULING: case PROP_STACK: case PROP_VALIDATE:
    case PROP_SIMPLE_CODEL: case PROP_FSM_CODEL: case PROP_TASK:
    case PROP_INTERRUPTS: case PROP_BEFORE: case PROP_AFTER:
      parserror(l, "property %s may not be defined for %ss",
                prop_strkind(prop_kind(p)), comp_strkind(c->kind));
      return errno = EINVAL;
  }

  assert(0);
  return errno = EINVAL;
}


/* --- comp_addids --------------------------------------------------------- */

/** add IDS members in a component
 */
idltype_s
comp_addids(tloc l, scope_s s)
{
  comp_s c = comp_active();
  idltype_s ids, t, m;
  hiter it;
  assert(c && s);

  /* create the ids type if needed */
  ids = comp_ids(c);
  if (!ids) {
    assert(scope_current() == c->scope);
    ids = type_newstruct(l, prop_strkind(PROP_IDS), c->idsscope);
    if (!ids) return NULL;

    hash_insert(c->props, prop_strkind(PROP_IDS), prop_newids(l, ids),
                (hrelease_f)prop_destroy);
  }

  /* create new ids members if needed */
  if (s != c->idsscope) {
    scope_set(c->idsscope);
    for(t = scope_firstype(s, &it); t; t = scope_nextype(&it)) {
      if (type_kind(t) != IDL_MEMBER) continue;
      m = scope_findtype(type_membersscope(ids), type_name(t));
      if (m && type_type(m) == type_type(t)) continue;

      m = type_newmember(type_loc(t), type_name(t), type_type(t));
      if (!m) /* error has been recorded/reported already */;
    }
    scope_pop();
    assert(scope_current() == c->scope);
  }

  /* recreate ids type, so that it always appears to be declared at this
   * point. This is important if the ids declares other nested types,
   * because the later have to always appear before the main type. */
  type_renew(ids);
  return ids;
}


/* --- comp_addiev --------------------------------------------------------- */

/** Add an event to a component
 */
idltype_s
comp_addevent(tloc l, const char *name)
{
  comp_s c = comp_active();
  idltype_s e;
  assert(c && name);
  assert(c->scope == scope_current());

  /* reuse if it exists */
  e = type_find(name);
  if (e && type_kind(e) == IDL_EVENT) return e;

  /* create */
  return type_newbasic(l, name, IDL_EVENT);
}


/* --- comp_strkind -------------------------------------------------------- */

/** Return a component kind as a string
 */

const char *
comp_strkind(compkind k)
{
  switch(k) {
    case COMP_REGULAR:		return "component";
    case COMP_IFACE:		return "interface";
  }

  assert(0);
  return NULL;
}


/* --- comp_dopragma ------------------------------------------------------- */

static void
comp_dopragma(comp_s c)
{
  hash_s h;
  hiter i;

  /* apply #pragma require declarations */
  h = dotgen_hrequire();
  if (h && prop_merge_list(c->props, h, 0/*ignore_dup*/))
    parserror(c->loc, "dropping #pragma requires directives for %s %s",
              comp_strkind(c->kind), c->name);

  /* apply #pragma provides declarations */
  if (c->kind == COMP_REGULAR) {
    h = dotgen_hprovide();
    if (h) {
      for(hash_first(h, &i); i.current; hash_next(&i)) {
        if (comp_merge(c, i.value, PROP_PROVIDES))
          parserror(c->loc, "dropping #pragma provides directives for %s %s",
                    comp_strkind(c->kind), c->name);
      }
    }
  }
}


/* --- comp_extends -------------------------------------------------------- */

static int
comp_extends(tloc l, comp_s c, hash_s extends, propkind k)
{
  hiter i;
  int s, e = 0;
  assert(k == PROP_EXTENDS || k == PROP_PROVIDES || k == PROP_USES);

  for(hash_first(extends, &i); i.current; hash_next(&i)) {
    s = comp_merge(c, i.value, k);
    if (s && !e) e = s;
  }

  return e;
}


/* --- comp_merge ---------------------------------------------------------- */

/** Implement 'extends', 'provides' and 'uses'
 */
static int
comp_merge(comp_s c, comp_s m, propkind k)
{
  hiter i;
  int e;
  assert(scope_current() == c->scope && active == c);
  assert(k == PROP_EXTENDS || k == PROP_PROVIDES || k == PROP_USES);

  e = prop_merge_list(c->props, m->props, c->kind != COMP_IFACE/*ignore_dup*/);

  for(hash_first(m->tasks, &i); i.current; hash_next(&i))
    if (!task_clone(i.value) && !e) e = errno;

  for(hash_first(m->ports, &i); i.current; hash_next(&i)) {
    if (!port_clone(i.value, k == PROP_USES /*flipdir*/) && !e) e = errno;
  }

  for(hash_first(m->services, &i); i.current; hash_next(&i)) {
    switch(k) {
      case PROP_EXTENDS: case PROP_PROVIDES:
        if (!service_clone(i.value)) e = errno;
        break;

      case PROP_USES:
        if (!remote_create(service_loc(i.value), i.value))
          e = errno;
        break;

      default: assert(0);
    }
  }

  for(hash_first(m->remotes, &i); i.current; hash_next(&i)) {
    if (!remote_clone(i.value)) e = errno;
  }

  if (!e)
    xwarnx("%s %s %s into %s %s",
           k == PROP_EXTENDS?"extended":(k == PROP_PROVIDES?"provided":"used"),
           comp_strkind(m->kind), m->name,
           comp_strkind(c->kind), c->name);
  return e;
}


/* --- comp_dumpall -------------------------------------------------------- */

/** Dump all components as a dotgen specification.
 */
int
comp_dumpall(FILE *out)
{
  comp_s c;
  for(c = clist; c; c = c->next) {
    comp_dump(c, stdout);
    if (c->next) fputs("\n", out);
  }

  return 0;
}


/* --- comp_dump ----------------------------------------------------------- */

/** Dump component description as a dotgen specification.
 */
int
comp_dump(comp_s c, FILE *out)
{
  const char *indent = "  ";
  const char *brace = " {\n";
  const char *colon = NULL;

  hash_s props;
  hiter i, j;
  citer k;
  cval v;

  fprintf(out, "%s %s", comp_strkind(comp_kind(c)), comp_name(c));

#define poptbrace                                                       \
  do { if (brace) { fputs(brace, out); brace = NULL; } } while(0)

  props = comp_props(c);
  if (props)
    for(hash_first(props, &i); i.current; hash_next(&i)) {
      switch(prop_kind(i.value)) {
	case PROP_DOC:
        case PROP_VERSION:
	case PROP_LANG:
        case PROP_EMAIL:
          poptbrace;
          fprintf(out, "%s%s\t\"%s\";\n", indent,
                  prop_strkind(prop_kind(i.value)), prop_text(i.value));
          break;

        case PROP_REQUIRE:
	case PROP_CODELS_REQUIRE:
          poptbrace;
          fprintf(out, "%s%s\n", indent, prop_strkind(prop_kind(i.value)));
          colon = NULL;
          for(clist_first(prop_list(i.value), &k); k.current; clist_next(&k)) {
            if (colon) fputs(colon, out);
            fprintf(out, "%1$s%1$s\"%2$s\"", indent, k.value->s);
            colon = ",\n";
          }
          fputs(";\n", out);
          break;

        case PROP_THROWS:
          poptbrace;
          fprintf(out, "%s%s\n", indent, prop_strkind(prop_kind(i.value)));
          colon = NULL;
          for(hash_first(prop_hash(i.value), &j); j.current; hash_next(&j)) {
            if (colon) fputs(colon, out);
            fprintf(out, "%1$s%1$s\"%2$s\"", indent, j.key);
            colon = ",\n";
          }
          fputs(";\n", out);
          break;

        case PROP_CLOCKRATE:
          poptbrace;
          v = type_constvalue(prop_value(i.value));
          if (const_convert(&v, CST_FLOAT)) break;
          fprintf(out, "%s%s\t%gs\n", indent,
                  prop_strkind(prop_kind(i.value)), v.f);
	  break;

        default: break;
      }
    }
#undef poptbrace

  fprintf(out, "%s;\n", brace?"":"}");
  return 0;
}
