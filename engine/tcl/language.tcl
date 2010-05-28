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
#                                           Anthony Mallet on Fri Jan 15 2010
#

namespace eval language {

    # --- fileext ----------------------------------------------------------

    # \proc language fileext lang [kind]
    #
    # Return the cannonical file extension for the given language.
    #
    # \arg lang	The language name. Must be one of {\tt c}.
    # \arg kind	Must be one of the strings {\tt source} or {\tt header}.
    #
    proc fileext { lang {kind source}} {
	return [[support $lang]::fileext $kind]
    }
    namespace export fileext


    # --- comment ----------------------------------------------------------

    # \proc language comment lang text
    #
    # Return a string that is a valid comment in the given language.
    #
    # \arg lang	The language name. Must be one of {\tt c}.
    # \arg text	The string to be commented.
    #
    proc comment { lang text } {
	switch -nocase -- $lang {
	    c {
		regsub -all "\n(?=.)" "/*${text}" "\n *" text
		set text "${text} */"
		return $text
	    }

	    shell {
		regsub -all "\n(?=.)" "${text}" "\n#" text
		return $text
	    }
	}

	template fatal "unsupported language $lang"
    }
    namespace export comment


    # --- mapping ----------------------------------------------------------

    # \proc language mapping lang [pattern]
    #
    # Generate and return the mapping of the types matching the glob {\em
    # pattern} (or all types if no pattern is given), for the given language.
    # The returned string is a valid source code for the language.
    #
    # \arg lang	The language name. Must be one of {\tt c}.
    # \arg pattern	A glob pattern to generate the mapping only for those
    #			type names that match the pattern.
    #
    proc mapping { lang {pattern *} } {
	set lns [support $lang]
	foreach t [dotgen types $pattern] {
	    append m [${lns}::gentype $t]
	}
	return $m
    }
    namespace export mapping


    # --- declarator -------------------------------------------------------

    # \proc language declarator lang {\em type} [{\em var}]
    #
    # Return the abstract declarator for {\em type} or for a variable {\em var}
    # of that type, in the given language.
    #
    # \arg lang	The language name. Must be one of {\tt c}.
    # \arg type	A type object.
    # \arg var	A string representing the name of a variable of type {\em
    #		type}.
    #
    proc declarator { lang type {var {}} } {
	return [[support $lang]::declarator $type $var]
    }
    namespace export declarator

    # \proc language declarator\& lang {\em type} [{\em var}]
    #
    # Return the abstract declarator of a reference to the {\em type} or to a
    # variable {\em var} of that type, in the given language.
    #
    # \arg lang	The language name. Must be one of {\tt c}.
    # \arg type	A type object.
    # \arg var	A string representing the name of a variable of type {\em
    #		type}.
    #
    proc declarator& { lang type {var {}} } {
	return [[support $lang]::declarator& $type $var]
    }
    namespace export declarator&

    # \proc language declarator* lang {\em type} [{\em var}]
    #
    # Return the abstract declarator of a pointer to the {\em type} or to a
    # variable {\em var} of that type, in the given language.
    #
    # \arg lang	The language name. Must be one of {\tt c}.
    # \arg type	A type object.
    # \arg var	A string representing the name of a variable of type {\em
    #		type}.
    #
    proc declarator* { lang type {var {}} } {
	return [[support $lang]::declarator* $type $var]
    }
    namespace export declarator*


    # --- reference --------------------------------------------------------

    # \proc language reference lang {\em type} [{\em var}]
    #
    # Return the expression representing a reference to a variable.
    #
    # \arg lang	The language name. Must be one of {\tt c}.
    # \arg type	A type object.
    # \arg var	A string representing the name of a variable of type {\em
    #		type}.
    #
    proc reference { lang type {var {}} } {
	return [[support $lang]::reference $type $var]
    }
    namespace export reference


    # --- dereference ------------------------------------------------------

    # \proc language dereference lang {\em type} [{\em var}]
    #
    # Return the expression that dereferences a variable.
    #
    # \arg lang	The language name. Must be one of {\tt c}.
    # \arg type	A type object.
    # \arg var	A string representing the name of a variable of type {\em
    #		type}.
    #
    proc dereference { lang type {var {}} } {
	return [[support $lang]::dereference $type $var]
    }
    namespace export dereference


    # --- cname ------------------------------------------------------------

    # \proc language cname lang {\em name}
    #
    # Return the cannonical name of a type name in the given language.
    #
    # \arg lang	The language name. Must be one of {\tt c}.
    # \arg name	The name to convert.
    #
    proc cname { lang name } {
	return [[support $lang]::cname $name]
    }
    namespace export cname


    # --- signature --------------------------------------------------------

    # \proc language signature codel {\em separator}
    #
    # Return the signature of a codel in the given language. If separator is
    # given, it is a string that is inserted between the return type of the
    # codel and the codel name (for instance, a \string\n{} in C so that the
    # symbol name is guaranteed to be on the first column).
    #
    # \arg lang	The language name. Must be one of {\tt c}.
    # \arg codel	A codel object.
    # \arg separator	A string, inserted between the return type and the
    #			codel symbol name.
    #
    proc signature { lang codel {symchar { }} } {
	return [[support $lang]::signature $codel $symchar]
    }
    namespace export signature


    # --- sizeof -----------------------------------------------------------

    # Return binary size of type, in bytes
    #
    proc sizeof { type } {
	switch -- [$type kind] {
	    {boolean}		{ return 1 }
	    {unsigned short}	-
	    {short}		{ return 2 }
	    {unsigned long}	-
	    {long}		{ return 4 }
	    {float}		{ return 4 }
	    {double}		{ return 8 }
	    {char}		-
	    {octet}		{ return 1 }
	    {string}		{
		if {[catch {$type length} l]} {
		    error "attempt to compute the size of an unbounded string"
		}
		return l
	    }
	    {any}		{ error "type any not supported yet" }

	    {const}		{ return [sizeof [$type type]] }
	    {enum}		{ return 4 }
	    {enumerator}	{ return 4 }

	    {struct}		{
		set s 0
		foreach e [$type members] {
		    # account for padding
		    incr s [expr int(ceil([sizeof $e]/4.))]
		}
	    }

	    {union}		-
	    {typedef}		{ set d [cname [$type fullname]] }

	    {struct member}	-
	    {union member}	-
	    {forward struct}	-
	    {forward union}	{ set d [declarator [$type type]] }

	    {array}		{
		if {[catch { $type length } l]} { set l {} }
		set d "[declarator [$type type]]\[$l\]"
	    }

	    {sequence}		{
		set d "struct { unsigned long _maximum, _length;"
		append d " [declarator [$type type]] *_buffer; }"
	    }

	    default		{
		template fatal "internal error: unhandled type '[$type kind]'"
	    }
	}
    }
    namespace export sizeof


    # --- hfill ------------------------------------------------------------

    # \proc language hfill text [filler] [column]
    #
    # Return a string of length {\em column} (by default 80), starting with
    # {\em text} and filled with the {\em filler} character (by default -).
    #
    # \arg text		The text to fill.
    # \arg filler	The filler character (by default -).
    # \arg column	The desired length of the returned string.
    #
    proc hfill { text { filler - } { column 80 } } {
	return \
	    "$text[string repeat - [expr {$column - [string length $text]}]]"
    }
    namespace export hfill


    # --- support ----------------------------------------------------------

    # Return the namespace for language or raise an error
    #
    variable lns [dict create c c]
    proc support { lang } {
	variable lns

	if { [catch {dict get $lns $lang} ns] } {
	    template fatal "unsupported language $lang"
	}
	return $ns
    }

    namespace ensemble create
}
