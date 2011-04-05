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
#                                           Anthony Mallet on Wed Mar 30 2011
#

package require Tcl 8.5

namespace eval merge {

  # --- auto ---------------------------------------------------------------
  #
  # interactively merge src and dst files into dst
  #
  proc auto { src dst interactive } {

    # read the two files
    set f [open $src r]
    set lsrc [split [read $f] \n]
    close $f
    set f [open $dst r]
    set ldst [split [read $f] \n]
    close $f

    # open temp destination
    set tmp [engine::mktemp]
    set f [open $tmp w]

    # set diff context
    if {[catch {file stat $dst sb}]} { set sb(mtime) [clock seconds] }
    set header "--- $dst\t[clock format $sb(mtime)]\n"
    if {[catch {file stat $src sb}]} { set sb(mtime) [clock seconds] }
    append header "+++ $dst\t[clock format $sb(mtime)]\n"
    set conflicts 0

    # set engine verbose mode and backup previous setting
    set verb [engine mode]
    set verb [lindex $verb [lsearch -glob $verb {?verbose}]]
    engine mode +verbose

    # iterate over the common sequence between the two files
    set isrc 0
    set idst 0
    foreach { x1 x2 } [lcs $lsrc $ldst] {
      foreach jsrc $x1 jdst $x2 {

        # lines of context before/after the current hunk
        set pre [lrange $ldst [expr {$idst-3}] [expr {$idst-1}]]
        set post [lrange $ldst $jdst [expr {$jdst+2}]]

        # lines only in dst
        set odst [list]
        set nodst 0
        while { $idst < $jdst } {
          set l [lindex $ldst $idst]
          if {[string trim $l] != "" && ![regexp {insert your code} $l]} {
            incr nodst
          }
          lappend odst $l
          incr idst
        }

        # lines only in src
        set osrc [list]
        set nosrc 0
        while { $isrc < $jsrc } {
          set l [lindex $lsrc $isrc]
          if {[string trim $l] != "" && ![regexp {insert your code} $l]} {
            incr nosrc
          }
          lappend osrc $l
          incr isrc
        }

        # automatic merge
        if {$nosrc && $nodst} {
          if {$interactive} {
            set dline [expr $idst - [llength $odst]]
            set slen [expr [llength $pre] + [llength $post] + [llength $osrc]]
            set dlen [expr [llength $pre] + [llength $post] + [llength $odst]]
            set patch $header
            append patch "@@ -$dline,$dlen +$dline,$slen @@\n"
            append patch \ [join $pre "\n "]\n
            append patch -[join $odst "\n-"]\n
            append patch +[join $osrc "\n+"]\n
            append patch \ [join $post "\n "]
            puts $patch
            while 1 {
              puts -nonewline {Apply this patch [y,n,c,q,?]? }
              flush stdout
              if {[gets stdin act] < 0} { set act q }
              switch -- $act {
                y { puts $f [join $osrc "\n"]; break }
                n { puts $f [join $odst "\n"]; break }
                c {
                  puts $f "<<<<<<< user version"
                  puts $f [join $odst "\n"]
                  puts $f "======="
                  puts $f [join $osrc "\n"]
                  puts $f ">>>>>>> template version"
                  incr conflicts
                  break
                }
                q { close $f; return 0 }
                default {
                  puts $patch
                  puts "y - apply this patch"
                  puts "n - skip this patch"
                  puts "c - generate a conflict marker"
                  puts "q - leave this file unchanged"
                  puts "? - print help"
                }
              }
            }
          } else {
            puts $f "<<<<<<< user version"
            puts $f [join $odst "\n"]
            puts $f "======="
            puts $f [join $osrc "\n"]
            puts $f ">>>>>>> template version"
            incr conflicts
          }
        } elseif {$nosrc} {
          puts $f [join $osrc "\n"]
        } elseif {[llength $odst]} {
          if {$idst < [llength $ldst]} {
            puts $f [join $odst "\n"]
          }
        }

        if {$idst < [llength $ldst]} {
          set l [lindex $ldst $idst]
          puts $f $l
        }
        incr isrc
        incr idst
      }
    }

    engine mode $verb
    close $f

    # move to final destination
    file copy -force $tmp $dst
    if {$conflicts} {
      template message "CONFLICTS during merge of $dst"
    }
    return 0
  }

  proc custom { tool src dst } {
    return [catch {exec $tool $src $dst}]
  }
}
