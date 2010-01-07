#
# Copyright(C) 2009 LAAS/CNRS
#
# GenoM autoconf file
#
AC_PREREQ(2.62)
AC_INIT([genom],[3.0],[openrobots@laas.fr])
AM_INIT_AUTOMAKE([foreign no-define subdir-objects])
AC_CONFIG_HEADERS([acgenom.h])

AC_SUBST(GENOM_TOOL, [genom3])

# engines
AC_ARG_ENABLE(tcl,
	AS_HELP_STRING([--disable-tcl], [don't compile support for Tcl engine]),
	[use_tcl=$enableval],[use_tcl=yes])
AC_ARG_VAR(TCLSH, [Default tclsh interpreter])


# compilers et al.
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_PROG_LEX
AC_PROG_YACC


# C library
AC_CONFIG_LIBOBJ_DIR([compat])
AC_REPLACE_FUNCS([strlcpy strlcat])


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


# look for a default tclsh interpreter - try to find >=8.5
if test x"$use_tcl" = xyes; then
  if test x"$TCLSH" != x; then ac_cv_path_TCLSH=$TCLSH; fi
  AC_PATH_PROGS_FEATURE_CHECK(TCLSH, [tclsh8.5 tclsh],
    [[v=`echo 'puts [expr $tcl_version >= 8.5]' | $ac_path_TCLSH`
      test "$v" = "1" && ac_cv_path_TCLSH=$ac_path_TCLSH ac_path_TCLSH_found=:]],
    [ac_cv_path_TCLSH=tclsh]
  )
  AC_DEFINE_UNQUOTED([TCLSH],["$ac_cv_path_TCLSH"],[Default tclsh interpreter])
fi


# define a temporary directory
AC_DEFINE([TMPDIR],["/tmp"],[Default temporary directory])

# output
AC_CONFIG_FILES([
	genom3.pc
	Makefile
	src/Makefile
	regress/Makefile
])
AC_OUTPUT
