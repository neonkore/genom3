#
# Copyright (c) 2010-2013 LAAS/CNRS
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
#                                           Anthony Mallet on Fri Jun 18 2010
#

namespace eval language::c++ {

    # --- filext -----------------------------------------------------------

    # Return the canonical file extensions in C++
    #
    proc fileext { kind } {
	if {$kind == "header"} { return ".h" } else { return ".cc" }
    }


    # --- comment ----------------------------------------------------------

    # Return a string that is a valid comment in C++.
    #
    proc comment { text } {
      return [concat "//" [join [split $text "\n"] "// "]]
    }


    # --- mapping ----------------------------------------------------------

    # Generate and return the C++ mapping of types matching the glob pattern
    #
    proc mapping { type } {
	foreach s [$type scope] {
	    # nested structs or union are defined by the enclosing type only
	    if {[lindex $s 0] ne "module"} { return "" }

	    append m "\nnamespace [lindex $s 1] \{"
	}

	switch -- [$type kind] {
	    {const}		{ append m [genconst $type] }
	    {enum}		{ append m [genenum $type] }
	    {struct}		{ append m [genstruct $type] }
	    {union}		{ append m [genunion $type] }
	    {typedef}		{ append m [gentypedef $type] }
            {sequence}		{ append m [gensequence $type] }

	    {forward struct}	-
	    {forward union}	{ append m [genforward $type] }

            {remote}		{ template fatal "fix mapping for remote" }

            default		{ return "" }
	}

	foreach s [$type scope] {
	    append m "\n\}\n"
	}

	set p ""
	if {[regexp {u?int(8|16|32|64)_t} $m]} {
	    append p "#include <stdint.h>\n"
	}
	if {[regexp {std::string} $m]} {
	    append p "#include <string>\n"
	}
	if {[regexp {std::vector<} $m]} {
	    append p "#include <vector>\n"
	}
	if {[regexp {genom3::bounded_string<} $m]} {
	    append p "#include <genom3/c++/idlstring.h>\n"
	}
	if {[regexp {genom3::bounded_vector<} $m]} {
	    append p "#include <genom3/c++/idlvector.h>\n"
	}

	return $p$m
    }


    # --- declarator -------------------------------------------------------

    # Return the C++ mapping of a type declarator.
    #
    proc declarator { type {var {}} } {
	switch -- [$type kind] {
	    {boolean}			{ set d "bool" }
	    {unsigned short}		{ set d "uint16_t" }
	    {short}			{ set d "int16_t" }
	    {unsigned long}		{ set d "uint32_t" }
	    {unsigned long long}	{ set d "uint64_t" }
	    {long}			{ set d "int32_t" }
	    {long long}			{ set d "int64_t" }
	    {float}			{ set d "float" }
	    {double}			{ set d "double" }
	    {char}			{ set d "int8_t" }
	    {octet}			{ set d "uint8_t" }
            {event}			{ set d "genom::event" }
	    {any}			{ error "type any not supported yet" }

	    {const}			-
	    {enum}			-
	    {struct}			-
	    {union}			-
	    {typedef}			{ set d [cname [$type fullname]] }

	    {enumerator}		-
	    {struct member}		-
	    {union member}		-
	    {forward struct}		-
	    {forward union}		{ set d [declarator [$type type]] }

	    {string} {
		if {[catch { $type length } l]} {
		    set d "std::string"
		} else {
		    set d "genom3::bounded_string<$l>"
		}
	    }

	    {array} {
		if {[catch { $type length } l]} { set l {} }
		set d "[declarator [$type type]]"
                set b [string first \[ $d]
                if {$b < 0} {
                  append d "\[$l\]"
                } else {
                  set d "[string range $d 0 $b-1]\[$l\][string range $d $b end]"
                }
	    }

	    {sequence} {
		set t [$type type]
		if {[catch {$type length} l]} {
		    set d "std::vector< [declarator $t] >"
		} else {
		    set d "genom3::bounded_vector< [declarator $t],$l >"
		}
	    }

	    default {
		template fatal "internal error: unhandled type '[$type kind]'"
	    }
	}

	if {[string length $var] > 0} { set d [cdecl $d $var] }
	return $d
    }


    # --- address ----------------------------------------------------------

    # Return the C++ mapping of a variable address.
    #
    proc address { type {var {}} } {
      switch -- [[$type final] kind] {
	{array}	{ return $var }
	default	{ return "&($var)" }
      }
    }


    # --- dereference ------------------------------------------------------

    # Return the C++ mapping of a variable dereference.
    #
    proc dereference { type {var {}} } {
      switch -- [[$type final] kind] {
	{array} { return $var }
	default	{ return "(*$var)" }
      }
    }


    # --- argument --------------------------------------------------------

    # Return the C++ mapping for declaring a parameter.
    #
    proc argument { type kind {var {}} } {
      switch -- $kind {
	{value}		{
	  return "const [declarator $type &$var]"
	}
	{reference}	{
	  switch -- [[$type final] kind] {
	    {array} {
	      return [declarator $type $var]
	    }
	    default {
	      return [declarator $type &$var]
	    }
	  }
	}
	default	{
	  template fatal \
	      "unknown argument kind \"$kind\": must be value or reference"
	}
      }
    }


    # --- pass -------------------------------------------------------------

    # Return the C++ mapping for passing a variable.
    #
    proc pass { type kind {var {}} } {
      switch -- $kind {
	{value}		{
	  return $var
	}
	{reference}	{
	  return $var
	}
	default	{
	  template fatal \
	      "unknown argument kind \"$kind\": must be value or reference"
	}
      }
    }


    # --- signature --------------------------------------------------------

    # Return the C++ signature of a codel
    #
    proc signature { codel {symchar " "} {location 0}} {
	set ret genom::event
	set sym [cname $codel]
	set arg [list]
	foreach p [$codel parameters] {
	    set a ""
	    switch -- [$p dir] {
		"in" {
		  append a [parameter value [$p type] [$p name]]
		}
		"inport" {
                  if {"handle" in [[$p port] kind]} {
                    append a [parameter reference [$p type] [$p name]]
                  } else {
                    append a [parameter value [$p type] [$p name]]
                  }
		}
		default	{
		  append a [parameter reference [$p type] [$p name]]
		}
	    }
	    lappend arg $a
	}
        if {[llength $arg] == 0} { set arg "void" }

        set m ""
        if {$location} {
	  set m [genloc $codel]\n
	}
        append m [join [list $ret ${sym}([join $arg {, }])] $symchar]
	return $m
    }


    # --- member -----------------------------------------------------------

    # Return the C construction to access members of type
    #
    proc member { type mlist } {
      if {[llength $mlist] == 0} return ""
      set mlist [lassign $mlist member]

      switch -- [$type kind] {
        {boolean}				-
        {unsigned short} - {short}		-
        {unsigned long} - {long}		-
        {unsigned long long} - {long long}	-
        {float} - {double}			-
        {char} - {octet}			-
        {enum} - {enumerator} - {string} {
          # basic types: no member
          template fatal "[$type kind] has no member"
        }

        {array} - {sequence} {
          append access "\[$member\]"
          set type [$type type]
        }

        {struct} {
          set m [$type members $member]
          if {[llength $m] == 0} {
            template fatal "[$type kind] has no member $member"
          }
          append access ".$member"
          set type $m
        }

        {union} {
          set m [$type members $member]
          if {[llength $m] == 0} {
            template fatal "[$type kind] has no member $member"
          }
          append access "._u.$member"
          set type $m
        }

        {forward struct} - {forward union}	-
        {struct member} - {union member}	-
        {typedef} {
          set type [$type type]
          set mlist [concat $member $mlist]
        }

        default { error "internal error: unhandled type '[$type kind]'" }
      }

      if {[llength $mlist] > 0} {
        append access [member $type $mlist]
      }

      return $access
    }


    # --- invoke -----------------------------------------------------------

    # Return the C++ invocation of a codel
    #
    proc invoke { codel params } {
	set sym [cname $codel]
	return "${sym}([join $params {, }])"
    }


    # --- genconst ---------------------------------------------------------

    # Return the C++ mapping of a const
    #
    proc genconst { type } {
	set t [declarator [$type type] [cname [$type name]]]

	set v [$type value]
	switch [$type valuekind] {
	    bool	{ set v [expr {$v?"true":"false"}] }
	    char	{ set v "'$v'" }
	    string	{ set v "\"[string map {\" \\\"} $v]\"" }
	    enum	{ set v [cname $v] }
	}

	append m [genloc $type]
	append m "\nconst $t = $v;"
	return [guard $m [cname [$type fullname]]]
    }


    # --- genenum ----------------------------------------------------------

    # Return the C++ mapping of an enum.
    #
    proc genenum { type } {
	append m [genloc $type]

	append m "\nenum [cname [$type name]] \{"
	set v -1
	foreach e [$type members] {
	    append m [genloc $e]
	    append m "\n  [cname [$e name]] = \t[incr v],"
	}
	append m "\n  _unused = \t0xffffffff"
	append m "\n\};"

	return [guard $m [cname [$type fullname]]]
    }


    # --- genstruct --------------------------------------------------------

    # Return the C++ mapping of a struct. The typedef is output first to handle
    # recursive structures with anonymous sequences.
    #
    proc genstruct { type } {
	set nested [list]
	set n [cname [$type name]]

	append m [genloc $type]
	append m "\nstruct $n;"

	set f "\nstruct $n {"
	foreach e [$type members] {
	    append s [genloc $e]
	    append s "\n [declarator $e [cname [$e name]]];"

	    # handle nested type definitions
	    set ntype [$e type]
	    lassign [lindex [$ntype scope] end] skind sname
	    if {$skind eq "struct" && $sname eq [$type name]} {
		while {[$ntype kind] eq "array"} { set ntype [$ntype type] }
		if {![catch {$ntype fullname} nname]} {
		    if {[lsearch $nested $nname] < 0} {
			switch -- [$ntype kind] {
			    struct { append f "\n[genstruct $ntype]" }
			    union  { append f "\n[genunion $ntype]" }
			    enum   { append f "\n[genenum $ntype]" }
			}
			lappend nested $nname
		    }
		}
	    }
	}
	append s "\n};"

	set n [cname [$type fullname]]
	return [guard $m $n][guard $f$s "${n}_definition"]
    }


    # --- genunion ---------------------------------------------------------

    # Return the C++ mapping of a union. The typedef is output first to handle
    # recursive and forward decls.
    #
    proc genunion { type } {
	set nested [list]
	set n [cname [$type name]]
	set discr [$type discriminator]

	append m [genloc $type]
	append m "\nstruct $n;"

	set f "\nstruct $n {"
	append s [genloc $discr]
	append s "\n [declarator $discr] _d;"
	append s "\n union {"
	foreach e [$type members] {
	    append s [genloc $e]
	    append s "\n  [declarator $e [cname [$e name]]];"

	    # C++ does not allow union members with non-trivial ctors
	    if {![has_trivial_ctor $e]} {
		set emsg ""
		if {![catch {$e loc} tloc]} {
		    lset tloc 0 [file tail [lindex $tloc 0]]
		    append emsg [join [lrange $tloc 0 1] :]
		    append emsg {: }
		}
		append emsg "member '[$e name]' of type [[$e type] kind]"
		append emsg " not allowed in C++"
		template fatal $emsg
	    }

	    # handle nested type definitions
	    set ntype [$e type]
	    lassign [lindex [$ntype scope] end] skind sname
	    if {$skind eq "union" && $sname eq [$type name]} {
		while {[$ntype kind] eq "array"} { set ntype [$ntype type] }
		if {![catch {$ntype fullname} nname]} {
		    if {[lsearch $nested $nname] < 0} {
			switch -- [$ntype kind] {
			    struct { append f "\n[genstruct $ntype]" }
			    union  { append f "\n[genunion $ntype]" }
			    enum   { append f "\n[genenum $ntype]" }
			}
			lappend nested $nname
		    }
		}
	    }
	}
	append s "\n } _u;"
	append s "\n};"

	set n [cname [$type fullname]]
	return [guard $m $n][guard $f$s "${n}_definition"]
    }


    # --- genforward -------------------------------------------------------

    # Return the C++ mapping of forward declaration.
    #
    proc genforward { type } {
	append m [genloc $type]
	append m "\nstruct [cname [$type name]];"
	return [guard $m [cname [$type fullname]]]
    }


    # --- gentypedef -------------------------------------------------------

    # Return the C++ mapping of a typedef.
    #
    proc gentypedef { type } {
	append m [genloc $type]
	append m "\ntypedef [declarator [$type type] [cname [$type name]]];"
	return [guard $m [cname [$type fullname]]]
    }


    # --- genloc -----------------------------------------------------------

    # Return the C++ mapping of a source location
    #
    proc genloc { type } {
	if {[catch {$type loc} tloc]} { return "" }

	set f [lindex $tloc 0]
	set l [lindex $tloc 1]
	return "\n#line $l \"[string map {\" \\\"} $f]\""
    }


    # --- has_trivial_ctor -------------------------------------------------

    # Check if the C++ mapping for type has a trivial constructor. Return true
    # if yes.
    #
    proc has_trivial_ctor { type } {
	switch -- [$type kind] {
	    {sequence}		-
	    {string}	{ return false }

	    {forward struct}	-
	    {forward union}	-
	    {struct member}	-
	    {union member}	-
	    {array}		-
	    {typedef}	{ return [has_trivial_ctor [$type type]] }

	    {struct}		-
	    {union}	{
		foreach m [$type members] {
		    if {![has_trivial_ctor $m]} { return false }
		}
		return true
	    }

	    default	{ return true }
	}
    }


    # --- guard -----------------------------------------------------------

    # Protect a string with guards
    #
    proc guard { string name } {
	set name [string map {:: _} $name]
	append m "\n#ifndef _${name}_type"
	append m "\n# define _${name}_type"
	append m $string
	append m "\n#endif /* _${name}_type */"
	append m "\n"
	return $m
    }


    # --- cdecl ------------------------------------------------------------

    # Given a type string and a name, declare name of type type. The subtlety
    # comes from arrays, because name should preceede the brackets, if any.
    #
    proc cdecl { type name } {
	set b [string first \[ $type]
	if { $b < 0 } {
	    if {[string index $type end] eq "*"} {
		return "$type$name"
	    }
	    return "$type $name"
	}

	return "[string range $type 0 $b-1] $name[string range $type $b end]"
    }


    # --- cname ------------------------------------------------------------

    # Cannonical name of an object or string in C++.
    # Remove leading :: and map other :: to _
    #
    proc cname { object } {
	if {![catch {$object class} class]} {
	    switch -- $class {
              codel	{ set object [$object name] }
              type	{ set object [$object fullname] }
	    }
	}

	if { [string first :: $object] == 0 } {
	    set object [string range $object 2 end]
	}
	return [string map {{ } _ + x} $object]
    }
}
