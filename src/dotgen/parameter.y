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

opt_parameter_src:
  /* empty */	{ $$ = P_NOSRC; }
  | IDS		{ $$ = P_IDS; }
  | LOCAL	{ $$ = P_LOCAL; }
  | PORT	{ $$ = P_PORT; }
  | REMOTE	{ $$ = P_REMOTE; }
;

parameter_dir:
    IN		{ $$ = P_IN; }
  | OUT		{ $$ = P_OUT; }
  | INOUT	{ $$ = P_INOUT; }
  /* | error */
  /* { */
  /*   $$ = P_NODIR; */
  /*   parserror(@1, "expected 'in', 'out' or 'inout'"); */
  /* } */
;

parameter_variable:
  identifier
  {
    cval k;
    k.k = CST_STRING; k.s = $1;
    $$ = clist_append(NULL, k, 0);
  }
  | parameter_variable '.' identifier
  {
    cval k;
    k.k = CST_STRING; k.s = $3;
    $$ = clist_append($1, k, 0);
  }
  | parameter_variable '[' positive_int_const ']'
  {
    assert($3.k == CST_UINT);
    $$ = clist_append($1, $3, 0);
  }
;

opt_initializer:
  /* empty */		{ $$ = NULL; }
  | '=' initializer	{ $$ = $2; }
;

initializers:
  /* empty */
  {
    $$ = NULL;
  }
  | initializer
  | initializers ',' initializer
  {
    $$ = $3 ? initer_append($1, $3) : $1;
  }
;

initializer:
  initializer_value
  | ':' string_literals
  {
    cval v; v.k = CST_VOID; v.u = -1U;
    if ($2) {
      $$ = initer_create(@1, -1U, NULL, NULL, v);
      (void)initer_setdoc($$, $2);
    } else
      $$ = NULL;
  }
  | initializer_value ':' string_literals
  {
    $$ = $1; if (!$1 || !$3) break;
    (void)initer_setdoc($1, $3);
  }
  | error { $$ = NULL; }
;

initializer_value:
  const_expr
  {
    $$ = initer_create(@1, -1U, NULL, NULL, $1);
  }
  | '{' initializers '}'
  {
    cval v; v.k = CST_VOID; v.u = -1U;
    $$ = $2 ? initer_create(@1, -1U, NULL, $2, v) : NULL;
  }
  | '[' positive_int_const ']' '=' const_expr
  {
    assert($2.k == CST_UINT);
    $$ = initer_create(@1, $2.u, NULL, NULL, $5);
  }
  | '[' positive_int_const ']' '=' '{' initializers '}'
  {
    cval v; v.k = CST_VOID; v.u = -1U;
    assert($2.k == CST_UINT);
    $$ = initer_create(@1, $2.u, NULL, $6, v);
  }
  | '[' positive_int_const ']' '='
  {
    cval v; v.k = CST_VOID; v.u = -1U;
    assert($2.k == CST_UINT);
    $$ = initer_create(@1, $2.u, NULL, NULL, v);
  }
  | '.' identifier '=' const_expr
  {
    $$ = initer_create(@1, -1U, $2, NULL, $4);
  }
  | '.' identifier '=' '{' initializers '}'
  {
    cval v; v.k = CST_VOID; v.u = -1U;
    $$ = initer_create(@1, -1U, $2, $5, v);
  }
  | '.' identifier '='
  {
    cval v; v.k = CST_VOID; v.u = -1U;
    $$ = initer_create(@1, -1U, $2, NULL, v);
  }
;
