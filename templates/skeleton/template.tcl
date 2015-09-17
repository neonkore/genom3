#
# Copyright (c) 2010-2015 LAAS/CNRS
# All rights reserved.
#
# Redistribution  and  use  in  source  and binary  forms,  with  or  without
# modification, are permitted provided that the following conditions are met:
#
#   1. Redistributions of  source  code must retain the  above copyright
#      notice and this list of conditions.
#   2. Redistributions in binary form must reproduce the above copyright
#      notice and  this list of  conditions in the  documentation and/or
#      other materials provided with the distribution.
#
# THE SOFTWARE  IS PROVIDED "AS IS"  AND THE AUTHOR  DISCLAIMS ALL WARRANTIES
# WITH  REGARD   TO  THIS  SOFTWARE  INCLUDING  ALL   IMPLIED  WARRANTIES  OF
# MERCHANTABILITY AND  FITNESS.  IN NO EVENT  SHALL THE AUTHOR  BE LIABLE FOR
# ANY  SPECIAL, DIRECT,  INDIRECT, OR  CONSEQUENTIAL DAMAGES  OR  ANY DAMAGES
# WHATSOEVER  RESULTING FROM  LOSS OF  USE, DATA  OR PROFITS,  WHETHER  IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR  OTHER TORTIOUS ACTION, ARISING OUT OF OR
# IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
#
#                                           Anthony Mallet on Tue Jan 12 2010
#

#/
# Skeleton template
# -----------------

# Default template for user skeleton file generation.

template usage "Skeleton generation template\n" [regsub -all [join {
  { *#/? ?} {----} {[*+]([^*+\n]+)[*+]} {::} { ::\n}
} |] {
  #/ The skeleton template generates the skeleton of the codel functions
  # defined in the input .gen file. It also generates a sample build
  # infrastructure for building them. By default, files are generated in the
  # same directory as the input .gen file. The `-C` option can be used to
  # specify another output directory.
  #
  # The `-l c++` option is specific to `C` codels. It generates a skeleton that
  # compiles the codels with a `C++` compiler. This is useful for invoking
  # `C++` code from the codels (Note that this is different from having
  # `C++` codels.)
  #
  # Files generated with this template are freely modifiable (and are actually
  # required to be modified in order to provide some real codels). They are
  # provided only as a sample - yet sensible - implementation. The only
  # requirement is that codels provide a `pkg-config` file (`.pc`)
  # named `<component>-genom3.pc` and telling the other templates how to
  # link with the codels library.
  #
  # The template can also be invoked in 'merge' mode, where it updates existing
  # skeletons. This mode tries to merge modifications in the .gen file, for
  # instance service addition or new interface definitions, into existing
  # codels. In case of conflicting files, there are several merge strategies:
  # option `-u` places conflicts markers in the source file, option `-i`
  # interactively asks what to do, and the generic option `-m tool` runs `tool`
  # on the conflicting files. `tool` can be any merge tool, for instance
  # `meld`.
  #
  # === Example
  # ----
  # user@host:~$ genom3 skeleton demo.gen
  # creating ./codels/demo_motion_codels.c
  # creating ./codels/demo_codels.c
  # [...]
  # creating ./codels/Makefile.am
  # ----
  #
  # === Supported options
  #
  # +-l c+++::
  # +--language=c+++ ::
  #		Compile C codels with a C++ compiler
  # +-C+::
  # +--directory='dir'+ ::
  #	Output files in dir instead of source directory
  # +-m+::
  # +--merge='tool'+ ::
  #		Merge conflicting files with tool
  # +-i+ ::
  #			Interactively merge conflicting files, alias for
  #			`-m interactive`
  # +-u+ ::
  #			Automatically merge conflicting files, alias for
  #			`-m auto`
  # +-f+::
  # +--force+ ::
  #			Overwrite existing files (use with caution)
  # +-h+::
  # +--help+ ::
  #			Print usage summary (this text)
} {\1}]

# defaults: no file overwrite
engine mode -overwrite -merge-if-change

# parse options
template options {
  -l - --language	{ set lang [template arg] }
  -C - --directory	{ set outdir [template arg] }
  -m - --merge		{
    engine merge-tool [template arg]; engine mode +merge-if-change
  }
  -i			{
    engine merge-tool interactive; engine mode +merge-if-change
  }
  -u			{
    engine merge-tool auto; engine mode +merge-if-change
  }
  -f - --force		{ engine mode +overwrite }
  -h - --help		{ puts [template usage]; exit 0 }
}

# check/process input files
if {![llength $argv]} { puts [template usage]; exit 2 }
foreach f $argv { dotgen parse file $f }
set input [file tail [lindex $argv 0]]
if {![info exists outdir]} {
  set outdir [file dirname [lindex $argv 0]]
}

# list of local idl source files
set idls [list]
set base [file normalize $outdir]
foreach f [dotgen input deps] {
  set f [file normalize $f]
  if {[string match $base/* $f]} {
    lappend idls [string range $f [string length $base/] end]
  }
}

# check options consistency
if {[catch {[dotgen component] lang} iface]} {
    set iface c
}
if {![info exists lang]} {
  set lang $iface
}

switch -- "$iface" {
  c {}
  default {
    template fatal "codel interface in $iface is not yet supported."
  }
}
if {$iface ne $lang} {
    switch -- "$iface|$lang" {
	c|c++ {}
	default {
	    template fatal \
		"codel interface in $iface compiled in $lang is not allowed"
	}
    }
}

# generate codel files
#
engine chdir $outdir
set src [lang $iface; fileext]
set ext [lang $lang; fileext]

foreach c [dotgen components] {
  # one source file for each task
  foreach t [$c tasks] {
    template parse					\
        args [list $c $t] file codels.codel$src		\
        file codels/[$c name]_[$t name]_codels$ext
  }

    # and one file for codels with no associated task
    template parse					\
	args [list $c ""] file codels.codel$src		\
	file codels/[$c name]_codels$ext

    # mandatory pkg-config file
    template parse					\
	args [list $c] file component.pc.in		\
	file [$c name]-genom3.pc.in
    template parse					\
	args [list $c] file component-uninstalled.pc.in	\
	file [$c name]-genom3-uninstalled.pc.in
}

# generate user build files fragment
#
template parse perm a+x					\
    string "#!/bin/sh\nautoreconf -vi\n"		\
    file bootstrap.sh
template parse						\
    file ag_templates.m4 file autoconf/ag_templates.m4
template parse						\
    args [list $input $lang] file top.configure.ac	\
    file configure.ac
template parse						\
    args [list $input $idls $lang] file top.Makefile.am	\
    file Makefile.am
template parse						\
    args [list $input $lang] file codels.Makefile.am	\
    file codels/Makefile.am
