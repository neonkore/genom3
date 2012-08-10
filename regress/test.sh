#!/bin/sh
#
# Copyright (c) 2010-2012 LAAS/CNRS
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
#                                            Anthony Mallet on Sat Jun 26 2010
#

# set default values for standalone runs
: ${SHELL:=/bin/sh}
: ${CC:=gcc}
: ${CXX:=g++}
: ${AWK:=awk}
: ${GENOM_BIN:=genom3}
: ${PACKAGE_VERSION:=`$GENOM_BIN --version ||:`}

thisdir=${0%${0##*/}}

# fail on errors
set -e

# clean up on success
trap 'rm -rf run.out run.err run.gen run.data run.t a.out' 0

# "testing ..." message
testing() {
    echo testing "$@"
}

# run program, capture std channels and expect an exit status
run() {
    expect=${1:-0}; shift
    s=0 >run.out 2>run.err
    "$@" 1>run.out 2>run.err || s=$?
    if test $s -ne $expect; then
	echo test failed
	echo "$@"
	cat run.out
	cat run.err
	return 2
    fi
    return 0
}

# pass awk program on the specified file
expect() {
    file=$1; shift
    if test -s run.$file; then
	if ${AWK} "$@" < run.$file; then
	    return 0
	fi
	echo wrong output produced:
	${AWK} '{print NR "\t" $0}' run.$file
    else
	echo no output produced
    fi
    echo expecting "$@"
    echo test failed
    return 2
}

# cmp stdin with the speficied file
require() {
    file=$1; shift
    cat >run.data
    if test -s run.$file; then
	if cmp run.data run.$file; then
	    return 0
	fi
	echo wrong output produced:
	${AWK} '{print NR "\t" $0}' run.$file
    else
	echo no output produced
    fi
    echo expecting:
    ${AWK} '{print NR "\t" $0}' run.data
    echo test failed
    return 2
}
