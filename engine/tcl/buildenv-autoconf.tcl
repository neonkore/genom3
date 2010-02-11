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

namespace eval buildenv::autoconf {

    # The subdirectory in which autoconf and build-related files are held
    variable aux	{autoconf}

    # The file listing subdirectories
    variable subdirs	"${aux}/subdirs.m4"


    # --- subdir -----------------------------------------------------------

    # Add subdir to the list of subdirectories to be considered by autoconf.
    # This procedure _appends_ to the file subdirs.m4 a set of m4_include
    # command for each .m4 or .ac file found in the $dir/$aux directory.
    #
    proc subdir { dir } {
	variable aux
	variable subdirs
	set odir [engine pwd]

	# get file list from dir
	if {![file exists [file join $odir $dir/Makefile.am]]} {
	    template fatal "template does not provide a Makefile.am"	\
		"in $dir directory"
	}

	set dirs [list]
	set files [glob -nocomplain -tails -dir $odir	\
		       $dir/$aux/*.m4 $dir/$aux/*.ac ]

	# get existing file list (generated by other templates)
	set s [file join [engine pwd] $subdirs]
	if {[file exists $s]} {
	    set c [open $s r]
	    set t [read $c]
	    set subs [regexp -all -inline {set_add([subdirs],\[(.*?)\])} $t]
	    set incs [regexp -all -inline {include\(\[(.*?)\]\)} $t]
	    close $c
	    foreach {m f} $subs {
		if {[file exists [file join $odir $f/Makefile.in]]} {
		    lappend dirs $f
		}
	    }
	    foreach {m f} $incs {
		if {[file exists [file join $odir $f]]} {
		    lappend files $f
		}
	    }
	}
	if {[lsearch -exact $dirs $dir] < 0} {
	    lappend dirs $dir
	}

	# build new list
	set t "dnl Generated by [dotgen genom version]. Do not edit.\n"
	append t "dnl\n"
	foreach f [lsort -unique $files] {
	    append t [format {m4_include([%s])} $f] "\n"
	}
	append t "\n"
	foreach f $dirs {
	    append t [format {m4_set_add([subdirs],[%s])} $f] "\n"
	    append t [format {AC_CONFIG_FILES([%s/Makefile])} $f] "\n"
	}

	# output new list
	template parse string $t file $subdirs
    }


    # --- create -----------------------------------------------------------

    # Create the main autoconf files. Those files are common to all templates
    #
    proc create { } {
	variable subdirs

	# set input file path. Must use 'file normalize' so that the path is
	# absolute and does not get prefixed with the current template dir
	set d [file normalize [file join [dotgen template sysdir] autoconf]]

	# relative path to user files
	set userdir [file-relativize [dotgen input dir] [engine pwd]]

	set t "dnl Generated by [dotgen genom version]. Do not edit.\n"
	append t "dnl\n"

	template parse					\
	    file [file join $d top.Makefile.am]		\
	    file Makefile.am
	template parse					\
	    string $t					\
	    file $subdirs
	template parse					\
	    args $userdir				\
	    file [file join $d top.configure.ac]	\
	    file configure.ac
    }
}