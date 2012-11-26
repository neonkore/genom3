/*
 * Copyright (c) 2009-2012 LAAS/CNRS
 * All rights reserved.
 *
 * Redistribution  and  use  in  source  and binary  forms,  with  or  without
 * modification, are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of  source  code must retain the  above copyright
 *      notice and this list of conditions.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice and  this list of  conditions in the  documentation and/or
 *      other materials provided with the distribution.
 *
 * THE SOFTWARE  IS PROVIDED "AS IS"  AND THE AUTHOR  DISCLAIMS ALL WARRANTIES
 * WITH  REGARD   TO  THIS  SOFTWARE  INCLUDING  ALL   IMPLIED  WARRANTIES  OF
 * MERCHANTABILITY AND  FITNESS.  IN NO EVENT  SHALL THE AUTHOR  BE LIABLE FOR
 * ANY  SPECIAL, DIRECT,  INDIRECT, OR  CONSEQUENTIAL DAMAGES  OR  ANY DAMAGES
 * WHATSOEVER  RESULTING FROM  LOSS OF  USE, DATA  OR PROFITS,  WHETHER  IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR  OTHER TORTIOUS ACTION, ARISING OUT OF OR
 * IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 *                                           Anthony Mallet on Mon Apr 20 2009
 */

/*/ @node Task declaration
 * @section Task declaration
 * @cindex task, declaration
 * @cindex declaration, task
 *
 * @ruleinclude task
 * @ruleinclude opt_properties
 * @ruleinclude properties
 * @sp 1
 * @ruleinclude task_property
 * @ruleinclude codel_property
 *
 * Tasks define an execution context suitable for running @emph{activities}
 * (@pxref{Component declaration}). A task may define a state machine and
 * associated codels (@pxref{Codel declaration}). The state machine starts in
 * the @code{start} state when the task is created during component
 * initialization.
 */
task:
  TASK identifier opt_properties semicolon
  {
    if (!$2) { parserror(@1, "dropped task"); break; }
    if (!task_create(@1, $2, $3)) {
      parsenoerror(@1, "dropped '%s' task", $2);
      if ($3) hash_destroy($3, 1);
    }
  }
;

/*/
 * Tasks are named can also define the following properties:
 *
 * @table @code
 * @item period
 * The granularity of the codel scheduler. Periodic task will sequence the
 * codels they manage at that frequency.
 *
 * @item delay
 * The delay from the beginning of each period after which codels are run. This
 * can be used to delay two tasks running at the same period in the same
 * component.
 *
 * @item priority
 * Can be used to prioritize different tasks whithin the same component.
 *
 * @item scheduling real-time
 * This indicates that the task requires real-time scheduling. This may not be
 * supported by all templates.
 *
 * @item stack
 * Defines the required stack size for this task. The stack size should be big
 * enough to run all codels that the task manages.
 * @end table
 */
task_property:
  PERIOD const_expr time_unit semicolon
  {
    if (const_binaryop(&$2, '*', $3)) {
      parserror(@3, "invalid numeric constant");
      break;
    }
    $$ = prop_newvalue(@1, PROP_PERIOD, $2);
  }
  | DELAY const_expr time_unit semicolon
  {
    if (const_binaryop(&$2, '*', $3)) {
      parserror(@3, "invalid numeric constant");
      break;
    }
    $$ = prop_newvalue(@1, PROP_DELAY, $2);
  }
  | PRIORITY positive_int_const semicolon
  {
    $$ = prop_newvalue(@1, PROP_PRIORITY, $2);
  }
  | SCHEDULING REAL_TIME semicolon
  {
    $$ = prop_newstring(@1, PROP_SCHEDULING, $2);
  }
  | STACK positive_int_const size_unit semicolon
  {
    if (const_binaryop(&$2, '*', $3)) {
      parserror(@3, "invalid numeric constant");
      break;
    }
    $$ = prop_newvalue(@1, PROP_STACK, $2);
  }
;
