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
	return [[support $lang]::fileext $kind]
    }
    namespace export fileext


    # --- comment ----------------------------------------------------------

    # Return the comment text for the given language.
    #
    proc comment { lang text } {
	return [[support $lang]::comment $text]
    }
    namespace export comment


    # --- mapping ----------------------------------------------------------

    # Generate the mapping of types matching the glob pattern, for the given
    # language.
    #
    proc mapping { lang {pattern *} } {
	set lns [support $lang]
	foreach t [dotgen types $pattern] {
	    append m [${lns}::gentype $t]
	}
	return $m
    }
    namespace export mapping


    # --- declarator -------------------------------------------------------

    # Return the abstract declarator of a type or a variable
    #
    proc declarator { lang type {var {}} } {
	return [[support $lang]::declarator $type $var]
    }
    namespace export declarator


    # --- declarator -------------------------------------------------------

    # Return the signature of a codel
    #
    proc signature { lang codel {symchar { }} } {
	return [[support $lang]::signature $codel $symchar]
    }
    namespace export signature


    # --- hfill ------------------------------------------------------------

    # Fill text with filler up to column
    #
    proc hfill { text { filler - } { column 80 } } {
	return \
	    "$text[string repeat - [expr {$column - [string length $text]}]]"
    }
    namespace export hfill


    # --- support ----------------------------------------------------------

    # Return the namespace for language or raise an error
    #
    variable lns [dict create c c]
    proc support { lang } {
	variable lns

	if { [catch {dict get $lns $lang} ns] } {
	    template fatal "unsupported language $lang"
	}
	return $ns
    }

    namespace ensemble create
}
