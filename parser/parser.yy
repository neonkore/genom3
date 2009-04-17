/* 
 * Copyright (c) 2002 LAAS/CNRS                       --  Tue Mar 26 2002
 * All rights reserved.
 *
 * Redistribution and use  in source  and binary  forms,  with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *   1. Redistributions of  source  code must retain the  above copyright
 *      notice, this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice,  this list of  conditions and the following disclaimer in
 *      the  documentation  and/or  other   materials provided  with  the
 *      distribution.
 *
 * THIS  SOFTWARE IS PROVIDED BY  THE  COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND  ANY  EXPRESS OR IMPLIED  WARRANTIES,  INCLUDING,  BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES  OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR  PURPOSE ARE DISCLAIMED. IN  NO EVENT SHALL THE COPYRIGHT
 * HOLDERS OR      CONTRIBUTORS  BE LIABLE FOR   ANY    DIRECT, INDIRECT,
 * INCIDENTAL,  SPECIAL,  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF  SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN  CONTRACT, STRICT LIABILITY, OR
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE   OF THIS SOFTWARE, EVEN   IF ADVISED OF   THE POSSIBILITY OF SUCH
 * DAMAGE.
 */

%{ /*** C/C++ Declarations ***/

#include <cstdio>
#include <string>
#include <vector>

#include "ast.h"
#include "idltype.h"

using namespace G3nom;
using namespace Idl;
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
    @$.begin.filename = @$.end.filename = driver.streamNamePtr();
};

/* The driver is passed by reference to the parser and to the scanner. This
 * provides a simple but effective pure interface, not relying on global
 * variables. */
%parse-param { class Driver& driver }

/* verbose error messages */
%error-verbose

 /*** BEGIN EXAMPLE - Change the example grammar's tokens below ***/

%union {
    char 			charVal;
    int  			integerVal;
    double 			doubleVal;
    std::string*		stringVal;
    Port*			portVal;
    Service*			serviceVal;
    Task*			taskVal;
    Idl::IdlType*		typeVal;
    Idl::Declarator::Vect*	declaratorVectVal;
    Idl::Declarator*		declaratorVal;
}

%token			ENDOFFILE 0   	"end of file"
%token			COMPONENT	"component"
%token			TASK		"task"
%token			SERVICE		"service"
%token			CODEL		"codel"

%token LBRACE RBRACE SEMICOLON COLON LESS_THAN GREATER_THAN COMMA LPAREN RPAREN

%token IN OUT INPORT OUTPORT
// type tokens
%token SHORT LONG FLOAT DOUBLE FIXED CHAR WCHAR STRING WSTRING BOOLEAN OCTET ANY VOID NATIVE
%token ENUM UNION SWITCH CASE DEFAULT STRUCT SEQUENCE
%token TYPEDEF UNSIGNED OBJECT

%token <charVal>	SPECIAL_CHAR	"char"
%token <integerVal> 	INTEGERLIT	"integer"
%token <doubleVal> 	DOUBLELIT	"double"
%token <stringVal> 	STRINGLIT	"string literal"
%token <stringVal> 	IDENTIFIER	"identifier"

%type <portVal>		port_decl
%type <taskVal>		task_decl
%type <serviceVal>	service_decl

%type <typeVal>			type_decl
/* %type <typeVal> 		type_declarator */
%type <typeVal>  		type_spec
%type <typeVal> 		simple_type_spec
%type <typeVal>			base_type_spec
%type <typeVal>			template_type_spec
%type <typeVal>			constr_type_spec
%type <declaratorVectVal>	declarators
%type <declaratorVal>		declarator
%type <declaratorVal>		simple_declarator
%type <declaratorVal>		array_declarator
%type <integerVal>		fixed_array_size
%type <typeVal>			floating_pt_type
%type <typeVal> 		fixed_pt_type
%type <typeVal>			integer_type
%type <typeVal>			signed_int
%type <typeVal>			signed_short_int
%type <typeVal>			signed_long_int
%type <typeVal> 		signed_long_long_int
%type <typeVal> 		unsigned_int
%type <typeVal> 		unsigned_short_int
%type <typeVal> 		unsigned_long_int
%type <typeVal> 		unsigned_long_long_int
%type <typeVal> 		char_type
%type <typeVal> 		wide_char_type
%type <typeVal> 		boolean_type
%type <typeVal> 		octet_type
%type <typeVal> 		any_type
/*%type <typeVal> 		object_type*/
%type <typeVal> 		struct_type
/*%type <intVal>			member 
%type <intVal>			members*/
/*%type <member_val> 		member_list
%type <member_val> 		member*/
/*%type <union_val> 		union_type
%type <union_val> 		union_header
%type <type_spec_val> 		switch_type_spec
%type <union_case_val> 		switch_body
%type <union_case_val> 		case_plus
%type <union_case_val> 		case
%type <case_label_val> 		case_labels
%type <case_label_val> 		case_label
%type <union_case_val> 		element_spec*/
%type <typeVal> 		enum_type
/*%type <enum_val> 		enum_header */
/*%type <enumerator_val> 		enumerators
%type <enumerator_val> 		enumerator*/
/*%type enumerator
%type enumerators*/
%type <typeVal> 		sequence_type
%type <typeVal> 		string_type
%type <typeVal> 		wide_string_type

%destructor { delete $$; } STRINGLIT
/*%destructor { delete $$; } port_decl service_decl task_decl*/

 /*** END EXAMPLE - Change the example grammar's tokens above ***/

%{

#include "driver.h"
#include "lexer.h"

/* this "connects" the bison parser in the driver to the flex scanner class
 * object. it defines the yylex() function call to pull the next token from the
 * current lexer object of the driver context. */
#undef yylex
#define yylex driver.lexer()->lex

%}

%% /*** Grammar Rules ***/

start:
  declarations ENDOFFILE

declarations:
    declaration SEMICOLON
  | declarations declaration SEMICOLON
{};

declaration:
/*    type_decl  */
   port_decl
{
    driver.component().addPort($1->name, $1);
}
| component_decl
{}
| task_decl
{
    driver.component().addTask($1->name, $1);
}
| service_decl 
{
    driver.component().addService($1->name, $1);
}
| type_decl
{
    driver.component().addType($1);
}
;

/*** Component information ***/

component_decl:
   COMPONENT IDENTIFIER LBRACE component_fields RBRACE
{
    driver.component().name = *$2;
};

component_fields:
  component_field SEMICOLON
  | component_fields component_field SEMICOLON
{}


component_field:
   IDENTIFIER COLON STRINGLIT
{
    if(*$1 == "language") {
	driver.component().pluginLanguage = *$3;
    } else if(*$1 == "version") {
	driver.component().version = *$3;
    } else {
      error(yyloc, std::string("Unknown component field: ") + *$1);
      YYERROR;
    }
}
| IDENTIFIER COLON INTEGERLIT
{}
| IDENTIFIER COLON IDENTIFIER
{
    if(*$1 == "ids") {
	Idl::IdlType *t = driver.component().typeFromName(*$3);
	driver.component().IDSType = t;
    } else {
      error(yyloc, std::string("Unknown component field: ") + *$1);
      YYERROR;
    }
};

/*** Inport or outport declaration ***/

port_decl:
  INPORT IDENTIFIER IDENTIFIER
{
    Idl::IdlType *type = driver.component().typeFromName(*$2);
    $$ = new Port(*$3, type, true);
}
| OUTPORT IDENTIFIER IDENTIFIER
{
    Idl::IdlType *type = driver.component().typeFromName(*$2);
    $$ = new Port(*$3, type, false);
};

/*** Task declaration ***/

task_decl:
  TASK IDENTIFIER LBRACE task_fields RBRACE
{
    Task *t = driver.currentTask();
    t->name = *$2;
    driver.setCurrentTask(0);
    $$ = t;
};

task_fields:
  task_field SEMICOLON
  | task_fields task_field SEMICOLON
{}

task_field:
  CODEL IDENTIFIER COLON codel_prototype
{}
| IDENTIFIER COLON INTEGERLIT 
{
    Task *t = driver.currentTask();

    if(*$1 == "priority")
      t->priority = $3;
    else if(*$1 == "period")
      t->period = $3;
    else if(*$1 == "stackSize")
      t->stackSize = $3;
    else {
      error(yyloc, std::string("Unknown task field: ") + *$1);
      YYERROR;
    }
};

/*** Service Declaration ***/

service_decl:
    SERVICE IDENTIFIER LBRACE service_fields RBRACE
{
    Service *s = driver.currentService();
    s->name = *$2;
    driver.setCurrentService(0);
    $$ = s;
};

service_fields:
  service_field SEMICOLON
  | service_fields service_field SEMICOLON
{}

service_field:
  CODEL IDENTIFIER COLON codel_prototype
{}
| IDENTIFIER COLON IDENTIFIER
{
    Service *s = driver.currentService();

    if(*$1 == "type") {
	if(*$3 == "init")
	  s->type = Service::Init;
	else if(*$3 == "control")
	  s->type = Service::Control;
        else if(*$3 == "exec")
	  s->type = Service::Exec;
	else {
	  error(yyloc, std::string("Unknown service type: ") + *$3);
	  YYERROR;
	}
    } else if(*$1 == "taskName") {
	s->taskName = *$3;
    } else if(*$1 == "input") {
	s->addInput(*$3);
    } else if(*$1 == "output") {
	s->output = *$3;
    } else {
      error(yyloc, std::string("Unknown service field: ") + *$1);
      YYERROR;
    }
}
| IDENTIFIER COLON STRINGLIT 
{
    Service *s = driver.currentService();
    if(*$1 == "doc") {
      s->doc = *$3;
    } else {
      error(yyloc, std::string("Unknown service field: ") + *$1);
      YYERROR;
    }
}
| IDENTIFIER COLON INTEGERLIT 
{};

/*** Codel Declaration ***/

codel_prototype:
  IDENTIFIER
{};

/*** Type Declaration ***/

type_decl:
TYPEDEF type_spec declarators
{ 
    $$ = new TypedefType($2, $3);
}
| struct_type              { $$ = $1; }
/*| union_type               { $$ = $1; } */
| enum_type                { $$ = $1; }
/*  | NATIVE IDENTIFIER {
      $$ = new Native(currentFile, yylineno, mainFile, $2);
    }*/
/*  | constr_forward_decl      { $$ = $1; }*/
    ;

/*type_declarator:
  type_spec declarators 
{
    $$ = $2;
};*/

type_spec:
  simple_type_spec 
{
    $$ = $1;
}
| constr_type_spec 
{ 
    $$ = $1;
};

simple_type_spec:
  base_type_spec     { $$ = $1; }
| template_type_spec { $$ = $1; }
| IDENTIFIER
{
    $$ = driver.component().typeFromName(*$1);
};

base_type_spec:
   floating_pt_type { $$ = $1; }
 | integer_type     { $$ = $1; }
 | char_type        { $$ = $1; }
 | wide_char_type   { $$ = $1; }
 | boolean_type     { $$ = $1; }
 | octet_type       { $$ = $1; }
 | any_type         { $$ = $1; }
/*  | object_type      { $$ = $1; } */
/* | value_base_type  { $$ = $1; }*/
;

template_type_spec:
   sequence_type    { $$ = $1; }
 | string_type      { $$ = $1; }
 | wide_string_type { $$ = $1; }
 | fixed_pt_type    { $$ = $1; }
;

constr_type_spec:
   struct_type { $$ = $1; }
/* | union_type  { $$ = $1; } */
 | enum_type   { $$ = $1; }
;

declarators:
  declarator 
{ 
    Declarator::Vect *v = new Declarator::Vect();
    v->push_back($1);
    $$ = v;
}
| declarators COMMA declarator 
{
      if($1) {
	  $1->push_back($3);
	  $$ = $1; 
      } else {
	  error(yyloc, "Problem with declarator map");
	  YYERROR;
      }  
};

declarator:
  simple_declarator  { $$ = $1; }
| array_declarator { $$ = $1; }
;

simple_declarator:
  IDENTIFIER 
{
    $$ = new Declarator(*$1);
};

array_declarator:
  simple_declarator fixed_array_size
{
    $1->addBound($2);
    $$ = $1;
}
| array_declarator fixed_array_size
{
    $1->addBound($2);
    $$ = $1;
};

fixed_array_size:
  '[' INTEGERLIT ']'
{
    $$ = $2;
};


/* Predefined types*/

floating_pt_type:
  FLOAT       { $$ = BaseType::floatType; }
| DOUBLE      { $$ = BaseType::doubleType; }
| LONG DOUBLE { $$ = BaseType::longdoubleType; }
;

fixed_pt_type:
  FIXED LESS_THAN INTEGERLIT COMMA INTEGERLIT GREATER_THAN
{
    $$ = new FixedType($3, $5);
}
| FIXED
{};

integer_type:
  signed_int   { $$ = $1; }
| unsigned_int { $$ = $1; }
;

signed_int:
  signed_short_int     { $$ = $1; }
| signed_long_int      { $$ = $1; }
| signed_long_long_int { $$ = $1; }
;

signed_short_int:
  SHORT { $$ = BaseType::shortType; }
;

signed_long_int:
  LONG { $$ = BaseType::longType; }
;

signed_long_long_int:
  LONG LONG { $$ = BaseType::longlongType; }
;

unsigned_int:
  unsigned_short_int     { $$ = $1; }
| unsigned_long_int      { $$ = $1; }
| unsigned_long_long_int { $$ = $1; }
;

unsigned_short_int:
  UNSIGNED SHORT { $$ = BaseType::ushortType; }
;

unsigned_long_int:
  UNSIGNED LONG { $$ = BaseType::ulongType; }
;

unsigned_long_long_int:
  UNSIGNED LONG LONG { $$ = BaseType::ulonglongType; }
;

char_type:
  CHAR { $$ = BaseType::charType; }
;

wide_char_type:
  WCHAR { $$ = BaseType::wcharType; }
;

boolean_type:
  BOOLEAN { $$ = BaseType::booleanType; }
;

octet_type:
  OCTET { $$ = BaseType::octetType; }
;

any_type:
  ANY { $$ = BaseType::anyType; }
;

/*object_type:
  OBJECT { $$ = DeclaredType::corbaObjectType; }
;*/

string_type:
  STRING LESS_THAN INTEGERLIT GREATER_THAN 
{
    $$ = new StringType($3); 
}
| STRING 
{
    $$ = StringType::unboundedStringType;
};

wide_string_type:
  WSTRING LESS_THAN INTEGERLIT GREATER_THAN 
{ 
    $$ = new WStringType($3); 
}
| WSTRING 
{
    $$ = WStringType::unboundedWStringType;
};

/* Complex types*/

/* Struct */

struct_type:
  STRUCT IDENTIFIER LBRACE members RBRACE 
{
    StructType *s = static_cast<StructType*>(driver.currentType());
    if(!s) {
	error(yyloc, "Empty struct ??");
	YYERROR;
    }
    s->setIdentifier(*$2);
    driver.setCurrentType(0);
    $$ = s;
};

members:
  member 
{}
| members member 
{};

member:
  type_spec declarators SEMICOLON 
{
    StructType *s = dynamic_cast<StructType*>(driver.currentType());
    if(!s) {
	s = new StructType();
	driver.setCurrentType(s);
    }
    s->addMember($1, $2);
};

/* Union */
/*
union_type:
  union_header SWITCH
     LPAREN switch_type_spec RPAREN
     LBRACE switch_body RBRACE 
{
    $1->finishConstruction($6->type(), $6->constr(), $12);
    delete $6;
    $$ = $1;
};

union_header:
 UNION IDENTIFIER 
{
    $$ = new Union($2);
};

switch_type_spec:
  integer_type { $$ = $1; }
| char_type    { $$ = $1; }
| boolean_type { $$ = $1; }
| enum_type    { $$ = $1->thisType(); }
/*| scoped_name 
{
    $$ = new TypeSpec(IdlType::scopedNameToType($1), false);
}
;

switch_body:
  case_plus { $$ = $1; }
;

cases:
  case { $$ = $1; }
| cases case 
{
    $1->append($2);
    $$ = $1;
};

case:
  case_labels element_spec SEMICOLON 
{
    $2->finishConstruction($1);
    $$ = $2;
};

case_labels:
  case_label { $$ = $1; }
| case_labels case_label 
{
    $1->append($2);
    $$ = $1;
};

case_label:
  CASE const_exp COLON
{
    $$ = new CaseLabel($2);
}
| DEFAULT COLON 
{
    $$ = new CaseLabel(0);
};

element_spec:
  type_spec declarator 
{
    $$ = new UnionCase($1->type(), $1->constr(), $2);
};

/* Enum */

enum_type:
    ENUM IDENTIFIER LBRACE enumerators RBRACE 
{
    EnumType *s = dynamic_cast<EnumType*>(driver.currentType());
    if(!s) {
	error(yyloc, "Empty enum ??");
	YYERROR;
    }
    s->setIdentifier(*$2);
    driver.setCurrentType(0);
    $$ = s;};

enumerators:
  enumerator {}
| enumerators COMMA enumerator 
{};

enumerator:
  IDENTIFIER 
{
    EnumType *s = dynamic_cast<EnumType*>(driver.currentType());
    if(!s) {
	s = new EnumType();
	driver.setCurrentType(s);
    }
    s ->addEnumerator(*$1);
};

/* Sequence */

sequence_type:
  SEQUENCE LESS_THAN simple_type_spec COMMA INTEGERLIT GREATER_THAN 
{
    $$ = new SequenceType($3, $5);
}
| SEQUENCE LESS_THAN simple_type_spec GREATER_THAN
{
    $$ = new SequenceType($3, 0);
};  

%% /*** Additional Code ***/

void G3nom::Parser::error(const Parser::location_type& l,
			    const std::string& m)
{
    driver.error(l, m);
}


