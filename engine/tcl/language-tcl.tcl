#
# Copyright (c) 2011 LAAS/CNRS
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
#                                           Anthony Mallet on Mon Jun 20 2011
#
package require Tcl 8.5

namespace eval language::tcl {

  # --- comment ----------------------------------------------------------

  # Return a string that is a valid comment in TCL.
  #
  proc comment { text } {
    return [regsub -all {#[ ]+\n} #[join [split $text "\n"] "\n# "] "#\n"]
  }

  # --- iter -------------------------------------------------------------

  # Return a TCL loop construct for 'type'. 'part' may be set to begin, end
  # or var.
  #
  proc iter { type part {level 0} {max {}} } {
    switch -- $part {
      begin {
        set v [iter $type var $level]
        if {[llength $max] == 0} {
          if {[catch {format "\${$v} < [$type length]"} max]} {
            set max {1}
          }
        } else {
          set max "$v < $max"
        }
        switch -- [$type kind] {
          {array} - {sequence} {
            lappend code "for {set $v 0} {${max}} {incr $v} \{" ++
          }

          default {
            template fatal "type '[$type kind]' cannot be iterated"
          }
        }
      }

      end {
        lappend code -- "\}"
      }

      var {
        lappend code "_genomi${level}"
      }
    }

    return $code
  }
}
