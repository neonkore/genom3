#
# Copyright (c) 2010-2012 LAAS/CNRS
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
#                                           Anthony Mallet on Wed Jun 30 2010
#

namespace eval object {

  # --- digest -------------------------------------------------------------

  # Compute a md5 hash of a genom object.
  #
  proc digest { object } {
    if {[catch {$object class} class]} { error "not a genom object" }

    md5::init
    switch $class {
      component - type { $class-digest $object }
    }
    return [md5::final]
  }
  namespace export digest


  # --- component-digest ---------------------------------------------------

  # Compute a md5 hash of the public interface of a component (must be called
  # from digest).
  #
  proc component-digest { component } {
    md5::update events
    type-digest [$component event]

    foreach p [$component ports] {
      md5::update port
      md5::update [$p kind]
      md5::update [$p name]
      type-digest [$p datatype]
    }

    foreach s [$component services] {
      md5::update service
      md5::update [$s name]
      foreach p [$s parameters] {
        md5::update [$p dir]
        md5::update [$p name]
        type-digest [$p type]
      }
    }
  }


  # --- type-digest --------------------------------------------------------

  # Compute a md5 hash of a type (must be called from digest).
  #
  proc type-digest { type } {
    switch -- [$type kind] {
      string {
        md5::update string
        if {![catch {$type length} l]} {
          md5::update $l
        }
      }

      const - typedef - {forward struct} - {forward union} - {struct member} {
        type-digest [$type type]
      }

      array - sequence {
        md5::update [$type kind]
        if {![catch { $type length } l]} {
          md5::update $l
        }
        type-digest [$type type]
      }

      enum - struct - union {
        foreach e [$type members] {
          md5::update [$e name]
          type-digest $e
        }
      }

      default {
        md5::update [$type kind]
      }
    }
  }


  # --- type-iter ----------------------------------------------------------

  # Internal implementation of type-foreach and type-with
  #
  proc type-iter { type base vlist body hier depth prefix} {
    set code [list]

    # set current type kind and var
    set k [$type kind]
    if {[llength $hier] && [llength $vlist] > 2} {
      set var "$prefix[language member $base $hier]"
    } else {
      set var $prefix
    }

    # invoke body for current type
    uplevel [list lassign [list $type $k $var] {*}$vlist]
    if {[catch {uplevel $body} r opts]} {
      switch [dict get $opts -code] {
        0	{ lappend code {*}$r }
        default	{ return -options $opts $r }
      }
    }

    # recurse into members - if needed
    switch -- $k {
      {enum} {
        foreach m [$type members] {
          lappend code {*}[uplevel [list object::type-iter $m $base	\
                                        $vlist $body $hier $depth $prefix]]
        }
      }

      {struct} - {union} {
        foreach m [$type members] {
          lappend hier [$m name]
          lappend code {*}[uplevel [list object::type-iter $m $base	\
                                        $vlist $body $hier $depth $prefix]]
          set hier [lrange $hier 0 end-1]
        }
      }

      {const} - {typedef} - {struct member} - {union member} {
        lappend code {*}[uplevel [list object::type-iter		\
                                      [$type type] $base		\
                                      $vlist $body $hier $depth $prefix]]
      }

      {array} - {sequence} {
        incr depth
        lappend hier [language::iter $type var $depth]
        set loop [list]

        uplevel [list lassign [list $type "$k element" $var] {*}$vlist]
        if {[catch {uplevel $body} r opts]} {
          switch [dict get $opts -code] {
            0		{ lappend loop {*}$r }
            default	{ return -options $opts $r }
          }
        }

        lappend loop {*}[uplevel [list object::type-iter		\
                                      [$type type] $base		\
                                      $vlist $body $hier $depth $prefix]]
        uplevel [list lassign [list $type "/$k element" $var] {*}$vlist]
        if {[catch {uplevel $body} r opts]} {
          switch [dict get $opts -code] {
            0		{ lappend loop {*}$r }
            default	{ return -options $opts $r }
          }
        }

        if {[llength $loop]} {
          if {[catch {$type length} l]} {
            lappend code {*}[language::iter $type begin $depth $var._length]
          } else {
            lappend code {*}[language::iter $type begin $depth]
          }
          lappend code {*}$loop
          lappend code {*}[language::iter $type end $depth]
        }

        set hier [lrange $hier 0 end-1]
        incr depth -1
      }

      {forward struct} - {forward union} {
        # note: this is deliberately entering a potentially endless
        # recursion if the body script never issue a 'continue' statement
        # to skip this branch.
        lappend code {*}[uplevel [list type-iter			\
                                      [$type type] $base		\
                                      $vlist $body $hier $depth $prefix]]
      }
    }

    # invoke body for the end of the current object - if needed
    switch -- $k {
      {enum} - {array} - {sequence} - {struct} - {struct member} - {union} -
      {forward struct} - {forward union} {
        uplevel [list lassign [list $type /$k $var] {*}$vlist]
        if {[catch {uplevel $body} r opts]} {
          switch [dict get $opts -code] {
            0		{ lappend code {*}$r }
            default	{ return -options $opts $r }
          }
        }
      }
    }

    return $code
  }


  # --- type-foreach -------------------------------------------------------

  # \proc {\em \$type} foreach {\em varlist} ?with {\em prefix}? {\em body}
  # \index foreach
  #
  # Evaluate {\em body} for each member of {\em type}, with {\em varlist} set
  # to the current element. The result of the command is an empty string.
  #
  # {\em varlist} is a list of one up to three variable names. The first
  # variable is set to the current current type element, the second variable is
  # set to the element kind and the third variable (if present) is set to a
  # string representing the code (in the current language) for accessing the
  # current element. If "with {\em prefix}" was given, this string is prefixed
  # with the {\em prefix} string, so that if prefix contains the top variable
  # containing {\em type}, the computed expresion will directly access the
  # current element.
  #
  # Exploration of the type tree is done in depth-first order. {\em body} shall
  # return a list of strings (maybe empty) that are concatenated and passed to
  # "indent" when the function returns.
  #
  # Note: an endless recursion is possible if the type in question has a
  # recursive definition (e.g. a structure contains a sequence of the same
  # structure). This potentially endless recursion is allowed on purpose, but
  # it is important that you handle this situation in the {\em body} script. A
  # potentially endless recursion can be detected if your {\em body} script
  # encounters either a {\tt forward struct} or a {\tt forward union}. It is up
  # to the caller to determine what to do in this case, but this typically
  # involves returning 'continue' at some point to skip further exploration of
  # that branch.
  #
  # \arg \$type	A type object.
  # \arg varlist
  #		A list of variable names of 1, 2 or 3 elements. First variable
  #		is set to the current element kind, second variable is set to
  #		the current element of object while iterating and optional
  #		third element is set to the current element and the hierarchy
  #		of elements leading to the current element.
  # \arg prefix	A string prepended to the 3rd element of {\em
  #		varlist}. Typically contains a reference to the variable
  #		holding {\em \$type}.
  # \arg body	A script evaluated for each element of object.
  #
  proc type-foreach { type vlist args } {
    switch [llength $args] {
      1	{
        set prefix ""
        set body [lindex $args 0]
      }
      3 {
        if {[lindex $args 0] ne "with"} {
          error "unknown argument \"[lindex $args]\": must be \"with\""
        }
        set prefix [lindex $args 1]
        set body [lindex $args 2]
      }
      default {
        error "wrong # args: should be \$type foreach vlist ?with prefix? body"
      }
    }
    set l [uplevel \
               [list object::type-iter $type $type $vlist $body {} 0 $prefix]]
    indent {*}$l
  }


  # the type-iter procedures cannot be aliased because it runs uplevel scripts
  # that must run in the slave's context. The easiest is to create each
  # procedure in the slave interpreter
  foreach p {type-iter type-foreach} {
    slave eval \
        [list namespace eval object \
             [list proc $p [info args $p] [info body $p]]]
  }

  namespace ensemble create
}
