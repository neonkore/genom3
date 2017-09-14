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
 *                                           Cédric Pasteur on Mon Apr 20 2009
 */
%{
#include "acgenom.h"

#include <assert.h>
#include <errno.h>
#include <stddef.h>
#include <string.h>

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

%name-prefix "dotgen"
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
%token <i>	WSTRING BOOLEAN OCTET OBJECT ANY
%token <i>	CONST ENUM UNION SWITCH CASE DEFAULT STRUCT SEQUENCE OPTIONAL
%token <i>	TYPEDEF
%token <i>	FALSE TRUE
%token <i>	integer_literal FIXED_LIT
%token <d>	FLOAT_LIT
%token <c>	CHAR_LIT
%token <s>	string_literal IDENTIFIER

%token <s>	S MS US K M REAL_TIME
%token <s>	INTERFACE COMPONENT TASK TASK_P FUNCTION ACTIVITY CODEL PORT IN
%token <s>	OUT INOUT LOCAL IDS ATTRIBUTE INPUT OUTPUT HANDLE VERSION LANG
%token <s>	EMAIL REQUIRE CODELSREQUIRE PERIOD DELAY PRIORITY STACK VALIDATE
%token <s>	YIELD PAUSE THROWS DOC INTERRUPTS BEFORE AFTER CLOCKRATE
%token <s>	SCHEDULING ASYNC REMOTE EXTENDS PROVIDES USES MULTIPLE NATIVE
%token <s>	EXCEPTION

%type <i>	specification statement idl_statements idl_statement
%type <i>	exports export

%type <comp>	component_name interface_scope interface_name
%type <i>	interface component ids attribute port task service
%type <skind>	service_kind
%type <pkind>	port_dir opt_multiple
%type <prop>	component_property interface_property task_property
%type <prop>	service_property codel_property throw_property property
%type <hash>	interface_list opt_properties properties
%type <hash>	attribute_parameters service_parameters codel_parameters
%type <initer>	opt_initializer initializer_value initializer initializers
%type <codel>	codel fsm_codel
%type <ckind>	opt_async
%type <param>	attribute_parameter service_parameter codel_parameter
%type <type>	local_variables
%type <vlist>	parameter_variable
%type <psrc>	opt_parameter_src
%type <pdir>	parameter_dir

%type <i>	module module_body
%type <type>	const_dcl const_type
%type <type>	type_dcl constructed_type exception_dcl forward_dcl
%type <dcl>	declarator simple_declarator array_declarator
%type <type>	type_declarator
%type <type>	type_spec simple_type_spec base_type_spec template_type_spec
%type <type>	constructed_type_spec switch_type_spec
%type <type>	integer_type unsigned_int unsigned_short_int unsigned_long_int
%type <type>	unsigned_longlong_int signed_int signed_short_int
%type <type>	signed_long_int signed_longlong_int
%type <type>	floating_pt_type float_type double_type
%type <type>	boolean_type char_type octet_type any_type
%type <type>	sequence_type optional_type string_type fixed_type named_type
%type <type>	struct_type union_type enum_type enumerator case switch_body
%type <type>	exception_list member member_list opt_member_list
%type <hash>	enumerator_list
%type <v>	case_label
%type <vlist>	case_label_list
%type <scope>	module_name struct_name union_name exception_name ids_name

%type <v>	fixed_array_size
%type <v>	positive_int_const const_expr unary_expr primary_expr
%type <v>	or_expr xor_expr and_expr shift_expr add_expr mult_expr
%type <v>	literal time_unit size_unit
%type <s>	scoped_name string_literals identifier
%type <type>	event_name
%type <hash>	event_list throw_list identifier_list
%type <vlist>	string_list
%type <c>	semicolon

%expect 0
%%
/*/
 * Elements of a description file
 * ------------------------------
 *
 * A `dot gen` specification consists of one or more statements. Statements are
 * either _data types_ definitions, via IDL statements, or specific _genom_
 * statements. `cpp` directives (see
 * link:preprocessing{outfilesuffix}[Preprocessing]) are handled at the lexical
 * level and do not appear in the grammar.
 *
 * ****
 * [horizontal]
 * <dotgen-rule-specification.adoc
 * <dotgen-rule-statement.adoc
 *
 * <dotgen-rule-idl-statement.adoc
 * ****
 *
 * IDL statements define link:typedef{outfilesuffix}[types],
 * link:typedef{outfilesuffix}[constants] or link:module{outfilesuffix}[IDL
 * modules] containing types and constants.  The syntax follows closely the
 * subset the 'OMG IDL' specification corresponding to type and constants
 * definitions (see Chapter 7 of the CORBA specification, Object Management
 * Group, version 3.1. Part I: CORBA interfaces).  Note that this subset of the
 * dogten grammar is not in any manner tied to OMG IDL and may diverge from
 * future OMG specifications.
 *
 * A `genom` statement defines link:component{outfilesuffix}[components] or
 * link:interface{outfilesuffix}[interfaces].
 *
 * === See also
 * * link:idltype{outfilesuffix}[Data type description]
 * * link:component{outfilesuffix}[Component declaration]
 * * link:interface{outfilesuffix}[Interface declaration]
 */

specification: /* empty */ { $$ = 0; } | specification statement;

statement:
  idl_statement
  | component
  | interface
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
  | const_dcl
  {
    $$ = 0;
  }
  | type_dcl
  {
    $$ = 0;
  }
;


/* --- GenoM objects ------------------------------------------------------- */

%include dotgen/component.y
%include dotgen/interface.y
%include dotgen/ids.y
%include dotgen/task.y
%include dotgen/port.y
%include dotgen/service.y
%include dotgen/codel.y
%include dotgen/parameter.y

/* --- IDL objects --------------------------------------------------------- */

%include dotgen/idlscope.y
%include dotgen/idltype.y
%include dotgen/idltype-base.y
%include dotgen/idltype-tmpl.y
%include dotgen/idltype-constr.y
%include dotgen/expr.y


/* --- Misc rules --------------------------------------------------------- */

opt_properties:
  /* empty */ { $$ = NULL; }
  | '{' properties '}' { $$ = $2; }

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
      if (errno != EEXIST || prop_merge($1, $2, 0/*ignore_dup*/)) {
        parserror(@2, "dropped %s declaration", prop_name($2));
        prop_destroy($2);
      }
    }
  }
  | properties error ';'
  {
    parserror(@1, "invalid property");
    $$ = NULL;
  }
;

property:
    component_property
  | interface_property
  | task_property
  | service_property
  | codel_property
  | throw_property
;

semicolon:
  ';'
  {
    $$ = ';';
  }
  | /* empty */
  {
    $$ = '\0';
    parserror(curloc, "expected ';' before %s",
              *dotgentext?dotgentext:"end-of-file");
    if (memcmp(&curloc, &@$, sizeof(curloc)))
      parsenoerror(@$, " for statement started here");
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
  parserror(curloc, "%s at `%s' (near column %d)", msg, dotgentext,
            curloc.col - strlen(dotgentext));
}
