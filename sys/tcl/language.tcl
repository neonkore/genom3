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
#                                           Anthony Mallet on Fri Jan 15 2010
#

namespace eval language {

    # --- fileext ----------------------------------------------------------

    # Return the cannonical file extension for the given language.
    #

    proc fileext { lang {kind source}} {
	switch -nocase -- $lang {
	    c { if {$kind == "header"} { return ".h" } else { return ".c" } }
	}

	template fatal "unsupported language $lang"
    }
    namespace export fileext


    # --- comment ----------------------------------------------------------

    # Return the comment text for the given language.
    #
    proc comment { lang text } {
	switch -nocase -- $lang {
	    c {
		regsub -all "\n(?=.)" "/*\n${text}" "\n * " text
		set text "${text} */"
	    }
	    default { template fatal "unsupported language $lang" }
	}

	return $text
    }
    namespace export comment

    namespace ensemble create
}
