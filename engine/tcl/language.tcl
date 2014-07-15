#
# Copyright (c) 2010-2014 LAAS/CNRS
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


# --- lang -----------------------------------------------------------------

#/ @nodebeproc{lang, Target programming language}
# @deffn {TCL Backend} {lang} @var{language}
#
# Set the current language for procedures that output a language dependent
# string
#
# @@args
# @item @var{language}
# The language name. Must be one of @code{c} or @code{c++}.
# @@end args
# @end deffn
#
proc lang { language } {
  set language::current [language::support $language]
}
slave alias lang lang


# --- comment --------------------------------------------------------------

#/ @nodebeproc{comment, Generate comment strings}
# @deffn {TCL Backend} {comment} [-@var{c}] @var{text}
#
# Return a string that is a valid comment in the current language.
#
# @@args
# @item @var{c}
# The string to use as a comment character (overriding current language).
# @item @var{test}
# The string to be commented.
# @@end args
# @end deffn
#
proc comment { args } {
  if {[string index [lindex $args 0] 0] == "-" } {
    set args [lassign $args c]
    set c [string range $c 1 end]
    return $c[join [split [join $args] "\n"] "\n$c"]
  }
  return [${language::current}::comment [join $args]]
}
slave alias comment comment


# --- fileext --------------------------------------------------------------

#/ @nodebeproc{fileext, Cannonical file extension}
# @deffn {TCL Backend} {fileext} [-@var{kind}]
#
# Return the cannonical file extension for the current language.
#
#
# @@args
# @item @var{kind}
# Must be one of the strings @code{source} or @code{header}.
# @@end args
# @end deffn
#
proc fileext { {kind source} } {
  return [${language::current}::fileext $kind]
}
slave alias fileext fileext


# --- indent ---------------------------------------------------------------

#/ @nodebeproc{indent, Generate indented text}
# @deffn {TCL Backend} {indent} [#@var{n}|@b{++}|@b{--}] [@var{text} @dots{}]
#
# Output @var{text}, indented to the current indent level. Each @var{text}
# argument is followed by a newline.
# Indent level can be changed by passing an absolute level with #@var{n}, or
# incremented or decremented with @code{++} or @code{--}.
#
# @@args
# @item @var{test}
# The string to output indented.
# @@end args
# @end deffn
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


# --- --- ------------------------------------------------------------------

#/ @nodebeproc{---, Generate filler string}
# @deffn {TCL Backend} {-@--@--} [-@var{column}] @var{text} @dots{} @var{filler}
#
# This command, spelled with 3 dashes (@code{-}), return a string of length
# @var{column} (70 by default), starting with @var{text} and filled with the
# last character of the @var{filler} string.
#
# @@args
# @item @var{text}
# The text to fill.
# @item @var{filler}
# The filler character.
# @item @var{column}
# The desired length of the returned string.
# @@end args
# @end deffn
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


# --- wrap -----------------------------------------------------------------

#/ @nodebeproc{wrap, Chop blocks of text}
# @deffn {TCL Backend} {wrap} [-@var{column}] @var{text} [@var{prefix}] @
# [@var{sep}]
#
# Chop a string into lines of length @var{column} (70 by default), prefixed
# with @var{prefix} (empty by default). The string is split at spaces by
# default, or at @var{sep} if given.
#
# @@args
# @item @var{text}
# The text to fill.
# @item @var{prefix}
# A string prefixed to each line.
# @item @var{sep}
# The separator for breaking text.
# @item @var{column}
# The desired maximum length of each line
# @@end args
# @end deffn
#
proc wrap { args } {
  set column 70
  if {[string index [lindex $args 0] 0] == "-" } {
    set args [lassign $args column]
    set column [string range $column 1 end]
    if {![string is digit $column]} {
      template fatal "bad column \"-$column\""
    }
  }
  if {[llength $args] > 2} {
    set sep [lindex $args 2]
  } else {
    set sep " "
  }
  set prefix [lindex $args 1]
  set text [lindex $args 0]

  for {set result {}} {[string length $text] > $column} {
    set text $prefix[string range $text [expr {$brk+1}] end]
  } {
    set brk [string last $sep $text $column]
    if { $brk < 0 } {
      set brk [string first $sep $text]
      if { $brk < 0 } break
    }
    if { ![string is space $sep] } { incr brk }
    append result [string range $text 0 [expr {$brk-1}]] \n
  }
  return $result$text
}
slave alias wrap wrap


namespace eval language {

  # --- cname --------------------------------------------------------------

  #/ @nodebeproc{cname, Cannonical object name}
  # @deffn {TCL Backend} {cname} @var{string|object}
  #
  # Return the cannonical name of the @var{string} or the @genom{}
  # @var{object}, according to the current language.
  #
  # If a regular string is given, this procedure typically maps IDL :: scope
  # separator into the native scope separator symbol for the current language.
  # If a codel object is given, this procedure returns the symbol name of the
  # codel for the current language.
  #
  # @@args
  # @item @var{string}
  # The name to convert.
  # @item @var{object}
  # A @genom{} object.
  # @@end args
  # @end deffn
  #
  proc cname { object } {
    variable current
    return [${current}::cname $object]
  }
  slave alias language::cname language::cname
  namespace export cname


  # --- mangle -------------------------------------------------------------

  #/ @nodebeproc{language mangle, Unique type name}
  # @deffn {TCL Backend} {language mangle} @var{type}
  #
  # Return a string containing a universally unique representation of the name
  # of the @var{type} object.
  #
  # @@args
  # @item @var{type}
  # A 'type' object.
  # @@end args
  # @end deffn
  #
  proc mangle { type } {
    switch -- [$type kind] {
      enum - enumerator - struct - union - exception - typedef - native {
	return [string map {:: _} t[$type fullname]]
      }

      array - sequence - optional {
        if {[catch {$type length} l]} {
          return [$type kind]_[mangle [$type type]]
        } else {
          return [$type kind]${l}_[mangle [$type type]]
        }
      }

      string {
        if {[catch {$type length} l]} {
          return string
        } else {
          return string${l}
        }
      }

      {struct member} - {union member} - {const} {
        return [mangle [$type type]]
      }

      default {
        return [string map {{ } _} [$type kind]]
      }
    }
  }
  slave alias language::mangle language::mangle
  namespace export mangle


  # --- mapping ------------------------------------------------------------

  #/ @nodebeproc{language mapping, IDL type language mapping}
  # @deffn {TCL Backend} {language mapping} [@var{type}]
  #
  # Generate and return a string containing the mapping of @var{type} for the
  # current language, or of all types if no argument is given.
  # The returned string is a valid source code for the language.
  #
  # @@args
  # @item @var{type}
  # A 'type' object.
  # @@end args
  # @end deffn
  #
  proc mapping { args } {
    variable current
    if {![llength $args]} { set args [dotgen types] }
    foreach type $args {
      append m [${current}::mapping $type]
    }
    return $m
  }
  slave alias language::mapping language::mapping
  namespace export mapping


  # --- declarator ---------------------------------------------------------

  #/ @nodebeproc{language declarator, Code for type declarations}
  # @deffn {TCL Backend} {language declarator} @var{type} [@var{var}]
  #
  # Return the abstract declarator for @var{type} or for a variable @var{var}
  # of that type, in the current language.
  #
  # @@args
  # @item @var{type}
  # A 'type' object.
  # @item @var{var}
  # A string representing the name of a variable of type @var{type}.
  # @@end args
  # @end deffn
  #
  proc declarator { type {var {}} } {
    variable current
    return [${current}::declarator $type $var]
  }
  slave alias language::declarator language::declarator
  namespace export declarator


  # --- address ------------------------------------------------------------

  #/ @nodebeproc{language address, Code for variable addresses}
  # @deffn {TCL Backend} {language address} @var{type} [@var{var}]
  #
  # Return an expression evaluating to the address of a variable in the current
  # language.
  #
  # @@args
  # @item @var{type}
  # A 'type' object.
  # @item @var{var}
  # A string representing the name of a variable of type @var{type}.
  # @@end args
  # @end deffn
  #
  proc address { type {var {}} } {
    variable current
    return [${current}::address $type $var]
  }
  slave alias language::address language::address
  namespace export address


  # --- dereference --------------------------------------------------------

  #/ @nodebeproc{language dereference, Code for dereferencing variables}
  # @deffn {TCL Backend} {language dereference} @var{type} @var{kind} @
  # [@var{var}]
  #
  # Return an expression dereferencing a parameter passed by value or
  # reference, in the current language.
  #
  # @@args
  # @item @var{type}
  # A 'type' object.
  # @item @var{kind}
  # Must be @code{value} or @code{reference}.
  # @item @var{var}
  # A string representing the name of a parameter of type @var{type}.
  # @@end args
  # @end deffn
  #
  proc dereference { type kind {var {}} } {
    variable current
    return [${current}::dereference $type $kind $var]
  }
  slave alias language::dereference language::dereference
  namespace export dereference


  # --- argument -----------------------------------------------------------

  #/ @nodebeproc{language argument, Code for declaring functions arguments}
  # @deffn {TCL Backend} {language argument} @var{type} @var{kind} [@var{var}]
  #
  # Return an expression that declares a parameter @var{var} of type
  # @var{type}, passed by value or reference according to @var{kind}.
  #
  # @@args
  # @item @var{type}
  # A 'type' object.
  # @item @var{kind}
  # Must be @code{value} or @code{reference}.
  # @item @var{var}
  # A string representing the name of a variable of type @var{type}.
  # @@end args
  # @end deffn
  #
  proc argument { type kind {var {}} } {
    variable current
    return [${current}::argument $type $kind $var]
  }
  slave alias language::argument language::argument
  namespace export argument


  # --- pass ---------------------------------------------------------------

  #/ @nodebeproc{language pass, Code for passing functions arguments}
  # @deffn {TCL Backend} {language pass} @var{type} @var{kind} [@var{var}]
  #
  # Return an expression that passes @var{var} of type @var{type} as a
  # parameter, by value or reference according to @var{kind}.
  #
  # @@args
  # @item @var{type}
  # A 'type' object.
  # @item @var{kind}
  # Must be @code{value} or @code{reference}.
  # @item @var{var}
  # A string representing the name of a variable of type @var{type}.
  # @@end args
  # @end deffn
  #
  proc pass { type kind {var {}} } {
    variable current
    return [${current}::pass $type $kind $var]
  }
  slave alias language::pass language::pass
  namespace export pass


  # --- member -----------------------------------------------------------

  #/ @nodebeproc{language member, Code for accessing structure members}
  # @deffn {TCL Backend} {language member} @var{type} @var{mlist}
  #
  # Return the language construction to access a member of a @var{type}.
  # @var{mlist} is a list interpreted as follow: if it starts with a letter,
  # @var{type} should be an aggregate type (like @code{struct}); if it starts
  # with a numeric digit, @var{type} should be an array type (like
  # @code{sequence}).
  #
  # @@args
  # @item @var{type}
  # A 'type' object.
  # @item @var{mlist}
  # A list of hierachical members to access.
  # @@end args
  # @end deffn
  #
  proc member { type mlist } {
    variable current
    return [${current}::member $type $mlist]
  }
  slave alias language::member language::member
  namespace export member


  # --- signature ----------------------------------------------------------

  #/ @nodebeproc{language signature, Code for declaring codel signatures}
  # @deffn {TCL Backend} {language signature} @var{codel} [@var{separator}] @
  # [@var{location}]
  #
  # Return the signature of a codel in the current language. If separator is
  # given, it is a string that is inserted between the return type of the
  # codel and the codel name (for instance, a @code{\n} in C so that the
  # symbol name is guaranteed to be on the first column).
  #
  # @@args
  # @item @var{code}
  # A 'codel' object.
  # @item @var{separator}
  # A string, inserted between the return type and the codel symbol name.
  # @item @var{location}
  # A boolean indicating whether to generate @code{#line} directives
  # corresponding to the codel location in @code{.gen} file.
  # @@end args
  # @end deffn
  #
  proc signature { codel {symchar { }} {location off}} {
    variable current
    return [${current}::signature $codel $symchar $location]
  }
  slave alias language::signature language::signature
  namespace export signature


  # --- invoke -------------------------------------------------------------

  #/ @nodebeproc{language invoke, Code for calling codels}
  # @deffn {TCL Backend} {language invoke} @var{codel} @var{params}
  #
  # Return a string corresponding to the invocation of a codel in the current
  # language.
  #
  # @@args
  # @item @var{code}
  # A 'codel' object.
  # @item @var{params}
  # The list of parameters passed to the codel. Each element of this list must
  # be a valid string in the current language corresponding to each parameter
  # value or reference to be passed to the codel (@pxref{language pass}).
  # @@end args
  # @end deffn
  #
  proc invoke { codel params } {
    variable current
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
