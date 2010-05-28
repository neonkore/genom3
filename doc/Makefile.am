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
				\
	engcmd-dotgen.tex

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

genom.pdf: Makefile $(pdfsrcs)
genom.pdf: tex/genom.tex
	export TEXINPUTS=$(srcdir)/tex:$(srcdir)/sty::;		\
	$(PDFLATEX) $<;						\
	$(PDFLATEX) $<

genom.html: Makefile $(htmlsrcs)
genom.html: tex/genom.tex
	cat < $(srcdir)/sty/genom.header.html > $@
	$(TTH) -w2 -r -Lgenom -p$(srcdir)/tex -p$(srcdir)/sty < $< |	\
		sed -e "s/@date@/`date '+%B %e, %Y'`/" >> $@
	cat < $(srcdir)/sty/genom.footer.html >> $@

genom.txt: Makefile $(txtsrcs)
genom.txt: genom.html
	$(W3M) -dump -cols 80 $< >$@

engcmd-dotgen.tex: $(srcdir)/docgen.awk
engcmd-dotgen.tex: $(top_srcdir)/engine/tcl/dotgen.c
	$(AWK) -f $(srcdir)/docgen.awk $< >$@
