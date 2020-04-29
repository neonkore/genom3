#
# Copyright (c) 2009-2020 LAAS/CNRS
#
# GenoM autoconf file
#
AC_PREREQ(2.62)
AC_INIT([genom],[2.99.37],[openrobots@laas.fr])
AC_SUBST(GENOM_CLIENT_PROTOCOL, 20140618)

AC_CONFIG_AUX_DIR([autoconf])
AC_CONFIG_MACRO_DIR([autoconf])
AC_CONFIG_HEADERS([autoconf/acgenom.h])
AM_INIT_AUTOMAKE([foreign no-define subdir-objects])

# define the genom executable name
AC_SUBST(GENOM_TOOL, [genom${PACKAGE_VERSION}])

# define the genom-pcpp path
cpppath='${libexecdir}/genom-pcpp'
AC_SUBST(cpppath)

# define a temporary directory
AC_DEFINE([TMPDIR],["/tmp"],[Default temporary directory])

# define templates directory (relative to datadir)
tmpldir='${datadir}/${PACKAGE_NAME}/templates'
AC_SUBST(tmpldir)

# define sys directory (relative to datadir)
sysdir='${datadir}/${PACKAGE_NAME}/engines'
AC_SUBST(sysdir)

# define site templates directory (relative to datadir)
sitetmpldir='${datadir}/${PACKAGE_NAME}/site-templates'
AC_SUBST(sitetmpldir)


# build the genom3 tool or just the runtime (default: tool + runtime)
AC_ARG_ENABLE(tool,
	AS_HELP_STRING([--disable-tool], [disable the code generation tool]),
	[],[enable_tool=yes])
AM_CONDITIONAL([ENABLE_TOOL], [test "$enable_tool" = yes])

AC_ARG_ENABLE(runtime,
	AS_HELP_STRING([--disable-runtime], [disable runtime installation]),
	[],[enable_runtime=yes])
AM_CONDITIONAL([ENABLE_RUNTIME], [test "$enable_runtime" = yes])


# generator engines - only tcl, but provide an option for documentation purpose.
AC_ARG_ENABLE(tcl,
	AS_HELP_STRING([--disable-tcl], [don't compile support for Tcl engine]),
	[use_tcl=$enableval],[use_tcl=yes])
AC_ARG_VAR(TCLSH, [Default tclsh interpreter])
AM_CONDITIONAL([WITH_TCL], [test "$use_tcl" = yes])


# compilers and tools
AC_PROG_INSTALL
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_CXX
LT_INIT

if test "$enable_tool" = yes; then
  AC_PROG_LEX
  if test x"$LEX" = x:; then
     AC_MSG_FAILURE([required lex/flex program could not be found.])
  fi

  AC_PROG_YACC
  set -- $YACC
  if ! command -v $1 >/dev/null; then
     AC_MSG_FAILURE([required yacc/bison program could not be found.])
  fi
fi

AC_CACHE_CHECK([for GNU make], [ac_cv_path_MAKE],
  [AC_PATH_PROGS_FEATURE_CHECK([MAKE], [make gmake],
    [case `$ac_path_MAKE --version 2>/dev/null` in
       *GNU*) ac_cv_path_MAKE=$ac_path_MAKE; ac_path_MAKE_found=:;;
     esac],
    [AC_MSG_ERROR([could not find GNU make])])])
AC_SUBST([MAKE], [$ac_cv_path_MAKE])

# C library
if test "$enable_tool" = yes; then
  AC_CONFIG_LIBOBJ_DIR([compat])
  AC_REPLACE_FUNCS([strlcpy strlcat getline])
  AC_CHECK_FUNCS([asprintf vasprintf])
  AC_SEARCH_LIBS([dlopen], [dl])
  AC_HEADER_SYS_WAIT
fi

# C compiler features
if test "$enable_tool" = yes; then
  AC_CACHE_CHECK([for C99 variadic macro support], ac_cv_have_variadic, [
    AC_COMPILE_IFELSE([AC_LANG_SOURCE(
        #define macro(...) func(__VA_ARGS__)
        int func(int a, ...);
        int test() { return macro(1, 2, 3); }
        )], [ ac_cv_have_variadic=yes ], [ ac_cv_have_variadic=no ])])
  if test $ac_cv_have_variadic = yes ; then
    AC_DEFINE([HAVE_CPP_VARARG_MACRO_GCC], 1,
      [Define to 1 if your compiler supports C99 variadic macros])
  fi
fi

# documentation (re)generation
AC_ARG_ENABLE(docgen,
	AS_HELP_STRING([--enable-docgen], [regenerate documentation]),
	[enable_docgen=$enableval],[enable_docgen=no])

AC_PATH_PROG(Y2L, [y2l], [:])
AC_PATH_PROG(ASCIIDOCTOR, [asciidoctor], [:])
AC_CACHE_CHECK([for asciidoc-diagram extension], [genom_cv_adoc_diagram],
  [genom_cv_adoc_diagram=no
   test "$ASCIIDOCTOR" != ":" &&
     $ASCIIDOCTOR -r asciidoctor-diagram - </dev/null >/dev/null 2>&1 &&
       genom_cv_adoc_diagram=yes])
AC_CACHE_CHECK([for coderay extension], [genom_cv_adoc_coderay],
  [genom_cv_adoc_coderay=no
   test "$ASCIIDOCTOR" != ":" &&
     $ASCIIDOCTOR -r coderay - </dev/null >/dev/null 2>&1 &&
       genom_cv_adoc_coderay=yes])

AM_CONDITIONAL([DOCGEN],
  [test "$enable_docgen" = yes -a "$Y2L" != ":" -a "$ASCIIDOCTOR" != ":" \
                               -a "$genom_cv_adoc_diagram" = "yes"       \
                               -a "$genom_cv_adoc_coderay" = "yes"])
AM_CONDITIONAL([DOCBUNDLED],
  [test -f "$srcdir/doc/genom3.1" -a -f "$srcdir/doc/html/index.html"])

AM_CONDITIONAL([DOCGENORBUNDLED],
  [AM_COND_IF([DOCGEN],[:],[AM_COND_IF([DOCBUNDLED],[:],[false])])])


# tcl configuration
if test x"$enable_tool$use_tcl" = xyesyes; then
  # look for tclConfig.sh
  SC_PATH_TCLCONFIG
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
	genom3-c-client.pc
	Makefile
	compat/Makefile
	src/Makefile
	engine/tcl/Makefile
	runtime/genom3/c/Makefile
	runtime/genom3/c/client.h
	runtime/genom3/c++/Makefile
	templates/common/Makefile
	templates/interactive/Makefile
	templates/example/Makefile
	templates/mappings/Makefile
	templates/skeleton/Makefile
	doc/Makefile
	pcpp/Makefile
	regress/Makefile
])

# summary
summary=["
----------------------------------------------------------------------------
Summary of build options:

Runtime installation:		$enable_runtime

Genom code generator:		$enable_tool"]

if test x"$enable_tool" = xyes; then
  summary=["$summary
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
fi

summary=["$summary

Documentation generation:"]
if test x"$enable_docgen" = xno; then
  if test -z "$DOCBUNDLED_TRUE"; then
    summary=["${summary}	bundled"]
  else
    summary=["${summary}	no"]
  fi
  summary=["${summary} (use --enable-docgen to regenerate)"]
else
  summary=["${summary}
    asciidoctor:		"]
  if test x"$ASCIIDOCTOR" = x:; then
    summary=["${summary}no: missing asciidoctor"]
    summary=["${summary}
	see http://asciidoctor.org/"]
  else
    summary=["${summary}yes
	(with ${ASCIIDOCTOR})"]
  fi
  summary=["${summary}
    asciidoctor-diagram:	"]
  if test x"$genom_cv_adoc_diagram" = xno; then
    summary=["${summary}no: missing ruby gem"]
    summary=["${summary}
	see http://asciidoctor.org/"]
  else
    summary=["${summary}yes"]
  fi
  summary=["${summary}
    coderay:			"]
  if test x"$genom_cv_adoc_coderay" = xno; then
    summary=["${summary}no: missing ruby gem"]
    summary=["${summary}
	see http://coderay.rubychan.de/"]
  else
    summary=["${summary}yes"]
  fi
  summary=["${summary}
    yacc to text:		"]
  if test x"$Y2L" = x:; then
    summary=["${summary}no: missing y2l"]
    summary=["${summary}
	see http://www.alchar.org/~aedil/Projects/y2l.html"]
  else
    summary=["${summary}yes
	(with ${Y2L})"]
  fi
fi

summary=["$summary
----------------------------------------------------------------------------"]

AC_CONFIG_COMMANDS([summary], [AS_ECHO(["$summary"])], [summary='$summary'])

AM_COND_IF([DOCGEN],[:],[
  if test "$enable_docgen" = yes; then
    AC_MSG_NOTICE([$summary])
    AC_MSG_ERROR(
      [--enable-docgen was specified but some dependencies are missing])
  fi
])

AC_OUTPUT
