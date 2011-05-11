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

  # --- iter ---------------------------------------------------------------

  # Return a loop construct for 'type'. 'part' may be set to begin, end or
  # var.
  #
  proc iter { type part {level 0} {max {}} } {
    variable current
    return [${current}::iter $type $part $level $max]
  }
  slave alias language::iter language::iter


  # --- mapping ------------------------------------------------------------

  # \proc language mapping ?{\em type}?
  # \index language mapping
  #
  # Generate and return the mapping of {\em type} for the current language, or
  # all types if no argument is given.
  # The returned string is a valid source code for the language.
  #
  # \arg type	A 'type' object.
  #
  proc mapping { args } {
    variable current
    if {![llength $args]} { set args [dotgen types] }
    foreach type $args {
      append m [${current}::mapping $type]
    }
    return $m
  }
  namespace export mapping


  # --- declarator ---------------------------------------------------------

  # \proc language declarator {\em type} [{\em var}]
  # \index language declarator
  #
  # Return the abstract declarator for {\em type} or for a variable {\em var}
  # of that type, in the current language.
  #
  # \arg type	A type object.
  # \arg var	A string representing the name of a variable of type {\em
  #		type}.
  #
  proc declarator { type {var {}} } {
    variable current
    return [${current}::declarator $type $var]
  }
  slave alias language::declarator language::declarator
  namespace export declarator


  # --- address ------------------------------------------------------------

  # \proc language address {\em type} [{\em var}]
  # \index language address
  #
  # Return an expression evaluating to the address of a variable in the current
  # language.
  #
  # \arg type	A type object.
  # \arg var	A string representing the name of a variable of type {\em
  #		type}.
  #
  proc address { type {var {}} } {
    variable current
    return [${current}::address $type $var]
  }
  slave alias language::address language::address
  namespace export address


  # --- dereference --------------------------------------------------------

  # \proc language dereference {\em type} [{\em var}]
  # \index language dereference
  #
  # Return an expression dereferencing a pointer on a variable in the current
  # language.
  #
  # \arg type	A type object.
  # \arg var	A string representing the name of a variable of type {\em
  #		type}.
  #
  proc dereference { type {var {}} } {
    variable current
    return [${current}::dereference $type $var]
  }
  slave alias language::dereference language::dereference
  namespace export dereference


  # --- argument -----------------------------------------------------------

  # \proc language argument {\em type} {\em kind} [{\em var}]
  # \index language argument
  #
  # Return an expression that declares a parameter {\em var} of type {\em
  # type}, passed by value or reference according to {\em kind}.
  #
  # \arg type	A type object.
  # \arg kind	Must be "value" or "reference".
  # \arg var	A string representing the name of a variable of type {\em
  #		type}.
  #
  proc argument { type kind {var {}} } {
    variable current
    return [${current}::argument $type $kind $var]
  }
  slave alias language::argument language::argument
  namespace export argument


  # --- pass ---------------------------------------------------------------

  # \proc language pass {\em type} {\em kind} [{\em var}]
  # \index language pass
  #
  # Return an expression that passes {\em var} of type {\em type} as a
  # parameter, by value or reference according to {\em kind}.
  #
  # \arg type	A type object.
  # \arg kind	Must be "value" or "reference".
  # \arg var	A string representing the name of a variable of type {\em
  #		type}.
  #
  proc pass { type kind {var {}} } {
    variable current
    return [${current}::pass $type $kind $var]
  }
  slave alias language::pass language::pass
  namespace export pass


  # --- member -----------------------------------------------------------

  # \proc language member {\em type} {\em mlist}
  # \index language member
  #
  # Return the language construction to access a member of a {\em type}.
  # {\em mlist} is a list interpreted as follow: if it starts with a letter,
  # {\em type} should be an aggregate type (like {\tt struct}); if it starts
  # with a numeric digit, {\em type} should be an array type (like {\tt
  # sequence}).
  #
  # \arg type		A type object
  # \arg mlist		A list of hierachical members to access.
  #
  proc member { type mlist } {
    variable current
    return [${current}::member $type $mlist]
  }
  slave alias language::member language::member
  namespace export member


  # --- signature ----------------------------------------------------------

  # \proc language signature codel [{\em separator} [{\em location}]]
  # \index language signature
  #
  # Return the signature of a codel in the current language. If separator is
  # given, it is a string that is inserted between the return type of the
  # codel and the codel name (for instance, a \string\n{} in C so that the
  # symbol name is guaranteed to be on the first column).
  #
  # \arg codel		A codel object.
  # \arg separator	A string, inserted between the return type and the
  #			codel symbol name.
  # \arg location	A boolean indicating whether to generate \#line
  #			directive corresponding to the codel definition in .gen
  #			file.
  #
  proc signature { codel {symchar { }} {location off}} {
    variable current
    return [${current}::signature $codel $symchar $location]
  }
  slave alias language::signature language::signature
  namespace export signature


  # --- invoke -------------------------------------------------------------

  # \proc language invoke {\em codel} {\em params}
  # \index language invoke
  #
  # Return a string corresponding to the invocation of a codel in the current
  # language.
  #
  # \arg codel	A codel object.
  # \arg params	The list of parameters passed to the codel. Each element of
  #		this list must be a valid string in the current language
  #		corresponding to each parameter value or reference to be passed
  #		to the codel.
  #
  proc invoke { codel params } {
    variable current

    if {[llength $params] != [llength [$codel parameters]]} {
      template fatal "wrong # arguments for codel [$codel name]"
    }
    return [${current}::invoke $codel $params]
  }
  slave alias language::invoke language::invoke
  namespace export invoke


  # --- support ------------------------------------------------------------

  # Return the namespace for language or raise an error
  #
  variable lns [dict create c c c++ c++ tcl tcl]
  proc support { lang } {
    variable lns

    if { [catch {dict get $lns $lang} ns] } {
      template fatal "unsupported language $lang"
    }
    return ::language::$ns
  }

  # current language, default to C
  variable current [support c]

  namespace ensemble create
}


# --- lang -----------------------------------------------------------------

# \proc lang language
# \index lang
#
# Set the current language for procedures that output a language dependent
# string
#
# \arg language		The language name. Must be {\tt c} or {\tt c++}.
#
proc lang { language } {
  set language::current [language::support $language]
}
slave alias lang lang


# --- cname ----------------------------------------------------------------

# \proc cname \{{\em string}|{\em object}\}
# \index cname
#
# Return the cannonical name of the {\em string} or the \GenoM{} {\em
# object}, according to the current language.
#
# If a regular string is given, this procedure typically maps IDL :: scope
# separator into the native symbol in the given language.
# If a codel object is given, this procedure returns the symbol name of the
# codel in the given language.
#
# \arg string	The name to convert.
# \arg object	A \GenoM{} object. Only class {\tt codel} is supported
#		at the moment.
#
proc cname { object } {
  return [${language::current}::cname $object]
}
slave alias cname cname


# --- indent ---------------------------------------------------------------

# \proc indent [\#n\string|++\string|-{}-] {\em ?text...?}
# \index indent
#
# Output {\em text}, indented to the current indent level. Each {\em text}
# argument is followed by a newline.
# Indent level can be changed by passing an absolute level with \#n, or
# incremented or decremented with ++ or -{}-.
#
# \arg text	The string to output.
#
proc indent { args } {
  global _ilevel

  foreach text $args {
    switch -glob -- $text {
      #* {
        set _ilevel [string range $text 1 end]
        if {![string is digit $_ilevel]} {
          error "bad indentation \"#$_ilevel\""
        }
      }
      ++ { incr _ilevel 2 }
      -- { incr _ilevel -2 }

      default {
        catch { set text "[string repeat " " $_ilevel]$text" }
        puts $text
      }
    }
  }
}
slave eval [list proc indent [info args indent] [info body indent]]


# --- comment --------------------------------------------------------------

# \proc comment [-c] text
# \index comment
#
# Return a string that is a valid comment in the current language.
#
# \arg c	The string to use as a comment character (overriding current
#		language).
# \arg text	The string to be commented.
#
proc comment { args } {
  if {[string index [lindex $args 0] 0] == "-" } {
    set args [lassign $args c]
    set c [string range $c 1 end]
    return $c[join [split [join $args]] "\n$c"]
  }
  return [${language::current}::comment [join $args]]
}
slave alias comment comment


# --- fileext --------------------------------------------------------------

# \proc fileext [kind]
# \index fileext
#
# Return the cannonical file extension for the current language.
#
# \arg kind	Must be one of the strings {\tt source} or {\tt header}.
#
proc fileext { {kind source} } {
  return [${language::current}::fileext $kind]
}
slave alias fileext fileext


# --- --- ------------------------------------------------------------------

# \proc -{}-{}- [-{\em column}] {\em text} {\em ?text...?} {\em filler}
# \index -{}-{}-
#
# Print a string of length {\em column} (70 by default), starting with
# {\em text} and filled with the last character of the {\em filler} string.
#
# \arg text	The text to fill.
# \arg filler	The filler character
# \arg column	The desired length of the returned string.
#
proc --- { args } {
  set column 70
  if {[string index [lindex $args 0] 0] == "-" } {
    set args [lassign $args column]
    set column [string range $column 1 end]
    if {![string is digit $column]} {
      template fatal "bad column \"-$column\""
    }
  }
  incr column -5
  set filler [string index [lindex $args end] end]
  set text [join [lrange $args 0 end-1] { }]
  set pre [string repeat $filler 3]
  set post [string repeat $filler [expr {$column-[string length $text]}]]
  return "$pre $text $post"
}
slave alias --- ---
