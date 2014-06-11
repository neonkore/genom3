/*
 * Copyright (c) 2013-2014 LAAS/CNRS
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
 *					Anthony Mallet on Thu Jun 12 2014
 */
#ifndef H_GENOM3_C_EXCEPTION
#define H_GENOM3_C_EXCEPTION

#include <stdint.h>

#include "genom3/c/event.h"
#include "genom3/c/context.h"

/*
 * Standard codel transitions and exceptions.
 */

/* no exception symbol */
genom_event genom_weak genom_ok = NULL;

/* exception while throwing an exception or other unrecoverable errors */
const char genom_extern_weak genom_fatal[] = "::genom::fatal";

/* unknown exception */
const char genom_extern_weak genom_unkex_id[] = "::genom::unkex";
typedef struct genom_unkex_detail { char what[128]; } genom_unkex_detail;

static inline genom_event
genom_unkex(genom_unkex_detail *d, genom_context self)
{
  return self->raise(genom_unkex_id, d, sizeof(*d), self);
}

/* generic system error */
const char genom_extern_weak genom_syserr_id[] = "::genom::syserr";
typedef struct genom_syserr_detail { int32_t code; } genom_syserr_detail;

static inline genom_event
genom_syserr(genom_syserr_detail *d, genom_context self)
{
  return self->raise(genom_syserr_id, d, sizeof(*d), self);
}

#endif /* H_GENOM3_C_EXCEPTION */
