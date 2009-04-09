/* $Id: parser.yy 19 2007-08-19 20:36:24Z tb $ -*- mode: c++ -*- */
/** \file parser.yy Contains the example Bison parser source */

%{ /*** C/C++ Declarations ***/

#include <stdio.h>
#include <string>
#include <vector>

#include "ast.h"

%}

/*** yacc/bison Declarations ***/

/* Require bison 2.3 or later */
%require "2.3"

/* add debug output code to generated parser. disable this for release
 * versions. */
%debug

/* start symbol is named "start" */
%start start

/* write out a header file containing the token defines */
%defines

/* use newer C++ skeleton file */
%skeleton "lalr1.cc"

/* namespace to enclose parser in */
%name-prefix="G3nom"

/* set the parser's class identifier */
%define "parser_class_name" "Parser"

/* keep track of the current position within the input */
%locations
%initial-action
{
    // initialize the initial location object
    @$.begin.filename = @$.end.filename = &driver.streamname;
};

/* The driver is passed by reference to the parser and to the scanner. This
 * provides a simple but effective pure interface, not relying on global
 * variables. */
%parse-param { class Driver& driver }

/* verbose error messages */
%error-verbose

 /*** BEGIN EXAMPLE - Change the example grammar's tokens below ***/

%union {
    int  			integerVal;
    double 			doubleVal;
    std::string*		stringVal;
    CalcNode*			calcnode;
}

%token			END	     0	"end of file"
%token			EOL		"end of line"
%token <integerVal> 	INTEGER		"integer"
%token <doubleVal> 	DOUBLE		"double"
%token <stringVal> 	STRING		"string"

%type <calcnode>	constant variable
%type <calcnode>	atomexpr powexpr unaryexpr mulexpr addexpr expr

%destructor { delete $$; } STRING
%destructor { delete $$; } constant variable
%destructor { delete $$; } atomexpr powexpr unaryexpr mulexpr addexpr expr

 /*** END EXAMPLE - Change the example grammar's tokens above ***/

%{

#include "driver.h"
#include "lexer.h"

/* this "connects" the bison parser in the driver to the flex scanner class
 * object. it defines the yylex() function call to pull the next token from the
 * current lexer object of the driver context. */
#undef yylex
#define yylex driver.lexer->lex

%}

%% /*** Grammar Rules ***/

start:
   declarations
{}

declarations:
   declarations declaration ';'
{}

declaration:
   type_dcl | port_decl | component_decl
   | task_decl | service_decl 
{}

component_decl:
   COMPONENT identifier '{' component_fields '}'
{}

component_fields:
   identifier ':' value
{}

/* --- type declaration ---------------------------------------------- */

type_dcl:
   TYPEDEF alias_type_list
   {
   }
   | constr_type_spec
   {
   }

constr_type_spec: constr_type
   {
   }
;

constr_type: struct_type | union_type | enum_type;

alias_type_list:
   type_spec declarator
   {
   }
   | alias_type_list ',' declarator
   {
   }
;

enum_type: ENUM IDENTIFIER '{' enumerator_list '}'
   {
   }
;

struct_type: STRUCT scope_push_new '{' member_list '}'
   {
   }
   | STRUCT scope_push_new error '}'
   {
   }
;

union_type: 
   UNION scope_push_new SWITCH '(' switch_type_spec ')' '{' switch_body '}'
   {
   }
   | UNION scope_push_new error '}'
   {
   }
;

/* --- type declarators ---------------------------------------------- */

declarator: simple_declarator | array_declarator;

simple_declarator: IDENTIFIER
   {
   }
;

array_declarator:
   simple_declarator fixed_array_size
   {
   }
   | array_declarator fixed_array_size
   {
   }
;

fixed_array_size: '[' positive_int_const ']'
   {
   }
;


/* --- type specification -------------------------------------------- */

/* these rules create and return an IdlerType structure */

type_spec: simple_type_spec | constr_type_spec;

simple_type_spec: base_type_spec | template_type_spec | named_type;

named_type: scoped_name
   {
   }
;

base_type_spec:
   floating_pt_type | integer_type | char_type | boolean_type
   | octet_type | any_type;

template_type_spec: sequence_type | string_type | fixed_type;

floating_pt_type:
   FLOAT
   {
   }
   | DOUBLE
   {
   }
;

integer_type: signed_int | unsigned_int;

signed_int: signed_long_int | signed_short_int;

signed_long_int: LONG
   {
   }
;

signed_short_int: SHORT
   {
   }
;

unsigned_int: unsigned_long_int | unsigned_short_int;

unsigned_long_int: UNSIGNED LONG
   {
   }
;

unsigned_short_int: UNSIGNED SHORT
   {
   }
;

char_type: CHAR
   {
   }
;

boolean_type: BOOLEAN
   {
   }
;

octet_type: OCTET
   {
   }
;

any_type: ANY
   {
   }
;

member_list:
   member ';'
   {
   }
   | member_list member ';'
   {
   }
;

member:
   type_spec declarator
   {
   }
   | member ',' declarator
   {
   }
;

switch_type_spec:
   integer_type | char_type | boolean_type | enum_type
   | scoped_name
   {
   }
;

switch_body: case | switch_body case
   {
   }
;

case: case_label_list element_spec ';'
   {
   }
;

case_label_list: case_label | case_label_list case_label
   {
   }
;

case_label:
   CASE const_exp ':'
   {
   }
   | DEFAULT ':'
   {
   }
;

element_spec: type_spec declarator
   {
   }
;

enumerator_list:
   enumerator
   {
   }
   | enumerator_list ',' enumerator
   {
   }
;

enumerator: IDENTIFIER
   {
   }
;

sequence_type:
   SEQUENCE '<' simple_type_spec ',' positive_int_const '>'
   {
   }
   | SEQUENCE '<' simple_type_spec '>'
   {
   }
;

string_type:
   STRING '<' positive_int_const '>'
   {
   }
   | STRING
   {
   }
;

fixed_type:
   FIXED '<' positive_int_const ',' positive_int_const '>'
   {
      ;
   }
   | FIXED
   {
      ;
   }
;

%% /*** Additional Code ***/

void G3nom::Parser::error(const Parser::location_type& l,
			    const std::string& m)
{
    driver.error(l, m);
}


