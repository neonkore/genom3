#
# Copyright (c) 2009 LAAS/CNRS
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

check_PROGRAMS=\
	preprocess	\
	datatypes

TESTS=		$(check_PROGRAMS)


# check cpp - both with and without -r (for default include path)
preprocess: Makefile
	@exec >$@;						\
	${ECHO} '#!${SHELL}';					\
	${ECHO} ${top_builddir}/src/genom3 -r			\
			-D CMDLINE_MACRO=world	 		\
			-E regress ${srcdir}/$@.gen "|"		\
		grep "'hello ${PACKAGE_VERSION} world'";	\
	${ECHO} ${top_builddir}/src/genom3			\
			-DCMDLINE_MACRO=me	 		\
			-E regress ${srcdir}/$@.gen "|"		\
		grep "'hello ${PACKAGE_VERSION} me'"
	@chmod u+x $@


# check data types parsing
datatypes: Makefile
	@exec >$@;						\
	${ECHO} '#!${SHELL}';					\
	${ECHO} ${top_builddir}/src/genom3 			\
			regress ${srcdir}/$@.gen
	@chmod u+x $@
