#
# Copyright (c) 2009-2010 LAAS/CNRS
#
# GenoM autoconf file
#
AC_PREREQ(2.62)
AC_INIT([genom],[2.99.1],[openrobots@laas.fr])

AC_CONFIG_AUX_DIR([autoconf])
AC_CONFIG_MACRO_DIR([autoconf])
AC_CONFIG_HEADERS([autoconf/acgenom.h])
AM_INIT_AUTOMAKE([foreign no-define subdir-objects])

# define the genom executable name
AC_SUBST(GENOM_TOOL, [genom${PACKAGE_VERSION}])

# define a temporary directory
AC_DEFINE([TMPDIR],["/tmp"],[Default temporary directory])

# define templates directory
eval tmpldir="\"$datadir/${PACKAGE_NAME}/${PACKAGE_VERSION}/templates\""
eval tmpldir="\"$tmpldir\""
AC_DEFINE_UNQUOTED([TMPLDIR],["${tmpldir}"],[Default templates directory])
AC_SUBST(tmpldir)

# define sys directory
eval sysdir="\"$datadir/${PACKAGE_NAME}/${PACKAGE_VERSION}/engines\""
eval sysdir="\"$sysdir\""
AC_DEFINE_UNQUOTED([SYSDIR],["${sysdir}"],[System files directory])
AC_SUBST(sysdir)

# generator engines - only tcl, but provide an option for documentation purpose.
AC_ARG_ENABLE(tcl,
	AS_HELP_STRING([--disable-tcl], [don't compile support for Tcl engine]),
	[use_tcl=$enableval],[use_tcl=yes])
AC_ARG_VAR(TCLSH, [Default tclsh interpreter])
AM_CONDITIONAL([WITH_TCL], [test "$use_tcl" = yes])


# compilers et al.
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_PROG_LEX
AC_PROG_YACC
AC_PATH_PROG(PDFLATEX, [pdflatex], [:])
AC_PATH_PROG(TTH, [tth], [:])
AC_PATH_PROG(W3M, [w3m], [:])
AC_PATH_PROG(Y2L, [y2l], [:])


# C library
AC_CONFIG_LIBOBJ_DIR([compat])
AC_REPLACE_FUNCS([strlcpy strlcat getline])
AC_CHECK_FUNCS([asprintf vasprintf])
AC_SEARCH_LIBS([dlopen], [dl])

# documentation (re)generation
AC_ARG_ENABLE(docgen,
	AS_HELP_STRING([--enable-docgen], [regenerate documentation]),
	[enable_docgen=$enableval],[enable_docgen=no])

AM_CONDITIONAL([REGEN_DOC_YACC],
  [test "$Y2L" != ":"])
AM_CONDITIONAL([REGEN_DOC_PDF],
  [test "$Y2L" != ":" -a "$PDFLATEX" != ":"])
AM_CONDITIONAL([REGEN_DOC_HTML],
  [test "$Y2L" != ":" -a "$PDFLATEX" != ":" -a "$TTH" != ":"])
AM_CONDITIONAL([REGEN_DOC_TXT],
  [test "$Y2L" != ":" -a "$PDFLATEX" != ":" -a "$TTH" != ":" -a "$W3M" != ":"])


# test that cpp accepts .gen files, maybe with -xc
AC_CACHE_CHECK([whether ${CPP} accepts .gen file extension],
  ac_cv_genom_cpp_gen,
  [
    echo TEST >conftest.gen
    if ${CPP} -DTEST=yes conftest.gen 2>&1 | grep yes >/dev/null; then
      ac_cv_genom_cpp_gen=yes
    elif ${CPP} -xc -DTEST=yes conftest.gen 2>&1 | grep yes >/dev/null; then
      ac_cv_genom_cpp_gen="with -xc"
    else
      ac_cv_genom_cpp_gen=no
    fi
    rm -f conftest.gen
  ]
)

gencpp=$CPP
if test x"${ac_cv_genom_cpp_gen}" != xno; then
  AC_DEFINE([CPP_DOTGEN],[1],[Whether CPP accepts .gen file extension])
fi
if test x"${ac_cv_genom_cpp_gen}" = "xwith -xc"; then
  gencpp="$gencpp -xc"
fi

AC_DEFINE_UNQUOTED([CPP],["$gencpp"],[Configured CPP program])

# tcl configuration
if test x"$use_tcl" = xyes; then
  # look for tclConfig.sh
  SC_PATH_TCLCONFIG
  SC_LOAD_TCLCONFIG

  # require >=8.5
  AC_MSG_CHECKING([for tcl version])
  AC_MSG_RESULT([$TCL_VERSION])
  case ${TCL_VERSION} in
       8.[[5-9]]*) ;;
       *)	 AC_MSG_ERROR([Tcl >= 8.5 is required]);;
  esac
  AC_MSG_CHECKING([for threaded tcl])
  if test "$TCL_THREADS" = 1; then
    AC_MSG_RESULT([yes])
    need_pthread=yes;
  else
    AC_MSG_RESULT([no])
  fi

  # add runtime path
  dir=`echo $TCL_LIB_SPEC | sed -ne 's|.*-L[[ 	]]*\([[^ 	]]*\).*|\1|p'`
  case "$dir" in
    "") ;;
    /usr/lib) ;;
    *) TCL_LIB_SPEC="-R${dir} ${TCL_LIB_SPEC}" ;;
  esac

  # check for eltclsh (for the 'interactive' template)
  AC_ELTCLSH
  if test x"$have_eltclsh" = xyes; then
    if test x"$tcl_extrapkg" = x; then
      tcl_extrapkg="\"$ELTCLSH_DATADIR\""
    else
      tcl_extrapkg="$tcl_extrapkg, \"$ELTCLSH_DATADIR\""
    fi
  fi

  # C definitions
  AC_DEFINE([WITH_TCL],[1],[Enable tcl engine])
  AC_DEFINE_UNQUOTED([TCL_EXTRAPKG],
	[{ $tcl_extrapkg }],[Path to extra tcl packages])
fi

# pthread support - this is required if using a threaded tcl library so that
# dlopening the tcl genom module works.
if test "$need_pthread" = yes; then
  AX_PTHREAD(, [AC_MSG_ERROR([pthread support not available])])
fi


# output
AC_CONFIG_FILES([
	genom3.pc
	Makefile
	src/Makefile
	engine/tcl/Makefile
	templates/interactive/Makefile
	templates/libcodels/Makefile
	templates/mappings/Makefile
	templates/skeleton/Makefile
	doc/Makefile
	regress/Makefile
])
AC_OUTPUT


# summary
summary=["
----------------------------------------------------------------------------
Summary of build options:

Cpp program:			$CPP
    cpp handles .gen extension:	$ac_cv_genom_cpp_gen

Tcl generator engine:		$use_tcl"]

if test x"$use_tcl" = xyes; then
  summary=["$summary
    version:			$TCL_VERSION
    prefix:			$TCL_PREFIX
    line editing facility:	$have_eltclsh"]
  if test x"$have_eltclsh" = xyes; then
    summary=["$summary
	(in $ELTCLSH_DATADIR)"]
  else
    summary=["$summary: see
	http://homepages.laas.fr/mallet/soft/shell/eltclsh"]
  fi
fi

summary=["$summary

Documentation generation:
    yacc to text:		"]
if test x"$Y2L" = x:; then
  summary=["${summary}no"]
  if test "$enable_docgen" = "yes"; then
    summary=["${summary}: see http://www.alchar.org/~aedil/Projects/y2l.html"]
  fi
else
  summary=["${summary}yes
	(with ${Y2L})"]
fi
summary=["$summary
    pdf:			"]
if test x"$Y2L" = x: -o x"$PDFLATEX" = x:; then
  summary=["${summary}no"]
  if test "$enable_docgen" = "yes" -a x"$PDFLATEX" = x:; then
    summary=["${summary}: see http://www.tug.org/texlive/"]
  fi
else
  summary=["${summary}yes
	(with ${PDFLATEX})"]
fi
summary=["$summary
    html:			"]
if test x"$Y2L" = x: -o x"$PDFLATEX" = x: -o x"$TTH" = x:; then
  summary=["${summary}no"]
  if test "$enable_docgen" = "yes" -a x"$TTH" = x:; then
    summary=["${summary}: see http://hutchinson.belmont.ma.us/tth/"]
  fi
else
  summary=["${summary}yes
	(with ${TTH})"]
fi
summary=["$summary
    text:			"]
if test x"$Y2L" = x: -o x"$PDFLATEX" = x: -o x"$TTH" = x: -o x"$W3M" = x:; then
  summary=["${summary}no"]
  if test "$enable_docgen" = "yes" -a x"$W3M" = x:; then
    summary=["${summary}: see http://w3m.sourceforge.net/"]
  fi
else
  summary=["${summary}yes
	(with ${W3M})"]
fi

summary=["$summary
----------------------------------------------------------------------------"]

AC_MSG_NOTICE([$summary])
