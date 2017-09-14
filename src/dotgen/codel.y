/*
 * Copyright (c) 2009-2015,2017 LAAS/CNRS
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
 * Codel declaration
 * -----------------
 *
 */

/*/
 * Codels are callbacks to the actual code implementing services in a
 * component. There are two kind of codels: `simple codels` and `finite state
 * machine codels`.
 *
 * ****
 * [horizontal]
 * <dotgen-rule-codel-property.adoc
 * <dotgen-rule-opt-async.adoc
 * ****
 *
 * Codels are normally functions that execute in a short amount of time
 * compared to the time characteristics of the component. During the execution
 * of a codel, the component is typically blocked. If a codel is
 * expected to last much longer than the desired reaction time of the
 * component, or block until some external event arrives, it can be labelled
 * `async`.
 *
 *
 * === Simple codels
 *
 * Simple codels are functions associated to service validation callbacks or
 * implementing component `functions`.
 *
 * ****
 * [horizontal]
 * <dotgen-rule-codel.adoc
 * ****
 *
 * The +identifier+ is the name of the external function implementing the
 * codel. +parameters+ are described in the third section.
 *
 * === Finite state machine codels
 *
 * Finite state machine (FSM) codels are functions associated to the execution
 * of `activities`.
 *
 * ****
 * [horizontal]
 * <dotgen-rule-fsm-codel.adoc
 * <dotgen-rule-event-list.adoc
 * <dotgen-rule-event-name.adoc
 * ****
 *
 * In addition to the +identifier+ and +parameters+ described in the previous
 * section, FSM codels define the state machine of their corresponding
 * `activity`. The state machine is defined with a list of states and possible
 * transitions.
 *
 * The bracketed +event list+, before the codel +identifier+ defines one (or
 * more) state of the FSM, and associates the codel with this state (or list of
 * states). When the FSM reaches the corresponding state, the codel is
 * executed. There are two predefined states : +start+ and +stop+. The +start+
 * state is required: this is the state in which the FSM begins its
 * execution. The +stop+ state is optional: the FSM goes into this state when
 * it receives an external interruption request.
 *
 * The +event list+ specified after the `yield` keyword statically defines all
 * possible transitions from this state to other states. For each possible
 * transition, a corresponding state must exist in the activity FSM. There is
 * one predefined transition called +ether+: this is the final state of the FSM
 * and it is used to indicate the regular termination of the activity. There
 * must always exist a path from the +start+ state to the +ether+ state. If a
 * +stop+ state is defined, a path from +stop+ to +ether+ must also exist.
 * The actual transition executed by the FSM depends on the codel function
 * return value.
 *
 * Transitions normally occur immediately, so that the FSM executes as fast as
 * possible. This execution can still be explicitely paused, for instance to
 * wait until the next period or until something new happens in the
 * component. For this purpose, a transition to a new state `s` can optionally
 * be defined in the `pause` namespace, by specifying the transition as
 * `pause::s`. A `paused` transition means that the FSM will pause its
 * execution either until the next period (within periodic tasks), or until a
 * new internal event occurs (within tasks with no period). Internal events
 * correspond to a +non-paused+ transition of another activity within the
 * component, or the successful execution of an `attribute` or `function`.
 *
 * === Example
 *
 * This would declare a FSM with one +main+ state, typically looping until some
 * condition is met, and a stop state in case of premature interruption.
 *
 * [source,C]
 * ----
 * codel<start> do_start()            yield main;
 * codel<main>  do_main(in condition) yield pause::main, ether;
 * codel<stop>  do_stop()             yield ether;
 * ----
 *
 * In this example, the +main+ loop will be paced according to the context in
 * which the codels are defined (a periodic task, for instance). Without the
 * `paused` transition, the loop would typically eat all the available CPU.
 *
 *
 * === Codel parameters
 *
 * ****
 * [horizontal]
 * <dotgen-rule-codel-parameters.adoc
 * <dotgen-rule-codel-parameter.adoc
 * <dotgen-rule-opt-parameter-src.adoc
 * <dotgen-rule-parameter-dir.adoc
 * <dotgen-rule-parameter-variable.adoc
 * ****
 */
codel:
  identifier '(' codel_parameters ')'
  {
    $$ = codel_create(@1, $1, CODEL_SYNC, NULL, NULL, $3);
  }
;

fsm_codel:
  '<' event_list '>' identifier '(' codel_parameters ')' YIELD event_list
  {
    if (!$2 || !$9) {
      parserror(@4, "dropped codel '%s'", $4); $$ = NULL; break;
    }
    $$ = codel_create(@4, $4, CODEL_SYNC, $2, $9, $6);
  }
  | '<' event_list '>' identifier '(' codel_parameters ')' error
  {
    $$ = NULL;
    parserror(@4, "missing 'yield' values for codel %s", $4);
    if ($2) hash_destroy($2, 1);
    if ($6) hash_destroy($6, 1);
  }
;

opt_async:
  /* empty */	{ $$ = CODEL_SYNC; }
  | ASYNC	{ $$ = CODEL_ASYNC; }
;

event_name:
  identifier
  {
    $$ = comp_addevent(@1, IDL_EVENT, $1);
  }
  | PAUSE COLONCOLON identifier
  {
    $$ = comp_addevent(@1, IDL_PAUSE_EVENT, $3);
  }
;

event_list:
  event_name
  {
    $$ = hash_create("event list", 2);
    if (!$$ || !$1) break;
    switch(hash_insert($$, type_fullname($1), $1, NULL)) {
      case EEXIST: parserror(@1, "duplicate event '%s'", type_name($1)); break;
    }
  }
  | event_list ',' event_name
  {
    $$ = $1; if (!$3) break;
    switch(hash_insert($$, type_fullname($3), $3, NULL)) {
      case EEXIST: parserror(@3, "duplicate event '%s'", type_name($3)); break;
    }
  }
;

codel_parameters:
  /* empty */
  {
    $$ = hash_create("parameter list", 0);
  }
  | codel_parameter
  {
    $$ = hash_create("parameter list", 3); if (!$$ || !$1) break;
    if (hash_insert($$, param_name($1), $1, (hrelease_f)param_destroy)) {
      param_destroy($1); break;
    }
  }
  | codel_parameters ',' codel_parameter
  {
    $$ = $1; if (!$3) break;
    switch(hash_insert($$, param_name($3), $3, (hrelease_f)param_destroy)) {
      case 0: break;
      case EEXIST:
	parserror(@3, "duplicate parameter '%s'", param_name($3));
	/*FALLTHROUGH*/
      default: param_destroy($3); break;
    }
  }
;

codel_parameter:
  opt_parameter_src parameter_dir parameter_variable
  {
    $$ = param_newcodel(@3, $1, $2, NULL, $3);
  }
  | opt_parameter_src parameter_dir parameter_variable COLONCOLON identifier
  {
    $$ = param_newcodel(@3, $1, $2, $5, $3);
  }
  | opt_parameter_src parameter_dir COLONCOLON identifier
  {
    $$ = param_newcodel(@3, $1, $2, $4, NULL);
  }
  | error { $$ = NULL; }
;

codel_property:
  opt_async CODEL codel semicolon
  {
    if ($3) codel_setkind($3, $1);
    $$ = $3 ? prop_newcodel(@1, PROP_SIMPLE_CODEL, $3) : NULL;
  }
  | opt_async CODEL fsm_codel semicolon
  {
    if ($3) codel_setkind($3, $1);
    $$ = $3 ? prop_newcodel(@1, PROP_FSM_CODEL, $3) : NULL;
  }
;
