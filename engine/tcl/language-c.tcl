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
#                                           Anthony Mallet on Tue Jan 26 2010
#
package require Tcl 8.5

namespace eval language::c {

    # --- gentype -----------------------------------------------------------

    # Return the C mapping of type.
    #
    proc fileext { kind } {
	if {$kind == "header"} { return ".h" } else { return ".c" }
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
	    {boolean}		{ set d "unsigned char" }
	    {unsigned short}	{ set d "unsigned short" }
	    {short}		{ set d "short" }
	    {unsigned long}	{ set d "unsigned long" }
	    {long}		{ set d "long" }
	    {float}		{ set d "float" }
	    {double}		{ set d "double" }
	    {char}		{ set d "char" }
	    {octet}		{ set d "unsigned char" }
	    {any}		{ error "type any not supported yet" }

	    {const}		-
	    {enum}		-
	    {enumerator}	-
	    {struct}		-
	    {union}		-
	    {typedef}		{ set d [cname [$type fullname]] }

	    {struct member}	-
	    {union member}	-
	    {forward struct}	-
	    {forward union}	{ set d [declarator [$type type]] }

	    {string}		{
		if {[catch { $type length } l]} {
		    set d "char *"
		} else {
		    set d "char\[$l\]"
		}
	    }

	    {array}		{
		if {[catch { $type length } l]} { set l {} }
		set d "[declarator [$type type]]\[$l\]"
	    }

	    {sequence}		{
		set t [$type type]

		set d "sequence"
		if {![catch {$type length} l]} { append d $l }

		append d [cname " [$t kind]"]
		if {![catch {$t length} l]} { append d $l }
		while {[$t kind] == "sequence"} {
		    set t [$t type]
		    append d [cname " [$t kind]"]
		    if {![catch {$t length} l]} { append d $l }
		}
	    }

	    default		{
		template fatal "internal error: unhandled type '[$type kind]'"
	    }
	}

	if {[string length $var] > 0} { set d [cdecl $d $var] }
	return $d
    }

    # Return the C mapping of a reference on the type declarator.
    #
    proc declarator& { type {var {}} } {
	switch -- [[$type final] kind] {
	    string	-
	    array	{ return [declarator $type $var] }

	    default	{ return [declarator $type *$var] }
	}
    }

    # Return the C mapping of a pointer on the type declarator.
    #
    proc declarator* { type {var {}} } {
	switch -- [[$type final] kind] {
	    {string}	{
		if {[catch { $type length }]} {
		    return [declarator $type $var]
		} else {
		    return [declarator $type *$var]
		}
	    }
	    {array}	{ return [declarator $type $var] }

	    default	{ return [declarator $type *$var] }
	}
    }


    # --- signature --------------------------------------------------------

    # Return the C signature of a codel
    #
    proc signature { codel {symchar " "}} {
	set ret [declarator [$codel return]]
	if {![catch {$codel service} s]} {
	    set sym [$s name]
	} elseif {![catch {$codel task} t]} {
	    set sym [$t name]
	}
	append sym _[cname [$codel name]]
	foreach p [$codel parameters] {
	    set a ""
	    switch -- [$p dir] {
		"in" - "inport"	{
		    append a "const [declarator& [$p type] [$p name]]"
		}
		default	{
		    append a [declarator* [$p type] [$p name]]
		}
	    }
	    lappend arg $a
	}
	return [join [list $ret ${sym}([join $arg {, }])] $symchar]
    }


    # --- genconst ---------------------------------------------------------

    # Return the C mapping of a const
    #
    proc genconst { type } {
	set n [cname [$type fullname]]
	set t [declarator [$type type]]

	set v [$type value]
	switch [$type valuekind] {
	    bool	{
		append m "\n#ifndef FALSE"
		append m "\n# define FALSE	(0)"
		append m "\n#endif /* FALSE */"
		append m "\n"
		append m "\n#ifndef TRUE"
		append m "\n# define TRUE	(1)"
		append m "\n#endif /* TRUE */"
		if {$v} { set v TRUE } else { set v FALSE }
	    }
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
	append m "\ntypedef unsigned long $n;"
	set v 0
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
	    append m "\n  unsigned long _maximum, _length;"
	    append m "\n  [declarator* [$type type] _buffer];"
	    append m "\n  void (*release)(void *_buffer);"
	} else {
	    append m "\n  const unsigned long _maximum;"
	    append m "\n  unsigned long _length;"
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

    # Remove leading :: and map other :: to _
    #
    proc cname { name } {
	if { [string first :: $name] == 0 } {
	    set name [string range $name 2 end]
	}
	return [string map {:: _ { } _} $name]
    }
}
