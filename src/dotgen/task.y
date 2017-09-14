/*
 * Copyright (c) 2009-2012,2014,2017 LAAS/CNRS
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

/*/
 * Task declaration
 * ----------------
 *
 * Tasks define an execution context suitable for running the codels of
 * link:service{outfilesuffix}[activities]).
 *
 * ****
 * [horizontal]
 * <dotgen-rule-task.adoc
 * <dotgen-rule-opt-properties.adoc
 * <dotgen-rule-properties.adoc
 * <dotgen-rule-property.adoc
 * ****
 *
 * A task may optionally define a state machine with
 * link:codel{outfilesuffix}[codels]. In this case, the state machine runs in
 * the background as soon as the component starts, independently of any
 * external service request. The state machine always starts in the `start`
 * state. If a codel yields to the `ether` state, the state machine stops.
 * When the component is shutdown, the `stop` codel (if it exists) is always
 * invoked.
 *
 * In any case, the task is always available for processing external service
 * requests, no matter if it has its own state machine or not.
 *
 * ****
 * [horizontal]
 * <dotgen-rule-fsm-codel.adoc
 * ****
 *
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
 * The task declaration also defines a few properties like a priority and a
 * stack size. They are not necessarily used by all templates, but this allows
 * to target real-time embedded systems.
 *
 * ****
 * [horizontal]
 * <dotgen-rule-task-property.adoc
 * ****
 *
 * +period+::
 *   The granularity of the codel scheduler. Periodic task will sequence the
 *   codels they manage at that frequency. If unspecified, the task has no
 *   period and sequences the codels as fast as possible.
 *
 * +delay+::
 *   The initial delay before starting the scheduler. This can be used to delay
 *   two tasks running at the same period in the same component.
 *
 * +priority+::
 *   Can be used to prioritize different tasks whithin the same component.
 *   Priorites are expressed as a integer between 0 and 255 (0 beeing the
 *   highest priority and 255 the lowest). This may not
 *   be supported by all templates.
 *
 * +scheduling real-time+::
 *   This indicates that the task requires real-time scheduling. This may not
 *   be supported by all templates.
 *
 * +stack+::
 *   Defines the required stack size for this task (in bytes if no unit is
 *   given). The stack size should be big enough to run all codels that the
 *   task manages. This may not be supported by all templates.
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

/*/
 * === Example
 *
 * The following declaration declares a task with an initialization codel
 * running only once when the task starts:
 *
 * [source,C]
 * ----
 * component foo {
 *   task t {
 *     codel <start> init_codel() yield ether;
 *   };
 * };
 * ----
 *
 * The following declare a periodic task, running some code in the background:
 *
 * [source,C]
 * ----
 * component foo {
 *   ids { long data };
 *
 *   task t {
 *     priority 100;
 *     stack 64k;
 *     period 50ms;
 *
 *     codel <start> do_init(out data) yield compute;
 *     codel <compute> do_compute(inout data) yield compute, stop;
 *     codel <stop> do_cleanup(in data) yield ether;
 *   };
 * };
 * ----
 */
