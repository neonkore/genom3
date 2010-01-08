#
# Copyright (c) 2010 LAAS/CNRS
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
#                                            Anthony Mallet on Fri Jan  8 2010
#

systcldir=$(sysdir)/tcl

dist_systcl_DATA=	\
	options.tcl	\
	engine.tcl

nodist_systcl_DATA=	\
	version.tcl	\
	pkgIndex.tcl

BUILT_SOURCES=version.tcl pkgIndex.tcl

version.tcl: Makefile
	exec >$@;						\
	$(ECHO) '# $(PACKAGE_STRING) system files';		\
	$(ECHO) 'package provide'				\
		'$(PACKAGE_NAME)-sys $(PACKAGE_VERSION)';

pkgIndex.tcl: Makefile
	exec >$@; l=;						\
	for s in $(dist_systcl_DATA); do			\
		 l="$$l -source $$s";				\
		test -f $$s || $(LN_S) $(srcdir)/$$s $$s;	\
	done;							\
	$(ECHO) puts [pkg::create -name $(PACKAGE_NAME)-sys	\
		-version $(PACKAGE_VERSION)			\
		-source version.tcl				\
		$$l]						\
	| $(TCLSH)
