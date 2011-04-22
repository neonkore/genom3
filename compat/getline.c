/* $NetBSD: getdelim.c,v 1.3 2009/07/14 18:29:41 roy Exp $ */

/*
 * Copyright (c) 2009 The NetBSD Foundation, Inc.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Roy Marples. It has been further adapted to use fgets() instead of
 * internal stdio FILE structure details to provide a generic replacement
 * function when a system getline() is missing.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <sys/param.h>

#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* Minimum buffer size we create. */
#define MINBUF	128

ssize_t
getline(char **__restrict buf, size_t *__restrict buflen, FILE *__restrict fp)
{
	char *p;
	size_t len, off, newlen;
	char *newb;

	assert(fp != NULL);

	if (buf == NULL || buflen == NULL) {
		errno = EINVAL;
		return -1;
	}

	/* If buf is NULL, we allocate MINBUF chars */
	if (*buf == NULL) {
		*buf = malloc(MINBUF);
		if (*buf == NULL)
			goto error;
		*buflen = MINBUF;
	}

	off = 0;
	for (;;) {
		/* If the input buffer is empty, refill it */
		if (fgets(*buf + off, *buflen - off, fp) == NULL) {
			/* POSIX requires we return -1 on EOF */
			if (off == 0)
				goto error;
			break;
		}

		/* Scan through looking for the separator */
		p = memchr(*buf + off, '\n', *buflen - off);
		if (p == NULL)
			len = *buflen - off;
		else
			len = (p - ( *buf + off )) + 1;

		if (p == NULL) {
			newlen = (*buflen) << 1;
			/* Ensure that the resultant buffer length fits in
			 * ssize_t */
			if (newlen > (size_t)SSIZE_MAX) {
				errno = EOVERFLOW;
				goto error;
			}

			newb = realloc(*buf, newlen);
			if (newb == NULL)
				goto error;
			*buf = newb;
			*buflen = newlen;
		}

		off += len;
		if (p != NULL)
			break;
	}
	if (*buf != NULL)
		*(*buf + off) = '\0';
	return off;

error:
	return -1;
}
