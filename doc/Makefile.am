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
	tex/dotgen-grammar.tex	\
	tex/tcl-engine.tex

pdfsrcs=\
	$(basesrcs)		\
	tex/genom.bib		\
	sty/genom.sty

htmlsrcs=\
	$(basesrcs)		\
	sty/genom.tth		\
	sty/genom.header.html	\
	sty/genom.footer.html

txtsrcs=\
	$(basesrcs)

generatedsrcs=\
	dotgen-rules.tex	\
	engcmd-template.tex	\
	engcmd-engine.tex	\
	engcmd-dotgen.tex	\
	engcmd-language.tex	\
	engcmd-autoconf.tex

EXTRA_DIST=	$(pdfsrcs) $(htmlsrcs) $(txtsrcs)
EXTRA_DIST+=	docgen.awk

if REGEN_DOC_YACC

# Generate a text translation of the .gen yacc grammar. y2l could directly
# generate TeX instead of text, but the generated file is a bit messy and tth
# does not convert it to clean html afterwards. Also, fiddle at little bit with
# the y2l output to generate one additional file per rule for the detailed
# description of each rule in the documentation.
#
dotgen-rules.tex: Makefile
dotgen-rules.tex: grammar-tokens
dotgen-rules.tex: $(top_srcdir)/src/dotgen.y
	$(Y2L) -- -p -O2 $< -t$(srcdir)/grammar-tokens | $(AWK) '	\
		BEGIN { name = ""; n = 0; }				\
		/^[^ \t]/ {						\
			if ( name != "" ) printdef(n, name, rule);	\
			n++; name = $$1; rule = $$0;			\
			next;						\
		}							\
		{							\
			rule = rule "\n" $$0;				\
		}							\
		END { printdef(n, name, rule) }				\
		function printdef(n, name, rule) {			\
			out = "dotgen-rule-" name ".tex";		\
			print "\\input{" out "}";			\
			print "\\begin{grammar}{" n "}" > out;		\
			print "\\begin{verbatim}" >> out;		\
			printf "%s", rule >> out;			\
			print "\\end{verbatim}" >> out;			\
			print "\\end{grammar}" >> out;			\
		}							\
	' >$@

if REGEN_DOC_PDF

# Generate main PDF with pdflatex. Some TeX sources are generated from the TCL
# engine source files with an adhoc awk script.
#
genom.pdf: Makefile $(pdfsrcs) $(generatedsrcs)
genom.pdf: tex/genom.tex
	export TEXINPUTS=$(srcdir)/tex:$(srcdir)/sty:$(srcdir)::;	\
	export BIBINPUTS=$(srcdir)/tex::;				\
	$(PDFLATEX) $< &&						\
	bibtex genom &&							\
	$(PDFLATEX) $< &&						\
	$(PDFLATEX) $<

engcmd-template.tex: Makefile $(srcdir)/docgen.awk
engcmd-template.tex: $(top_srcdir)/engine/tcl/template.tcl
	$(AWK) -f $(srcdir)/docgen.awk $< >$@

engcmd-engine.tex: Makefile $(srcdir)/docgen.awk
engcmd-engine.tex: $(top_srcdir)/engine/tcl/engine.tcl
	$(AWK) -f $(srcdir)/docgen.awk $< >$@

engcmd-dotgen.tex: Makefile $(srcdir)/docgen.awk
engcmd-dotgen.tex: $(top_srcdir)/engine/tcl/dotgen.c
	$(AWK) -f $(srcdir)/docgen.awk $< >$@

engcmd-language.tex: Makefile $(srcdir)/docgen.awk
engcmd-language.tex: $(top_srcdir)/engine/tcl/language.tcl
	$(AWK) -f $(srcdir)/docgen.awk $< >$@

engcmd-autoconf.tex: Makefile $(srcdir)/docgen.awk
engcmd-autoconf.tex: $(top_srcdir)/engine/tcl/buildenv-autoconf.tcl
	$(AWK) -f $(srcdir)/docgen.awk $< >$@

if REGEN_DOC_HTML

# Generate HTML from PDF. CSS style is inlined within the preambule. Also,
# trailing whitespace is stripped (mostly to appease default git hooks).
#
genom.html: Makefile $(htmlsrcs) $(generatedsrcs) genom.bbl
genom.html: tex/genom.tex
	cat < $(srcdir)/sty/genom.header.html > $@
	$(TTH) -w2 -r -Lgenom -p$(srcdir)/sty:$(srcdir)/tex < $< |	\
		sed -e 's/[ 	]*$$//g' -e "s/@date@/`date '+%B %e, %Y'`/"\
		>> $@
	cat < $(srcdir)/sty/genom.footer.html >> $@

if REGEN_DOC_TXT

# Generate text from HTML. Nothing fancy here, that's the easy part.
#
genom.txt: Makefile $(txtsrcs)
genom.txt: genom.html
	$(W3M) -dump -cols 80 $< >$@

endif REGEN_DOC_TXT
endif REGEN_DOC_HTML
endif REGEN_DOC_PDF
else !REGEN_DOC_YACC

# For the dist target, make sure the documentation is regenerated
dist-hook:
	@echo '=============================================================='
	@echo '    For doing `$(MAKE) dist`, you have to enable documentation'
	@echo '    (re)generation by configuring with --enable-docgen.'
	@echo '=============================================================='
	@exit 2

endif !REGEN_DOC_YACC
