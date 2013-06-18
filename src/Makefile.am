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
#                                            Anthony Mallet on Wed Dec 16 2009
#

bin_PROGRAMS=\
	genom3

genom3_SOURCES=\
	genom.h		\
	options.txt	\
	cpp.c		\
	hash.c		\
	property.c	\
	component.c	\
	task.c		\
	service.c	\
	port.c		\
	codel.c		\
	parameter.c	\
	initializer.c	\
	const.c		\
	scope.c		\
	declarator.c	\
	idltype.c	\
	postproc.c	\
	string.c	\
			\
	engine.c	\
	engine-tcl.c

nodist_genom3_SOURCES=\
	dotgen.y	\
	dotgen.l

genom3_GRAMMAR=\
	dotgen/main.y		\
	dotgen/component.y	\
	dotgen/interface.y	\
	dotgen/ids.y		\
	dotgen/task.y		\
	dotgen/port.y		\
	dotgen/service.y	\
	dotgen/codel.y		\
	dotgen/parameter.y	\
	dotgen/idlscope.y	\
	dotgen/idltype.y	\
	dotgen/idldecl.y	\
	dotgen/expr.y

genom3_YFLAGS= -v -d
genom3_CFLAGS = $(PTHREAD_CFLAGS)
genom3_LDADD = ../compat/libcompat.a libmain.a $(PTHREAD_LIBS)
genom3_LDFLAGS=	-export-dynamic

# emulate per-file CPPFLAGS
noinst_LIBRARIES = libmain.a
libmain_a_SOURCES = main.c
libmain_a_CPPFLAGS = -DCPPPATH='"$(cpppath)"'
libmain_a_CPPFLAGS += -DSITETMPLPATH='"$(sitetmpldir)"'
libmain_a_CPPFLAGS += -DTMPLDIR='"$(tmpldir)"'
libmain_a_CPPFLAGS += -DSYSDIR='"$(sysdir)"'


# generate concatenated grammar source
EXTRA_DIST=	$(genom3_GRAMMAR) dotgen.l
BUILT_SOURCES=	dotgen.y genom3-dotgen.h
CLEANFILES=	dotgen.y
CLEANFILES+=	dotgen.c genom3-dotgen.c genom3-dotgen.h genom3-dotgen.output

dotgen.y: Makefile $(genom3_GRAMMAR)
dotgen.y: dotgen/main.y
	$(AWK) '							\
	  /^%include/ {							\
	    while((getline line < ("$(srcdir)/" $$2))>0) print line;	\
	    close("$(srcdir)/" $$2); next;				\
	  }								\
	  { print }' $< >$@


# generate options strings and usage doc
BUILT_SOURCES+=	options.c
CLEANFILES+=	options.c

options.c: Makefile $(top_srcdir)/doc/optgen.awk
options.c: options.txt
	$(AWK) -f $(top_srcdir)/doc/optgen.awk -- -c $< >$@

# install a versioned copy of genom3
install-exec-hook:
	cd $(DESTDIR)$(bindir) &&				\
	mv -f genom3$(EXEEXT) $(GENOM_TOOL)$(EXEEXT) &&		\
	$(LN_S) $(GENOM_TOOL)$(EXEEXT) genom3$(EXEEXT)

uninstall-hook:
	rm -f $(DESTDIR)$(bindir)/$(GENOM_TOOL)
