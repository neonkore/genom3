#
# Copyright (c) 2009-2013 LAAS/CNRS
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
#                                            Anthony Mallet on Mon Dec 21 2009
#
TESTS=	basics/cpp.test
TESTS+=	basics/parser.test
TESTS+=	basics/require.test
TESTS+=	basics/extends.test
TESTS+=	basics/provides.test
TESTS+=	basics/interrupts.test
TESTS+=	basics/initializer.test

TESTS+=	templates/genom.test

if WITH_TCL
  TESTS+=	templates/mappings.test
  TESTS+=	templates/skeleton.test

  TESTS+=	tcl-engine/engine.test
  TESTS+=	tcl-engine/merge.test
  TESTS+=	tcl-engine/dotgen.test
  TESTS+=	tcl-engine/type.test
  TESTS+=	tcl-engine/service.test
  TESTS+=	tcl-engine/remote.test
  TESTS+=	tcl-engine/codel.test
  TESTS+=	tcl-engine/component.test
  TESTS+=	tcl-engine/template.test
  TESTS+=	tcl-engine/language.test
  TESTS+=	tcl-engine/language-c.test
  TESTS+=	tcl-engine/language-c++.test
endif

TESTS_ENVIRONMENT=	${SHELL} -e

EXTRA_DIST=		${TESTS}
EXTRA_DIST+=		test.sh
EXTRA_DIST+=		datatypes.gen
EXTRA_DIST+=		component.gen


# export useful variables to the test scripts
export AWK
export CPP=${top_builddir}/pcpp/genom-pcpp
export CC
export CXX
export PKG_CONFIG
export GENOM_BIN=${top_builddir}/src/genom3 \
		-s ${top_builddir}/engine -t ${top_srcdir}/templates

export PACKAGE_VERSION
export top_srcdir


# check component generation
generate: Makefile
	@exec >$@;						\
	${ECHO} '#!${SHELL}';					\
	${ECHO} ${top_builddir}/src/genom3 -v 			\
			-t ${srcdir} -s ${top_builddir}/engine	\
			template ${srcdir}/$@.gen
	@chmod u+x $@
