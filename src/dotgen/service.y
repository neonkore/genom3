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

/*/ @node Attribute declaration
 * @section Attribute declaration
 * @cindex attribute, declaration
 * @cindex declaration, attribute
 *
 * @ruleinclude attribute
 * @ruleinclude attribute_parameters
 * @ruleinclude attribute_parameter
 * @ruleinclude opt_properties
 * @ruleinclude properties
 * @sp 1
 * @ruleinclude service_property
 */
attribute:
  ATTRIBUTE identifier '(' attribute_parameters ')' opt_properties semicolon
  {
    task_p = 0;
    param_setlocals(NULL);
    if (!$2 || !$4) {
      if ($2) parserror(@1, "dropped '%s' attribute", $2);
      if ($4) hash_destroy($4, 1);
      if ($6) hash_destroy($6, 1);
      break;
    }
    if (!service_create(@1, S_ATTRIBUTE, $2, $4, $6)) {
      parserror(@1, "dropped '%s' attribute", $2);
      hash_destroy($4, 1);
      if ($6) hash_destroy($6, 1);
    }
  }
;

/*/ @node Service declaration
 * @section Service declaration
 * @cindex service, declaration
 * @cindex declaration, service
 *
 * @ruleinclude service
 * @ruleinclude service_kind
 * @ruleinclude service_parameters
 * @ruleinclude service_parameter
 * @ruleinclude opt_properties
 * @ruleinclude properties
 * @ruleinclude property
 * @sp 1
 * @ruleinclude service_property
 * @ruleinclude codel_property
 * @ruleinclude opt_async
 */

service:
  service_kind identifier '(' service_parameters ')' opt_properties semicolon
  {
    task_p = 0;
    param_setlocals(NULL);
    if (!$2 || !$4) {
      if ($2) parserror(@1, "dropped '%s' %s", $2, service_strkind($1));
      if ($4) hash_destroy($4, 1);
      if ($6) hash_destroy($6, 1);
      break;
    }
    if (!service_create(@1, $1, $2, $4, $6)) {
      parserror(@1, "dropped '%s' %s", $2, service_strkind($1));
      hash_destroy($4, 1);
      if ($6) hash_destroy($6, 1);
    }
  }
;

service_kind:
  FUNCTION	{ $$ = S_FUNCTION; }
  | ACTIVITY	{ $$ = S_ACTIVITY; }
;

service_property:
  TASK_P identifier semicolon
  {
    if (!$2) { parserror(@1, "dropped '%s' property", $1); $$ = NULL; break; }
    $$ = prop_newtask(@1, $2);
  }
  | INTERRUPTS identifier_list semicolon
  {
    if (!$2) { parserror(@1, "dropped '%s' property", $1); $$ = NULL; break; }
    $$ = prop_newhash(@1, PROP_INTERRUPTS, $2);
  }
  | BEFORE identifier_list semicolon
  {
    if (!$2) { parserror(@1, "dropped '%s' property", $1); $$ = NULL; break; }
    $$ = prop_newhash(@1, PROP_BEFORE, $2);
  }
  | AFTER identifier_list semicolon
  {
    if (!$2) { parserror(@1, "dropped '%s' property", $1); $$ = NULL; break; }
    $$ = prop_newhash(@1, PROP_AFTER, $2);
  }
  | VALIDATE codel semicolon
  {
    $$ = $2 ? prop_newcodel(@1, PROP_VALIDATE, $2) : NULL;
  }
;

/*/ @node Service parameters
 * @section Service parameters
 * @cindex service, parameters
 * @cindex parameters, service
 *
 * @ruleinclude attribute_parameter
 * @ruleinclude service_parameter
 * @sp 1
 * @ruleinclude parameter_dir
 * @ruleinclude parameter_variable
 * @ruleinclude opt_initializer
 * @ruleinclude initializers
 * @ruleinclude initializer
 * @ruleinclude initializer_value
 */

attribute_parameters:
  /* empty */
  {
    task_p = 1; $$ = hash_create("parameter list", 0); param_setlocals($$);
  }
  | attribute_parameter
  {
    task_p = 1; $$ = param_locals(); if (!$1) break;
    if (!$$) {
      $$ = hash_create("parameter list", 3); param_setlocals($$);
      if (!$$) break;
    }
    if (hash_insert($$, param_name($1), $1, (hrelease_f)param_destroy)) {
      param_destroy($1); break;
    }
  }
  | attribute_parameters ',' attribute_parameter
  {
    $$ = param_locals(); if (!$3) break;
    if (!$$) {
      $$ = hash_create("parameter list", 3); param_setlocals($$);
      if (!$$) break;
    }
    switch(hash_insert($$, param_name($3), $3, (hrelease_f)param_destroy)) {
      case 0: break;
      case EEXIST:
	parserror(@3, "duplicate parameter '%s'", param_name($3));
	/*FALLTHROUGH*/
      default: param_destroy($3); break;
    }
  }
;

attribute_parameter:
  parameter_dir parameter_variable opt_initializer
  {
    if (!$2) { $$ = NULL; break; }
    $$ = param_newids(@2, $1, NULL, $2, $3);
  }
  | error { $$ = NULL; }
;

service_parameters:
  /* empty */
  {
    task_p = 1; $$ = hash_create("parameter list", 0); param_setlocals($$);
  }
  | service_parameter
  {
    task_p = 1; $$ = param_locals(); if (!$1) break;
    if (!$$) {
      $$ = hash_create("parameter list", 3); param_setlocals($$);
      if (!$$) break;
    }
    if (hash_insert($$, param_name($1), $1, (hrelease_f)param_destroy)) {
      param_destroy($1); break;
    }
  }
  | service_parameters ',' service_parameter
  {
    $$ = param_locals(); if (!$3) break;
    if (!$$) {
      $$ = hash_create("parameter list", 3); param_setlocals($$);
      if (!$$) break;
    }
    switch(hash_insert($$, param_name($3), $3, (hrelease_f)param_destroy)) {
      case 0: break;
      case EEXIST:
	parserror(@3, "duplicate parameter '%s'", param_name($3));
	/*FALLTHROUGH*/
      default: param_destroy($3); break;
    }
  }
;

service_parameter:
  parameter_dir type_spec declarator opt_initializer
  {
    clist_s m;
    cval n;
    if (!$2 || !$3) { $$ = NULL; break; }
    if (!dcl_settype($3, $2)) { $$ = NULL; break; }
    n.k = CST_STRING;
    n.s = dcl_name($3);
    m = clist_append(NULL, n, 0);
    $$ = param_newlocal(@3, $1, NULL, m, dcl_type($3), $4);
  }
  | error { $$ = NULL; }
;
