/*
 * Copyright (c) 2011-2012 LAAS/CNRS
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

#include <errno.h>
#include <stdint.h>
#include <genom3/c/idlsequence.h>

struct genom_port_handle;
struct sequence_genom_port_handle;

enum {
  GENOM_PORT_CLOSED =	0x0,	/* closed handle */
  GENOM_PORT_OPEN =	0x1,	/* open handle */
  GENOM_PORT_IN =	0x10,	/* inport handle */
  GENOM_PORT_OUT =	0x20	/* outport handle */
};

struct genom_port_pollhandle {
  struct genom_port_handle *h;	/**< port handle */
  uint16_t events;		/**< events to look for */
  uint16_t revents;		/**< events returned */
};

#define GENOM_PORT_POLLIN	0x1	/* data may be read without blocking */
#define GENOM_PORT_POLLOUT	0x2	/*          written without blocking */


/* --- port handle codel API ----------------------------------------------- */

/*
 * Define weak aliases for the genom_port_* functions when possible. This is
 * done for two reasons, the first one being the most important:
 *
 *  - Force the genom_port_* symbols to appear in the codels DSO, even if the
 *    codels do not use the functions. This ensures that codels DSO can always
 *    be recompiled without re-linking the final executable. (otherwise an
 *    executable linked with codels not using the API initially must be
 *    recompiled whenever the codels change and decide to make use of the API).
 *
 *  - Provide a nice fallback if the codels are linked outside a genom module
 *    or with a template not implementing the full port handle API. This is
 *    mostly cosmetic, though.
 *
 * If weak aliases are not available, this code still works as expected but
 * without the features mentionned above: a component may have to be relinked
 * if codels use/do not use the port API. (stubs are not defined in this case,
 * to prevent 'defined but not used' warnings).
 *
 * The macro GENOM_PORT_IMPL_ must be defined by the template implementation,
 * so that the API symbols are defined as regular (strong) ones.
 */

#ifndef GENOM_PORT_IMPL_
# if defined(__GNUC__)
#  define GENOM_PORT_WANTS_STUB_
#  define genom_weak_alias(x) __attribute__ ((weak, alias( #x )))
# else
#  define genom_weak_alias(x)
# endif
#else
#  define genom_weak_alias(x)
#endif

#ifdef __cplusplus
extern "C" {
#endif

  genom_weak_alias(genom_port_open_stub)
  int	genom_port_open(struct genom_port_handle *h);
  genom_weak_alias(genom_port_set_open_stub)
  int	genom_port_set_open(struct sequence_genom_port_handle *hs,
		uint32_t index, const char *path);
  genom_weak_alias(genom_port_close_stub)
  int	genom_port_close(struct genom_port_handle *h);
  genom_weak_alias(genom_port_set_close_stub)
  int	genom_port_set_close(struct sequence_genom_port_handle *hs,
		uint32_t index);
  genom_weak_alias(genom_port_status_stub)
  int	genom_port_status(struct genom_port_handle *h);
  genom_weak_alias(genom_port_set_status_stub)
  int	genom_port_set_status(struct sequence_genom_port_handle *hs,
		uint32_t index);

  genom_weak_alias(genom_port_rdlock_stub)
  const void *	genom_port_rdlock(const struct genom_port_handle *h);
  genom_weak_alias(genom_port_set_rdlock_stub)
  const void *	genom_port_set_rdlock(
			const struct sequence_genom_port_handle *hs,
			uint32_t index);
  genom_weak_alias(genom_port_wrlock_stub)
  void *	genom_port_wrlock(const struct genom_port_handle *h);
  genom_weak_alias(genom_port_set_wrlock_stub)
  void *	genom_port_set_wrlock(
			const struct sequence_genom_port_handle *hs,
			uint32_t index);
  genom_weak_alias(genom_port_unlock_stub)
  int		genom_port_unlock(const struct genom_port_handle *h);
  genom_weak_alias(genom_port_set_unlock_stub)
  int		genom_port_set_unlock(
			const struct sequence_genom_port_handle *hs,
			uint32_t index);

  genom_weak_alias(genom_port_poll_stub)
  int	genom_port_poll(struct genom_port_pollhandle *hs, int nfds,
		int timeout);
  genom_weak_alias(genom_port_set_poll_stub)
  int	genom_port_set_poll(struct sequence_genom_port_handle *hs,
		uint32_t index, int nfds, int timeout);

#ifdef __cplusplus
}
#endif


/* --- stub implementation ------------------------------------------------- */

#ifdef GENOM_PORT_WANTS_STUB_

#ifdef __cplusplus
extern "C" {
#endif

  static int
  genom_port_open_stub(struct genom_port_handle *h)
  {
    return ENXIO;
  }

  static int
  genom_port_set_open_stub(struct sequence_genom_port_handle *hs,
                           uint32_t index, const char *path)
  {
    return ENXIO;
  }

  static int
  genom_port_close_stub(struct genom_port_handle *h)
  {
    return ENXIO;
  }

  static int
  genom_port_set_close_stub(struct sequence_genom_port_handle *hs,
                            uint32_t index)
  {
    return ENXIO;
  }

  static int
  genom_port_status_stub(struct genom_port_handle *h)
  {
    return GENOM_PORT_CLOSED;
  }

  static int
  genom_port_set_status_stub(struct sequence_genom_port_handle *hs,
                             uint32_t index)
  {
    return GENOM_PORT_CLOSED;
  }

  static const void *
  genom_port_rdlock_stub(const struct genom_port_handle *h)
  {
    return NULL;
  }

  static const void *
  genom_port_set_rdlock_stub(const struct sequence_genom_port_handle *hs,
                             uint32_t index)
  {
    return NULL;
  }

  static void *
  genom_port_wrlock_stub(const struct genom_port_handle *h)
  {
    return NULL;
  }

  static void *
  genom_port_set_wrlock_stub(const struct sequence_genom_port_handle *hs,
                             uint32_t index)
  {
    return NULL;
  }

  static int
  genom_port_unlock_stub(const struct genom_port_handle *h)
  {
    return ENXIO;
  }

  static int
  genom_port_set_unlock_stub(const struct sequence_genom_port_handle *hs,
                             uint32_t index)
  {
    return ENXIO;
  }

  static int
  genom_port_poll_stub(struct genom_port_pollhandle *hs, int nfds,
                       int timeout)
  {
    return ENXIO;
  }

  static int
  genom_port_set_poll_stub(struct sequence_genom_port_handle *hs,
                           uint32_t index, int nfds, int timeout)
  {
    return ENXIO;
  }

#ifdef __cplusplus
}
#endif

#undef GENOM_PORT_WANTS_STUB_
#endif /* GENOM_PORT_WANTS_STUB_ */

#endif /* H_GENOM3_C_PORT */
