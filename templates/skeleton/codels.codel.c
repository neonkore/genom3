<'
# Copyright (c) 2010 LAAS/CNRS
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
#                                           Anthony Mallet on Sun Jan 24 2010

# check arguments
if {[llength $argv] != 2} { error "expected arguments: component task" }
lassign $argv component task

# generate copyright notice (if any)
catch {puts [language comment c [dotgen input notice]]}

# mapcar-like proc
proc map {lambda list} {
    set result {}
    foreach item $list {
	lappend result [apply $lambda $item]
    }
    return $result
}
'>

#include "<"[$component name]">_types.h"

<'# --- Task codels -------------------------------------------------------- '>
<'if {$task ne ""} {'>

/* --- Task <"[language hfill "[$task name] " - 62]"> */
<' foreach codel [$task codels] { '>


/** Codel <"[$codel name]"> of task <"[$task name]">.
 *
 * Triggered by <"[join [map {e {return [$e name]}} [$codel triggers]] {, }]">.
 * Yields to <"[join [map {e {return [$e name]}} [$codel yields]] {, }]">.
 * Throws <"[join [map {e {return [$e name]}} [$task throws]] {, }]">.
 */
<"[language signature c $codel \n]">
{
  /* insert your code */
  return <"[language declarator c [lindex [$codel yields] 0]]">;
}
<' } '>
<'}'>


<'# --- Validation codels -------------------------------------------------- '>
<'if {$task eq ""} {'>

<' foreach service [$component services] {
      if {[llength [$service validate]] == 0} continue'>
/* --- Service <"[language hfill "[$service name] " - 59]"> */
<'       foreach codel [$service validate] { '>


/** Validation codel <"[$codel name]"> of service <"[$service name]">.
 *
 * Returns <"[join [map {e {return [$e name]}} [$codel yields]] {, }]">.
 * Throws <"[join [map {e {return [$e name]}} [$service throws]] {, }]">.
 */
<"[language signature c $codel \n]">
{
  /* insert your code */
  return <"[language declarator c [lindex [$codel yields] 0]]">;
}
<'       } '>
<'    } '>
<'} '>


<'# --- Services codels ---------------------------------------------------- '>

<' foreach service [$component services] {
      if {[catch {$service task} t]} { set t "" }
      if {$t != $task || [llength [$service codels]] == 0} {
         continue
      } '>
/* --- Service <"[language hfill "[$service name] " - 59]"> */
<'    foreach codel [$service codels] { '>


/** Codel <"[$codel name]"> of service <"[$service name]">.
 *
 * Triggered by <"[join [map {e {return [$e name]}} [$codel triggers]] {, }]">.
 * Yields to <"[join [map {e {return [$e name]}} [$codel yields]] {, }]">.
 * Throws <"[join [map {e {return [$e name]}} [$service throws]] {, }]">.
 */
<"[language signature c $codel \n]">
{
  /* insert your code */
  return <"[language declarator c [lindex [$codel yields] 0]]">;
}
<'    } '>
<' } '>
