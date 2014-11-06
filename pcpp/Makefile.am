#
# Copyright (c) 2011,2014 LAAS/CNRS
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
	token.c		\
	pragma.c

genom_pcpp_YFLAGS=	-v -d
genom_pcpp_CPPFLAGS=	-I$(top_srcdir)/src
genom_pcpp_LDADD=	../compat/libcompat.a

genom_pcpp-token.o genom_pcpp-cpp.o: genom_pcpp-cpy.h

# regression tests - from vendor pcc
CLEANFILES=	${TESTS}

TESTS=		do-test1 do-test2 do-test3 do-test4 do-test5 do-test6
TESTS+=		do-test7 do-test8 do-test9 do-test10 do-test11
TESTS+=		do-test12 do-test13

EXTRA_DIST=	tests/test1 tests/test2 tests/test3 tests/test4 tests/test5
EXTRA_DIST+=	tests/test6 tests/test7 tests/test8 tests/test9 tests/test10
EXTRA_DIST+=	tests/test11 tests/test12 tests/test13

EXTRA_DIST+=	tests/res1 tests/res2 tests/res3 tests/res4 tests/res5
EXTRA_DIST+=	tests/res6 tests/res7 tests/res8 tests/res9 tests/res10
EXTRA_DIST+=	tests/res11 tests/res12 tests/res13

CLEANFILES+=	run1 run2 run3 run4 run5 run6 run7 run8 run9 run10 run11 run12
CLEANFILES+=	run13

# regression tests - for specific additions
TESTS+=		do-test100 do-test101 do-test102
XFAIL_TESTS=	do-test101 do-test102

EXTRA_DIST+=	tests/test100 tests/test101 tests/test102
EXTRA_DIST+=	tests/res100
EXTRA_DIST+=	tests/x.pc

CLEANFILES+=	run100 data100 run101 run102

do-test%: Makefile
	@echo '#!${SHELL}' >$@
	@echo 'res="$(srcdir)/tests/res$*"' >>$@
	@echo 'in="$(srcdir)/tests/test$*"' >>$@
	@echo 'out="./run$*"' >>$@
	@echo './genom-pcpp <$$in >$$out && cmp $$out $$res' >>$@
	@chmod u+x $@

do-test100: data100
data100:
	echo $@>$@

# running the tests
TESTS_ENVIRONMENT=	export PKG_CONFIG=$(PKG_CONFIG);
TESTS_ENVIRONMENT+=	export PKG_CONFIG_PATH=$(srcdir)/tests;
