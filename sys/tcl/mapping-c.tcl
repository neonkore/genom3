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


    # --- gentyperef -------------------------------------------------------

    # Return the C mapping of type reference.
    #
    proc gentyperef { type } {
	switch -- [$type kind] {
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
	    {typedef}		{ return [cname [$type fullname]] }

	    {struct member}	-
	    {union member}	{
		return [cdecl [gentyperef [$type type]] [cname [$type name]]]
	    }

	    {forward struct}	-
	    {forward union}	{ return [gentyperef [$type type]] }

	    {array}		{
		if {[catch { $type length } l]} { set l {} }
		return "[gentyperef [$type type]]\[$l\]"
	    }

	    {sequence}		{
		append m "struct { unsigned long _maximum, _length;"
		append m " [gentyperef [$type type]] *_buffer; }"
		return $m
	    }
	}

	template fatal "internal error: unhandled type '[$type kind]'"
    }


    # --- genconst ---------------------------------------------------------

    # Return the C mapping of a const
    #
    proc genconst { type } {
	set n [cname [$type fullname]]
	set t [gentyperef [$type type]]

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


    # --- genstruct --------------------------------------------------------

    # Return the C mapping of a struct. The typedef is output first to handle
    # recursive structures with anonymous sequences.
    #
    proc genstruct { type } {
	set n [cname [$type fullname]]

	append m [genloc $type]
	append m "\ntypedef struct $n $n;"

	append s "\nstruct $n {"
	foreach e [$type members] {
	    append s [genloc $e]
	    append s "\n [gentyperef $e];"
	}
	append s "\n};"
	return [guard $m $n][guard $s "${n}_definition"]
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

	append s "\nstruct $n {"
	append s [genloc $discr]
	append s "\n [gentyperef $discr] _d;"
	append s "\n union {"
	foreach e [$type members] {
	    append s [genloc $e]
	    append s "\n  [gentyperef $e];"
	}
	append s "\n } _u;"
	append s "\n};"
	return [guard $m $n][guard $s "${n}_definition"]
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

	append m [genloc $type]
	append m "\ntypedef [cdecl [gentyperef [$type type]] $n];"
	return [guard $m $n]
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
