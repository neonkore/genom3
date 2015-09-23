#
# Copyright (c) 2015 LAAS/CNRS
# All rights reserved.
#
# Permission to use, copy, modify, and distribute this software for any purpose
# with or without   fee is hereby granted, provided   that the above  copyright
# notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
# REGARD TO THIS  SOFTWARE INCLUDING ALL  IMPLIED WARRANTIES OF MERCHANTABILITY
# AND FITNESS. IN NO EVENT SHALL THE AUTHOR  BE LIABLE FOR ANY SPECIAL, DIRECT,
# INDIRECT, OR CONSEQUENTIAL DAMAGES OR  ANY DAMAGES WHATSOEVER RESULTING  FROM
# LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
# OTHER TORTIOUS ACTION,   ARISING OUT OF OR IN    CONNECTION WITH THE USE   OR
# PERFORMANCE OF THIS SOFTWARE.
#
#                                             Anthony Mallet on Tue Sep 22 2015
#

# --- thread-safe-codel ----------------------------------------------------
#
# Return an empty list if a codel can safely execute in parallel with other
# codels of other tasks. This is a static analysis of all the FSMs, so it's
# suboptimal.
#
# If the codel is not thread safe, the function returns the list of conflicting
# codels.
#
proc thread-safe-codel { component codel } {
  set conflicts [list]

  # codel info
  set input [list]
  set output [list]
  set all [list]
  foreach p [$codel parameters] {
    if {[$p src] eq "local"} continue
    switch -- [$p dir] {
      in { lappend input $p; lappend all $p }
      inout { lappend input $p; lappend output $p; lappend all $p }
      out { lappend output $p; lappend all $p }

      default { error "unsupported direction" }
    }
  }

  if {[catch {$codel task} task]} { set task "" }

  # scan permanent activities in all other tasks
  foreach t [$component tasks] {
    if {$t == $task} continue

    # do not consider the start state which is guaranteed to have run already
    set start [$t fsm ::[$component name]::start]
    if {[llength $start]} {
      foreach c [codel-reachable $t [$start yield]] {
        lappend conflicts {*}[codel-intersect $input $output $c]
      }
    }
  }

  # scan all services in other tasks
  foreach s [$component services] {
    if {[catch {$s task} t]} { set t "" }
    if {$t == $task} continue

    # service ids parameters
    foreach p [$s parameters] {
      if {[$p src] ne "ids"} continue
      switch -- [$p dir] {
        in - inout { set check $all }
        out { set check $output }
      }

      foreach q $check {
        if {[parameter-match $p $q]} {
          lappend conflicts $s
          break
        }
      }
      if {$s in $conflicts} break
    }

    # validation codels and regular codels
    foreach c [$s validate] {
      lappend conflicts {*}[codel-intersect $input $output $c]
    }

    # do not further consider this service if it interrupts the service to
    # which the given codel belongs
    if {![catch {$codel service} service]} {
      if {$service in [$s interrupts]} continue
    }

    # regular codels
    foreach c [list {*}[$s validate] {*}[$s codels]] {
      lappend conflicts {*}[codel-intersect $input $output $c]
    }
  }

  return [lsort -unique $conflicts]
}


# --- thread-safe-error ----------------------------------------------------
#
# Log an error message with the conflicts
#
proc thread-safe-error { codel conflicts } {
  set file [lindex [split [lindex [$codel loc] 0] /] end]
  set prefix "$file:[lindex [$codel loc] 1]"
  append prefix " async codel [$codel name] is incompatible with"

  foreach c $conflicts {
    set file2 [lindex [split [lindex [$c loc] 0] /] end]
    set file2 "$file2:[lindex [$c loc] 1]"

    puts stderr "$prefix [$c name]"
    puts stderr "$file2: [$c name] defined here"
  }
  return -code error "aborting"
}


# --- internal helper functions --------------------------------------------

proc codel-reachable { obj states {codels {}} } {
  foreach e $states {
    set c [$obj fsm $e]
    if {$c eq ""} continue
    if {$c in $codels} continue

    lappend codels $c
    lappend codels {*}[codel-reachable $obj [$c yields] $codels]
  }

  return $codels
}

proc codel-intersect { input output codel } {
  set output2 [list]
  set all2 [list]
  foreach p [$codel parameters] {
    if {[$p src] eq "local"} continue
    switch -- [$p dir] {
      in { lappend all2 $p }
      inout - out { lappend output2 $p; lappend all2 $p }

      default { error "unsupported direction" }
    }
  }
  foreach p [$codel parameters out inout] {
    if {[$p src] ne "local"} { lappend output2 $p }
  }

  foreach p $input {
    foreach q $output2 {
      if {[parameter-match $p $q]} { return $codel }
    }
  }
  foreach p $output {
    foreach q $all2 {
      if {[parameter-match $p $q]} { return $codel }
    }
  }

  return [list]
}

proc parameter-match { p q } {
  if {[$p base] ne [$q base]} { return no }
  set pm [$p member]
  set qm [$q member]

  if {$pm eq ""} { return yes }
  if {$qm eq ""} { return yes }
  if {[string first $pm $qm] == 0} { return yes }
  if {[string first $qm $pm] == 0} { return yes }
  return no
}
