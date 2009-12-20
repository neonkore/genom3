#
# Copyright(C) 2009 LAAS/CNRS
#
# GenoM autoconf file
#
AC_PREREQ(2.59)
AC_INIT([genom],[3.0],[openrobots@laas.fr])
AM_INIT_AUTOMAKE([foreign no-define])
AC_CONFIG_HEADERS([acgenom.h])

AC_SUBST(GENOM_TOOL, [genom3])


# compilers et al.
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_PROG_LEX
AC_PROG_YACC


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


# define a temporary directory
AC_DEFINE([TMPDIR],["/tmp"],[Default temporary directory])

# output
AC_CONFIG_FILES([
	genom3.pc
	Makefile
	src/Makefile
])
AC_OUTPUT
