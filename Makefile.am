#
# Copyright (c) 2009-2010 LAAS/CNRS
# All rights reserved.
#
# Permission to use, copy, modify, and distribute this software for any purpose
# with or without   fee is hereby granted, provided   that the above  copyright
# notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
# REGARD TO THIS  SOFTWARE INCLUDING ALL  IMPLIED WARRANTIES OF MERCHANTABILITY
# AND FITNESS. IN NO EVENT SHALL THE AUTHOR  BE LIABLE FOR ANY SPECIAL, DIRECT,
# INDIRECT, OR CONSEQUENTIAL DAMAGES OR  ANY DAMAGES WHATSOEVER RESULTING  FROM
# LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
# OTHER TORTIOUS ACTION,   ARISING OUT OF OR IN    CONNECTION WITH THE USE   OR
# PERFORMANCE OF THIS SOFTWARE.
#
#                                             Anthony Mallet on Wed Dec 16 2009
#
ACLOCAL_AMFLAGS = -I autoconf

if WITH_TCL
  TCLDIR=\
	engine/tcl		\
	templates/interactive	\
	templates/example	\
	templates/libcodels	\
	templates/mappings	\
	templates/skeleton
endif

SUBDIRS=\
	src			\
	$(TCLDIR)		\
	runtime/genom3/c	\
	runtime/genom3/c++	\
	doc			\
	regress

EXTRA_DIST=\
	compat/getline.c	\
	compat/strlcat.c	\
	compat/strlcpy.c


# distcheck configuration
DISTCHECK_CONFIGURE_FLAGS=	CFLAGS='-Wall -Werror'
DISTCHECK_CONFIGURE_FLAGS+=	--enable-docgen
if WITH_TCL
  DISTCHECK_CONFIGURE_FLAGS+=	--with-tcl="$(TCL_BIN_DIR)"
endif

# message
install-data-hook:
	@echo '=============================================================='
	@echo '    Installation complete.'
	@echo ''
	@echo '    Documentation is available in'
	@echo '    $(docdir)'
	@echo '=============================================================='
