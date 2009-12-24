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
  clist_s	vlist;
  hash_s	hash;
  scope_s	scope;
  dcl_s		dcl;
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
%type <type>	type_dcl constr_type
%type <dcl>	declarator simple_declarator array_declarator
%type <type>	type_spec simple_type_spec base_type_spec template_type_spec
%type <type>	constr_type_spec switch_type_spec
%type <type>	integer_type unsigned_int unsigned_short_int unsigned_long_int
%type <type>	signed_int signed_short_int signed_long_int
%type <type>	floating_pt_type float_type double_type
%type <type>	boolean_type char_type octet_type any_type
%type <type>	sequence_type string_type fixed_type named_type
%type <type>	union_type enum_type enumerator case switch_body
%type <hash>	enumerator_list
%type <v>	case_label
%type <vlist>	case_label_list
%type <scope>	scope_push
%type <v>	fixed_array_size
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
  MODULE scope_push '{' spec '}'
  {
    scope_s s = scope_pop();

    assert(s == $2);
    if (scope_name(s)[0] == '&') {
      /* there was an error during the creation of the scope. */
      parserror(@1, "dropped declaration for '%s'", &scope_name(s)[1]);
      scope_destroy(s);
    }
  }
  | MODULE scope_push '{' '}'
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
  | named_type
  {
    $$ = $1; if (!$$) break;

    switch(type_kind(type_final($$))) {
      case IDL_BOOL: case IDL_USHORT: case IDL_SHORT: case IDL_ULONG:
      case IDL_LONG: case IDL_FLOAT: case IDL_DOUBLE: case IDL_CHAR:
      case IDL_OCTET: case IDL_STRING: case IDL_ENUM:
	break;

      case IDL_ANY: case IDL_ENUMERATOR: case IDL_ARRAY:
      case IDL_SEQUENCE: case IDL_STRUCT: case IDL_UNION:
	parserror(@1, "%s %s is not a valid constant type",
		  type_strkind(type_kind($$)), $1);
	parsenoerror(type_loc($$), "  %s %s declared here",
		     type_strkind(type_kind($$)), $1);
	$$ = NULL;
	break;

      case IDL_CASE: case IDL_MEMBER: case IDL_CONST: case IDL_TYPEDEF:
	/* not a valid return from type_final() */
	assert(0); break;
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

constr_type: /* struct_type | */ union_type | enum_type;

union_type:
  UNION scope_push SWITCH '(' switch_type_spec ')' '{' switch_body '}'
  {
    scope_s s = scope_detach(scope_pop());
    assert(s == $2);

    if (scope_name(s)[0] == '&') {
      /* there was an error during the creation of the scope. */
      parserror(@1, "dropped declaration for '%s'", &scope_name(s)[1]);
      if ($5 && !type_name($5)) type_destroy($5);
      scope_destroy(s);
      break;
    }
    if (!$8 || !$5) {
      parserror(@1, "dropped declaration for '%s'", scope_name(s));
      if ($5 && !type_name($5)) type_destroy($5);
      scope_destroy(s);
      break;
    }

    $$ = type_newunion(@1, scope_name(s), $5, s);
    if (!$$) {
      parserror(@1, "dropped declaration for '%s'", scope_name(s));
      if ($5 && !type_name($5)) type_destroy($5);
      scope_destroy(s);
    }
  }
  | UNION scope_push error '}'
  {
    scope_s s = scope_detach(scope_pop());
    assert(s == $2);

    if (scope_name(s)[0] == '&') {
      /* there was an error during the creation of the scope. */
      parserror(@1, "dropped declaration for '%s'", &scope_name(s)[1]);
    } else
      parserror(@1, "dropped declaration for '%s'", scope_name(s));

    scope_destroy(s);
  }
;

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


/* --- type declarators ---------------------------------------------------- */

declarator: simple_declarator | array_declarator;

simple_declarator: IDENTIFIER
  {
    $$ = dcl_create(@1, $1);
  }
;

array_declarator:
  simple_declarator fixed_array_size
  {
    assert($2.k == CST_UINT);
    $$ = $1 ? dcl_adddim($1, $2.u) : NULL;
  }
  | array_declarator fixed_array_size
  {
    assert($2.k == CST_UINT);
    $$ = $1 ? dcl_adddim($1, $2.u) : NULL;
  }
;

fixed_array_size: '[' positive_int_const ']'
  {
    $$ = $2;
  }
;


/* --- type specification -------------------------------------------------- */

type_spec: simple_type_spec | constr_type_spec;

simple_type_spec: base_type_spec | template_type_spec | named_type;
constr_type_spec: constr_type;

named_type: scoped_name
  {
    $$ = type_find($1);
    if (!$$) parserror(@1, "unknown type %s", $1);
  }
;

base_type_spec:
  boolean_type | integer_type | floating_pt_type | char_type | octet_type |
  any_type;
template_type_spec: sequence_type | string_type | fixed_type;

integer_type: signed_int | unsigned_int;
floating_pt_type: float_type | double_type;

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

sequence_type:
  SEQUENCE '<' simple_type_spec ',' positive_int_const '>'
  {
    assert($5.k == CST_UINT);
    $$ = $3 ? type_newsequence(@1, NULL, $3, $5.u) : NULL;
  }
  | SEQUENCE '<' simple_type_spec '>'
  {
    $$ = $3 ? type_newsequence(@1, NULL, $3, -1UL) : NULL;
  }
;

fixed_type:
  FIXED '<' positive_int_const ',' positive_int_const '>'
  {
    $$ = NULL;
    parserror(@1, "fixed point types not implemented");
  }
  | FIXED
  {
    $$ = NULL;
    parserror(@1, "fixed point types not implemented");
  }
;

switch_type_spec:
  integer_type | char_type | boolean_type | enum_type
  | named_type
  {
    $$ = $1; if (!$$) break;

    switch(type_kind(type_final($$))) {
      case IDL_BOOL: case IDL_USHORT: case IDL_SHORT: case IDL_ULONG:
      case IDL_LONG: case IDL_CHAR: case IDL_ENUM:
	break;

      case IDL_FLOAT: case IDL_DOUBLE: case IDL_OCTET: case IDL_STRING:
      case IDL_ANY: case IDL_ENUMERATOR: case IDL_ARRAY: case IDL_SEQUENCE:
      case IDL_STRUCT: case IDL_UNION:
	parserror(@1, "%s %s is not a valid type for union switch",
		  type_strkind(type_kind($$)), $1);
	parsenoerror(type_loc($$), "  %s %s declared here",
		     type_strkind(type_kind($$)), $1);
	$$ = NULL;
	break;

      case IDL_CASE: case IDL_MEMBER: case IDL_CONST: case IDL_TYPEDEF:
	/* not a valid return from type_final() */
	assert(0); break;
    }
  }
;

switch_body: case | switch_body case
  {
    $$ = $1 ? $1 : $2;
  }
;

case: case_label_list type_spec declarator ';'
  {
    dcliter i;

    if (!$1 || !$2 || !$3) {
      $$ = NULL;
      if ($2 && !type_name($2)) type_destroy($2);
      if ($3) parserror(@3, "dropped declaration for '%s'", dcl_name($3));
      dcl_destroy($3);
      break;
    }

    /* for arrays, create intermediate anon array types */
    for(dcl_inner($3, &i); i.value != -1UL; dcl_next(&i)) {
      $2 = type_newarray(@3, NULL, $2, i.value);
      if (!$2) break;
    }
    if (!$2) {
      parserror(@3, "dropped declaration for '%s'", dcl_name($3));
      dcl_destroy($3);
      break;
    }

    $$ = type_newcase(@3, dcl_name($3), $2, $1);
    if (!$$) parserror(@3, "dropped declaration for '%s'", dcl_name($3));
    dcl_destroy($3);
  }
;

case_label_list:
  case_label
  {
    $$ = clist_append(NULL, $1);
  }
  | case_label_list case_label
  {
    $$ = clist_append($1, $2);
    if (!$$) {
      parsewarning(@2, "ignoring duplicate %s label",
		   $2.k == CST_VOID?"default":"case");
      $$ = $1;
    }
  }
;

case_label:
  CASE const_expr ':'
  {
    $$ = $2;
  }
  | DEFAULT ':'
  {
    $$.k = CST_VOID;
    $$.u = -1UL;
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


/* --- scopes -------------------------------------------------------------- */

/* scopes are created as a side effect of certain declarations (modules,
 * interfaces, ...) or types (structures, unions, ...) */

scope_push: IDENTIFIER
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
  parserror(curloc, "%s around column %d", msg, curloc.col);
}
