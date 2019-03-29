/*
 * Copyright (c) 2010-2012,2018-2019 LAAS/CNRS
 * All rights reserved.
 *
 * Redistribution and use  in source  and binary  forms,  with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *   1. Redistributions of  source  code must retain the  above copyright
 *      notice, this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice,  this list of  conditions and the following disclaimer in
 *      the  documentation  and/or  other   materials provided  with  the
 *      distribution.
 *
 * THIS  SOFTWARE IS PROVIDED BY  THE  COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND  ANY  EXPRESS OR IMPLIED  WARRANTIES,  INCLUDING,  BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES  OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR  PURPOSE ARE DISCLAIMED. IN  NO EVENT SHALL THE COPYRIGHT
 * HOLDERS OR      CONTRIBUTORS  BE LIABLE FOR   ANY    DIRECT, INDIRECT,
 * INCIDENTAL,  SPECIAL,  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF  SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN  CONTRACT, STRICT LIABILITY, OR
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE   OF THIS SOFTWARE, EVEN   IF ADVISED OF   THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 *					Anthony Mallet on Thu Aug 19 2010
 */

#ifndef H_GENOM3_C_IDLSEQUENCE
#define H_GENOM3_C_IDLSEQUENCE

#include <errno.h>
#include <stdint.h>

#ifndef __cplusplus
# include <stdlib.h>
# include <string.h>
# define genom_sequence_type void
#else
# include <cstdlib>
# include <cstring>

/* C++ needs a template function because it cannot cast from void * back to the
 * actual sequence element type */
template<typename genom_sequence_type>
#endif
static __inline__ genom_sequence_type *
_genom_resize_buffer(uint32_t length, size_t esize,
                     uint32_t *_length, uint32_t *_maximum,
                     genom_sequence_type *_buffer,
                     void (**_release)(void *))
{
  genom_sequence_type *buffer;

  if (!length) {
    if (*_release && _buffer) (*_release)(_buffer);
    *_maximum = *_length = 0;
    *_release = NULL;
    return NULL;
  }

  if (*_release == free && _buffer) {
    buffer = (genom_sequence_type *)realloc(_buffer, length * esize);
    if (!buffer) return NULL;

    *_maximum = length;
    if (length < *_length)
      *_length = length;
    else if (length > *_length)
      memset((char *)buffer + *_length * esize, 0, (length - *_length) * esize);

    return buffer;
  }

  buffer = (genom_sequence_type *)calloc(length, esize);
  if (!buffer) return NULL;

  *_maximum = length;
  if (length < *_length) *_length = length;
  if (*_length > 0 && _buffer) memcpy(buffer, _buffer, *_length * esize);
  if (*_release && _buffer) (*_release)(_buffer);
  *_release = free;

  return buffer;
}

#ifndef __cplusplus
# undef genom_sequence_type
#endif

#define genom_sequence_reserve(sequence, length)                        \
  (((sequence)->_buffer = _genom_resize_buffer(				\
    length, sizeof(*(sequence)->_buffer),				\
    &(sequence)->_length, &(sequence)->_maximum,			\
    (sequence)->_buffer, &(sequence)->_release))?0:((length)!=0?-1:0))

#endif /* H_GENOM3_C_IDLSEQUENCE */
