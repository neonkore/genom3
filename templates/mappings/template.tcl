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

# Template for type mappings generation.

template usage {*}{
    "Supported options:\n"
    "  -l,--language=lang\tgenerate mappings for language\n"
    "  -s,--suffix=string\tset output file name suffix\n"
    "  -C,--directory=dir\toutput files in dir instead of current directory\n"
    "  -p,--preserve\tdo not overwrite existing files\n"
    "  -h.--help\t\tprint usage summary (this text)"
}

# defaults
engine mode +overwrite +move-if-change
set suffix _types

# parse options
template options {
    -s - --suffix	{ set suffix [template arg] }
    -l - --language	{ lappend lang [template arg] }
    -C - --directory	{ engine chdir [template arg] }
    -p - --preserve	{ engine mode -overwrite }
    -h - --help		{ template message [template usage]; exit 0 }
}

# generate types definition
dotgen foreach components {k c} * {
    set l [if {[info exists lang]} { set lang } else {
	dict get $c properties language value
    }]
    foreach l $l {
	template parse file header string header
	template parse					\
	    string [language comment $l $header]	\
	    string [mapping generate $l *]		\
	    file "[dict get $c name]$suffix[language fileext $l header]"
    }
}
