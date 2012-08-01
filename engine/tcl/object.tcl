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
      component - service - remote - type { $class-digest $object }
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
      md5::update [$s kind]
      md5::update [$s name]
      service-digest $s
    }
  }


  # --- service-digest -----------------------------------------------------

  # Compute a md5 hash of the interface of a service (must be called from
  # digest).
  #
  proc service-digest { service } {
    foreach p [$service parameters] {
      md5::update [$p dir]
      type-digest [$p type]
    }
  }


  # --- remote-digest -----------------------------------------------------

  # Compute a md5 hash of the interface of a remote (must be called from
  # digest).
  #
  proc remote-digest { remote } {
    foreach p [$remote parameters] {
      md5::update [$p dir]
      type-digest [$p type]
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


  namespace ensemble create
}
