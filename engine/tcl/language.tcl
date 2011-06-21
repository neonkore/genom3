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
#                                           Anthony Mallet on Fri Jan 15 2010
#

namespace eval language {

    # --- fileext ----------------------------------------------------------

    # \proc language fileext lang [kind]
    # \index language fileext
    #
    # Return the cannonical file extension for the given language.
    #
    # \arg lang	The language name. Must be {\tt c} or {\tt c++}.
    # \arg kind	Must be one of the strings {\tt source} or {\tt header}.
    #
    proc fileext { lang {kind source}} {
	return [[support $lang]::fileext $kind]
    }
    namespace export fileext


    # --- comment ----------------------------------------------------------

    # \proc language comment lang text
    # \index language comment
    #
    # Return a string that is a valid comment in the given language.
    #
    # \arg lang	The language name. Must be {\tt c}, {\tt c++} or {\tt shell}.
    # \arg text	The string to be commented.
    #
    proc comment { lang text } {
	switch -nocase -- $lang {
	    shell {
		regsub -all "\n(?=.)" "${text}" "\n#" text
		return $text
	    }
	}

	set lns [support $lang]
	return [${lns}::comment $text]
    }
    namespace export comment


    # --- mapping ----------------------------------------------------------

    # \proc language mapping lang [pattern]
    # \index language mapping
    #
    # Generate and return the mapping of the types matching the glob {\em
    # pattern} (or all types if no pattern is given), for the given language.
    # The returned string is a valid source code for the language.
    #
    # \arg lang	The language name. Must be {\tt c} or {\tt c++}.
    # \arg pattern	A glob pattern to generate the mapping only for those
    #			type names that match the pattern.
    #
    proc mapping { lang {pattern *} } {
	set lns [support $lang]
	return [${lns}::mapping $pattern]
    }
    namespace export mapping


    # --- declarator -------------------------------------------------------

    # \proc language declarator lang {\em type} [{\em var}]
    # \index language declarator
    #
    # Return the abstract declarator for {\em type} or for a variable {\em var}
    # of that type, in the given language.
    #
    # \arg lang	The language name. Must be {\tt c} or {\tt c++}.
    # \arg type	A type object.
    # \arg var	A string representing the name of a variable of type {\em
    #		type}.
    #
    proc declarator { lang type {var {}} } {
	return [[support $lang]::declarator $type $var]
    }
    namespace export declarator


    # --- address ----------------------------------------------------------

    # \proc language address lang {\em type} [{\em var}]
    # \index language address
    #
    # Return an expression evaluating to the address of a variable.
    #
    # \arg lang	The language name. Must be {\tt c} or {\tt c++}.
    # \arg type	A type object.
    # \arg var	A string representing the name of a variable of type {\em
    #		type}.
    #
    proc address { lang type {var {}} } {
	return [[support $lang]::address $type $var]
    }
    namespace export address


    # --- dereference ------------------------------------------------------

    # \proc language dereference lang {\em type} [{\em var}]
    # \index language dereference
    #
    # Return an expression able to dereference a pointer on a variable.
    #
    # \arg lang	The language name. Must be {\tt c} or {\tt c++}.
    # \arg type	A type object.
    # \arg var	A string representing the name of a variable of type {\em
    #		type}.
    #
    proc dereference { lang type {var {}} } {
	return [[support $lang]::dereference $type $var]
    }
    namespace export dereference


    # --- parameter --------------------------------------------------------

    # \proc language parameter lang {\em kind} {\em type} [{\em var}]
    # \index language parameter
    #
    # Return an expression that declares a parameter {\em var} of type {\em
    # type}, passed by value or reference according to {\em kind}.
    #
    # \arg lang	The language name. Must be {\tt c} or {\tt c++}.
    # \arg kind	Must be "value" or "reference".
    # \arg type	A type object.
    # \arg var	A string representing the name of a variable of type {\em
    #		type}.
    #
    proc parameter { lang kind type {var {}} } {
	return [[support $lang]::parameter $kind $type $var]
    }
    namespace export parameter


    # --- argument ---------------------------------------------------------

    # \proc language argument lang {\em kind} {\em type} [{\em var}]
    # \index language argument
    #
    # Return an expression that passes {\em var} of type {\em type} as a
    # parameter, by value or reference according to {\em kind}.
    #
    # \arg lang	The language name. Must be {\tt c} or {\tt c++}.
    # \arg kind	Must be "value" or "reference".
    # \arg type	A type object.
    # \arg var	A string representing the name of a variable of type {\em
    #		type}.
    #
    proc argument { lang kind type {var {}} } {
	return [[support $lang]::argument $kind $type $var]
    }
    namespace export argument


    # --- member -----------------------------------------------------------

    # \proc language member {\em lang} {\em type} {\em members}
    # \index language member
    #
    # Return the language construction to access a member of a {\em type}. If
    # {\em members} is a list, it is interpreted as a hierachy. Each element of
    # {\em members} is interpreted as follow: if it starts with a letter, {\em
    # type} should be an aggregate type (like {\tt struct}); if it starts with
    # a numeric digit, {\em type} should be an array type (like {\tt
    # sequence}).
    #
    # This procedure can typically be used with parameters objects, for which
    # the 'member' subcommand returns a compatible {\em members} list.
    #
    # \arg lang		The language name. Must be {\tt c} or {\tt c++}.
    # \arg type		A type object
    # \arg members	A member name, or a list of hierachical members to
    #			access.
    #
    proc member { lang type members } {
	return [[support $lang]::member $type $members]
    }
    namespace export member


    # --- cname ------------------------------------------------------------

    # \proc language cname {\em lang} \{{\em string}|{\em object}\}
    # \index language cname
    #
    # Return the cannonical name of the {\em string} or the \GenoM{} {\em
    # object}, according to the language {\em lang}.
    #
    # If a regular string is given, this procedure typically maps IDL :: scope
    # separator into the native symbol in the given language.
    # If a codel object is given, this procedure returns the symbol name of the
    # codel in the given language.
    #
    # \arg lang		The language name. Must be {\tt c} or {\tt c++}.
    # \arg string	The name to convert.
    # \arg object	A \GenoM{} object. Only class {\tt codel} is supported
    #			at the moment.
    #
    proc cname { lang object } {
	return [[support $lang]::cname $object]
    }
    namespace export cname


    # --- signature --------------------------------------------------------

    # \proc language signature lang codel [{\em separator} [{\em location}]]
    # \index language signature
    #
    # Return the signature of a codel in the given language. If separator is
    # given, it is a string that is inserted between the return type of the
    # codel and the codel name (for instance, a \string\n{} in C so that the
    # symbol name is guaranteed to be on the first column).
    #
    # \arg lang	The language name. Must be {\tt c} or {\tt c++}.
    # \arg codel	A codel object.
    # \arg separator	A string, inserted between the return type and the
    #			codel symbol name.
    # \arg location	A boolean indicating whether to generate \#line directive
    #			corresponding to the codel definition in .gen file.
    #
    proc signature { lang codel {symchar { }} {location off}} {
	return [[support $lang]::signature $codel $symchar $location]
    }
    namespace export signature


    # --- arguments --------------------------------------------------------

    # \proc language arguments {\em lang} {\em codel} {\em base}
    # \index language arguments
    #
    # Return a list of strings corresponding to each parameter of a codel fetch
    # from the given {\em base}.
    #
    # \arg lang		The language name. Must be {\tt c} or {\tt c++}.
    # \arg codel	A codel object.
    # \arg base		A string from which parameters are fetched.
    #
    proc arguments { lang codel base } {
      set plist [list]
      set cparam [$codel parameters]
      foreach p $cparam {
	set member "$base[language member $lang [$p base] [$p member]]"
	switch -- [$p dir] {
	  "in" - "inport" {
	    lappend plist [argument $lang value [$p type] $member]
	  }
	  default {
	    lappend plist [argument $lang reference [$p type] $member]
	  }
	}
      }
      return $plist
    }
    namespace export arguments


    # --- invoke -----------------------------------------------------------

    # \proc language invoke {\em lang} {\em codel} {\em params}
    # \index language invoke
    #
    # Return a string corresponding to the invocation of a codel in the given
    # language.
    #
    # \arg lang		The language name. Must be {\tt c} or {\tt c++}.
    # \arg codel	A codel object.
    # \arg params	The list of parameters passed to the codel. Each
    #			element of this list must be a valid string in
    #			the {\em lang} language corresponding to each parameter
    #			value or reference to be passed to the codel.
    proc invoke { lang codel params } {
	if {[llength $params] != [llength [$codel parameters]]} {
	    template fatal "wrong # arguments for codel [$codel name]"
	}

	return [[support $lang]::invoke $codel $params]
    }
    namespace export invoke


    # --- hfill ------------------------------------------------------------

    # \proc language hfill text [filler] [column]
    # \index language hfill
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
	    "$text[string repeat $filler \
		[expr {$column-[string length $text]}]]"
    }
    namespace export hfill


    # --- support ----------------------------------------------------------

    # Return the namespace for language or raise an error
    #
    variable lns [dict create c c c++ c++ tcl tcl]
    proc support { lang } {
	variable lns

	if { [catch {dict get $lns $lang} ns] } {
	    template fatal "unsupported language $lang"
	}
	return $ns
    }

    namespace ensemble create
}
