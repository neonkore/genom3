#
# Copyright (c) 2010-2011 LAAS/CNRS
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

# Default template for user skeleton file generation.

template usage {*}{
    "\n"
    "Skeleton generation template.\n"
    "\n"
    "This template generates the skeleton of codel functions. Files are\n"
    "put in a \"codels\" subdirectory of the directory of the .gen file.\n"
    "The -C option can be used to specify another output directory.\n"
    "\n"
    "The -l option can be used to define the language in which the codels\n"
    "are written. This is not to be confused with the codel interface\n"
    "language which is defined in the .gen file. The -l switch is only\n"
    "useful to have a codel interface in C with codels written in C++; other\n"
    "combinations are not supported.\n"
    "\n"
    "Supported options:\n"
    "  -l, --language=lang\tset codels source code language\n"
    "  -C, --directory=dir\toutput files in dir instead of source directory\n"
    "  -m, --merge=tool\tmerge conflicting files with tool\n"
    "  -i\t\t\tinteractively merge conflicting files, alias for\n"
    "\t\t\t-m interactive\n"
    "  -u\t\t\tautomatically merge conflicting files, alias for\n"
    "\t\t\t-m auto\n"
    "  -f, --force\t\toverwrite existing files (use with caution)\n"
    "  -h, --help\t\tprint usage summary (this text)"
}

# defaults: no file overwrite, C interface, C source and output in "codel"
# subdir.
engine mode -overwrite -merge-if-change
if {[catch {[dotgen component] lang} iface]} {
    set iface c
}
set lang $iface
set outdir [dotgen input dir]

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
  -h - --help		{ engine mode +verbose; puts [template usage]; exit 0 }
}

# check options consistency
if {$iface ne $lang} {
    switch -- "$iface|$lang" {
	c|c++ {}
	default {
	    template fatal \
		"codel interface in $iface compiled in $lang is not allowed"
	}
    }
}

engine chdir $outdir

# generate codel files
#
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
	args [list $c] file libcodels.pc.in		\
	file lib[$c name]_codels.pc.in
}

# generate user build files fragment
#
template parse perm a+x					\
    string "mkdir -p build-aux\nautoreconf -vi\n"	\
    file autogen.sh
template parse						\
    args [list $lang] file top.configure.ac		\
    file configure.ac
template parse						\
    file top.Makefile.am				\
    file Makefile.am
template parse						\
    args [list $lang] file codels.Makefile.am		\
    file codels/Makefile.am
