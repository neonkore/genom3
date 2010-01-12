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

    # dictionary of types found in the .gen file
    variable types


    # --- for-types --------------------------------------------------------

    # Execute script for each type matching the glob pattern. vars must be a
    # list of two variable names which are bound to the key and value of each
    # entry.
    #
    proc for-types { vars pattern script } {
        variable types

	if {[llength $vars] != 2} { error "must have two variable names" }
        upvar [lindex $vars 0] k
        upvar [lindex $vars 1] v

        dict for [list k v] $types {
	    if {[string match $pattern $k]} { uplevel $script }
	}
    }
    namespace export for-types


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
