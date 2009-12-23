/*
 * Copyright (c) 2009 LAAS/CNRS
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
 *                                           Cédric Pasteur on Mon Apr 20 2009
 */
%{
#include "acgenom.h"

#include <assert.h>

#include "genom.h"

#define YYLTYPE	tloc

#define YYLLOC_DEFAULT(c, rhs, n)		\
  do {						\
    (c).file = YYRHSLOC(rhs, (n)?1:0).file;	\
    (c).line = YYRHSLOC(rhs, (n)?1:0).line;	\
    (c).col = YYRHSLOC(rhs, (n)?1:0).col;	\
  } while (0)

%}

%name-prefix "dotgen"
%define api.pure
%locations

%code provides {
  extern YYLTYPE curloc;

  extern int dotgenlex(YYSTYPE *lvalp, YYLTYPE *llocp);
  extern void dotgenerror(const char *msg);
}

%union {
  int		i;
  double	d;
  char		c;
  char *	s;
  cval		v;
  hash_s	hash;
  scope_s	scope;
  idltype_s	type;
}

%token <i>	PRAGMA
%token <i>	COLONCOLON SL SR
%token <i>	MODULE
%token <i>	UNSIGNED SHORT LONG FIXED FLOAT DOUBLE CHAR WCHAR STRING
%token <i>	WSTRING BOOLEAN OCTET OBJECT ANY VOID PROPERTY
%token <i>	CONST NATIVE ENUM UNION SWITCH CASE DEFAULT STRUCT SEQUENCE
%token <i>	TYPEDEF
%token <i>	COMPONENT TASK SERVICE CODEL INPORT OUTPORT IN OUT INOUT IDS
%token <i>	INPUT OUTPUT EVENT IMPORT FROM
%token <i>	FALSE TRUE
%token <i>	INTEGER_LIT FIXED_LIT
%token <d>	FLOAT_LIT
%token <c>	CHAR_LIT
%token <s>	STRING_LIT IDENTIFIER

%type <i>	spec statement
%type <i>	module
%type <type>	const_dcl const_type
%type <type>	type_dcl constr_type enum_type enumerator
%type <hash>	enumerator_list
%type <type>	integer_type
%type <type>	unsigned_int unsigned_short_int unsigned_long_int
%type <type>	signed_int signed_short_int signed_long_int
%type <type>	floating_pt_type float_type double_type;
%type <type>	boolean_type char_type octet_type any_type
%type <type>	string_type
%type <scope>	scope_push_module
%type <v>	positive_int_const const_expr unary_expr primary_expr
%type <v>	or_expr xor_expr and_expr shift_expr add_expr mult_expr
%type <v>	literal
%type <s>	scoped_name
%type <i>	cpphash

%start spec

%%

spec: statement | spec statement;

statement:
  module ';'
  | const_dcl ';'	{ }
  | type_dcl ';'	{ }
  | cpphash
  | error
  {
    parserror(@1, "raah");
  }
;

/* --- modules ------------------------------------------------------------- */

/** modules are IDL namespaces */

module:
   MODULE scope_push_module '{' spec '}'
   {
     scope_s s = scope_pop();

     assert(s == $2);
     if (scope_name(s)[0] == '&') {
       /* there was an error during the creation of the scope. */
       parserror(@1, "dropped declaration for '%s'", &scope_name(s)[1]);
       scope_destroy(s);
     }
   }
   | MODULE scope_push_module '{' '}'
   {
     scope_s s = scope_pop();

     assert(s == $2);
     if (scope_name(s)[0] == '&') {
       /* there was an error during the creation of the scope. */
       parserror(@1, "dropped declaration for '%s'", &scope_name(s)[1]);
       scope_destroy(s);
     } else
       parsewarning(@1, "empty module '%s'", scope_name(s));
   }
;


/* --- constant definition ------------------------------------------------- */

/* these rules handle the `const' keyword. */

const_dcl:
  CONST const_type IDENTIFIER '=' const_expr
  {
    assert($3);
    $$ = $2 ? type_newconst(@1, $3, $2, $5) : NULL;
    if (!$$) {
      if ($2 && !type_name($2)) type_destroy($2);
      parserror(@1, "dropped declaration for '%s'", $3);
    }
  }
;

const_type:
  integer_type | char_type | boolean_type | floating_pt_type | octet_type
  | string_type
  | scoped_name
  {
    $$ = type_find($1);
    if (!$$) {
      parserror(@1, "unknown type %s", $1);
      break;
    }

    switch(type_kind(type_final($$))) {
      case IDL_ANY: case IDL_ENUMERATOR: case IDL_ARRAY:
      case IDL_SEQUENCE: case IDL_STRUCT: case IDL_UNION:
	parserror(@1, "%s %s is not a valid constant type",
		  type_strkind(type_kind($$)), $1);
	parsenoerror(type_loc($$), "  %s %s declared here",
		     type_strkind(type_kind($$)), $1);
	$$ = NULL;
	break;

      case IDL_CONST: case IDL_TYPEDEF:
	/* not a valid return from type_final() */
	assert(0); break;

      default: break;
    }
  }
;


/* --- IDL type definitions ------------------------------------------------ */

/* These rules cover the `typedef' token, but also `struct', `enum' and
 * `union'. */

type_dcl:
   /* TYPEDEF alias_type_list */
   /* { */
   /* } */
/*   | */ constr_type
;

constr_type: /* struct_type | union_type | */ enum_type;

enum_type: ENUM IDENTIFIER '{' enumerator_list '}'
  {
    $$ = $4 ? type_newenum(@2, $2, $4) : NULL;
    if (!$$) {
      if ($4) {
	hiter i;
	for(hash_first($4, &i); i.current; hash_next(&i))
	  type_destroy(i.value);
	hash_destroy($4);
      }
      parserror(@1, "dropped declaration for '%s'", $2);
    }
  }
;


/* --- type specification -------------------------------------------------- */

/* all the ways to reference a type definition */

integer_type: signed_int | unsigned_int;
floating_pt_type: float_type | double_type;

string_type:
  STRING '<' positive_int_const '>'
  {
    assert($3.k == CST_UINT);
    $$ = type_newstring(@1, NULL, $3.u);
  }
  | STRING
  {
    $$ = type_newstring(@1, NULL, -1UL);
  }
;

enumerator_list:
  enumerator
  {
    $$ = hash_create("enumerator list", 3);
    if (!$$) break;

    if (!$1) break;
    if (hash_insert($$, type_name($1), $1, NULL))
      type_destroy($1);
  }
  | enumerator_list ',' enumerator
  {
    $$ = $1;
    if (!$3) break;
    if (hash_insert($$, type_name($3), $3, NULL))
      type_destroy($3);
  }
;

enumerator: IDENTIFIER
  {
    $$ = type_newenumerator(@1, $1);
  }
;

signed_int: signed_long_int | signed_short_int;
unsigned_int: unsigned_long_int | unsigned_short_int;

unsigned_short_int: UNSIGNED SHORT { $$ = type_newbasic(@1, NULL, IDL_USHORT); };
unsigned_long_int: UNSIGNED LONG   { $$ = type_newbasic(@1, NULL, IDL_ULONG); };
signed_short_int: SHORT		   { $$ = type_newbasic(@1, NULL, IDL_SHORT); };
signed_long_int: LONG		   { $$ = type_newbasic(@1, NULL, IDL_LONG); };
float_type: FLOAT		   { $$ = type_newbasic(@1, NULL, IDL_FLOAT); };
double_type: DOUBLE		   { $$ = type_newbasic(@1, NULL, IDL_DOUBLE); };
char_type: CHAR			   { $$ = type_newbasic(@1, NULL, IDL_CHAR); };
boolean_type: BOOLEAN		   { $$ = type_newbasic(@1, NULL, IDL_BOOL); };
octet_type: OCTET		   { $$ = type_newbasic(@1, NULL, IDL_OCTET); };
any_type: ANY			   { $$ = type_newbasic(@1, NULL, IDL_ANY); };


/* --- scopes -------------------------------------------------------------- */

/* scopes are created as a side effect of certain declarations (modules,
 * interfaces, ...) or types (structures, unions, ...) */

scope_push_module: IDENTIFIER
  {
    $$ = scope_push(@1, $1);
    if (!$$) {
      /* on error, still create a scope to continue the parsing
       * but with a special name -- it will be deleted afterwards */
      $$ = scope_push(@1, strings("&", $1, NULL));
      if (!$$) {
	/* still failed, just resign */
	YYABORT;
      }
    }
  }
;

scoped_name:
  IDENTIFIER
  | COLONCOLON IDENTIFIER
  {
    $$ = strings("::", $2, NULL);
  }
  | scoped_name COLONCOLON IDENTIFIER
  {
    $$ = strings($1, "::", $3, NULL);
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
    if (const_convert(&$$, CST_UINT))
      parserror(@1, "positive integer constant required at column %d", @1.col);
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
  | scoped_name
  {
    idltype_s t = type_find($1);
    if (!t) {
      parserror(@1, "unknown type %s", $1);
      $$.k = CST_UINT;
      $$.u = 0;
      break;
    }

    switch(type_kind(t)) {
      case IDL_CONST:		$$ = type_constvalue(t);	break;
      case IDL_ENUMERATOR:	$$.k = CST_ENUM; $$.e = t;	break;

      default:
	parserror(@1, "%s %s is not valid in an expression",
		  type_strkind(type_kind(t)), $1);
	parsenoerror(type_loc(t), "  %s %s declared here",
		     type_strkind(type_kind(t)), $1);
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
  | INTEGER_LIT
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
  | STRING_LIT
  {
    $$.k = CST_STRING;
    $$.s = $1;
  }
;


/* --- # directives from cpp ----------------------------------------------- */

cpphash:
  '#' INTEGER_LIT STRING_LIT '\n'
  {
    curloc.file = $3;
    curloc.line = $2;
    curloc.col = 1;
  }
  | '#' INTEGER_LIT STRING_LIT INTEGER_LIT '\n'
  {
    curloc.file = $3;
    curloc.line = $2;
    curloc.col = 1;
  }
  | '#' PRAGMA IDENTIFIER error '\n'
  {
    parsewarning(@1, "ignoring pragma: %s", $3);
    yyerrok;
  }
  | '#' error '\n'
  {
    parsewarning(@1, "unknown # directive");
    yyerrok;
  }
;

%%

 /* --- dotgenerror -------------------------------------------------------- */

void
dotgenerror(const char *msg)
{
  ;
}
