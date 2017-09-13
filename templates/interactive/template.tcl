#
# Copyright (c) 2010,2012,2014,2017 LAAS/CNRS
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

#/
# Interactive template
# --------------------

template usage "Interactive TCL template\n" [regsub -all [join {
  { *#/? ?} {----} {[*+]([^*+\n]+)[*+]} {::} { ::\n}
} |] {
  #/ This template exports all the objects from the input .gen file for
  # interactive use in a `tclsh` interpreter. The `genom` TCL engine
  # procedures are available as in regular (scripted) templates.
  #
  # This template is mostly useful for development of new templates or
  # troubleshooting existing ones.
  #
  # === Example
  # ----
  # user@host:~$ genom3 interactive demo.gen
  # % foreach c [dotgen components] { puts [$c name] }
  # demo
  # % exit
  # user@host:~$
  # ----
  #
  # === Supported options
  #
  # +-b+ ::
  #	Batch mode: disable line editing facility
  # +-h+::
  # +--help+ ::
  #	Print usage summary (this text)
} {\1}]

# defaults
variable tcl_interactive 1
engine mode -overwrite

template options {
    -b			{ variable tcl_interactive 0 }
    -h - --help		{ puts [template usage]; exit 0 }
}

# process input files
dotgen parse file {*}$argv

# provide a default 'interactive' function in case eltclsh cannot be loaded
proc interactive {} {
    global tcl_interactive

    while {1} {
	if {$tcl_interactive} { puts -nonewline {% }; flush stdout }
	while {[gets stdin line] >= 0} {
	    append cmd $line
	    if {![info complete $cmd]} {
		append cmd "\n"
		if {$tcl_interactive} { puts -nonewline {> }; flush stdout }
		continue
	    }

	    set r [catch {uplevel #0 $cmd} cmd opts]
	    if {$tcl_interactive} {
		if {$cmd != ""} { puts $cmd }
		puts -nonewline {% }; flush stdout
	    } else {
		if {$r} { return -options $opts $cmd }
	    }
	    unset cmd
	}
	if {![info exists cmd]} break
	unset cmd
	if {$tcl_interactive} { puts {} }
    }
    return
}

if {$tcl_interactive} {
    # load eltclsh
    catch { package require eltclsh }

    # interactive loop
    puts "
Interactive Tcl template - [dotgen genom version]
Type Ctrl-d or 'exit' to leave."
}

interactive
