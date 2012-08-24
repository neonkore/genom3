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
 *                                           Cédric Pasteur on Mon Apr 20 2009
 */
%{
#include "acgenom.h"

#include <assert.h>
#include <stddef.h>
#include <errno.h>

#include "genom.h"

#ifndef NDEBUG
# define YYDEBUG 1
#endif

#define YYLTYPE	tloc

#define YYLLOC_DEFAULT(c, rhs, n)		\
  do {						\
    (c).file = YYRHSLOC(rhs, (n)?1:0).file;	\
    (c).line = YYRHSLOC(rhs, (n)?1:0).line;	\
    (c).col = YYRHSLOC(rhs, (n)?1:0).col;	\
  } while (0)

  extern char *dotgentext;
%}

%name-prefix="dotgen"
%pure-parser
%locations

%union {
  int		i;
  double	d;
  char		c;
  char *	s;
  cval		v;
  psrc		psrc;
  pdir		pdir;
  svckind	skind;
  portkind	pkind;
  codelkind	ckind;
  clist_s	vlist;
  prop_s	prop;
  codel_s	codel;
  port_s	port;
  param_s	param;
  initer_s	initer;
  hash_s	hash;
  comp_s	comp;
  scope_s	scope;
  dcl_s		dcl;
  idltype_s	type;
}

%{
  extern YYLTYPE curloc;
  extern int task_p;

  extern int dotgenlex(YYSTYPE *lvalp, YYLTYPE *llocp);
  extern void dotgenerror(const char *msg);
%}

%token <i>	COLONCOLON SL SR
%token <i>	MODULE
%token <i>	UNSIGNED SHORT LONG FIXED FLOAT DOUBLE CHAR WCHAR STRING
%token <i>	WSTRING BOOLEAN OCTET OBJECT ANY VOID
%token <i>	CONST ENUM UNION SWITCH CASE DEFAULT STRUCT SEQUENCE
%token <i>	TYPEDEF
%token <i>	FALSE TRUE
%token <i>	integer_literal FIXED_LIT
%token <d>	FLOAT_LIT
%token <c>	CHAR_LIT
%token <s>	string_literal IDENTIFIER

%token <s>	S MS US K M REAL_TIME
%token <s>	INTERFACE COMPONENT TASK TASK_P FUNCTION ACTIVITY CODEL PORT IN
%token <s>	OUT INOUT SERVICE IDS ATTRIBUTE INPUT OUTPUT HANDLE VERSION LANG
%token <s>	EMAIL REQUIRE CODELSREQUIRE PERIOD DELAY PRIORITY STACK VALIDATE
%token <s>	YIELD THROWS DOC INTERRUPTS BEFORE AFTER CLOCKRATE SCHEDULING
%token <s>	ASYNC REMOTE MULTIPLE

%type <i>	specification statement idl_statements idl_statement
%type <i>	exports export

%type <comp>	component_name interface_name
%type <i>	interface component ids attribute port task service remote
%type <skind>	service_kind
%type <pkind>	port_dir opt_multiple
%type <prop>	component_property task_property service_property codel_property
%type <prop>	throw_property property
%type <hash>	opt_properties properties
%type <hash>	attribute_parameters service_parameters codel_parameters
%type <initer>	opt_initializer initializer_value initializer initializers
%type <codel>	codel fsm_codel
%type <ckind>	opt_async
%type <param>	attribute_parameter service_parameter codel_parameter
%type <vlist>	parameter_variable
%type <psrc>	opt_parameter_src
%type <pdir>	parameter_dir

%type <i>	module module_body
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
%type <scope>	module_name scope_push_struct scope_push_union ids_name

%type <v>	fixed_array_size
%type <v>	positive_int_const const_expr unary_expr primary_expr
%type <v>	or_expr xor_expr and_expr shift_expr add_expr mult_expr
%type <v>	literal time_unit size_unit
%type <s>	scoped_name string_literals identifier
%type <hash>	event_list identifier_list
%type <vlist>	string_list

%expect 0
%%
/*/ @node Specification
 * @section Elements of a @genom{3} specification
 * @cindex specification
 * @cindex @genom{3}, specification
 * @cindex @code{dotgen}, specification
 *
 * A dotgen specification consists of one or more statements. Statements are
 * either @genom{} statements, IDL statements. @command{cpp} directives
 * (@pxref{Preprocessing}) are handled at the lexical level and do not
 * interfere with the specification grammar.
 *
 * @ruleinclude specification
 * @ruleinclude statement
 * @sp 1
 * @ruleinclude idl_statement
 *
 * Definitions are named by the mean of identifiers, @pxref{Reserved keywords}.
 *
 * A @genom{} statement defines components (@pxref{Component declaration}) or
 * interfaces.
 *
 * An @acronym{IDL} statement defines types (@pxref{Type declaration}),
 * constants (@pxref{Constant declaration}) or @acronym{IDL} modules containing
 * types and constants (@pxref{Module declaration}).  The syntax follows
 * closely the subset the @acronym{OMG} @acronym{IDL} specification
 * corresponding to type and constants definitions (see Chapter 7 of
 * @cite{CORBA specification, Object Management Group, version 3.1. Part I:
 * CORBA interfaces}).  Note that this subset of the dogten grammar is not in
 * any manner tied to OMG IDL and may diverge from future OMG specifications.
 */

specification: /* empty */ { $$ = 0; } | specification statement;

statement:
    component
  | interface
  | idl_statement
  | error ';'
  {
    parserror(@1, "expected `component', `interface' or IDL definition");
  }
  | error
  {
    parserror(@1, "expected `component', `interface' or IDL definition");
    parsenoerror(@1, "maybe a missing ';'");
    YYABORT;
  }
;

idl_statements: idl_statement | idl_statements idl_statement;
idl_statement:
  module
  | const_dcl ';'
  {
    $$ = 0;
  }
  | type_dcl ';'
  {
    $$ = 0;
  }
;


/* --- GenoM objects ------------------------------------------------------- */

/*/ @node Component declaration
 * @section Component declaration
 * @cindex component, declaration
 * @cindex declaration, component
 *
 * @ruleinclude component
 * @ruleinclude component_name
 * @ruleinclude component_body
 * @sp 1
 * @ruleinclude exports
 * @ruleinclude export
 */
component: COMPONENT component_name component_body ';'
  {
    comp_s c = comp_pop();
    if (c) assert(c == $2);
  }
;

component_name: identifier
  {
    if (!$1) { parserror(@1, "dropped component"); YYABORT; }
    $$ = comp_push(@1, $1, COMP_REGULAR);
    if (!$$) {
      parserror(@1, "dropped component '%s'", $1);
      YYABORT;
    }
  }
;

interface: INTERFACE interface_name component_body ';'
  {
    comp_s c = comp_pop();
    if (c) assert(c == $2);
  }
;

interface_name: identifier
  {
    if (!$1) { parserror(@1, "dropped interface"); YYABORT; }
    $$ = comp_push(@1, $1, COMP_IFACE);
    if (!$$) {
      parserror(@1, "dropped interface '%s'", $1);
      YYABORT;
    }
  }
;

component_body: /* empty */ | '{' exports '}';

exports:
  /* empty */		{ $$ = 0; }
  | exports export	{ assert(!task_p); }
;

export:
    ids
  | attribute
  | port
  | task
  | service
  | remote
  | idl_statement
  | property
  {
    $$ = 0; if (!$1) break;
    if (comp_addprop(@1, $1)) {
      parsenoerror(@1, "dropped %s declaration", prop_name($1));
      prop_destroy($1);
    }
  }
;

port:
  PORT opt_multiple port_dir type_spec identifier ';'
  {
    if (!$5) { parserror(@1, "dropped port"); break; }
    if (!$4) { parserror(@1, "dropped '%s' port", $5); break; }
    if (!port_create(@1, $3, $2, $5, $4))
      parserror(@1, "dropped '%s' port", $5);
  }
;

port_dir:
    IN	{ $$ = PORT_IN; }
  | OUT	{ $$ = PORT_OUT; }
;

opt_multiple:
  /* empty */	{ $$ = PORT_SIMPLE; }
  | MULTIPLE	{ $$ = PORT_MULTIPLE; }
;

ids:
  ids_name '{' member_list '}' ';'
  {
    scope_s s = scope_pop(); assert(s == $1);
    if (!comp_addids(@1, s)) parserror(@1, "dropped ids declaration");
  }
  | ids_name error ';'
  {
    scope_s s = scope_pop(); assert(s == $1);
    parserror(@1, "dropped ids declaration");
  }
  | ids_name error
  {
    scope_s s = scope_pop(); assert(s == $1);
    parsenoerror(@1, "maybe a missing ';'");
    parserror(@1, "dropped ids declaration");
  }
;

ids_name: IDS
  {
    $$ = comp_idsscope(comp_active());
    scope_set($$);
  };

task:
  TASK identifier opt_properties ';'
  {
    if (!$2) { parserror(@1, "dropped task"); break; }
    if (!task_create(@1, $2, $3)) {
      parsenoerror(@1, "dropped '%s' task", $2);
      if ($3) hash_destroy($3, 1);
    }
  }
;

attribute:
  ATTRIBUTE identifier '(' attribute_parameters ')' opt_properties ';'
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
  | ATTRIBUTE identifier '(' error ')' opt_properties ';'
  {
    task_p = 0;
    param_setlocals(NULL);
    parserror(@1, "dropped '%s' attribute", $2);
  }
;

service:
  service_kind identifier '(' service_parameters ')' opt_properties ';'
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
  | service_kind identifier '(' error ')' opt_properties ';'
  {
    task_p = 0;
    param_setlocals(NULL);
    parserror(@1, "dropped '%s' %s", $2, service_strkind($1));
  }
;

remote:
  REMOTE identifier '(' service_parameters ')' ';'
  {
    task_p = 0;
    param_setlocals(NULL);
    if (!$2 || !$4) {
      if ($2) parserror(@1, "dropped '%s' remote", $2);
      if ($4) hash_destroy($4, 1);
      break;
    }
    if (!remote_create(@1, $2, $4)) {
      parserror(@1, "dropped '%s' remote", $2);
      hash_destroy($4, 1);
    }
  }
  | REMOTE identifier '(' error ')' ';'
  {
    task_p = 0;
    param_setlocals(NULL);
    parserror(@1, "dropped '%s' remote", $2);
  }
;

service_kind:
  FUNCTION	{ $$ = S_FUNCTION; }
  | ACTIVITY	{ $$ = S_ACTIVITY; }
;


/* --- GenoM object properties --------------------------------------------- */

opt_properties: /* empty */ { $$ = 0; } | '{' properties '}' { $$ = $2; }

properties:
  /* empty */
  {
    $$ = hash_create("property list", 5);
  }
  | properties property
  {
    $$ = $1;
    if (!$$ || !$2) break;
    if (hash_insert($$, prop_name($2), $2, (hrelease_f)prop_destroy)) {
      if (errno != EEXIST || prop_merge($1, $2)) {
        parserror(@2, "dropped %s declaration", prop_name($2));
        prop_destroy($2);
      }
    }
  }
;

property:
    component_property | task_property | service_property | codel_property
  | throw_property
  | error ';'
  {
    parserror(@1, "invalid property");
    $$ = NULL;
  }
;

component_property:
  DOC string_literals ';'
  {
    if (!$2) { parserror(@1, "dropped '%s' property", $1); $$ = NULL; break; }
    $$ = prop_newstring(@1, PROP_DOC, $2);
  }
  | VERSION string_literals ';'
  {
    if (!$2) { parserror(@1, "dropped '%s' property", $1); $$ = NULL; break; }
    $$ = prop_newstring(@1, PROP_VERSION, $2);
  }
  | LANG string_literals ';'
  {
    if (!$2) { parserror(@1, "dropped '%s' property", $1); $$ = NULL; break; }
    $$ = prop_newstring(@1, PROP_LANG, $2);
  }
  | EMAIL string_literals ';'
  {
    if (!$2) { parserror(@1, "dropped '%s' property", $1); $$ = NULL; break; }
    $$ = prop_newstring(@1, PROP_EMAIL, $2);
  }
  | REQUIRE string_list ';'
  {
    if (!$2) { parserror(@1, "dropped '%s' property", $1); $$ = NULL; break; }
    $$ = prop_newrequire(@1, PROP_REQUIRE, $2);
  }
  | CODELSREQUIRE string_list ';'
  {
    if (!$2) { parserror(@1, "dropped '%s' property", $1); $$ = NULL; break; }
    $$ = prop_newrequire(@1, PROP_CODELS_REQUIRE, $2);
  }
  | CLOCKRATE const_expr time_unit ';'
  {
    if (const_binaryop(&$2, '*', $3)) {
      parserror(@3, "invalid numeric constant");
      break;
    }
    $$ = prop_newvalue(@1, PROP_CLOCKRATE, $2);
  }
;

task_property:
  PERIOD const_expr time_unit ';'
  {
    if (const_binaryop(&$2, '*', $3)) {
      parserror(@3, "invalid numeric constant");
      break;
    }
    $$ = prop_newvalue(@1, PROP_PERIOD, $2);
  }
  | DELAY const_expr time_unit ';'
  {
    if (const_binaryop(&$2, '*', $3)) {
      parserror(@3, "invalid numeric constant");
      break;
    }
    $$ = prop_newvalue(@1, PROP_DELAY, $2);
  }
  | PRIORITY positive_int_const ';'
  {
    $$ = prop_newvalue(@1, PROP_PRIORITY, $2);
  }
  | SCHEDULING REAL_TIME ';'
  {
    $$ = prop_newstring(@1, PROP_SCHEDULING, $2);
  }
  | STACK positive_int_const size_unit ';'
  {
    if (const_binaryop(&$2, '*', $3)) {
      parserror(@3, "invalid numeric constant");
      break;
    }
    $$ = prop_newvalue(@1, PROP_STACK, $2);
  }
;

service_property:
  TASK_P identifier ';'
  {
    if (!$2) { parserror(@1, "dropped '%s' property", $1); $$ = NULL; break; }
    $$ = prop_newtask(@1, $2);
  }
  | INTERRUPTS identifier_list ';'
  {
    if (!$2) { parserror(@1, "dropped '%s' property", $1); $$ = NULL; break; }
    $$ = prop_newhash(@1, PROP_INTERRUPTS, $2);
  }
  | BEFORE identifier_list ';'
  {
    if (!$2) { parserror(@1, "dropped '%s' property", $1); $$ = NULL; break; }
    $$ = prop_newhash(@1, PROP_BEFORE, $2);
  }
  | AFTER identifier_list ';'
  {
    if (!$2) { parserror(@1, "dropped '%s' property", $1); $$ = NULL; break; }
    $$ = prop_newhash(@1, PROP_AFTER, $2);
  }
  | VALIDATE codel ';'
  {
    $$ = $2 ? prop_newcodel(@1, PROP_VALIDATE, $2) : NULL;
  }
;

codel_property:
  opt_async CODEL codel ';'
  {
    if ($3) codel_setkind($3, $1);
    $$ = $3 ? prop_newcodel(@1, PROP_SIMPLE_CODEL, $3) : NULL;
  }
  | opt_async CODEL fsm_codel ';'
  {
    if ($3) codel_setkind($3, $1);
    $$ = $3 ? prop_newcodel(@1, PROP_FSM_CODEL, $3) : NULL;
  }
;

throw_property:
  THROWS identifier_list ';'
  {
    $$ = $2 ? prop_newhash(@1, PROP_THROWS, $2) : NULL;
  }
;


/* --- codels -------------------------------------------------------------- */

codel:
  identifier '(' codel_parameters ')'
  {
    $$ = codel_create(@1, $1, CODEL_SYNC, NULL, NULL, $3);
  }
;

fsm_codel:
  event_list identifier '(' codel_parameters ')' YIELD identifier_list
  {
    if (!$1 || !$7) {
      parserror(@1, "dropped codel '%s'", $2); $$ = NULL; break;
    }
    $$ = codel_create(@3, $2, CODEL_SYNC, $1, $7, $4);
  }
  | event_list identifier '(' codel_parameters ')' error
  {
    $$ = NULL;
    parserror(@1, "missing 'yield' values for codel %s", $2);
    if ($1) hash_destroy($1, 1);
    if ($4) hash_destroy($4, 1);
  }
;

opt_async:
  /* empty */	{ $$ = CODEL_SYNC; }
  | ASYNC	{ $$ = CODEL_ASYNC; }
;

event_list:
  '<' identifier_list '>'
  {
    $$ = $2;
  }
;


/* --- parameters ---------------------------------------------------------- */

attribute_parameters:
  /* empty */
  {
    $$ = hash_create("parameter list", 0);
    task_p = 1;
    param_setlocals($$);
  }
  | attribute_parameter
  {
    $$ = hash_create("parameter list", 3); if (!$$ || !$1) break;
    if (hash_insert($$, param_name($1), $1, (hrelease_f)param_destroy)) {
      param_destroy($1); break;
    }
    task_p = 1;
    param_setlocals($$);
  }
  | attribute_parameters ',' attribute_parameter
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

attribute_parameter:
  parameter_dir parameter_variable opt_initializer
  {
    if (!$2) { $$ = NULL; break; }
    $$ = param_newids(@2, $1, NULL, $2, $3);
  }
  | parameter_dir error
  {
    parserror(@2, "expected ids member");
    $$ = NULL;
  }
;

service_parameters:
  /* empty */
  {
    $$ = hash_create("parameter list", 0);
    task_p = 1;
    param_setlocals($$);
  }
  | service_parameter
  {
    $$ = hash_create("parameter list", 3); if (!$$ || !$1) break;
    if (hash_insert($$, param_name($1), $1, (hrelease_f)param_destroy)) {
      param_destroy($1); break;
    }
    task_p = 1;
    param_setlocals($$);
  }
  | service_parameters ',' service_parameter
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
;

opt_parameter_src:
  /* empty */	{ $$ = P_NOSRC; }
  | IDS		{ $$ = P_IDS; }
  | SERVICE	{ $$ = P_SERVICE; }
  | PORT	{ $$ = P_PORT; }
  | REMOTE	{ $$ = P_REMOTE; }
;

parameter_dir:
    IN		{ $$ = P_IN; }
  | OUT		{ $$ = P_OUT; }
  | INOUT	{ $$ = P_INOUT; }
  | error
  {
    $$ = P_NODIR;
    parserror(@1, "expected 'in', 'out' or 'inout'");
  }
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


/* --- initializers -------------------------------------------------------- */

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
      $$ = initer_create(-1U, NULL, NULL, v);
      (void)initer_setdoc($$, $2);
    } else
      $$ = NULL;
  }
  | initializer_value ':' string_literals
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
  | '{' initializers '}'
  {
    cval v; v.k = CST_VOID; v.u = -1U;
    $$ = $2 ? initer_create(-1U, NULL, $2, v) : NULL;
  }
  | '[' positive_int_const ']' '=' const_expr
  {
    assert($2.k == CST_UINT);
    $$ = initer_create($2.u, NULL, NULL, $5);
  }
  | '[' positive_int_const ']' '=' '{' initializers '}'
  {
    cval v; v.k = CST_VOID; v.u = -1U;
    assert($2.k == CST_UINT);
    $$ = initer_create($2.u, NULL, $6, v);
  }
  | '.' identifier '=' const_expr
  {
    $$ = initer_create(-1U, $2, NULL, $4);
  }
  | '.' identifier '=' '{' initializers '}'
  {
    cval v; v.k = CST_VOID; v.u = -1U;
    $$ = initer_create(-1U, $2, $5, v);
  }
;


/* --- IDL modules --------------------------------------------------------- */

/*/ @node Module declaration
 * @section Module declaration
 * @cindex module, declaration
 *
 * A module definition satisfies the following syntax:
 *
 * @ruleinclude module
 * @ruleinclude module_name
 * @ruleinclude module_body
 * @ruleinclude idl_statements
 *
 * The only effect of a module is to scope @acronym{IDL} identifiers. It is
 * similar to a @acronym{C++} or Java namespace; it is considered good practice
 * to enclose your type definitions inside a module definition to prevent name
 * clashes between components.
 */
module:
  MODULE module_name '{' module_body '}' ';'
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

module_body: /* empty */ { $$ = 0; } | idl_statements;


/* --- constant definition ------------------------------------------------- */

/*/ @node Constant declaration
 * @section Constant declaration
 * @cindex Constant, declaration
 *
 * @ruleinclude const_dcl
 */

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
      case IDL_FORWARD_UNION: case IDL_PORT: case IDL_REMOTE:
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

/*/ @node Type declaration
 * @section Type declaration
 * @cindex Type, declaration
 *
 * Type declarations define new data types and associate a name (an identifier)
 * with it. The @code{typedef} keyword can be used to name an existing
 * type. The constructed types @code{struct}, @code{union} and @code{enum} also
 * name the type they define. The syntax is the following:
 *
 * @ruleinclude type_dcl
 * @ruleinclude constructed_type
 * @ruleinclude alias_list
 */

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
	hash_destroy($4, 1);
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

/*/ @node Type specification
 * @section Type specification
 * @cindex Type, specification
 *
 * A type specification is the description of a type. It can be used in a
 * @code{typedef} construct or anywhere a typed value is expected.
 *
 * @ruleinclude type_spec
 * @ruleinclude simple_type_spec
 * @ruleinclude base_type_spec
 * @ruleinclude template_type_spec
 * @ruleinclude constructed_type_spec
 * @ruleinclude named_type
 * @ruleinclude scoped_name
 * @ruleinclude declarator
 * @ruleinclude simple_declarator
 */

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
    $$ = type_newstring(@1, NULL, -1U);
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
    $$ = $3 ? type_newsequence(@1, NULL, $3, -1U) : NULL;
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
      case IDL_FORWARD_UNION: case IDL_PORT: case IDL_REMOTE:
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
 * components, interfaces, ...) or types (structures, unions, ...) */

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
  | SERVICE | ASYNC | REMOTE | MULTIPLE
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

%%

 /* --- dotgenerror -------------------------------------------------------- */

void
dotgenerror(const char *msg)
{
  if (!*dotgentext) {
    parserror(curloc, "premature end-of-file");
    return;
  }
  parserror(curloc, "%s at `%s' (near column %d)", msg, dotgentext, curloc.col);
}
