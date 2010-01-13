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
#                                           Anthony Mallet on Fri Jan  8 2010
#
package require Tcl 8.5

namespace eval engine {

    # --- open -------------------------------------------------------------

    # Open file for writing, return a channel id.
    #
    proc open { type file {user ""} } {
	return [open $file w]
    }
    namespace export open


    # --- puts -------------------------------------------------------------

    # Put string into channel.
    #
    proc puts { channel string } {
	return [puts $channel $string]
    }
    namespace export puts


    # --- close ------------------------------------------------------------

    # Close open file.
    #
    proc close { channel } {
	return [close $channel]
    }
    namespace export close

    namespace ensemble create
}