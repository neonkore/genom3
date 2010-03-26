#
# Copyright (c) 2010 LAAS/CNRS
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
    "put in a \"codels\" subdirectory of the directory of the .gen file,\n"
    "unless -C option is used (but you should normally not use this option).\n"
    "\n"
    "Supported options:\n"
    "  -C, --directory=dir\toutput files in dir instead of source directory\n"
    "  -f, --force\t\toverwrite existing files (use with caution)\n"
    "  -h, --help\t\tprint usage summary (this text)"
}

# defaults
engine mode -overwrite
set outdir [dotgen input dir]

# parse options
template options {
    -C - --directory	{ set outdir [template arg] }
    -f - --force	{ engine mode +overwrite }
    -h - --help		{ engine mode +verbose; puts [template usage]; exit 0 }
}

engine chdir $outdir

# generate codel files
foreach c [dotgen components] {
    set ext [language fileext [$c language]]
    foreach t [$c tasks] {
	template parse					\
	    args [list $c $t] file codels.codel$ext	\
	    file codels/[$c name]_[$t name]$ext
    }
}

# generate user build files fragment
template parse					\
    file top.configure.user.ac			\
    file configure.user.ac
template parse					\
    args [list $c] file codels.Makefile.user.am	\
    file codels/Makefile.user.am
