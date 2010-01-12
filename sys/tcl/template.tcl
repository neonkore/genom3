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

namespace eval template {

    # default template options: just -h
    variable options {
	-h - --help {
	    template message "no options available for this template"
	}
    }

    # default usage message
    variable usage {usage not available}

    # set to 1 when options have been parsed
    variable gotopt	0


    # --- options ----------------------------------------------------------

    # Define the list of supported options for the template. ospec is a
    # switch-like script that must define all supported options. -h - --help is
    # always appended to this script.
    #
    proc options { ospec } {
	variable options
	variable gotopt

	if {$gotopt} {
	    fatal "options specification must be set before any"	\
		"'generate' commands"
	}

	if {[llength $ospec] % 2} { fatal "invalid options specification" }
	lappend ospec -h - --help { template fatal "help not implemented" }

	set options $ospec
    }
    namespace export options


    # --- string -----------------------------------------------------------

    # Put string into file
    #
    proc string { src type dst } {
	set c [engine open $type $dst]
	engine puts $c $src
	engine close $c
    }
    namespace export string


    # --- message ----------------------------------------------------------

    # Print message so that it is visible to the end-user. The text is sent on
    # stderr so that genom3 will display it unconditionnaly.
    #
    proc message { m } {
	puts stderr $m
    }
    namespace export message


    # --- fatal ------------------------------------------------------------

    # Print an error message and stop. In verbose mode, print the source
    # location as reported by [info frame]
    #
    proc fatal { args } {
	set info [info frame -1]
	set l ""
	if {[dict exists $info file]} {
	    append l "[file tail [dict get $info file]]: "
	    if {[dict exists $info line]} {
		append l "[dict get $info line] "
	    }
	}
	if {[dict exists $info proc]} {
	    append l "in [dict get $info proc]"
	}
	puts $l
	error [join $args " "]
    }
    namespace export fatal


    # --- getopt -----------------------------------------------------------

    # Parse options according to the template specification.
    #
    proc getopt { } {
	global argc argv
	variable usage
	variable options
	variable gotopt
	if {$gotopt} { return } else { set gotopt 1 }

	# process options
	while {$argc > 0} {
	    set arg [lindex $argv 0]
	    uplevel #0 switch -glob -- $arg [concat $options * "{
		template message \"unknown option $arg\"
		template fatal $usage
	    }"]

	    incr argc -1
	    set argv [lreplace $argv 0 0]
	}

	# process extraneous arguments
	if {$argc > 0} {
	    message "too many arguments -- $argv"
	    fatal $usage
	}

	return
    }

    namespace ensemble create
}
