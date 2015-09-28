<'
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
#                                           Anthony Mallet on Sun Jan 24 2010

# check arguments
if {[llength $argv] != 2} { error "expected arguments: component task" }
lassign $argv component task

# mapcar-like proc
proc map {lambda list} {
    set result {}
    foreach item $list {
	lappend result [apply $lambda $item]
    }
    return $result
}

lang c

# generate copyright notice (if any)
if {![catch {dotgen input notice} text]} {
  puts [comment $text]
}

'>
#include "ac<"[$component name]">.h"

#include "<"[$component name]_c">_types.h"
<'
# --- Task codels --------------------------------------------------------
if {$task ne ""} {'>


/* <"[--- Task [$task name] ----------------------------------------------]"> */
<'  foreach codel [$task codels] {'>


/** Codel <"[$codel name]"> of task <"[$task name]">.
 *
<'    if {[llength [$codel triggers]]} {'>
<'      set triggers [join [map {e {return [$e cname]}} [$codel triggers]] {, }]'>
<"[wrap " * Triggered by $triggers." { * }]">
<'    }'>
<'    set yields [join [map {e {return [$e cname]}} [$codel yields]] {, }]'>
<"[wrap " * Yields to $yields." { *           }]">
<'    if {[llength [$task throws]]} {'>
<'      set throws [join [map {e {return [$e cname]}} [$task throws]] {, }]'>
<"[wrap " * Throws $throws." { *        }]">
<'    }'>
 */
<'
    if {![catch {dict get $generated_codels [$codel name]} prev]} {
      puts "\n/* already defined in $prev */"
      continue
    }
    dict set generated_codels [$codel name] "task [$task name]"
'>
<"[$codel signature \n]">
{
  /* skeleton sample: insert your code */
  /* skeleton sample */ return <"[[lindex [$codel yields] 0] cname]">;
}
<' } '>
<'}


# --- Validation codels --------------------------------------------------

if {$task eq ""} {
  foreach service [$component services] {
    if {[llength [$service validate]] == 0} continue'>


/* <"[--- [string toupper [$service kind] 0] [$service name] ------------]"> */
<'    foreach codel [$service validate] {'>

/** Validation codel <"[$codel name]"> of <"[$service kind]"> <"[$service name]">.
 *
 * Returns genom_ok.
<'   set throws [join [map {e {return [$e cname]}} [$service throws]] {, }]'>
<"[wrap " * Throws $throws." { * }]">
 */
<'
      if {![catch {dict get $generated_codels [$codel name]} prev]} {
        puts "\n/* already defined in $prev */"
        continue
      }
      dict set generated_codels [$codel name] "service [$service name] validation"
'>
<"[$codel signature \n]">
{
  /* skeleton sample: insert your code */
  /* skeleton sample */ return genom_ok;
}
<'       } '>
<'    } '>
<'}

# --- Services codels ----------------------------------------------------

foreach service [$component services] {
  if {[catch {$service task} t]} { set t "" }
  if {$t != $task || [llength [$service codels]] == 0} {
    continue
  } '>


/* <"[--- [string toupper [$service kind] 0] [$service name] ------------]"> */
<' foreach codel [$service codels] {'>

/** Codel <"[$codel name]"> of <"[$service kind]"> <"[$service name]">.
 *
<'   if {[llength [$codel triggers]]} {'>
<'     set triggers [join [map {e {return [$e cname]}} [$codel triggers]] {, }]'>
<"[wrap " * Triggered by $triggers." { * }]">
<'   }'>
<'   if {[llength [$codel yields]]} {'>
<'    set yields [join [map {e {return [$e cname]}} [$codel yields]] {, }]'>
<"[wrap " * Yields to $yields." { *           }]">
<'   } else {'>
 * Returns genom_ok.
<'   }'>
<'   if {[llength [$service throws]]} {'>
<'     set throws [join [map {e {return [$e cname]}} [$service throws]] {, }]'>
<"[wrap " * Throws $throws." { *        }]">
<'   }'>
 */
<'
   if {![catch {dict get $generated_codels [$codel name]} prev]} {
     puts "\n/* already defined in $prev */"
     continue
   }
   dict set generated_codels [$codel name] "service [$service name]"
'>
<"[$codel signature \n]">
{
  /* skeleton sample: insert your code */
<'   if {[llength [$codel yields]]} {'>
  /* skeleton sample */ return <"[[lindex [$codel yields] 0] cname]">;
<'   } else {'>
  /* skeleton sample */ return genom_ok;
<'   }'>
}
<'    } '>
<' } '>
