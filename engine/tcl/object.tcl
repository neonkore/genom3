#
# Copyright (c) 2010-2013 LAAS/CNRS
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

  # --- types --------------------------------------------------------------

  # Return the list of types defined by a genom object.
  #
  proc types { object visibility {filter {v {return 1}}} } {
    if {[catch {$object class} class]} { error "not a genom object" }

    switch $visibility {
      public - private {}
      default { error "bad parameter $visibility: must be public or private" }
    }

    switch $class {
      component - type {
        set types [$class-types $object $visibility $filter]
      }

      default { error "bad genom object type $class" }
    }

    return [dict values $types]
  }
  namespace export types


  # --- component-types ----------------------------------------------------

  # Return the list of types defined by a component (must be called from
  # types).
  #
  proc component-types { component visibility {filter {v {return 1}}} } {
    set types [dict create]
    if {![eval [list apply $filter $component]]} return $types

    foreach p [$component ports] {
      set r [type-types [$p datatype] $visibility $filter]
      set types [dict merge $types $r]

      if {$visibility == "private"} {
        set r [type-types [$p type] $visibility $filter]
        set types [dict merge $types $r]
      }
    }

    foreach s [$component services] {
      foreach p [$s parameters] {
        switch [$p dir] {
          "inport" - "outport" {
            # handled above
          }
          default {
            set r [type-types [$p type] $visibility $filter]
            set types [dict merge $types $r]
          }
        }
      }

      foreach t [$s throws] {
        set types [dict merge $types [type-types $t $visibility $filter]]
      }
    }

    if {$visibility == "private" || $visibility == "extern"} {
      foreach r [$component remotes] {
        foreach p [$r parameters] {
          set r [type-types [$p type] $visibility $filter]
          set types [dict merge $types $r]
        }
        foreach t [$r throws] {
          set types [dict merge $types [type-types $t $visibility $filter]]
        }
      }
    }

    if {$visibility == "private"} {
      if {![catch {$component ids} ids]} {
        set types [dict merge $types [type-types $ids $visibility $filter]]
      }
    }

    return $types
  }


  # --- type-types ---------------------------------------------------------

  # Return the list of recursive types defined by a type (must be called from
  # types).
  #
  proc type-types { type visibility {filter {v {return 1}}} } {
    if {![eval [list apply $filter $type]]} return [dict create]

    set types [dict create]
    switch -- [$type kind] {
      array - sequence - typedef {
        set r [type-types [$type type] $visibility $filter]
        set types [dict merge $types $r]
        dict set types [$type mangle] $type
      }

      {struct member} - {union member} {
        set r [type-types [$type type] $visibility $filter]
        set types [dict merge $types $r]
      }

      struct - union - exception {
        foreach e [$type members] {
          set types [dict merge $types [type-types $e $visibility $filter]]
        }
        dict set types [$type mangle] $type
      }

      default {
        dict set types [$type mangle] $type
      }
    }

    return $types
  }


  # --- digest -------------------------------------------------------------

  # Compute a md5 hash of a genom object.
  #
  proc digest { object {filter {v {return 1}}} } {
    if {[catch {$object class} class]} { error "not a genom object" }

    md5::init
    switch $class {
      component - service - remote - type { $class-digest $object $filter }
    }
    return [md5::final]
  }
  namespace export digest


  # --- component-digest ---------------------------------------------------

  # Compute a md5 hash of the public interface of a component (must be called
  # from digest).
  #
  proc component-digest { component filter } {
    if {![eval [list apply $filter $component]]} continue

    foreach p [$component ports] {
      if {![eval [list apply $filter $p]]} continue

      md5::update port
      md5::update [$p kind]
      md5::update [$p name]
      type-digest [$p datatype] $filter
    }

    foreach s [$component services] {
      if {![eval [list apply $filter $s]]} continue

      md5::update [$s kind]
      md5::update [$s name]
      service-digest $s $filter
    }
  }


  # --- service-digest -----------------------------------------------------

  # Compute a md5 hash of the interface of a service (must be called from
  # digest).
  #
  proc service-digest { service filter } {
    if {![eval [list apply $filter $service]]} continue

    foreach p [$service parameters] {
      if {![eval [list apply $filter $p]]} continue

      md5::update [$p dir]
      type-digest [$p type] $filter
    }
    foreach t [$service throws] {
      type-digest $t $filter
    }
  }


  # --- remote-digest -----------------------------------------------------

  # Compute a md5 hash of the interface of a remote (must be called from
  # digest).
  #
  proc remote-digest { remote filter } {
    if {![eval [list apply $filter $remote]]} continue

    foreach p [$remote parameters] {
      if {![eval [list apply $filter $p]]} continue

      md5::update [$p dir]
      type-digest [$p type] $filter
    }
    foreach t [$remote throws] {
      type-digest $t $filter
    }
  }


  # --- type-digest --------------------------------------------------------

  # Compute a md5 hash of a type (must be called from digest).
  #
  proc type-digest { type filter } {
    if {![eval [list apply $filter $type]]} continue

    switch -- [$type kind] {
      string {
        md5::update string
        if {![catch {$type length} l]} {
          md5::update $l
        }
      }

      const - typedef - {forward struct} - {forward union} - {struct member} {
        type-digest [$type type] $filter
      }

      array - sequence {
        md5::update [$type kind]
        if {![catch { $type length } l]} {
          md5::update $l
        }
        type-digest [$type type] $filter
      }

      enum - struct - union - exception {
        foreach e [$type members] {
          md5::update [$e name]
          type-digest $e $filter
        }
      }

      default {
        md5::update [$type kind]
      }
    }
  }


  namespace ensemble create
}
