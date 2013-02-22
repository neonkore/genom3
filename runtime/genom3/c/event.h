/*
 * Copyright (c) 2013 LAAS/CNRS
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
 *					Anthony Mallet on Fri Feb 22 2013
 */

#ifndef H_GENOM3_C_EVENT
#define H_GENOM3_C_EVENT

#include <assert.h>
#include <stddef.h>
#include <stdint.h>

/*
 * Definition of codel transitions and exceptions.
 *
 * Events are declared as global weak symbols to guarantee the unicity of their
 * value among different translation units defining different sets of events,
 * and make for instance the == operator work on such objects.
 *
 * The support for a "weak" attribute is not mandatory. An implementation can
 * fallback to e.g. strings and the use strcmp() instead of == to compare
 * events. (note that this is the CORBA approach).
 *
 * These definitions can be #ifdef'ed depending on the compiler.
 */
typedef const char *genom_event;

#define genom_weak __attribute__((weak))

#ifdef __cplusplus
extern "C" {
#endif
  genom_event	genom_throw(genom_event ex, void *detail, size_t size);
  const void *	genom_thrown(genom_event *ex);
#ifdef __cplusplus
}
#endif

/* no exception symbol */
genom_event genom_weak genom_ok = NULL;

/* unknown exception */
const char genom_weak genom_unkex_id[] = "::genom::unkex";
typedef struct genom_unkex_detail { char what[128]; } genom_unkex_detail;

static inline genom_event
genom_unkex(genom_unkex_detail *d)
{
  genom_throw(genom_unkex_id, d, sizeof(*d));
  return genom_unkex_id;
}

/* generic system error */
const char genom_weak genom_syserr_id[] = "::genom::syserr";
typedef struct genom_syserr_detail { uint32_t code; } genom_syserr_detail;

static inline genom_event
genom_syserr(genom_syserr_detail *d)
{
  genom_throw(genom_syserr_id, d, sizeof(*d));
  return genom_syserr_id;
}

#endif /* H_GENOM3_C_EVENT */
