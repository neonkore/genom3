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
 *					Anthony Mallet on Fri Feb 28 2014
 */

#ifndef H_GENOM3_CXX_IDLOPTIONAL
#define H_GENOM3_CXX_IDLOPTIONAL

namespace genom {
  template <typename T>
  struct optional {
    // types:
    typedef T                                     value_type;
    typedef value_type&                           reference;
    typedef const value_type&                     const_reference;

    bool _present;
    value_type _value;
  };
};

#endif /* H_GENOM3_CXX_IDLOPTIONAL */
