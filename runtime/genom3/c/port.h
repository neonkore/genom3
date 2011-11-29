/*
 * Copyright (c) 2011 LAAS/CNRS
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
 *					Anthony Mallet on Tue Oct 18 2011
 */

#ifndef H_GENOM3_C_PORT
#define H_GENOM3_C_PORT

#include <stdint.h>
#include <genom3/c/idlsequence.h>

#ifndef _genom_port_handle_type
# define _genom_port_handle_type
typedef uint32_t genom_port_handle;
#endif /* _genom_port_handle_type */

#ifndef _sequence_genom_port_handle_type
# define _sequence_genom_port_handle_type
typedef struct sequence_genom_port_handle {
  uint32_t _maximum, _length;
  genom_port_handle *_buffer;
  void (*_release)(void *_buffer);
} sequence_genom_port_handle;
#endif /* _sequence_genom_port_handle_type */

#ifndef _genom_port_handle_set_type
# define _genom_port_handle_set_type
typedef sequence_genom_port_handle genom_port_handle_set;
#endif /* _genom_port_handle_set_type */

struct genom_port_pollhandle {
  genom_port_handle fd;		/* port handle */
  uint16_t events;		/* events to look for */
  uint16_t revents;		/* events returned */
};

#define GENOM_PORT_POLLIN	0x1	/* data may be read without blocking */
#define GENOM_PORT_POLLOUT	0x2	/*          written without blocking */

genom_port_handle	genom_port_open(const char *path, int mode);
int			genom_port_close(genom_port_handle h);

ssize_t		genom_port_read(genom_port_handle h, void *buf, size_t size);
ssize_t		genom_port_pread(genom_port_handle h, void *buf, size_t size,
			size_t off);
ssize_t		genom_port_write(genom_port_handle h, const void *buf,
			size_t size);
ssize_t		genom_port_pwrite(genom_port_handle h, void *buf, size_t size,
			size_t off);

int		genom_port_poll(struct genom_port_pollhandle *hs, int nfds,
			int timeout);

#endif /* H_GENOM3_C_PORT */
