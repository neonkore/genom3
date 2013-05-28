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

#ifndef H_GENOM3_CXX_EVENT
#define H_GENOM3_CXX_EVENT

#include <stdexcept>

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
 * In C++, a name of file scope that is explicitly declared const, and not
 * explicitly declared extern, has internal linkage, while in C it would have
 * external linkage (C++03 Standard Annex C Compatibility C.1.2 Clause 3: basic
 * concepts).
 *
 * These definitions can be #ifdef'ed depending on the compiler.
 */
namespace genom {
  typedef const char *event;

#define genom_weak __attribute__((weak))
#define genom_extern_weak extern genom_weak

  /* no exception symbol */
  event genom_weak ok = NULL;

  /* generic genom exception */
  struct exception : public std::runtime_error {};

  /* unknown exception */
  const char genom_extern_weak unkex_id[] = "::genom::unkex";
  struct unkex_detail { char what[128]; };
  struct unkex : public exception {
    unkex_detail detail;
    const char *what() { return unkex_id; };
  };

  /* generic system error */
  const char genom_extern_weak syserr_id[] = "::genom::syserr";
  struct syserr_detail { uint32_t code; };
  struct syserr : public exception {
    syserr_detail detail;
    const char *what() { return syserr_id; };
  };
};

#endif /* H_GENOM3_CXX_EVENT */
