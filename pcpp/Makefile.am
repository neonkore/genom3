#
# Copyright (c) 2011 LAAS/CNRS
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
#                                           Anthony Mallet on Thu Jun 23 2011
#

libexec_PROGRAMS=\
	genom-pcpp

genom_pcpp_SOURCES=\
	cpp.c		\
	cpp.h		\
	cpy.y		\
	token.c

genom_pcpp_YFLAGS=	-v -d
genom_pcpp_CPPFLAGS=	-I$(top_srcdir)/src
genom_pcpp_LDADD=	../compat/libcompat.a

genom_pcpp-token.o genom_pcpp-cpp.o: genom_pcpp-cpy.h

# regression tests
TESTS=	tests/test1 tests/test2 tests/test3 tests/test4	tests/test5 tests/test6
TESTS+=	tests/test7 tests/test8 tests/test9 tests/test10 tests/test11
TESTS+=	tests/test12 tests/test13

EXTRA_DIST=	${TESTS}
EXTRA_DIST+=	tests/res1 tests/res2 tests/res3 tests/res4 tests/res5
EXTRA_DIST+=	tests/res6 tests/res7 tests/res8 tests/res9 tests/res10
EXTRA_DIST+=	tests/res11 tests/res12 tests/res13

CLEANFILES=	run1 run2 run3 run4 run5 run6 run7 run8 run9 run10 run11 run12
CLEANFILES+=	run13

TEST_IN=	$$0
TEST_OUT=	./run$${0\#\#*test}
TEST_RES=	$${0%test*}res$${0\#\#*test}
TEST_RUN=	./genom-pcpp <${TEST_IN} >${TEST_OUT}
TEST_CMP=	cmp ${TEST_OUT} ${TEST_RES}

TESTS_ENVIRONMENT=	$(SHELL) -c '${TEST_RUN} && ${TEST_CMP}'
