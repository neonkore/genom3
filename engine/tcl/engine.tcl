#
# Copyright (c) 2010-2014,2017 LAAS/CNRS
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
    #/
    # `engine` TCL engine command
    # ---------------------------

    # debug mode
    variable debug		[dotgen genom debug]

    # informative message
    variable silent		off

    # overwrite existing files
    variable overwrite		off

    # move-if-change files (takes precedence over 'overwrite')
    variable move-if-change	on

    # merge-if-change files (takes precedence over 'overwrite')
    variable merge-if-change	off

    # available engine modes
    variable modes {overwrite move-if-change merge-if-change silent debug}

    # default merge tool (builtin interactive)
    variable merge-tool	{interactive}

    # default output directory
    variable outdir	{}

    # markups
    variable markup
    array set markup {
	open	{<(["'])}	close	{(['"])>}
	full	{<(["'])(.*?)(['"])>}
    }

    # interpreter evaluating templates substitutions. Objects from the .gen as
    # well as the commands 'dotgen', 'language' and 'object' are available,
    # (but not the 'template' commands that are reserved to the toplevel
    # template.tcl file). 'puts' is redefined to catch template output within
    # <' '> markups.
    interp share {} stderr slave
    slave hide puts
    slave alias puts slave invokehidden puts
    slave alias dotgen dotgen
    slave alias language language
    slave alias object object


    # --- mode -------------------------------------------------------------

    #/
    # [[engine_mode]]
    # === `engine mode`: Engine output configuration
    # ****
    # `engine mode` ['[`+-`]modespec'] ...
    # ****
    #
    # Configures various engine operating modes. `engine mode` can be invoked
    # without argument to retrieve the current settings for all supported
    # modes. The command can also be invoked with one or more mode
    # specification to set these modes (see 'modespec' argument below).
    #
    # ==== Arguments
    # 'modespec'::
    # A mode specification string. If `mode` string is prefixed with a
    # dash (`-`), it is turned off. If mode is prefixed with a plus
    # (`+`) or not prefixed, it is turned on. Supported 'modespec'
    # are:
    #   `overwrite`::: when turned on, newly generated files will overwrite
    #   existing files without warning. When turned off, the engine will stop
    #   with an error if a newly generated file would overwrite an existing
    #   file. `overwrite` is by default off.
    #
    #   'move-if-change'::: when turned on, an existing file with the same
    #   content as a newly generated file will not be modified (preserving the
    #   last modification timestamp). When off, files are systematically
    #   updated. `move-if-change` is on by default.
    #
    #   'merge-if-change'::: when turned on, existing destination files will be
    #   merged with new content by the engine, instead of being overwritten
    #   (<<engine_merge_tool,`engine merge tool>>). `merge-if-change` is off by
    #   default.
    #
    #   'silent'::: when on, this mode avoids scattering standard output with
    #   informative messages from the code generator.
    #
    #   'debug'::: when on, this mode preserves temporary files and tcl
    #   programs generated in the temporary directory. Useful only for
    #   debugging the template.
    #
    # ==== Return value
    #
    # When called without arguments, the command returs the current
    # configuration of all engine modes.
    #
    # ==== Example
    # ****
    # engine mode -overwrite +move-if-change
    # ****
    #
    proc mode { args } {
	variable modes

	if {[llength $args] == 0} {
	    foreach m $modes {
		variable $m
		if {[set $m]} { lappend r $m } else { lappend r -$m }
	    }
	    return $r
	}

	foreach m $args {
	    switch -- [string index $m 0] {
		+	{ set v on;  set m [string replace $m 0 0] }
		-	{ set v off; set m [string replace $m 0 0] }
		default { set v on; }
	    }

	    if {[lsearch $modes $m] < 0} {
		template fatal "unknown mode $m: should be [join $modes {, }]"
	    }

	    variable $m
            if {([set $m] && !$v) || (![set $m] && $v)} {
              set $m $v
              template message "$m mode $v"
	    }
	}
    }
    namespace export mode


    # --- merge-tool -------------------------------------------------------

    #/
    # [[engine_merge_tool]]
    # === `engine merge-tool`: Automatic merge of generated content
    # ****
    # `engine merge-tool` 'tool'
    # ****
    #
    # Changes the engine merge tool. When the engine is in 'merge-if-change'
    # mode (see <<engine_mode,`engine mode`>>), a merge tool is inkoked with
    # the two conflicting versions of the destination file. If the merge tools
    # exits successfuly, the generated file is replaced by the merged version.
    #
    # There are two builtin tools: `interactive` and `auto`. `interactive`
    # interactively prompts the user for each patch to be applied to merge the
    # final destination. The user can accept or reject the patch, or leave the
    # destination file unchanged. The `auto` builtin tool automatically merges
    # the two files and places conflict markers (`<<<<<<<` and `>>>>>>>`) were
    # appropriate in the destination file.
    #
    # ==== Arguments
    # 'tool'::
    # The path to the merge tool executable (e.g. `meld`), or one of
    # the builtin keywords `interactive` or `auto`.
    #
    proc merge-tool { tool } {
      variable merge-tool
      set merge-tool $tool
    }
    namespace export merge-tool


    # --- chdir ------------------------------------------------------------

    #/
    # [[engine_chdir]]
    # === `engine chdir`: Change output directory
    # ****
    # `engine chdir` 'dir'
    # ****
    #
    # Change the engine output directory. By default, files are generated in
    # the current directory. This command can be used to generate output in
    # any other directory.
    #
    # ==== Arguments
    # 'dir'::
    # The new output directory, absolute or relative to the current working
    # directory.
    #
    proc chdir { d } {
	variable outdir
	if {$outdir != $d} {
	    set outdir $d
	    template message "set output directory to '$d'"
	}
    }
    namespace export chdir


    # --- pwd --------------------------------------------------------------

    #/
    # === `engine pwd`: Get current output directory
    # ****
    # `engine pwd`
    # ****
    #
    # ==== Return value
    # The current engine output directory.
    #
    proc pwd { } {
	variable outdir
	if {$outdir eq ""} { return . } else { return $outdir }
    }
    namespace export pwd


    # --- args -------------------------------------------------------------

    # Set argc/argv in slave interpreter and return previous value of argv.
    #
    proc args { l } {
	if {[catch {slave eval [list set argv]} prev]} {
	    set prev [list]
	}
	if {[catch {
	    slave eval [list set argc [llength $l]]
	    slave eval [list set argv $l]
	} m options]} {
	    return -options $options $m
	}

	return $prev
    }


    # --- open -------------------------------------------------------------

    # Open file or string (via a reflected channel) for reading or writing.
    # Return a tcl channel. Type must be 'file' or 'string', dst is either a
    # file name (file/read or write) a string (string/read) or a global
    # variable name (string/write).
    #
    proc open { type dst mode } {
	switch -glob -- $type|$mode {
	    string|read {
		set ::_tmp $dst
		return [chan create read [namespace code {strchan _tmp}]]
	    }
	    string|write {
		return [chan create write [namespace code [list strchan $dst]]]
	    }
	    file|read {
              set dst [file join [dotgen template dir] $dst]
              message "reading $dst"
              set c [::open $dst r]
              fconfigure $c -encoding utf-8
              return $c
	    }
	    file|write {
		# record a move on close
		variable moc
		variable outdir

		set t [mktemp]
		set c [::open $t w]
                fconfigure $c -encoding utf-8
		dict set moc $c [list $t [file join $outdir $dst]]

		template message "generating $dst in $t"
		return $c
	    }
	}
	template fatal "invalid channel type $type: must be file or string"
    }


    # --- close ------------------------------------------------------------

    # Close channel. If a "move on close" was recorded by open, execute it.
    # If permissions are specified, apply them.
    #
    variable moc [dict create]
    proc close { channel {perm {}} } {
	variable moc
	variable silent
	variable overwrite
	variable move-if-change
	variable merge-if-change
	variable merge-tool

	::close $channel
	if {![dict exists $moc $channel]} return

	lassign [dict get $moc $channel] tmp dst
	dict unset moc $channel

	if {${move-if-change} && [file exists $dst]} {
	    set t [::open $tmp r]; set d [::open $dst r]
	    while { "[read $t 4096]" == "[read $d 4096]" } {
		if { [eof $t] && [eof $d] } {
		    close $t; close $d
		    if {[llength $perm]} {
		      file attributes $dst -permissions $perm
		    }
                    if {!$silent} { puts "$dst is up-to-date" }
		    return
		}
	    }
	    ::close $t; ::close $d
	}

        if {${merge-if-change} && [file exists $dst]} {
          if {!$silent} { puts "merging $dst" }
          if {[switch ${merge-tool} {
            interactive	{ merge::auto $tmp $dst on}
            auto	{ merge::auto $tmp $dst off}
            default	{
              merge::custom ${merge-tool} $tmp $dst
            }
          }]} {
            error "stop: could not merge changes in $dst"
          }

          if {[llength $perm]} {
            file attributes $dst -permissions $perm
          }
          return
        }

	if {!$overwrite && [file exists $dst]} {
          error [subst [join {
            "stop: file $dst exists and would be overwritten.\n"
            "See [dotgen template name] -h for available merge or overwrite"
            " modes."
          } {}]]
	}

	if {[file exists $dst]} {
          if {!$silent} { puts "overwriting $dst" }
	} else {
          if {!$silent} { puts "creating $dst" }
	}
	file mkdir [file dirname $dst]
	file copy -force $tmp $dst
        if {[llength $perm]} {
	  file attributes $dst -permissions $perm
	}
	return
    }


    # --- process ----------------------------------------------------------

    # Read template source file from 'in' channel and write parsing result to
    # 'out' channel.
    #
    # Text outside markers is replaced by a TCL program printing the text and
    # <''> and <""> are replaced by the appropriate TCL code (verbatim for <''>
    # and [subst] for <"">. A temporary program is built in a string and then
    # evaluated in the 'slave' interpreter.
    #
    # <''> always produce an empty string, but code inside is evaluated and
    # might invoke 'puts' to produce some output, while <""> is replaced by the
    # result of its evaluation by 'subst'.
    #
    proc process { src in out } {
      variable silent
      variable debug
      variable markup

      # initalize template program source code file
      set tpath [mktemp]
      set tfile [::open $tpath w]
      set linenum 1

      if {!$silent && $debug} {
        puts "generating template code for $src in $tpath"
      }

      # read source and build program
      while { ![eof $in] } {
        # read input by chunks of 4k, and avoid stopping on a tag limit
        append raw [read $in 4096]
        while { [string equal {<} [string index $raw end]] } {
          if [eof $in] break
          append raw [read $in 1]
        }

        # look for complete tags - x is the full match including markers, o
        # and c are the opening and closing markers and t the enclosed
        # text
        while {[regexp $markup(full) $raw x o t c]} {

          # get match indices (x is overwritten)
          regexp -indices $markup(full) $raw x io it ic

          # no \n has been discarded yet
          set nldiscarded 0

          # flush raw data before opening tag, if any
          if {[lindex $x 0] > 0} {

            set notag [string range $raw 0 [lindex $x 0]-1]

            if [regexp -indices $markup(open) $notag l] {
              incr linenum [linecount $notag 0 [lindex $l 0]]
              error "$src:$linenum: missing closing tag"
            }
            if [regexp -indices $markup(close) $notag l] {
              incr linenum [linecount $notag 0 [lindex $l 0]]
              error "$src:$linenum: missing opening tag"
            }

            # update current line number in the template file
            incr linenum [linecount $notag]

            # if the character immediately preceeding the opening <'
            # tag  is a \n, it is discarded.
            if {$o == "'" && [string index $notag end] == "\n"} {
              set notag [string range $notag 0 end-1]
              incr nldiscarded
            }

            # output raw data. One cannot use [list] to quote the text, because
            # in case of unbalanced braces, tcl quotes everything and, in
            # particular, newlines as \n, thus messing the line counting wrt.
            # the original code.
            if {$notag != ""} {
              set notag [string map {\{ \\\{ \} \\\} \\ \\\\} $notag]
              puts -nonewline $tfile ";\2' {$notag} '\3;"
            }
            if {$nldiscarded} { puts -nonewline $tfile "\n" }
          }

          # generate tag program
          switch -- $o$c {
            {''} {
              puts -nonewline $tfile $t
            }
            {""} {
              set t [string map {\{ \\\{ \} \\\} \\ \\\\} $t]
              puts -nonewline $tfile ";\2'' {$t} ''\3;"
            }
            default {
              if {[string equal $o $c]} {
                error "$src:$linenum: unknown tag '<$o'"
              } else {
                error "$src:$linenum: unbalanced tags '<$o' and '$c>'"
              }
            }
          }

          # update current line number in the template file
          incr linenum [linecount $t]

          # discard processed source text - if the character immediately
          # following the closing '> tag is a \n, it is discarded, except
          # if a similar \n was discarded before the tag.
          if {!$nldiscarded &&
              $c == "'" && [string index $raw [lindex $x 1]+1] == "\n"} {
            lset x 1 [expr [lindex $x 1] + 1]
            puts -nonewline $tfile "\n"
            incr linenum
          }
          set raw [string replace $raw 0 [lindex $x 1]]
        }

        # finished processing all tags in the current buffer, now check for
        # orphaned closing tag (error)
        if [regexp -indices $markup(close) $raw l] {
          error "$src:$linenum: missing opening tag"
        }

        # incomplete opening tag: must read more text
        if [regexp $markup(open) $raw] { continue }

        # concatenate remaining raw output (properly quoted)
        set raw [string map {\{ \\\{ \} \\\} \\ \\\\} $raw]
        puts -nonewline $tfile ";\2' {$raw} '\3;"
        set raw {}
      }

      # finished processing all tags in the file, now check for orphaned tags
      # (error)
      if {[string length $raw] > 0} {
        if [regexp -indices $markup(open) $raw l] {
          error "$src:$linenum: missing closing tag"
        }
        if [regexp -indices $markup(close) $raw l] {
          error "$src:$linenum: missing opening tag"
        }
      }
      close $tfile

      # execute template program in slave interpreter
      slave alias puts {*}[namespace code "slave-output $out"]
      slave eval {
        trace add execution gsource leavestep {slave-monitor [eline]}
      }
      set s [slave eval [list catch [list gsource $tpath] ::__m ::__ctx]]
      slave eval {
        trace remove execution gsource leavestep {slave-monitor [eline]}
      }
      slave alias puts slave invokehidden puts
      if {$s} {
        set line [slave eval {dict get $__ctx -errorline}]
        set file $src
        set err [list]
        catch {
          set bt [slave eval set ::__backtrace]
          foreach t [lreverse $bt] {
            if {[dict exists $t file]} {
              set file [dict get $t file]
              set line [dict get $t line]
              if {[file normalize $file] == [file normalize $tpath]} {
                set file $src
              }

              if {[dict get $t line] > 0} {
                set c [lindex [split [dict get $t cmd] \n] 0]
                set c [string map {
                  ;\2''\ \{    <\"    \2''\ \{    <\"
                  \}\ ''\3;    \">    \}\ ''\3    \">

                  ;\2'\ \{     '>     \2'\ \{     '>
                  \}\ '\3;     <'     \}\ '\3     <'
                } $c]
                if {[string length $c] > 40} {
                  set c [string range $c 0 40]...
                }
                set err [linsert $err 0 "$file:$line: $c"]
                incr line [expr {[dict get $t errorline]-1}]
              }
            }
          }
        }
        if {$debug} {
          set m "$file:$line: [slave eval {dict get $__ctx -errorinfo}]"
        } else {
          set m "$file:$line: [slave eval set ::__m]"
        }
        set err [linsert $err 0 $m]
        return -code error [join $err "\n called by: "]
      }

      return
    }


    # --- ' and " ----------------------------------------------------------

    # The two commands that output raw template source. They have obscure names
    # and a final dummy argument just to robustly (?) detect them and convert
    # them back to markups when printing error backtraces.
    #
    slave eval {
      proc "\2'" {text dummy} {
        puts -nonewline [subst -noc -nov $text]
      }

      proc "\2''" {text dummy} {
        if {[catch [list uplevel [list subst [subst -noc -nov $text]]] r]} {
          return -level 1 -code error $r
        }
        puts -nonewline $r
      }
    }


    # --- slave-monitor ----------------------------------------------------

    # Trace slave executions and construct a backtrace upon errors.
    #
    slave eval {
      set ::__backtrace {}

      proc slave-monitor {eline cmd code result op} {
        global __backtrace

        if {[lindex $cmd 0] == "catch"} return
        if {$code == 0} { set __backtrace {}; return }
        if {$code != 1} return

        set f [info frame -2]
        dict set f errorline $eline
        lappend __backtrace $f
        return
      }
    }


    # --- slave-output -----------------------------------------------------

    # An alias for puts in 'slave' interpreter. Catches all invokations of puts
    # with no channel, that are redirected to var, otherwise call
    # standard puts.
    #
    proc slave-output { out args } {
	if {[lindex $args 0] == "-nonewline" } {
	    set args [lrange $args 1 end]
	    set nl ""
	} else {
	    set nl "\n"
	}
	if {[llength $args] != 1} {
	    set args [linsert $args 0 slave invokehidden puts]
	    return [eval $args]
	}

	puts -nonewline $out [lindex $args 0]${nl}
	return
    }


    # --- linecount --------------------------------------------------------

    # Count the number of lines (\n) in the string, optinally starting from
    # index 'start' and stopping at index 'end'.
    #
    proc linecount { s { start 0 } { end end } } {
      return [regexp -all "\n" [string range $s $start $end]]
    }


    # --- mktemp -----------------------------------------------------------

    # Return a unique temporary file name
    #
    variable seed [expr int(rand()*9999)]
    proc mktemp { } {
	variable silent
	variable seed

	for { set r 1 } { 1 } { incr r } {
	    # 4 digits from pid, plus 4 random
	    set f [format %04d%04d [pid] [incr seed]]
	    set f [file join [dotgen template tmpdir] "template$f"]

	    if { ![file exists $f] } { break; }
	}
	if {!$silent && $r > 1} {
	    puts "$r iterations required for finding a temporary file"
	}
	return $f
    }
    namespace export mktemp


    # --- strchan ----------------------------------------------------------

    # Reflected channel working on string global variables.
    #
    variable strchans [dict create]

    proc strchan { var cmd args } {
	variable strchans

	switch -- $cmd {
	    initialize {
		upvar #0 $var src
		lassign $args c m

		if {$m == "write" } { set src {} }

		dict set strchans $c [encoding convertto identity $src]
		return {initialize finalize watch read write}
	    }

	    finalize {
		upvar #0 $var src
		lassign $args c

		set src [encoding convertfrom identity [dict get $strchans $c]]
		dict unset strchans $c
	    }

	    watch {}

	    read {
		lassign $args c n
		set buffer [dict get $strchans $c]

		set r [string range $buffer 0 $n-1]
		dict set strchans $c [string replace $buffer 0 $n-1]
		return $r
	    }

	    write {
		lassign $args c d
		dict append strchans $c $d
		return [string bytelength $d]
	    }
	}
    }

    namespace ensemble create
}
