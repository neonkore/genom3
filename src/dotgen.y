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
%token <i>	INTEGER_LIT
%token <d>	FLOAT_LIT
%token <c>	CHAR_LIT
%token <s>	STRING_LIT IDENTIFIER

%type <i>	spec statement
%type <i>	module
%type <type>	type_dcl constr_type enum_type enumerator
%type <hash>	enumerator_list
%type <scope>	scope_push_module
%type <i>	cpphash

%start spec

%%

spec: statement | spec statement;

statement:
    module ';'
  | type_dcl ';'
  { }
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
       /* there was an error during the creation of the scope. We just
	* delete it here with no warning, since the error has already
	* been reported. */
       scope_destroy(s);
     }
   }
   | MODULE scope_push_module '{' '}'
   {
     scope_s s = scope_pop();

     assert(s == $2);
     if (scope_name(s)[0] == '&') {
       /* there was an error during the creation of the scope. We just
	* delete it here with no warning, since the error has already
	* been reported. */
       scope_destroy(s);
     } else
       parsewarning(@1, "empty module '%s'", scope_name(s));
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
    $$ = type_newenum(@2, $2, $4);
    if (!$$) {
      hiter i;
      for(hash_first($4, &i); i.current; hash_next(&i))
	type_destroy(i.value);
      hash_destroy($4);
    }
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
    /* idlerParseError(spec, &$<noval>1.p, IDLER_E_WARNING, */
    /* 		    "ignoring pragma: %s", $3.v); */
    /* idlerObjectUnref(spec, $3.v); */
    yyerrok;
  }
  | '#' error '\n'
  {
    /* idlerParseError(spec, &$<noval>1.p, IDLER_E_WARNING, */
    /* 		    "unknown # directive"); */
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
