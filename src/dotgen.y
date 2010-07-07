/*
 * Copyright (c) 2009-2010 LAAS/CNRS
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
#include <stddef.h>
#include <errno.h>
#include <sys/stat.h>

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
  pdir		pdir;
  portkind	pkind;
  clist_s	vlist;
  prop_s	prop;
  codel_s	codel;
  port_s	port;
  param_s	param;
  initer_s	initer;
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
%token <i>	FALSE TRUE
%token <i>	integer_literal FIXED_LIT
%token <d>	FLOAT_LIT
%token <c>	CHAR_LIT
%token <s>	string_literal IDENTIFIER

%token <s>	S MS US K M REAL_TIME
%token <s>	COMPONENT TASK SERVICE CODEL INPORT OUTPORT IN OUT INOUT IDS
%token <s>	ATTRIBUTE INPUT OUTPUT EVENT DATA VERSION LANG EMAIL
%token <s>	REQUIRE BUILDREQUIRE PERIOD DELAY PRIORITY STACK VALIDATE YIELD
%token <s>	THROWS DOC INTERRUPTS BEFORE AFTER CLOCKRATE SCHEDULING

%type <i>	spec idlspec statement idldef idlstatement genomstatement

%type <i>	component port task service
%type <pkind>	port_dir
%type <type>	port_type
%type <prop>	attr
%type <hash>	attr_list param_list nodir_param_list
%type <initer>	initializer_value initializer initializer_list
%type <codel>	validate codel
%type <param>	param named_param nodir_param
%type <vlist>	param_member
%type <pdir>	param_dir

%type <i>	module
%type <type>	const_dcl const_type
%type <type>	type_dcl constructed_type forward_dcl
%type <dcl>	declarator simple_declarator array_declarator
%type <type>	type_spec simple_type_spec base_type_spec template_type_spec
%type <type>	constructed_type_spec switch_type_spec
%type <type>	integer_type unsigned_int unsigned_short_int unsigned_long_int
%type <type>	unsigned_longlong_int signed_int signed_short_int
%type <type>	signed_long_int signed_longlong_int
%type <type>	floating_pt_type float_type double_type
%type <type>	boolean_type char_type octet_type any_type
%type <type>	sequence_type string_type fixed_type named_type
%type <type>	struct_type union_type enum_type enumerator case switch_body
%type <type>	alias_list member member_list
%type <hash>	enumerator_list
%type <v>	case_label
%type <vlist>	case_label_list
%type <scope>	module_name scope_push_struct scope_push_union

%type <v>	fixed_array_size
%type <v>	positive_int_const const_expr unary_expr primary_expr
%type <v>	or_expr xor_expr and_expr shift_expr add_expr mult_expr
%type <v>	literal time_unit size_unit
%type <s>	scoped_name string_literals identifier
%type <hash>	event_list identifier_list
%type <vlist>	string_list

%type <i>	cpphash

%start spec

%%

spec: statement | spec statement;

statement: idlstatement | genomstatement | cpphash;

idlstatement:
  module ';'
  | const_dcl ';'
  {
    $$ = 0;
  }
  | type_dcl ';'
  {
    $$ = 0;
  }
  | error ';'
  {
    parserror(@1, "syntax error");
    YYABORT;
  }
  | error
  {
    parserror(@1, "maybe a missing ';'");
    YYABORT;
  }
;

genomstatement:
  component ';'
  | attribute ';'
  | port ';'
  | task ';'
  | service ';'
;


/* --- GenoM objects ------------------------------------------------------- */

component:
  COMPONENT identifier '{' attr_list '}'
  {
    if (!$2 || !$4) {
      if ($2) parserror(@1, "dropped '%s' component", $2);
      if ($4) hash_destroy($4);
      break;
    }
    if (!comp_create(@1, $2, $4)) YYABORT;
  }
  | COMPONENT identifier
  {
    hash_s p = hash_create("property list", 0);
    if (!$2 || !p) {
      if ($2) parserror(@1, "dropped '%s' component", $2);
      if (p) hash_destroy(p);
      break;
    }
    if (!comp_create(@1, $2, p)) YYABORT;
  }
;

port:
  port_dir '<' port_type '>' identifier
  {
    if (!$5) { parserror(@1, "dropped port"); break; }
    if (!$3) {
      if ($1 == PORT_INDATA || $1 == PORT_OUTDATA) {
	parserror(@1, "%s port '%s' port cannot be void",
		  port_strkind($1), $5);
	break;
      }
    }
    if (!comp_addport(@1, $1, $5, $3))
      parserror(@1, "dropped '%s' port", $5);
  }
;

port_type: type_spec | /* empty */ { $$ = NULL; };

port_dir:
    INPORT DATA		{ $$ = PORT_INDATA; }
  | INPORT EVENT	{ $$ = PORT_INEVENT; }
  | OUTPORT DATA	{ $$ = PORT_OUTDATA; }
  | OUTPORT EVENT	{ $$ = PORT_OUTEVENT; }
;

attribute:
  ATTRIBUTE nodir_param_list
  {
    if (!$2) { parserror(@1, "dropped '%s' property", $1); break; } else {
      if (comp_addattr(@2, $2))
	parserror(@1, "dropping '%s' property", $1);
    }
  }
;

task:
  TASK identifier '{' attr_list '}'
  {
    if (!$2 || !$4) {
      if ($2) parserror(@1, "dropped '%s' task", $2);
      if ($4) hash_destroy($4);
      break;
    }
    if (!comp_addtask(@1, $2, $4)) {
      parserror(@1, "dropped '%s' task", $2);
      hash_destroy($4);
    }
  }
  | TASK identifier
  {
    hash_s h = hash_create("property list", 0);
    if (!$2 || !h) {
      if ($2) parserror(@1, "dropped '%s' task", $2);
      if (h) hash_destroy(h);
      break;
    }
    if (!comp_addtask(@1, $2, h)) {
      parserror(@1, "dropped '%s' task", $2);
      hash_destroy(h);
    }
  }
;

service:
  SERVICE identifier '(' param_list ')' '{' attr_list '}'
  {
    if (!$2 || !$4 || !$7) {
      if ($2) parserror(@1, "dropped '%s' service", $2);
      if ($4) hash_destroy($4);
      if ($7) hash_destroy($7);
      break;
    }
    if (!comp_addservice(@1, $2, $4, $7)) {
      parserror(@1, "dropped '%s' service", $2);
      hash_destroy($4);
      hash_destroy($7);
    }
  }
  | SERVICE identifier '(' param_list ')'
  {
    hash_s h = hash_create("property list", 0);
    if (!$2 || !$4 || !h) {
      if ($2) parserror(@1, "dropped '%s' service", $2);
      if ($4) hash_destroy($4);
      if (h) hash_destroy(h);
      break;
    }
    if (!comp_addservice(@1, $2, $4, h)) {
      parserror(@1, "dropped '%s' service", $2);
      hash_destroy($4);
      hash_destroy(h);
    }
  }
;


/* --- GenoM object properties --------------------------------------------- */

attr_list:
  attr ';'
  {
    $$ = hash_create("property list", 5);
    if (!$$ || !$1) break;

    if (hash_insert($$, prop_name($1), $1, (hrelease_f)prop_destroy))
      prop_destroy($1);
  }
  | attr_list attr ';'
  {
    $$ = $1;
    if (!$$ || !$2) break;
    if (hash_insert($$, prop_name($2), $2, (hrelease_f)prop_destroy)) {
      if (errno == EEXIST) {
	prop_s p = hash_find($$, prop_name($2)); assert(p);
	parserror(@2, "duplicate %s declaration", prop_name($2));
	parsenoerror(prop_loc(p), " %s declared here", prop_name(p));
      }
      prop_destroy($2);
    }
  }
;

attr:
  DOC ':' string_literals
  {
    if (!$3) { parserror(@1, "dropped '%s' property", $1); $$ = NULL; break; }
    $$ = prop_newstring(@1, PROP_DOC, $3);
  }
  | IDS ':' named_type
  {
    if (!$3) { parserror(@1, "dropped '%s' property", $1); $$ = NULL; break; }
    $$ = prop_newids(@1, $3);
  }
  | VERSION ':' string_literals
  {
    if (!$3) { parserror(@1, "dropped '%s' property", $1); $$ = NULL; break; }
    $$ = prop_newstring(@1, PROP_VERSION, $3);
  }
  | LANG ':' string_literals
  {
    if (!$3) { parserror(@1, "dropped '%s' property", $1); $$ = NULL; break; }
    $$ = prop_newstring(@1, PROP_LANG, $3);
  }
  | EMAIL ':' string_literals
  {
    if (!$3) { parserror(@1, "dropped '%s' property", $1); $$ = NULL; break; }
    $$ = prop_newstring(@1, PROP_EMAIL, $3);
  }
  | REQUIRE ':' string_list
  {
    if (!$3) { parserror(@1, "dropped '%s' property", $1); $$ = NULL; break; }
    $$ = prop_newrequire(@1, PROP_REQUIRE, $3);
  }
  | BUILDREQUIRE ':' string_list
  {
    if (!$3) { parserror(@1, "dropped '%s' property", $1); $$ = NULL; break; }
    $$ = prop_newrequire(@1, PROP_BUILD_REQUIRE, $3);
  }
  | CLOCKRATE ':' const_expr time_unit
  {
    if (const_binaryop(&$3, '*', $4)) {
      parserror(@3, "invalid numeric constant");
      break;
    }
    $$ = prop_newvalue(@1, PROP_CLOCKRATE, $3);
  }
  | PERIOD ':' const_expr time_unit
  {
    if (const_binaryop(&$3, '*', $4)) {
      parserror(@3, "invalid numeric constant");
      break;
    }
    $$ = prop_newvalue(@1, PROP_PERIOD, $3);
  }
  | DELAY ':' const_expr time_unit
  {
    if (const_binaryop(&$3, '*', $4)) {
      parserror(@3, "invalid numeric constant");
      break;
    }
    $$ = prop_newvalue(@1, PROP_DELAY, $3);
  }
  | PRIORITY ':' positive_int_const
  {
    $$ = prop_newvalue(@1, PROP_PRIORITY, $3);
  }
  | SCHEDULING ':' REAL_TIME
  {
    $$ = prop_newstring(@1, PROP_SCHEDULING, $3);
  }
  | STACK ':' positive_int_const size_unit
  {
    if (const_binaryop(&$3, '*', $4)) {
      parserror(@3, "invalid numeric constant");
      break;
    }
    $$ = prop_newvalue(@1, PROP_STACK, $3);
  }
  | THROWS ':' event_list
  {
    $$ = $3 ? prop_newhash(@1, PROP_THROWS, $3) : NULL;
  }
  | TASK ':' identifier
  {
    if (!$3) { parserror(@1, "dropped '%s' property", $1); $$ = NULL; break; }
    $$ = prop_newtask(@1, $3);
  }
  | INTERRUPTS ':' identifier_list
  {
    if (!$3) { parserror(@1, "dropped '%s' property", $1); $$ = NULL; break; }
    $$ = prop_newhash(@1, PROP_INTERRUPTS, $3);
  }
  | BEFORE ':' identifier_list
  {
    if (!$3) { parserror(@1, "dropped '%s' property", $1); $$ = NULL; break; }
    $$ = prop_newhash(@1, PROP_BEFORE, $3);
  }
  | AFTER ':' identifier_list
  {
    if (!$3) { parserror(@1, "dropped '%s' property", $1); $$ = NULL; break; }
    $$ = prop_newhash(@1, PROP_AFTER, $3);
  }
  | VALIDATE ':' validate
  {
    $$ = $3 ? prop_newcodel(@1, PROP_VALIDATE, $3) : NULL;
  }
  | CODEL codel
  {
    $$ = $2 ? prop_newcodel(@1, PROP_CODEL, $2) : NULL;
  }
  | error
  {
    $$ = NULL;
  }
;


/* --- codels -------------------------------------------------------------- */

validate:
  identifier '(' param_list ')'
  {
    $$ = codel_create(@1, $1, NULL, NULL, $3);
  }

codel:
  event_list ':' identifier '(' param_list ')' YIELD event_list
  {
    if (!$1 || !$8) {
      parserror(@1, "dropped codel '%s'", $3); $$ = NULL; break;
    }
    $$ = codel_create(@3, $3, $1, $8, $5);
  }
  | event_list ':' identifier '(' param_list ')' error
  {
    $$ = NULL;
    parserror(@1, "missing 'yield' values for codel %s", $3);
    if ($1) hash_destroy($1);
    if ($5) hash_destroy($5);
  }
;

event_list: identifier_list
  {
    $$ = comp_addievs(@1, $1) ? NULL : $1;
  }
;

param_list:
  /* empty */
  {
    $$ = hash_create("parameter list", 0);
  }
  | param
  {
    $$ = hash_create("parameter list", 3); if (!$$ || !$1) break;
    switch(hash_insert($$, param_name($1), $1, (hrelease_f)param_destroy)) {
      case 0: break;
      case EEXIST:
	parserror(@1, "duplicate parameter '%s'", param_name($1));
	/*FALLTHROUGH*/
      default: param_destroy($1); break;
    }
  }
  | param_list ',' param
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

nodir_param_list:
  nodir_param
  {
    $$ = hash_create("parameter list", 3); if (!$$ || !$1) break;
    switch(hash_insert($$, param_name($1), $1, (hrelease_f)param_destroy)) {
      case 0: break;
      case EEXIST:
	parserror(@1, "duplicate parameter '%s'", param_name($1));
	/*FALLTHROUGH*/
      default: param_destroy($1); break;
    }
  }
  | nodir_param_list ',' nodir_param
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

param:
  param_dir named_param
  {
    $$ = $2; if (!$2) break;
    if (param_setdir($2, $1))
      parserror(@2, "dropping initializer for '%s'", param_name($2));
  }
  | param_dir named_param '=' initializer
  {
    $$ = $2; if (!$2 || !$4) break;
    if (param_setdir($2, $1) || param_setinitv(@4, $2, $4))
      parserror(@3, "dropping initializer for '%s'", param_name($2));
  }
;

nodir_param:
  named_param
  {
    $$ = $1; if (!$1) break;
    if (param_setdir($1, P_INOUT))
      parserror(@1, "dropping initializer for '%s'", param_name($1));
  }
  | named_param '=' initializer
  {
    $$ = $1; if (!$1 || !$3) break;
    if (param_setdir($1, P_INOUT) || param_setinitv(@3, $1, $3))
      parserror(@1, "dropping initializer for '%s'", param_name($1));
  }
;

named_param:
  param_member
  {
    const char *n = NULL;
    citer i;
    if (!$1) { $$ = NULL; break; }
    for (clist_first($1, &i); i.value; clist_next(&i))
      if (i.value->k == CST_STRING) n = i.value->s;
    if (!n) { $$ = NULL; clist_destroy($1); break; }

    $$ = param_new(@1, n, $1);
  }
  | param_member COLONCOLON identifier
  {
    if (!$1 || !$3) { $$ = NULL; break; }
    $$ = param_new(@1, $3, $1);
  }
  | /* empty */ COLONCOLON identifier
  {
    if (!$2) { $$ = NULL; break; }
    $$ = param_new(@2, $2, NULL);
  }
;

param_member:
  identifier
  {
    cval v = { .k = CST_STRING, { .s = $1 } };
    if (!$1) { $$ = NULL; break; }
    $$ = clist_append(NULL, v, 0/*!unique*/);
  }
  | param_member '.' identifier
  {
    cval v = { .k = CST_STRING, { .s = $3 } };
    if ($1)
      $$ = clist_append($1, v, 0/*!unique*/);
    else
      $$ = NULL;
  }
  | param_member '[' positive_int_const ']'
  {
    assert($3.k == CST_UINT);
    if ($1)
      $$ = clist_append($1, $3, 0/*!unique*/);
    else
      $$ = NULL;
  }
;

param_dir:
    IN		{ $$ = P_IN; }
  | OUT		{ $$ = P_OUT; }
  | INOUT	{ $$ = P_INOUT; }
  | INPORT	{ $$ = P_INPORT; }
  | OUTPORT	{ $$ = P_OUTPORT; }
;

initializer_list:
  /* empty */
  {
    $$ = NULL;
  }
  | initializer
  | initializer_list ',' initializer
  {
    $$ = $3 ? initer_append($1, $3) : $1;
  }
;

initializer:
  initializer_value
  | COLONCOLON string_literals
  {
    cval v; v.k = CST_VOID; v.u = -1U;
    if ($2) {
      $$ = initer_create(-1U, NULL, NULL, v);
      (void)initer_setdoc($$, $2);
    } else
      $$ = NULL;
  }
  | initializer_value COLONCOLON string_literals
  {
    $$ = $1; if (!$1 || !$3) break;
    (void)initer_setdoc($1, $3);
  }
;

initializer_value:
  const_expr
  {
    $$ = initer_create(-1U, NULL, NULL, $1);
  }
  | '{' initializer_list '}'
  {
    cval v; v.k = CST_VOID; v.u = -1U;
    $$ = $2 ? initer_create(-1U, NULL, $2, v) : NULL;
  }
  | '[' positive_int_const ']' '=' const_expr
  {
    assert($2.k == CST_UINT);
    $$ = initer_create($2.u, NULL, NULL, $5);
  }
  | '[' positive_int_const ']' '=' '{' initializer_list '}'
  {
    cval v; v.k = CST_VOID; v.u = -1U;
    assert($2.k == CST_UINT);
    $$ = initer_create($2.u, NULL, $6, v);
  }
  | '.' identifier '=' const_expr
  {
    $$ = initer_create(-1U, $2, NULL, $4);
  }
  | '.' identifier '=' '{' initializer_list '}'
  {
    cval v; v.k = CST_VOID; v.u = -1U;
    $$ = initer_create(-1U, $2, $5, v);
  }
;


/* --- IDL modules --------------------------------------------------------- */

/** modules are IDL namespaces */

module:
  MODULE module_name '{' idlspec '}'
  {
    scope_s s = scope_pop();

    assert(s == $2);
    if (scope_name(s)[0] == '&') {
      /* there was an error during the creation of the scope. */
      parserror(@1, "dropped declaration for '%s'", &scope_name(s)[1]);
      scope_destroy(s);
    }
  }
;

module_name: identifier
  {
    $$ = scope_push(@1, $1, SCOPE_MODULE);
    if (!$$) {
      /* on error, still create a scope to continue the parsing
       * but with a special name -- it will be deleted afterwards */
      $$ = scope_push(@1, strings("&", $1, NULL), SCOPE_MODULE);
      if (!$$) { /* still failed, just resign */ YYABORT;
      }
    }
  }
;

idldef: idlstatement | cpphash | /* empty */ { $$ = 0; };
idlspec: idldef | idlspec idldef;


/* --- constant definition ------------------------------------------------- */

/* these rules handle the `const' keyword. */

const_dcl:
  CONST const_type identifier '=' const_expr
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
      case IDL_LONG: case IDL_ULONGLONG: case IDL_LONGLONG: case IDL_FLOAT:
      case IDL_DOUBLE: case IDL_CHAR: case IDL_OCTET: case IDL_STRING:
      case IDL_ENUM:
	break;

      case IDL_ANY: case IDL_ENUMERATOR: case IDL_ARRAY: case IDL_SEQUENCE:
      case IDL_STRUCT: case IDL_UNION: case IDL_FORWARD_STRUCT:
      case IDL_FORWARD_UNION:
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
  constructed_type
  | TYPEDEF alias_list {
    $$ = $2;
  }
  | forward_dcl
;

constructed_type: struct_type | union_type | enum_type;

alias_list:
  type_spec declarator
  {
    $$ = $1; if (!$2) break;

    if ($1) $1 = dcl_settype($2, $1);
    if (!$1 || !type_newalias(@2, dcl_name($2), $1))
      parserror(@2, "dropped declaration for '%s'", dcl_name($2));

    dcl_destroy($2);
  }
  | alias_list ',' declarator
  {
    $$ = $1; if (!$3) break;

    if ($1) $1 = dcl_settype($3, $1);
    if (!$1 || !type_newalias(@3, dcl_name($3), $1))
      parserror(@3, "dropped declaration for '%s'", dcl_name($3));

    dcl_destroy($3);
  }
;

struct_type: STRUCT scope_push_struct '{' member_list '}'
  {
    scope_s s = scope_detach(scope_pop());
    assert(s == $2);

    if (scope_name(s)[0] == '&') {
      /* there was an error during the creation of the scope. */
      parserror(@1, "dropped declaration for '%s'", &scope_name(s)[1]);
      scope_destroy(s);
      break;
    }

    $$ = type_newstruct(@1, scope_name(s), s);
    if (!$$) {
      parserror(@1, "dropped declaration for '%s'", scope_name(s));
      scope_destroy(s);
    }
  }
  | STRUCT scope_push_struct error '}'
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

union_type:
  UNION scope_push_union SWITCH '(' switch_type_spec ')' '{' switch_body '}'
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
  | UNION scope_push_union error '}'
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

enum_type: ENUM identifier '{' enumerator_list '}'
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

forward_dcl:
  STRUCT identifier
  {
    $$ = type_newforward(@1, $2, IDL_FORWARD_STRUCT);
  }
  | UNION identifier
  {
    $$ = type_newforward(@1, $2, IDL_FORWARD_UNION);
  }
;


/* --- type declarators ---------------------------------------------------- */

declarator: simple_declarator | array_declarator;

simple_declarator: identifier
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

type_spec: simple_type_spec
  {
    /* forward declarations are invalid type specification */
    if ($1) if (type_kind($1) == IDL_FORWARD_STRUCT ||
		type_kind($1) == IDL_FORWARD_UNION) {
	parserror(@1, "cannot use %s %s before it is fully defined",
		  type_strkind(type_kind($1)), type_name($1));
	$$ = NULL;
      }
  }
  | constructed_type_spec;

simple_type_spec: base_type_spec | template_type_spec | named_type;
constructed_type_spec: constructed_type;

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

signed_int: signed_longlong_int | signed_long_int | signed_short_int;
unsigned_int: unsigned_longlong_int | unsigned_long_int | unsigned_short_int;

unsigned_short_int: UNSIGNED SHORT
  { $$ = type_newbasic(@1, NULL, IDL_USHORT); };
unsigned_long_int: UNSIGNED LONG
  { $$ = type_newbasic(@1, NULL, IDL_ULONG); };
unsigned_longlong_int: UNSIGNED LONG LONG
  { $$ = type_newbasic(@1, NULL, IDL_ULONGLONG); };
signed_short_int: SHORT
  { $$ = type_newbasic(@1, NULL, IDL_SHORT); };
signed_long_int: LONG
  { $$ = type_newbasic(@1, NULL, IDL_LONG); };
signed_longlong_int: LONG LONG
  { $$ = type_newbasic(@1, NULL, IDL_LONGLONG); };
float_type: FLOAT
  { $$ = type_newbasic(@1, NULL, IDL_FLOAT); };
double_type: DOUBLE
  { $$ = type_newbasic(@1, NULL, IDL_DOUBLE); };
char_type: CHAR
  { $$ = type_newbasic(@1, NULL, IDL_CHAR); };
boolean_type: BOOLEAN
  { $$ = type_newbasic(@1, NULL, IDL_BOOL); };
octet_type: OCTET
  { $$ = type_newbasic(@1, NULL, IDL_OCTET); };
any_type: ANY
  { $$ = type_newbasic(@1, NULL, IDL_ANY); };

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
      case IDL_LONG: case IDL_ULONGLONG: case IDL_LONGLONG: case IDL_CHAR:
      case IDL_ENUM:
	break;

      case IDL_FLOAT: case IDL_DOUBLE: case IDL_OCTET: case IDL_STRING:
      case IDL_ANY: case IDL_ENUMERATOR: case IDL_ARRAY: case IDL_SEQUENCE:
      case IDL_STRUCT: case IDL_UNION: case IDL_FORWARD_STRUCT:
      case IDL_FORWARD_UNION:
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

member_list: member ';' | member_list member ';';

member:
  type_spec declarator
  {
    $$ = $1; if (!$2) break;

    if ($1) $1 = dcl_settype($2, $1);
    if (!$1 || !type_newmember(@2, dcl_name($2), $1))
      parserror(@2, "dropped declaration for '%s'", dcl_name($2));

    dcl_destroy($2);
  }
  | member ',' declarator
  {
    $$ = $1; if (!$3) break;

    if ($1) $1 = dcl_settype($3, $1);
    if (!$1 || !type_newmember(@3, dcl_name($3), $1))
      parserror(@3, "dropped declaration for '%s'", dcl_name($3));

    dcl_destroy($3);
  }
;

case: case_label_list type_spec declarator ';'
  {
    $$ = NULL;
    if (!$1 || !$2 || !$3) {
      if ($2 && !type_name($2)) type_destroy($2);
      if ($3) parserror(@3, "dropped declaration for '%s'", dcl_name($3));
      dcl_destroy($3);
      break;
    }

    $2 = dcl_settype($3, $2);
    if ($2) $$ = type_newcase(@3, dcl_name($3), $2, $1);
    if (!$2 || !$$)
      parserror(@3, "dropped declaration for '%s'", dcl_name($3));
    dcl_destroy($3);
  }
;

case_label_list:
  case_label
  {
    $$ = clist_append(NULL, $1, 1/*unique*/);
  }
  | case_label_list case_label
  {
    $$ = clist_append($1, $2, 1/*unique*/);
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

enumerator: identifier
  {
    $$ = type_newenumerator(@1, $1);
  }
;


/* --- scopes -------------------------------------------------------------- */

/* scopes are created as a side effect of certain declarations (modules,
 * interfaces, ...) or types (structures, unions, ...) */

scope_push_struct: identifier
  {
    $$ = scope_push(@1, $1, SCOPE_STRUCT);
    if (!$$) {
      /* on error, still create a scope to continue the parsing
       * but with a special name -- it will be deleted afterwards */
      $$ = scope_push(@1, strings("&", $1, NULL), SCOPE_STRUCT);
      if (!$$) { /* still failed, just resign */ YYABORT;
      }
    }
  }
;

scope_push_union: identifier
  {
    $$ = scope_push(@1, $1, SCOPE_UNION);
    if (!$$) {
      /* on error, still create a scope to continue the parsing
       * but with a special name -- it will be deleted afterwards */
      $$ = scope_push(@1, strings("&", $1, NULL), SCOPE_UNION);
      if (!$$) { /* still failed, just resign */ YYABORT; }
    }
  }
;

scoped_name:
  identifier
  | COLONCOLON identifier
  {
    $$ = strings("::", $2, NULL);
  }
  | scoped_name COLONCOLON identifier
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


/* --- identifiers --------------------------------------------------------- */

/* when the context allows it, identifiers can be GenoM special keywords */

identifier:
  IDENTIFIER | S | MS | US | K | M | REAL_TIME
  | COMPONENT | IDS | ATTRIBUTE | VERSION | LANG | EMAIL | REQUIRE
  | BUILDREQUIRE | CLOCKRATE | TASK | PERIOD | DELAY | PRIORITY | SCHEDULING
  | STACK | CODEL | VALIDATE | YIELD | THROWS | DOC | INTERRUPTS | BEFORE
  | AFTER | EVENT | DATA | INPORT | OUTPORT | IN | OUT | INOUT
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


/* --- # directives from cpp ----------------------------------------------- */

cpphash:
  '#' integer_literal string_literal '\n'
  {
    if (!runopt.cppdotgen) {
      struct stat s, si;
      if (!stat($3, &s) && !stat(runopt.input, &si))
	if (si.st_dev == s.st_dev && si.st_ino == s.st_ino)
	  $3 = string(runopt.input);
    }

    curloc.file = $3;
    curloc.line = $2;
    curloc.col = 1;
  }
  | '#' integer_literal string_literal integer_literal '\n'
  {
    if (!runopt.cppdotgen) {
      struct stat s, si;
      if (!stat($3, &s) && !stat(runopt.input, &si))
	if (si.st_dev == s.st_dev && si.st_ino == s.st_ino)
	  $3 = string(runopt.input);
    }

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
  parserror(curloc, "%s before or at column %d", msg, curloc.col);
}
