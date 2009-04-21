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
// #include <boost/variant.hpp>

#include "ast.h"
#include "idltype.h"

using namespace G3nom;
using namespace Idl;
%}

/* this code will be placed in parser.hpp before the definition of the parser class*/
%code requires {
#include "ast.h"
#include "idltype.h"
/*
typedef boost::variant<
  char, int, double, std::string, Idl::IdlType::Ptr,
    Idl::Declarator::Vect::Ptr, Idl::Declarator::Ptr> variant_type;*/

struct variant_type {
    char 			charVal;
    int  			integerVal;
    double 			doubleVal;
    std::string			stringVal;
    G3nom::Codel::Ptr		codelVal;
    G3nom::Idl::IdlType::Ptr		typeVal;
    G3nom::Idl::Declarator::VectPtr	declaratorVectVal;
    G3nom::Idl::Declarator::Ptr		declaratorVal;
};
#define YYSTYPE variant_type

}
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

/*%union {
    char 			charVal;
    int  			integerVal;
    double 			doubleVal;
    std::string*		stringVal;
    Port::Ptr			portVal;
    Service::Ptr		serviceVal;
    Task::Ptr			taskVal; 
    Idl::IdlType*		typeVal;
    Idl::Declarator::Vect*	declaratorVectVal;
    Idl::Declarator*		declaratorVal;
}*/

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

/*token SPECIAL_CHAR	"char"
%token INTEGERLIT	"integer"
%token DOUBLELIT	"double"
%token STRINGLIT	"string literal"
%token IDENTIFIER	"identifier"*/
%token <charVal>	SPECIAL_CHAR	"char"
%token <integerVal> 	INTEGERLIT	"integer"
%token <doubleVal> 	DOUBLELIT	"double"
%token <stringVal> 	STRINGLIT	"string literal"
%token <stringVal> 	IDENTIFIER	"identifier"

/* %type <portVal>		port_decl */
/* %type <taskVal>		task_decl */
/* %type <serviceVal>	service_decl */
%type <codelVal>		codel_prototype;

%type <typeVal>			type_decl
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
%type <typeVal> 		struct_type
%type <typeVal> 		enum_type
%type <typeVal> 		sequence_type
%type <typeVal> 		string_type
%type <typeVal> 		wide_string_type

/*%type <union_val> 		union_type
%type <union_val> 		union_header
%type <type_spec_val> 		switch_type_spec
%type <union_case_val> 		switch_body
%type <union_case_val> 		case_plus
%type <union_case_val> 		case
%type <case_label_val> 		case_labels
%type <case_label_val> 		case_label
%type <union_case_val> 		element_spec*/


/* %destructor { delete $$; } STRINGLIT */
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
{}
| component_decl
{}
| task_decl
{}
| service_decl 
{}
| type_decl
{
    driver.component().addType($1);
}
;

/*** Component information ***/

component_decl:
   COMPONENT IDENTIFIER LBRACE component_fields RBRACE
{
    driver.component().setName($2);
};

component_fields:
  component_field SEMICOLON
  | component_fields component_field SEMICOLON
{}


component_field:
   IDENTIFIER COLON STRINGLIT
{
    if($1 == "language") {
	driver.component().pluginLanguage = $3;
    } else if($1 == "version") {
	driver.component().version = $3;
    } else {
      error(yyloc, std::string("Unknown component field: ") + $1);
      YYERROR;
    }
}
| IDENTIFIER COLON INTEGERLIT
{}
| IDENTIFIER COLON IDENTIFIER
{
    if($1 == "ids") {
	Idl::IdlType::Ptr p(driver.component().typeFromName($3));
	driver.component().IDSType = p;
    } else {
      error(yyloc, std::string("Unknown component field: ") + $1);
      YYERROR;
    }
};

/*** Inport or outport declaration ***/

port_decl:
  INPORT IDENTIFIER IDENTIFIER
{
    Idl::IdlType::Ptr type = driver.component().typeFromName($2);
    Port::Ptr p(new Port($3, type, true));
    driver.component().addPort(p);
}
| OUTPORT IDENTIFIER IDENTIFIER
{
    Idl::IdlType::Ptr type = driver.component().typeFromName($2);
    Port::Ptr p(new Port($3, type, false));
    driver.component().addPort(p);
};

/*** Task declaration ***/

task_decl:
  TASK IDENTIFIER 
{
    Task::Ptr t(new Task($2));
    driver.setCurrentTask(t);
}
LBRACE task_fields RBRACE
{
    driver.component().addTask(driver.currentTask());
};

task_fields:
  task_field SEMICOLON
  | task_fields task_field SEMICOLON
{}

task_field:
  CODEL IDENTIFIER COLON codel_prototype
{
    driver.currentTask()->addCodel($2, $4);
}
| IDENTIFIER COLON INTEGERLIT 
{
    Task::Ptr t = driver.currentTask();

    if($1 == "priority")
      t->priority = $3;
    else if($1 == "period")
      t->period = $3;
    else if($1 == "stackSize")
      t->stackSize = $3;
    else {
      error(yyloc, std::string("Unknown task field: ") + $1);
      YYERROR;
    }
};

/*** Service Declaration ***/

service_decl:
    SERVICE IDENTIFIER
{
    Service::Ptr s(new Service($2));
    driver.setCurrentService(s);
}
 LBRACE service_fields RBRACE
{
    driver.component().addService(driver.currentService());
};

service_fields:
  service_field SEMICOLON
  | service_fields service_field SEMICOLON
{}

service_field:
  CODEL IDENTIFIER COLON codel_prototype
{
    driver.currentService()->addCodel($2, $4);
}
| IDENTIFIER COLON IDENTIFIER
{
    Service::Ptr s = driver.currentService();

    if($1 == "type") {
	if($3 == "init")
	  s->type = Service::Init;
	else if($3 == "control")
	  s->type = Service::Control;
        else if($3 == "exec")
	  s->type = Service::Exec;
	else {
	  error(yyloc, std::string("Unknown service type: ") + $3);
	  YYERROR;
	}
    } else if($1 == "taskName") {
	s->taskName = $3;
    } else if($1 == "input") {
	s->addInput($3);
    } else if($1 == "output") {
	s->output = $3;
    } else {
      error(yyloc, std::string("Unknown service field: ") + $1);
      YYERROR;
    }
}
| IDENTIFIER COLON STRINGLIT 
{
    Service::Ptr s = driver.currentService();
    if($1 == "doc") {
      s->doc = $3;
    } else {
      error(yyloc, std::string("Unknown service field: ") + $1);
      YYERROR;
    }
}
| IDENTIFIER COLON INTEGERLIT 
{};

/*** Codel Declaration ***/

codel_prototype:
  IDENTIFIER 
{
    Codel::Ptr c(new Codel($1));
    driver.setCurrentCodel(c);
}
  LPAREN codel_fields RPAREN
{
    $$ = driver.currentCodel();
};

codel_fields:
  codel_field
{}
| codel_fields COMMA codel_field
{};

codel_field:
  IN IDENTIFIER
{
    driver.currentCodel()->addInType($2);
}
| OUT IDENTIFIER
{
    driver.currentCodel()->addOutType($2);
}
| INPORT IDENTIFIER
{
    driver.currentCodel()->addInPort($2);
}
| OUTPORT IDENTIFIER
{
    driver.currentCodel()->addOutPort($2);
};

/*** Type Declaration ***/

type_decl:
TYPEDEF type_spec declarators
{
    IdlType::Ptr p(new TypedefType($2, $3));
    $$ = p;
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
    $$ = driver.component().typeFromName($1);
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
    Declarator::VectPtr v(new Declarator::Vect());
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
    Declarator::Ptr p(new Declarator($1));
    $$ = p;
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
    IdlType::Ptr p(new FixedType($3, $5));
    $$ = p;
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
    IdlType::Ptr p(new StringType($3));
    $$ = p; 
}
| STRING 
{
    $$ = StringType::unboundedStringType;
};

wide_string_type:
  WSTRING LESS_THAN INTEGERLIT GREATER_THAN 
{ 
    IdlType::Ptr p(new WStringType($3));
    $$ = p; 
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
    IdlType::Ptr p = driver.currentType();
    StructType *s = static_cast<StructType*>(p.get());
    if(!s) {
	error(yyloc, "Empty struct ??");
	YYERROR;
    }
    s->setIdentifier($2);
    driver.setCurrentType(IdlType::Ptr());
    $$ = p;
};

members:
  member 
{}
| members member 
{};

member:
  type_spec declarators SEMICOLON 
{
    StructType *s = dynamic_cast<StructType*>(driver.currentType().get());
    if(!s) {
	IdlType::Ptr p(new StructType());
	s = dynamic_cast<StructType*>(p.get());
	driver.setCurrentType(p);
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
    IdlType::Ptr p = driver.currentType();
    EnumType *s = dynamic_cast<EnumType*>(p.get());
    if(!s) {
	error(yyloc, "Empty enum ??");
	YYERROR;
    }
    s->setIdentifier($2);
    driver.setCurrentType(IdlType::Ptr());
    $$ = p;
};

enumerators:
  enumerator {}
| enumerators COMMA enumerator 
{};

enumerator:
  IDENTIFIER 
{
    EnumType *s = dynamic_cast<EnumType*>(driver.currentType().get());
    if(!s) {
	IdlType::Ptr e(new EnumType());
	s =  dynamic_cast<EnumType*>(e.get());
	driver.setCurrentType(e);
    }
    s->addEnumerator($1);
};

/* Sequence */

sequence_type:
  SEQUENCE LESS_THAN simple_type_spec COMMA INTEGERLIT GREATER_THAN 
{
    IdlType::Ptr p(new SequenceType($3, $5));
    $$ = p;
}
| SEQUENCE LESS_THAN simple_type_spec GREATER_THAN
{
    IdlType::Ptr p(new SequenceType($3, 0));  
    $$ = p;
};  

%% /*** Additional Code ***/

void G3nom::Parser::error(const Parser::location_type& l,
			    const std::string& m)
{
    driver.error(l, m);
}


