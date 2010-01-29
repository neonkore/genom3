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
#                                           Anthony Mallet on Fri Jan 22 2010
#

# Interactive template
template usage {*}{
    "\n"
    "Interactive Tcl template.\n"
    "\n"
    "Objects from the .gen file and template engine procedures are available\n"
    "as in regular (scripted) templates. This template is mostly useful\n"
    "for development of new templates or troubleshooting existing ones.\n"
    "\n"
    "Supported options:\n"
    "  -h.--help\t\tprint usage summary (this text)"
}

# defaults
engine mode -overwrite verbose

template options {
    -h - --help		{ engine mode +verbose; puts [template usage]; exit 0 }
}

# provide a default 'interactive' function in case eltclsh cannot be loaded
proc interactive {} {
    while {1} {
	puts -nonewline {% }; flush stdout
	while {[gets stdin line] >= 0} {
	    append cmd $line
	    if {![info complete $cmd]} {
		append cmd "\n"
		puts -nonewline {> }; flush stdout
		continue
	    }

	    catch {uplevel #0 $cmd} cmd
	    if {$cmd != ""} { puts $cmd }
	    unset cmd
	    puts -nonewline {% }; flush stdout
	}
	if {![info exists cmd]} break
	unset cmd
	puts {}
    }
    return
}

# load eltclsh
catch { package require eltclsh }

# interactive loop
puts "
Interactive Tcl template - [dotgen genom version]
Type Ctrl-d or 'exit' to leave."

interactive
