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
#                                           Anthony Mallet on Thu Jan 28 2010
#

namespace eval buildenv {

    # --- autoconf ---------------------------------------------------------

    # autoconf build environment setup. This is actually the only system
    # supported now but, as you can see, provisions have been taken to make
    # this enventually extendable.
    #
    proc autoconf { args } {
	if {[llength $args] == 0} {
	    template fatal \
		"wrong # args: should be \"buildenv autoconf ?command?\""
	}

	namespace eval autoconf $args
    }
    namespace export autoconf

    namespace ensemble create
}

# --- file-relativize ------------------------------------------------------

# Return a relative path to reach name from dir
#
proc file-relativize { name dir } {
    set src [file split [file normalize $name]]
    set dst [file split [file normalize $dir]]

    if { $src eq $dst } { return . }

    while { [lindex $src 0] eq [lindex $dst 0] } {
	set src [lreplace $src 0 0]
	set dst [lreplace $dst 0 0]
    }

    if { [llength $dst] == 0 } { return [file join {*}$src] }
    return [file join {*}[concat [lrepeat [llength $dst] ..] $src]]
}
