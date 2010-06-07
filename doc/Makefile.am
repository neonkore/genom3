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
#                                            Anthony Mallet on Sat May  8 2010
#

dist_doc_DATA=\
	genom.pdf		\
	genom.html		\
	genom.txt

basesrcs=\
	tex/genom.tex		\
	tex/tcl-engine.tex	\
				\
	dotgen-yacc.tex		\
				\
	engcmd-template.tex	\
	engcmd-engine.tex	\
	engcmd-dotgen.tex	\
	engcmd-language.tex	\
	engcmd-autoconf.tex

pdfsrcs=\
	$(basesrcs)		\
	sty/genom.sty

htmlsrcs=\
	$(basesrcs)		\
	sty/genom.tth		\
	sty/genom.header.html	\
	sty/genom.footer.html

txtsrcs=\
	$(basesrcs)

EXTRA_DIST=	$(pdfsrcs) $(htmlsrcs) $(txtsrcs)
EXTRA_DIST+=	docgen.awk

if REGEN_DOC_YACC
dotgen-yacc.tex: $(top_srcdir)/src/dotgen.y
	$(Y2L) -O2 $< | $(SED) 's/\#/\\\#/g'  >$@

if REGEN_DOC_PDF

engcmd-template.tex: $(srcdir)/docgen.awk
engcmd-template.tex: $(top_srcdir)/engine/tcl/template.tcl
	$(AWK) -f $(srcdir)/docgen.awk $< >$@

engcmd-engine.tex: $(srcdir)/docgen.awk
engcmd-engine.tex: $(top_srcdir)/engine/tcl/engine.tcl
	$(AWK) -f $(srcdir)/docgen.awk $< >$@

engcmd-dotgen.tex: $(srcdir)/docgen.awk
engcmd-dotgen.tex: $(top_srcdir)/engine/tcl/dotgen.c
	$(AWK) -f $(srcdir)/docgen.awk $< >$@

engcmd-language.tex: $(srcdir)/docgen.awk
engcmd-language.tex: $(top_srcdir)/engine/tcl/language.tcl
	$(AWK) -f $(srcdir)/docgen.awk $< >$@

engcmd-autoconf.tex: $(srcdir)/docgen.awk
engcmd-autoconf.tex: $(top_srcdir)/engine/tcl/buildenv-autoconf.tcl
	$(AWK) -f $(srcdir)/docgen.awk $< >$@

genom.pdf: Makefile $(pdfsrcs)
genom.pdf: tex/genom.tex
	export TEXINPUTS=$(srcdir)/tex:$(srcdir)/sty:$(srcdir)::;	\
	$(PDFLATEX) $< &&						\
	$(PDFLATEX) $<

if REGEN_DOC_HTML

genom.html: Makefile $(htmlsrcs)
genom.html: tex/genom.tex
	cat < $(srcdir)/sty/genom.header.html > $@
	$(TTH) -w2 -r -Lgenom -p$(srcdir)/sty:$(srcdir)/tex < $< |	\
		sed -e "s/@date@/`date '+%B %e, %Y'`/" >> $@
	cat < $(srcdir)/sty/genom.footer.html >> $@

if REGEN_DOC_TXT

genom.txt: Makefile $(txtsrcs)
genom.txt: genom.html
	$(W3M) -dump -cols 80 $< >$@

endif REGEN_DOC_TXT
endif REGEN_DOC_HTML
endif REGEN_DOC_PDF
endif REGEN_DOC_YACC
