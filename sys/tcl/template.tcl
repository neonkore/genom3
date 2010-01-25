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
	    template fatal "no options available for this template"
	}
    }

    # default usage message
    variable usage "Supported options:\n  -h.--help\t\tprint usage summary"

    # set to 1 when options have been parsed
    variable gotopt	0


    # --- parse ------------------------------------------------------------

    # Main template function to parse template source and instanciate it.
    # Invoke with 'parse file|string src ... file|string dst'. There can be
    # multiple 'file|string src' argument pairs.
    #
    proc parse { args } {
	if {[llength $args] < 4 || [llength $args] % 2} {
	    template fatal "wrong # args"
	}

	# process options, if not done already
	getopt

	# produce output
	lassign [lrange $args end-1 end] dtype dst
	set out [engine::open $dtype $dst write]

	set savedargs [engine::args [list]]
	foreach { stype src } [lrange $args 0 end-2] {
	    switch -- $stype {
		args	{ engine::args $src }

		raw {
		    set in [engine::open string $src read]
		    if {[catch {chan copy $in $out} m]} {
			template fatal "$m"
		    }
		    engine::close $in
		}

		string  -
		file	{
		    set in [engine::open $stype $src read]
		    if {$stype == "string"} { set src "<string>" }
		    if {[catch {engine::process $src $in $out} m]} {
			template fatal "$m"
		    }
		    engine::close $in
		}

		default {
		    engine::close $out
		    template fatal \
			"unknown source '$stype': must be args, string or file"
		}
	    }
	}
	engine::args $savedargs

	engine::close $out
	return
    }
    namespace export parse


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
		"'template parse' command"
	}

	if {[llength $ospec] % 2} { fatal "invalid options specification" }
	lappend ospec -h - --help { template fatal "help not implemented" }

	set options $ospec

	# process options, if not done already
	getopt
    }
    namespace export options


    # --- arg --------------------------------------------------------------

    # Return next argument, raise an error is none left
    #
    proc arg { } {
	global argc argv

	set optarg [lindex $argv 1]
	if {$optarg == ""} {
	    template fatal "missing argument to [lindex $argv 0]"
	}

	set argv [lreplace $argv 1 1]
	incr argc -1
	return $optarg
    }
    namespace export arg


    # --- usage ------------------------------------------------------------

    # Define the usage message
    #
    proc usage { args } {
	variable usage

	if {[llength $args] == 0} { return $usage }
	set usage [join $args {}]
	return
    }
    namespace export usage


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
	    append l "[file tail [dict get $info file]]:"
	    if {[dict exists $info line]} {
		append l "[dict get $info line]:"
	    }
	    append l " "
	}
	if {[dict exists $info proc]} {
	    append l "in [dict get $info proc]"
	}
	return -code error -level 2 -errorinfo "[join $args]\n    $l" \
	    [join $args]
    }
    namespace export fatal


    # --- getopt -----------------------------------------------------------

    # Parse options according to the template specification.
    #
    proc getopt { } {
	variable gotopt
	if {$gotopt} { return } else { set gotopt 1 }

	global argc argv
	variable usage
	variable options

	# process options
	while {$argc > 0} {
	    incr argc -1
	    set arg [lindex $argv 0]
	    if {[regexp -- {(--?.+)=(.+)} $arg m o a]} {
		set arg $o
		set argv [linsert $argv 1 $a]
	    }

	    uplevel #0 switch -glob -- $arg [concat $options * "{
		template message \"unknown option $arg\"
		template fatal {$usage}
	    }"]

	    set argv [lrange $argv 1 end]
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
