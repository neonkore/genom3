/*
 * Copyright (c) 2009-2013 LAAS/CNRS
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

/* --- identifiers --------------------------------------------------------- */

/*/ @node Reserved keywords
 * @section Identifiers and reserved keywords
 * @cindex Dotgen, identifier
 *
 * An @I{identifier} is a sequence of @acronym{ASCII} alphabetic, digit, and
 * underscore (@code{_}) characters.  The first character must be an
 * @acronym{ASCII} alphabetic character.
 *
 * @ruleinclude identifier
 * @ruleinclude identifier_list
 *
 * Words that are reserved keywords in the dotgen language are valid
 * identifiers where their use is not ambiguous.
 */
identifier:
  IDENTIFIER | S | MS | US | K | M | REAL_TIME
  | INTERFACE | COMPONENT | IDS | ATTRIBUTE | FUNCTION | ACTIVITY | VERSION
  | LANG | EMAIL | REQUIRE | CODELSREQUIRE | CLOCKRATE | TASK | TASK_P | PERIOD
  | DELAY | PRIORITY | SCHEDULING | STACK | CODEL | VALIDATE | YIELD | THROWS
  | DOC | INTERRUPTS | BEFORE | AFTER | HANDLE | PORT | IN | OUT | INOUT
  | LOCAL | ASYNC | REMOTE | EXTENDS | PROVIDES | USES | MULTIPLE | NATIVE
  | EXCEPTION
;

identifier_list:
  identifier
  {
    $$ = hash_create("identifier list", 3); if (!$$ || !$1) break;
    switch(hash_insert($$, $1, $1, NULL)) {
      case EEXIST: parserror(@1, "duplicate identifier '%s'", $1); break;
    }
  }
  | identifier_list ',' identifier
  {
    $$ = $1; if (!$3) break;
    switch(hash_insert($$, $3, $3, NULL)) {
      case EEXIST: parserror(@3, "duplicate identifier '%s'", $3); break;
    }
  }
;

/* --- constant expressions ------------------------------------------------ */

/* constant expressions refer to numeric constants whithin the
 * specification. A cval value is returned by all these rules. Note that
 * the original form of the constant definition is lost: only the numeric
 * result is kept (e.g. 2*2 is transformed into 4, and `2*2' is lost). */

const_expr: or_expr;

positive_int_const: const_expr
  {
    $$ = $1;
    if (const_convert(&$$, CST_UINT)) {
      parserror(@1, "positive integer constant required at column %d", @1.col);
      $$.k = CST_UINT; $$.u = 1;
    }
  }
;

or_expr: xor_expr | or_expr '|' xor_expr
  {
    $$ = $1;
    if (const_binaryop(&$$, '|', $3)) {
      parserror(
	@2, "invalid use of binary '%c' operator on constant at column %d",
	'|', @2.col);
    }
  }
;

xor_expr: and_expr | xor_expr '^' and_expr
  {
    $$ = $1;
    if (const_binaryop(&$$, '^', $3)) {
      parserror(
	@2, "invalid use of binary '%c' operator on constant at column %d",
	'^', @2.col);
    }
  }
;

and_expr: shift_expr | and_expr '&' shift_expr
  {
    $$ = $1;
    if (const_binaryop(&$$, '&', $3)) {
      parserror(
	@2, "invalid use of binary '%c' operator on constant at column %d",
	'&', @2.col);
    }
  }
;

shift_expr:
  add_expr
  | shift_expr SR add_expr
  {
    $$ = $1;
    if (const_binaryop(&$$, '>', $3)) {
      parserror(
	@2, "invalid use of binary '%c' operator on constant at column %d",
	'>', @2.col);
    }
  }
  | shift_expr SL add_expr
  {
    $$ = $1;
    if (const_binaryop(&$$, '<', $3)) {
      parserror(
	@2, "invalid use of binary '%c' operator on constant at column %d",
	'<', @2.col);
    }
  }
;

add_expr:
  mult_expr
  | add_expr '+' mult_expr
  {
    $$ = $1;
    if (const_binaryop(&$$, '+', $3)) {
      parserror(
	@2, "invalid use of binary '%c' operator on constant at column %d",
	'+', @2.col);
    }
  }
  | add_expr '-' mult_expr
  {
    $$ = $1;
    if (const_binaryop(&$$, '-', $3)) {
      parserror(
	@2, "invalid use of binary '%c' operator on constant at column %d",
	'-', @2.col);
    }
  }
;

mult_expr:
  unary_expr
  | mult_expr '*' unary_expr
  {
    $$ = $1;
    if (const_binaryop(&$$, '*', $3)) {
      parserror(
	@2, "invalid use of binary '%c' operator on constant at column %d",
	'*', @2.col);
    }
  }
  | mult_expr '/' unary_expr
  {
    $$ = $1;
    if (const_binaryop(&$$, '/', $3)) {
      parserror(
	@2, "invalid use of binary '%c' operator on constant at column %d",
	'/', @2.col);
    }
  }
  | mult_expr '%' unary_expr
  {
    $$ = $1;
    if (const_binaryop(&$$, '%', $3)) {
      parserror(
	@2, "invalid use of binary '%c' operator on constant at column %d",
	'%', @2.col);
    }
  }
;

unary_expr:
  primary_expr
  | '-' primary_expr
  {
    $$ = $2;
    if (const_unaryop(&$$, '-')) {
      parserror(
	@1, "invalid use of unary '%c' operator on constant at column %d",
	'-', @1.col);
    }
  }
  | '+' primary_expr
  {
    $$ = $2;
    if (const_unaryop(&$$, '+')) {
      parserror(
	@1, "invalid use of unary '%c' operator on constant at column %d",
	'+', @1.col);
    }
  }
  | '~' primary_expr
  {
    $$ = $2;
    if (const_unaryop(&$$, '~')) {
      parserror(
	@1, "invalid use of unary '%c' operator on constant at column %d",
	'~', @1.col);
    }
  }
;

primary_expr:
  literal
  | '(' const_expr ')'
  {
    $$ = $2;
  }
  | named_type
  {
    if (!$1) { $$.k = CST_UINT; $$.u = 0; break; }

    switch(type_kind($1)) {
      case IDL_CONST:		$$ = type_constvalue($1);	break;
      case IDL_ENUMERATOR:	$$.k = CST_ENUM; $$.e = $1;	break;

      default:
	parserror(@1, "%s %s is not valid in an expression",
		  type_strkind(type_kind($1)), type_name($1));
	parsenoerror(type_loc($1), "  %s %s declared here",
		     type_strkind(type_kind($1)), type_name($1));
	$$.k = CST_UINT;
	$$.u = 0;
	break;
    }
  }
;

literal:
  TRUE
  {
    $$.k = CST_BOOL;
    $$.b = 1;
  }
  | FALSE
  {
    $$.k = CST_BOOL;
    $$.b = 0;
  }
  | integer_literal
  {
    $$.k = ($1 < 0) ? CST_INT : CST_UINT;
    $$.i = $1;
  }
  | FLOAT_LIT
  {
    $$.k = CST_FLOAT;
    $$.f = $1;
  }
  | FIXED_LIT
  {
    $$.k = CST_UINT;
    $$.i = 0;
    parserror(@1, "fixed point types not implemented");
  }
  | CHAR_LIT
  {
    $$.k = CST_CHAR;
    $$.c = $1;
  }
  | string_literals
  {
    $$.k = CST_STRING;
    $$.s = $1;
  }
;

string_literals: string_literal | string_literals string_literal
  {
    if ($1)
      $$ = strings($1, $2, NULL);
    else
      $$ = $2;
  }
;

string_list:
  string_literals
  {
    cval c;
    if (!$1) { $$ = NULL; break; }
    c.k = CST_STRING;
    c.s = string($1);
    $$ = clist_append(NULL, c, 1/*unique*/);
  }
  | string_list ',' string_literals
  {
    cval c;
    if (!$3) { $$ = $1; break; }
    c.k = CST_STRING;
    c.s = string($3);
    $$ = clist_append($1, c, 1/*unique*/);
    if (!$$) {
      $$ = $1;
      parsewarning(@3, "ignoring duplicate '%s' string in list", $3);
    }
  }
;

time_unit:
  {
    $$.k = CST_FLOAT; $$.f = 1.;
  }
  | S
  {
    $$.k = CST_FLOAT; $$.f = 1.;
  }
  | MS
  {
    $$.k = CST_FLOAT; $$.f = 1e-3;
  }
  | US
  {
    $$.k = CST_FLOAT; $$.f = 1e-6;
  }
;

size_unit:
  {
    $$.k = CST_UINT; $$.u = 1;
  }
  | K
  {
    $$.k = CST_UINT; $$.u = 1024;
  }
  | M
  {
    $$.k = CST_UINT; $$.u = 1024*1024;
  }
;
