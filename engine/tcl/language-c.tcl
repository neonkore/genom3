#
# Copyright (c) 2010-2011 LAAS/CNRS
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
#                                           Anthony Mallet on Tue Jan 26 2010
#
package require Tcl 8.5

namespace eval language::c {

    # --- filext -----------------------------------------------------------

    # Return the canonical file extensions in C
    #
    proc fileext { kind } {
	if {$kind == "header"} { return ".h" } else { return ".c" }
    }


    # --- comment ----------------------------------------------------------

    # Return a string that is a valid comment in C.
    #
    proc comment { text } {
	regsub -all "\n(?=.)" "/*${text}" "\n *" text
	set text "${text} */"
	return $text
    }


    # --- mapping ----------------------------------------------------------

    # Generate and return the C mapping of types matching the glob pattern
    #
    proc mapping { pattern } {
	set m ""
	foreach t [dotgen types $pattern] {
	    append m [gentype $t]
	}

	set p ""
	if {[regexp {u?int(8|16|32|64)_t} $m]} {
	    append p "#include <stdint.h>\n"
	}
	if {[regexp {bool} $m]} {
	    append p "#include <stdbool.h>\n"
	}
	if {[regexp {sequence} $m]} {
	    append p "#include <genom3/c/idlsequence.h>\n"
	}

	return $p$m
    }


    # --- gentype-prolog ---------------------------------------------------

    # Return the required C includes for mappings
    #
    proc gentype-prolog { } {
	return "\n#include <stdbool.h>\n#include <stdint.h>\n"
    }


    # --- gentype -----------------------------------------------------------

    # Return the C mapping of type.
    #
    proc gentype { type } {
	switch -- [$type kind] {
	    {const}		{ return [genconst $type] }
	    {enum}		{ return [genenum $type] }
	    {struct}		{ return [genstruct $type] }
	    {union}		{ return [genunion $type] }
	    {typedef}		{ return [gentypedef $type] }

	    {forward struct}	-
	    {forward union}	{ return [genforward $type] }

	    {enumerator}	-
	    {struct member}	-
	    {union member}	{ return "" }
	}

	template fatal \
	    "internal error: unhandled type '[$type kind]' at top-level"
    }


    # --- declarator -------------------------------------------------------

    # Return the C mapping of a type declarator.
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
	    {char}			{ set d "char" }
	    {octet}			{ set d "uint8_t" }
	    {any}			{ error "type any not supported yet" }

	    {const}			-
	    {enum}			-
	    {enumerator}		-
	    {struct}			-
	    {union}			-
	    {typedef}			{ set d [cname [$type fullname]] }

	    {struct member}		-
	    {union member}		-
	    {forward struct}		-
	    {forward union}		{ set d [declarator [$type type]] }

	    {string} {
		if {[catch { $type length } l]} {
		    set d "char *"
		} else {
		    set d "char\[$l\]"
		}
	    }

	    {array} {
		if {[catch { $type length } l]} { set l {} }
		set d "[declarator [$type type]]\[$l\]"
	    }

	    {sequence} {
		set t [$type type]

		set d "sequence"
		if {![catch {$type length} l]} { append d $l }

		if {[catch {$t fullname}]} {
		    append d [cname " [cname [$t kind]]"]
		} else {
		    append d [cname " [cname [$t fullname]]"]
		}
		if {![catch {$t length} l]} { append d $l }
		while {[$t kind] == "sequence"} {
		    set t [$t type]
		    if {[catch {$t fullname}]} {
			append d [cname " [cname [$t kind]]"]
		    } else {
			append d [cname " [cname [$t fullname]]"]
		    }
		    if {![catch {$t length} l]} { append d $l }
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

    # Return the C mapping of a variable address.
    #
    proc address { type {var {}} } {
	switch -- [[$type final] kind] {
	    {string} -
	    {array}	{ return $var }
	    default	{ return "&($var)" }
	}
    }


    # --- dereference ------------------------------------------------------

    # Return the C mapping for dereferencing a pointer on a variable.
    #
    proc dereference { type {var {}} } {
	switch -- [[$type final] kind] {
	    {string} -
	    {array}	{ return $var }
	    default	{ return "(*$var)" }
	}
    }


    # --- parameter --------------------------------------------------------

    # Return the C mapping for declaring a parameter.
    #
    proc parameter { kind type {var {}} } {
      switch -- $kind {
	{value}		{
	   switch -- [[$type final] kind] {
	       {string} -
	       {array} {
		   return "const [declarator $type $var]"
	       }
	       default {
		   return "const [declarator $type *$var]"
	       }
	   }
	}
	{reference}	{
	  switch -- [[$type final] kind] {
	    {string} -
	    {array} {
	      if {[catch { [$type final] length }]} {
		return [declarator $type *$var]
	      } else {
		return [declarator $type $var]
	      }
	    }
	    default {
	      return [declarator $type *$var]
	    }
	  }
	}
	default	{
	  template fatal \
	      "unknown argument kind \"$kind\": must be value or reference"
	}
      }
    }


    # --- argument ---------------------------------------------------------

    # Return the C mapping for passing a variable.
    #
    proc argument { kind type {var {}} } {
      switch -- $kind {
	{value}		{
	  return [address $type $var]
	}
	{reference}	{
	  switch -- [[$type final] kind] {
	    {string} -
	    {array} {
	      if {[catch { [$type final] length }]} {
		return "&($var)"
	      } else {
		return $var
	      }
	    }
	    default {
	      return [address $type $var]
	    }
	  }
	}
	default	{
	  template fatal \
	      "unknown argument kind \"$kind\": must be value or reference"
	}
      }
    }


    # --- signature --------------------------------------------------------

    # Return the C signature of a codel
    #
    proc signature { codel {symchar " "} {location 0}} {
	set ret [declarator [$codel return]]
	set sym [cname $codel]
	set arg [list]
	foreach p [$codel parameters] {
	    set a ""
	    switch -- [$p dir] {
		"in" - "inport"	{
		  append a [parameter value [$p type] [$p name]]
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
	  append m [genloc $codel]\n
	}
        append m [join [list $ret ${sym}([join $arg {, }])] $symchar]
	return $m
    }


    # --- member -----------------------------------------------------------

    # Return the C construction to access members of type
    #
    proc member { type members } {
	if {[llength $members] == 0} return ""
	set members [lassign $members member]

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

	    {array} {
		if {![string is digit [string index $member 0]]} {
		    template fatal "[$type kind] has no member $member"
		}
		append access "\[$member\]"
		set type [$type type]
	    }

	    {sequence} {
		if {![string is digit [string index $member 0]]} {
		    template fatal "[$type kind] has no member $member"
		}
		append access "._buffer\[$member\]"
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
		set members [concat $member $members]
	    }

	    default { error "internal error: unhandled type '[$type kind]'" }
	}

	if {[llength $members] > 0} {
	    append access [member $type $members]
	}

	return $access
    }


    # --- invoke -----------------------------------------------------------

    # Return the C invocation of a codel
    #
    proc invoke { codel params } {
	set sym [cname $codel]
	return "${sym}([join $params {, }])"
    }


    # --- genconst ---------------------------------------------------------

    # Return the C mapping of a const
    #
    proc genconst { type } {
	set n [cname [$type fullname]]
	set t [declarator [$type type]]

	set v [$type value]
	switch [$type valuekind] {
	    bool	{ set v [expr {$v?"true":"false"}] }
	    char	{ set v "'$v'" }
	    string	{ set v "\"[string map {\" \\\"} $v]\"" }
	    enum	{ set v [cname $v] }
	}

	append m [genloc $type]
	append m "\n# define $n ((const $t)($v))"
	return [guard $m $n]
    }


    # --- genenum ----------------------------------------------------------

    # Return the C mapping of an enum.
    #
    proc genenum { type } {
	set n [cname [$type fullname]]

	append m [genloc $type]
	append m "\ntypedef uint32_t $n;"
	set v -1
	foreach e [$type members] {
	    append m [genloc $e]
	    append m "\n# define [cname [$e fullname]]\t([incr v])"
	}
	return [guard $m $n]
    }


    # --- gensequence ------------------------------------------------------

    # Return the C mapping of a sequence.
    #
    proc gensequence { type } {
	set n [declarator $type]

	set f ""
	if {[[$type type] kind] eq "sequence"} {
	    append f [gensequence [$type type]]
	}
	append m [genloc $type]
	append m "\ntypedef struct $n {"
	if {[catch {$type length} l]} {
	    append m "\n  uint32_t _maximum, _length;"
	    append m "\n  [declarator [$type type] *_buffer];"
	    append m "\n  void (*_release)(void *_buffer);"
	} else {
	    append m "\n  const uint32_t _maximum;"
	    append m "\n  uint32_t _length;"
	    append m "\n  [declarator [$type type] _buffer\[$l\]];"
	}
	append m "\n} $n;"
	return $f[guard $m $n]
    }


    # --- genstruct --------------------------------------------------------

    # Return the C mapping of a struct. The typedef is output first to handle
    # recursive structures with anonymous sequences.
    #
    proc genstruct { type } {
	set n [cname [$type fullname]]

	append m [genloc $type]
	append m "\ntypedef struct $n $n;"

	set f ""
	set s "\nstruct $n {"
	foreach e [$type members] {
	    append s [genloc $e]
	    append s "\n [declarator $e [cname [$e name]]];"
	    if {[[$e type] kind] == "sequence"} {
		append f [gensequence [$e type]]
	    }
	}
	append s "\n};"
	return [guard $m $n]$f[guard $s "${n}_definition"]
    }


    # --- genunion ---------------------------------------------------------

    # Return the C mapping of a union. The typedef is output first to handle
    # recursive and forward decls.
    #
    proc genunion { type } {
	set n [cname [$type fullname]]
	set discr [$type discriminator]

	append m [genloc $type]
	append m "\ntypedef struct $n $n;"

	set f ""
	append s "\nstruct $n {"
	append s [genloc $discr]
	append s "\n [declarator $discr] _d;"
	append s "\n union {"
	foreach e [$type members] {
	    append s [genloc $e]
	    append s "\n  [declarator $e [cname [$e name]]];"
	    if {[[$e type] kind] == "sequence"} {
		append f [gensequence [$e type]]
	    }
	}
	append s "\n } _u;"
	append s "\n};"
	return [guard $m $n]$f[guard $s "${n}_definition"]
    }


    # --- genforward -------------------------------------------------------

    # Return the C mapping of forward declaration.
    #
    proc genforward { type } {
	set n [cname [$type fullname]]

	append m [genloc $type]
	append m "\ntypedef struct $n $n;"
	return [guard $m $n]
    }


    # --- gentypedef -------------------------------------------------------

    # Return the C mapping of a typedef.
    #
    proc gentypedef { type } {
	set n [cname [$type fullname]]

	set f ""
	if {[[$type type] kind] == "sequence"} {
	    append f [gensequence [$type type]]
	}

	append m [genloc $type]
	append m "\ntypedef [declarator [$type type] $n];"
	return $f[guard $m $n]
    }


    # --- genloc -----------------------------------------------------------

    # Return the C mapping of a source location
    #
    proc genloc { type } {
	if {[catch {$type loc} tloc]} { return "" }

	set f [lindex $tloc 0]
	set l [lindex $tloc 1]
	return "\n#line $l \"[string map {\" \\\"} $f]\""
    }


    # --- guard -----------------------------------------------------------

    # Protect a string with guards
    #
    proc guard { string name } {
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

    # Cannonical name of an object or string in C.
    # Remove leading :: and map other :: to _
    #
    proc cname { object } {
	if {![catch {$object class} class]} {
	    switch -- $class {
		codel	{
                  set object [$object name]
		}
	    }
	}

	if { [string first :: $object] == 0 } {
	    set object [string range $object 2 end]
	}

	return [string map {:: _ { } _ + x} $object]
    }
}
