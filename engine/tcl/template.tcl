#
# Copyright (c) 2010-2012 LAAS/CNRS
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

    # save template options
    variable args [set ::argv]

    # save template processed files (dependencies)
    variable deps [list]


    # --- require ----------------------------------------------------------

    #/ @nodebeproc{template require, Source additional template code}
    # @deffn {TCL Backend} {template require} @var{file}
    #
    # Source tcl @var{file} and make its content available to the template
    # files. The file name can be absolute or relative. If it is relative, it
    # is interpreted as relative to the template directory (pxref@{dotgen
    # template dir@}).
    #
    # @@args
    # @item file
    # Tcl input file to source. Any procedure that it creates is made available
    # to the template files.
    # @@end args
    # @end deffn
    #
    proc require { src } {
        variable deps

	set src [file join [dotgen template dir] $src]
	puts "sourcing $src"

	if {[catch {
          uplevel #0 source $src
          slave invokehidden source $src
	} m]} {
	    template fatal "$m"
	}

        set src [file normalize $src]
        if {$src ni $deps} {
          lappend deps $src
        }
    }
    namespace export require


    # --- parse ------------------------------------------------------------

    #/ @nodebeproc{template parse, Generate template content}
    # @deffn {TCL Backend} {template parse} [@b{args} @var{list}] @
    # [@b{perm} @var{mode}] [@b{file|string|raw} @var{file} @dots{}]
    #
    # This is the main template function that parses a template source file and
    # instanciate it, writing the result into the current template directory
    # (or in a global variable). This procedure should be invoked for each
    # source file that form a @genom{} template.
    #
    # When invoking @code{template parse}, the last two arguments are the
    # destination file or string. A destination file is specified as @code{file
    # @var{file}} (the filename is relative to the current template output
    # directory). Alternatively, a destination string is specified as
    # @code{string @var{var}}, where @var{var} is the name of a @emph{global}
    # variable in which the template engine will store the result of the source
    # instantiation.
    #
    # The output destination file or string is generated by the template from
    # one or several input source. An input source is typically a source file,
    # but it can also be a string or raw (unprocessed) text. An input source
    # file is specified with @code{file @var{file}}, where @var{file} is a file
    # name relative to the template directory. An input source read from a
    # string is specified as @code{string @var{text}}, where text is the text
    # processed by the template engine. Finally, a raw, unprocessed source that
    # is copied verbatim to the destination is specified as @code{raw
    # @var{text}}, where @var{text} is the text to be output.
    #
    # Additionnaly, each input source, defined as above, can be passed a list
    # of optional arguments by using the special @code{args @var{list}}
    # construction as the @emph{first} argument of the @code{template parse}
    # command. The list given after @code{args} can be retrieved from within
    # the processed template source files from the usual @var{argv} variable.
    #
    # @@args
    # @item args @var{list}
    # This optional argument should be followed by a list of arguments to pass
    # to the template source file. It should be the very first argument,
    # otherwise it is ignored. Each element of the list is available from the
    # template source file in the @var{argv} array.
    #
    # @item perm @var{mode}
    # This optional argument may be set to specify the permissions to be set
    # for the created file.
    # @@end args
    #
    # @b{Examples:}
    # @example
    # template parse file mysrc file mydst
    # @end example
    # Will parse the input file @code{mysrc}, process it and save the result in
    # @code{mydst}.
    #
    # @example
    # template parse args @{one two@} file mysrc file mydst
    # @end example
    # Will do the same as above, but the template code in the input file
    # @code{mysrc} will have the list @code{@{one two@}} accessible via the
    # @var{argv} variable.
    #
    # @example
    # template parse string "test" file mydst
    # @end example
    # Will process the string "test" and save the result in @code{mydst}.
    # @end deffn
    #
    proc parse { args } {
        variable deps

	if {[llength $args] < 4 || [llength $args] % 2} {
	    template fatal "wrong # args"
	}
	# process template options, if not done already
	getopt

	# produce output
        set perm {}
	lassign [lrange $args end-1 end] dtype dst
	set out [engine::open $dtype $dst write]

	set savedargs [engine::args [list]]
	foreach { stype src } [lrange $args 0 end-2] {
	    switch -- $stype {
		args	{ engine::args $src }
		perm	{ set perm $src }

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
			engine::close $in
			template message $m
			exit 2
		    }
		    engine::close $in

                    if {$stype == "file"} {
                      set src [file join [dotgen template dir] $src]
                      if {$src ni $deps} {
                        lappend deps [file normalize $src]
                      }
                    }
		}

		default {
		    engine::close $out
		    template fatal \
			"unknown source '$stype': must be args, string or file"
		}
	    }
	}
	engine::args $savedargs

	engine::close $out $perm
	return
    }
    namespace export parse


    # --- link -------------------------------------------------------------

    #/ @nodebeproc{template link, Create symbolic links}
    # @deffn {TCL Backend} {template link} @var{src} @var{dst}
    #
    # Link source file @var{src} to destination file @var{dst}. If relative,
    # the source file @var{src} is interpreted as relative to the template
    # directory and @var{dst} is interpreted as relative to the current output
    # directory. Absolute file name can be given to override this behaviour.
    # @end deffn
    #
    proc link { src dst } {
        variable deps

	set src [file join [dotgen template dir] $src]
	set dst [file join [engine pwd] $dst]
	if {[file normalize $src] == [file normalize $dst]} return

        if {[file normalize $src] ni $deps} {
          lappend deps [file normalize $src]
        }

	if {[file exists $dst]} {
	    if {[lsearch [engine mode] move-if-change] >= 0} {
		set d [file normalize [file link $dst]]
		set s [file normalize $src]
		if {$s == $d} {
		    template message "$dst is up-to-date"
		    return
		}
	    }
	    if {[lsearch [engine mode] -overwrite] >= 0} {
		template fatal "file $dst would be overwritten"
		return
	    }

	    template message "overwriting $dst"
	    file delete $dst
	} else {
	    template message "creating link $dst"
	}

	file mkdir [file dirname $dst]
	file link $dst $src
	return
    }
    namespace export link


    # --- options ----------------------------------------------------------

    #/ @nodebeproc{template options, Define template options}
    # @deffn {TCL Backend} {template options} @
    # @{ @var{pattern} @var{body} @dots{} @}
    #
    # Define the list of supported options for the template. Argument is a
    # Tcl switch-like script that must define all supported options. It
    # consists of pairs of @var{pattern body}. If an option matching the
    # @var{pattern} is passed to the template, the @var{body} script is
    # evaluated. A special body specified as "-" means that the body for the
    # next pattern will be used for this pattern.
    #
    # @b{Examples:}
    # @example
    # template options @{
    #     -h - --help	@{ puts "help option" @}
    # @}
    # @end example
    #
    # This will make the template print the text "help option" whenever -h or
    # --help option is passed to the template.
    # @end deffn
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


    # --- deps -------------------------------------------------------------

    #/ @nodebeproc{template deps, Template dependencies}
    # @deffn {TCL Backend} {template deps}
    #
    # Return the comprehensive list of template files processed so far. This
    # includes files processed via @code{template require}, @code{template
    # parse} and @code{template link}. This list is typically used to generate
    # dependency information in a Makefile.
    # @end deffn
    #
    proc deps { } {
      variable deps

      # add main template file to the list
      set src [file join [dotgen template dir] template.tcl]
      if {$src ni $deps} {
        lappend deps $src
      }
      return $deps
    }
    namespace export deps


    # --- arg --------------------------------------------------------------

    #/ @nodebeproc{template arg, Retrieve options passed to templates}
    # @deffn {TCL Backend} {template arg}
    #
    # Return the next argument passed to the template, or raise an error is
    # no argument remains.
    # @end deffn
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

    #/ @nodebeproc{template usage, Define template help string}
    # @deffn {TCL Backend} {template usage} [@var{string}]
    #
    # With a @var{string} argument, this procedure defines the template "usage"
    # message. Unless the template redefines a @code{-h} option with
    # @code{template options} (@pxref{template options}), the default behaviour
    # of the template is to print the content of the @code{template usage}
    # string when @code{-h} or @code{--help} option is passed to the template.
    #
    # @code{template usage}, when invoked without argument, returns the last
    # usage message defined.
    # @end deffn
    #
    proc usage { args } {
	variable usage

	if {[llength $args] == 0} { return $usage }
	set usage [join $args {}]
	return
    }
    namespace export usage


    # --- message ----------------------------------------------------------

    #/ @nodebeproc{template message, Print runtime information}
    # @deffn {TCL Backend} {template message} [@var{string}]
    #
    # Print @var{string} so that it is visible to the end-user. The text is
    # sent on the standard error channel unconditionnaly.
    # @end deffn
    #
    proc message { m } {
	puts stderr $m
    }
    namespace export message


    # --- fatal ------------------------------------------------------------

    #/ @nodebeproc{template fatal, Abort template processing}
    # @deffn {TCL Backend} {template fatal} [@var{string}]
    #
    # Print an error message and stop. In verbose mode, print the source
    # location as reported by the @code{TCL} command [info frame].
    # @end deffn
    #
    proc fatal { args } {
      set info [info frame -1]
      set l ""
      if {[dict exists $info file]} {
        append l "(file \"[file tail [dict get $info file]]\""
        if {[dict exists $info line]} {
          append l " line [dict get $info line]"
        }
        append l ")"
      }
      if {[dict exists $info proc]} {
        append l " in [dict get $info proc]"
      }

      return -code error -level 2 \
          -errorinfo "error: [join $args]\n    $l" "error: [join $args]"
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
		template fatal \"unknown option $arg\"
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
