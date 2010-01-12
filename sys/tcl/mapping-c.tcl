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
#                                           Anthony Mallet on Mon Jan 11 2010
#
package require Tcl 8.5

namespace eval mapping::c {

    # --- gentype -----------------------------------------------------------

    # Return the C mapping of type.
    #
    proc gentype { type } {
	switch -- [dict get $type kind] {
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

	template fatal {*}{
	    "internal error:"
	    "unhandled type '[dict get $type kind]' at top-level"
	}
    }


    # --- gentyperef -------------------------------------------------------

    # Return the C mapping of type reference.
    #
    proc gentyperef { type } {
	switch -- [dict get $type kind] {
	    {boolean}		{ return "unsigned int" }
	    {unsigned short}	{ return "unsigned short" }
	    {short}		{ return "short" }
	    {unsigned long}	{ return "unsigned long" }
	    {long}		{ return "long" }
	    {float}		{ return "float" }
	    {double}		{ return "double" }
	    {char}		{ return "char" }
	    {octet}		{ return "unsigned char" }
	    {string}		{ return "char *" }
	    {any}		{ error "type any not supported yet" }

	    {const}		-
	    {enum}		-
	    {enumerator}	-
	    {struct}		-
	    {union}		-
	    {typedef}		{ return [cname [dict get $type fullname]] }

	    {struct member}	-
	    {union member}	{
		set e [dotgen typeref [dict get $type type]]
		return [cdecl [gentyperef $e] [cname [dict get $type name]]]
	    }

	    {array}		{
		set e [dotgen typeref [dict get $type element]]
		if {[dict exists $type length]} {
		    set l [dict get $type length]
		} else {
		    set l {}
		}
		return "[gentyperef $e]\[$l\]"
	    }

	    {sequence}		{
		set e [dotgen typeref [dict get $type element]]
		append m "struct { unsigned long _maximum, _length;"
		append m " [gentyperef $e] *_buffer; }"
		return $m
	    }
	}

	template fatal {*}{
	    "internal error: unhandled type '[dict get $type kind]'"
	}
    }


    # --- genconst ---------------------------------------------------------

    # Return the C mapping of a const
    #
    proc genconst { type } {
	set n [cname [dict get $type fullname]]
	set t [gentyperef [dotgen typeref [dict get $type type]]]

	set v [dict get $type value value]
	switch [dict get $type value kind] {
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
	return [guard $m $n nodefine]
    }


    # --- genenum ----------------------------------------------------------

    # Return the C mapping of an enum.
    #
    proc genenum { type } {
	set n [cname [dict get $type fullname]]
	set elems [dict get $type members]

	append m [genloc $type]
	append m "\ntypedef unsigned long $n;"
	set v 0
	foreach e $elems {
	    set et [dotgen typeref $e]
	    set en [cname [dict get $et fullname]]
	    append m [genloc $et]
	    append m "\n# define $en\t([incr v])"
	}
	return [guard $m $n]
    }


    # --- genstruct --------------------------------------------------------

    # Return the C mapping of a struct. The typedef is output first to handle
    # recursive structures with anonymous sequences.
    #
    proc genstruct { type } {
	set n [cname [dict get $type fullname]]
	set elems [dict get $type members]

	append m [genloc $type]
	append m "\ntypedef struct $n $n;"

	append s "\nstruct $n {"
	foreach e $elems {
	    set et [dotgen typeref $e]
	    set en [cname [dict get $et fullname]]
	    append s [genloc $et]
	    append s "\n [gentyperef $et];"
	}
	append s "\n};"
	return [guard $m $n][guard $s "${n}_definition"]
    }


    # --- genunion ---------------------------------------------------------

    # Return the C mapping of a union. The typedef is output first to handle
    # recursive and forward decls.
    #
    proc genunion { type } {
	set n [cname [dict get $type fullname]]
	set discr [dotgen typeref [dict get $type discriminator]]
	set elems [dict get $type members]

	append m [genloc $type]
	append m "\ntypedef struct $n $n;"

	append s "\nstruct $n {"
	append s [genloc $discr]
	append s "\n [gentyperef $discr] _d;"
	append s "\n union {"
	foreach e $elems {
	    set et [dotgen typeref $e]
	    set en [cname [dict get $et fullname]]
	    append s [genloc $et]
	    append s "\n  [gentyperef $et];"
	}
	append s "\n } _u;"
	append s "\n};"
	return [guard $m $n][guard $s "${n}_definition"]
    }


    # --- genforward -------------------------------------------------------

    # Return the C mapping of forward declaration.
    #
    proc genforward { type } {
	set n [cname [dict get $type fullname]]

	append m [genloc $type]
	append m "\ntypedef struct $n $n;"
	return [guard $m $n]
    }


    # --- gentypedef -------------------------------------------------------

    # Return the C mapping of a typedef.
    #
    proc gentypedef { type } {
	set n [cname [dict get $type fullname]]
	set t [dotgen typeref [dict get $type type]]

	append m [genloc $type]
	append m "\ntypedef [cdecl [gentyperef $t] $n];"
	return [guard $m $n]
    }


    # --- genloc -----------------------------------------------------------

    # Return the C mapping of a source location
    #
    proc genloc { type } {
	if {![dict exists $type loc]} return ""

	set tloc [dict get $type loc]
	set f [lindex $tloc 0]
	set l [lindex $tloc 1]
	return "\n#line $l \"[string map {\" \\\"} $f]\""
    }


    # --- guard -----------------------------------------------------------

    # Protect a string with guards
    #
    proc guard { string name {nodefine ""}} {
	append m "\n#ifndef _${name}_type"
	if { "$nodefine" != "nodefine" }  {
	    append m "\n# define _${name}_type"
	}
	append m $string
	append m "\n#endif /* _${name}_type */"
	return $m
    }


    # --- cdecl ------------------------------------------------------------

    # Given a type string and a name, declare name of type type. The subtlety
    # comes from arrays, because name should preceede the brackets, if any.
    #
    proc cdecl { type name } {
	set b [string first \[ $type]
	if { $b < 0 } { return "$type $name" }

	return "[string range $type 0 $b-1] $name[string range $type $b end]"
    }


    # --- cname ------------------------------------------------------------

    # Remove leading :: and map other :: to _
    #
    proc cname { name } {
	if { [string first :: $name] == 0 } {
	    set name [string range $name 2 end]
	}
	return [string map {:: _} $name]
    }
}
