/*
 * Copyright (c) 2014-2015,2017 LAAS/CNRS
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
 *					Anthony Mallet on Sat May 31 2014
 */
#ifndef H_GENOM3_C_CONTEXT
#define H_GENOM3_C_CONTEXT

#include <stddef.h>

#include "genom3/c/event.h"

/*
 * Definition of codels and clients context.
 *
 * genom_context_data is implementation dependent and may be provided by a
 * template to store additional data.
 */
typedef const struct genom_context_iface *genom_context;

struct genom_context_data;

struct genom_context_iface {
  /* exception throwing interface */
  genom_event (*raise)(genom_event ex, void *detail, size_t size,
                               genom_context self);
  const void *(*raised)(genom_event *ex, genom_context self);

  /* implementation specific data */
  struct genom_context_data *data;
};

#endif /* H_GENOM3_C_CONTEXT */
