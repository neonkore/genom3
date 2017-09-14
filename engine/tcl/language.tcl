#
# Copyright (c) 2010-2015,2017 LAAS/CNRS
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

#/
# `language` TCL engine command
# -----------------------------

# --- lang -----------------------------------------------------------------

#/
# [[lang]]
# === `lang`: Target programming language
# ****
# `lang` 'language'
# ****
#
# Set the current language for procedures that output a language dependent
# string
#
# ==== Arguments
# @item 'language'
# The language name. Must be one of @code{c} or @code{c++}.
# @@end args
# @end deffn
#
proc lang { language } {
  set language::current [language::support $language]
}
slave alias lang lang


# --- comment --------------------------------------------------------------

#/
# [[comment]]
# === `comment`: Generate comment strings
# ****
# `comment` ['-c'] 'text'
# ****
#
# Return a string that is a valid comment in the current language.
#
# ==== Arguments
# 'c'::
# The string to use as a comment character (overriding current language).
# 'text'::
# The string to be commented.
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

#/
# [[fileext]]
# === `fileext`: Cannonical file extension
# ****
# `fileext` ['-kind']
# ****
#
# Return the cannonical file extension for the current language.
#
#
# ==== Arguments
# 'kind'::
# Must be one of the strings `source` or `header`.
#
proc fileext { {kind source} } {
  return [${language::current}::fileext $kind]
}
slave alias fileext fileext


# --- indent ---------------------------------------------------------------

#/
# [[indent]]
# === `indent`: Generate indented text
# ****
# `indent` [#'n'|++|--] ['text' ...]
# ****
#
# Output 'text', indented to the current indent level. Each 'text'
# argument is followed by a newline.
# Indent level can be changed by passing an absolute level with #'n', or
# incremented or decremented with `++` or `--`.
#
# ==== Arguments
# 'text'::
# The string to output indented.
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

#/
# [[___]]
# === `---`: Generate filler string
# ****
# `---` [-'column'] 'text' ... 'filler'
# ****
#
# This command, spelled with 3 dashes (`-`), return a string of length
# 'column' (70 by default), starting with 'text' and filled with the
# last character of the 'filler' string.
#
# ==== Arguments
# 'text'::
# The text to fill.
# 'filler'::
# The filler character.
# 'column'::
# The desired length of the returned string.
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

#/
# [[wrap]]
# === `wrap`: Chop blocks of text
# ****
# `wrap` [-'column'] 'text' ['prefix'] ['sep']
# ****
#
# Chop a string into lines of length 'column' (70 by default), prefixed
# with 'prefix' (empty by default). The string is split at spaces by
# default, or at 'sep' if given.
#
# ==== Arguments
# 'text'::
# The text to fill.
# 'prefix'::
# A string prefixed to each line.
# 'sep'::
# The separator for breaking text.
# 'column'::
# The desired maximum length of each line
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

  #/
  # [[cname]]
  # === `cname`: Cannonical object name
  # ****
  # `cname` `string|object`
  # ****
  #
  # Return the cannonical name of the 'string' or the `genom3` 'object',
  # according to the current language.
  #
  # If a regular string is given, this procedure typically maps IDL `::` scope
  # separator into the native scope separator symbol for the current language.
  # If a codel object is given, this procedure returns the symbol name of the
  # codel for the current language.
  #
  # ==== Arguments
  # 'string'::
  # The name to convert.
  # 'object'::
  # A `genom3` object.
  #
  proc cname { object } {
    variable current
    return [${current}::cname $object]
  }
  slave alias language::cname language::cname
  namespace export cname


  # --- mangle -------------------------------------------------------------

  #/
  # [[mangle]]
  # === `language mangle`: Unique type name
  # ****
  # `language mangle` 'type'
  # ****
  #
  # Return a string containing a universally unique representation of the name
  # of the 'type' object.
  #
  # ==== Arguments
  # 'type'::
  # A 'type' object.
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

  #/
  # [[mapping]]
  # === `language mapping`: IDL type language mapping
  # ****
  # `language mapping` ['type'] ['locations']
  # ****
  #
  # Generate and return a string containing the mapping of 'type' for the
  # current language, or of all types if no argument is given.
  # The returned string is a valid source code for the language.
  #
  # ==== Arguments
  # 'type'::
  # A 'type' object.
  # 'locations'::
  # A boolean variable controlling the generation of type location information
  # (#line directives for cpp).
  #
  proc mapping { {types {}} {locations true} } {
    variable current
    if {![llength $types]} { set types [dotgen types] }
    foreach type $types {
      append m [${current}::mapping $type $locations]
    }
    return $m
  }
  slave alias language::mapping language::mapping
  namespace export mapping


  # --- declarator ---------------------------------------------------------

  #/
  # [[declarator]]
  # === `language declarator`: Code for type declarations
  # ****
  # `language declarator` 'type' ['var']
  # ****
  #
  # Return the abstract declarator for 'type' or for a variable 'var'
  # of that type, in the current language.
  #
  # ==== Arguments
  # 'type'::
  # A 'type' object.
  # 'var'::
  # A string representing the name of a variable of type 'type'.
  #
  proc declarator { type {var {}} } {
    variable current
    return [${current}::declarator $type $var]
  }
  slave alias language::declarator language::declarator
  namespace export declarator


  # --- address ------------------------------------------------------------

  #/
  # [[address]]
  # === `language address`: Code for variable addresses
  # ****
  # `language address` 'type' ['var']
  # ****
  #
  # Return an expression evaluating to the address of a variable in the current
  # language.
  #
  # ==== Arguments
  # 'type'::
  # A 'type' object.
  # 'var'::
  # A string representing the name of a variable of type 'type'.
  #
  proc address { type {var {}} } {
    variable current
    return [${current}::address $type $var]
  }
  slave alias language::address language::address
  namespace export address


  # --- dereference --------------------------------------------------------

  #/
  # [[dereference]]
  # === `language dereference`: Code for dereferencing variables
  # ****
  # `language dereference` 'type' 'kind' ['var']
  # ****
  #
  # Return an expression dereferencing a parameter passed by value or
  # reference, in the current language.
  #
  # ==== Arguments
  # 'type'::
  # A 'type' object.
  # 'kind'::
  # Must be `value` or `reference`.
  # 'var'::
  # A string representing the name of a parameter of type 'type'.
  #
  proc dereference { type kind {var {}} } {
    variable current
    return [${current}::dereference $type $kind $var]
  }
  slave alias language::dereference language::dereference
  namespace export dereference


  # --- argument -----------------------------------------------------------

  #/
  # [[argument]]
  # === `language argument`: Code for declaring functions arguments
  # ****
  # `language argument` 'type' 'kind' ['var']
  # ****
  #
  # Return an expression that declares a parameter 'var' of type
  # 'type', passed by value or reference according to 'kind'.
  #
  # ==== Arguments
  # 'type'::
  # A 'type' object.
  # 'kind'::
  # Must be `value` or `reference`.
  # 'var'::
  # A string representing the name of a variable of type 'type'.
  #
  proc argument { type kind {var {}} } {
    variable current
    return [${current}::argument $type $kind $var]
  }
  slave alias language::argument language::argument
  namespace export argument


  # --- pass ---------------------------------------------------------------

  #/
  # [[pass]]
  # === `language pass`: Code for passing functions arguments
  # ****
  # `language pass` 'type' 'kind' ['var']
  # ****
  #
  # Return an expression that passes 'var' of type 'type' as a
  # parameter, by value or reference according to 'kind'.
  #
  # ==== Arguments
  # 'type'::
  # A 'type' object.
  # 'kind'::
  # Must be `value` or `reference`.
  # 'var'::
  # A string representing the name of a variable of type 'type'.
  #
  proc pass { type kind {var {}} } {
    variable current
    return [${current}::pass $type $kind $var]
  }
  slave alias language::pass language::pass
  namespace export pass


  # --- member -----------------------------------------------------------

  #/
  # [[member]]
  # === `language member`: Code for accessing structure members
  # ****
  # `language member` 'type' 'mlist'
  # ****
  #
  # Return the language construction to access a member of a 'type'.  'mlist'
  # is a list interpreted as follow: if it starts with a letter, 'type' should
  # be an aggregate type (like `struct`); if it starts with a numeric digit,
  # 'type' should be an array type (like `sequence`).
  #
  # ==== Arguments
  # 'type'::
  # A 'type' object.
  # 'mlist'::
  # A list of hierachical members to access.
  #
  proc member { type mlist } {
    variable current
    return [${current}::member $type $mlist]
  }
  slave alias language::member language::member
  namespace export member


  # --- signature ----------------------------------------------------------

  #/
  # [[signature]]
  # === `language signature`: Code for declaring codel signatures
  # ****
  # `language signature` 'codel' ['separator'] ['location']
  # ****
  #
  # Return the signature of a codel in the current language. If separator is
  # given, it is a string that is inserted between the return type of the codel
  # and the codel name (for instance, a `\n` in C so that the symbol name is
  # guaranteed to be on the first column).
  #
  # ==== Arguments
  # 'code'::
  # A 'codel' object.
  # 'separator'::
  # A string, inserted between the return type and the codel symbol name.
  # 'location'::
  # A boolean indicating whether to generate `#line` directives
  # corresponding to the codel location in `.gen` file.
  #
  proc signature { codel {symchar { }} {location off}} {
    variable current
    return [${current}::signature $codel $symchar $location]
  }
  slave alias language::signature language::signature
  namespace export signature


  # --- invoke -------------------------------------------------------------

  #/
  # [[invoke]]
  # === `language invoke`: Code for calling codels
  # ****
  # `language invoke` 'codel' 'params'
  # ****
  #
  # Return a string corresponding to the invocation of a codel in the current
  # language.
  #
  # ==== Arguments
  # 'codel'::
  # A 'codel' object.
  # 'params'::
  # The list of parameters passed to the codel. Each element of this list must
  # be a valid string in the current language corresponding to each parameter
  # value or reference to be passed to the codel (<<pass,`language pass`>>).
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
