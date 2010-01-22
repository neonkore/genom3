#
# Copyright (c) 2010 LAAS/CNRS
#
# eltclsh autoconf file
#
AC_DEFUN([AC_ELTCLSH], [

  # define a user option
  AC_ARG_WITH(eltclsh,
    AC_HELP_STRING([--with-eltclsh], [prefix directory of eltclsh installation]),
    [use_eltclsh=${withval}], [use_eltclsh=])

  # append path in front of PKG_CONFIG_PATH if required
  if test x"$use_eltclsh" = xyes; then
    :
  elif test x"$use_eltclsh" = xno; then
    :
  elif test x"$use_eltclsh" = x; then
    :
  else
    PKG_CONFIG_PATH="$use_eltclsh"
  fi

  # call pkg-config
  if test x"$use_eltclsh" = xno; then
    have_eltclsh=no
  else
    eltclsh_reqd="eltclsh >= 1.11"
    PKG_CHECK_EXISTS([$eltclsh_reqd], [have_eltclsh=yes], [have_eltclsh=no])
    if test "$have_eltclsh" = yes; then
      ELTCLSH_DATADIR=`$PKG_CONFIG --variable=datadir "$eltclsh_reqd"`
    fi
  fi
])