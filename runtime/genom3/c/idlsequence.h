/*
 * Copyright (c) 2010-2012 LAAS/CNRS
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
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static __inline__ int
_genom3_resize_buffer(uint32_t length, size_t esize,
		      uint32_t *_length, uint32_t *_maximum, void **_buffer,
		      void (**_release)(void *))
{
  void *buffer;

  if (*_release == free && *_buffer) {
    buffer = realloc(*_buffer, length * esize);
    if (!buffer) return -1;

    *_maximum = length;
    if (length < *_length)
      *_length = length;
    else if (length > *_length)
      memset((char *)buffer + *_length * esize, 0, (length - *_length) * esize);

    *_buffer = buffer;
    return 0;
  }

  buffer = calloc(length, esize);
  if (!buffer) return -1;

  *_maximum = length;
  if (length < *_length) *_length = length;
  if (*_length > 0 && *_buffer) memcpy(buffer, *_buffer, *_length * esize);
  if (*_release && *_buffer) (*_release)(*_buffer);
  *_buffer = buffer;
  *_release = free;

  return 0;
}

#ifdef __cplusplus
}
#endif

#define genom3_sequence_resize(sequence, length)			\
  _genom3_resize_buffer(						\
    length, sizeof(*(sequence)->_buffer),				\
    &(sequence)->_length, &(sequence)->_maximum,			\
    (void **)&(sequence)->_buffer, &(sequence)->_release)

#endif /* H_GENOM3_C_IDLSEQUENCE */
