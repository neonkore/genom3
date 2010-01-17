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
#                                           Anthony Mallet on Mon Jan 11 2010
#
package require Tcl 8.5

namespace eval dotgen {

    # path to the .gen file
    variable input

    # this template directory
    variable template

    # path to the temporary directory
    variable tmpdir

    # dictionary of types found in the .gen file
    variable types


    # --- genom ------------------------------------------------------------

    # Return the genom version
    #
    proc genom { } {
	return $::genom
    }
    namespace export genom


    # --- srcdir -----------------------------------------------------------

    # Return the directory where the input .gen file resides
    #
    proc srcdir { } {
	variable input

	return [file dirname $input]
    }
    namespace export srcdir


    # --- input ------------------------------------------------------------

    # Return the .gen file
    #
    proc input { } {
	variable input

	return $input
    }
    namespace export input


    # --- template ---------------------------------------------------------

    # Return the template directory
    #
    proc template { } {
	variable template

	return $template
    }
    namespace export template


    # --- tmpdir -----------------------------------------------------------

    # Return the temporary directory
    #
    proc tmpdir { } {
	variable tmpdir

	return $tmpdir
    }
    namespace export tmpdir


    # --- foreach ----------------------------------------------------------

    # Execute script for each type matching the glob pattern. vars must be a
    # list of two variable names which are bound to the key and value of each
    # entry. 'data' must be either 'components' or 'types'.
    #
    proc foreach { data vars pattern script } {
	if {[lsearch -exact {components types} $data] < 0} {
	    ::template fatal "unknown data '$data': must be components or types"
	}
	if {[llength $vars] != 2} {
	    ::template fatal "invalid key '$vars': must be a list of two" \
		"variable names"
	}
        variable $data

        upvar [lindex $vars 0] k
        upvar [lindex $vars 1] v

        dict for [list k v] [set $data] {
	    if {[string match $pattern $k]} { uplevel $script }
	}
    }
    namespace export foreach


    # --- typeref ----------------------------------------------------------

    # Return the type referenced in type
    #
    proc typeref { type } {
        variable types

	while {[dict exists $type ref]} {
	    set type [dict get $types [dict get $type ref]]
	}

	return $type
    }
    namespace export typeref

    namespace ensemble create
}
