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
    class Port*			portVal;
    class Service*		serviceVal;
    class Task*			taskVal;
}

%token			END	     	"end of file"
%token			COMPONENT	"component"
%token			TASK		"task"
%token			SERVICE		"service"
%token			CODEL		"codel"

%token LBRACE RBRACE SEMICOLON COLON

%token IN OUT INPORT OUTPORT
// type tokens
%token SHORT LONG FLOAT DOUBLE FIXED CHAR WCHAR STRING WSTRING BOOLEAN OCTET ANY VOID NATIVE
%token ENUM UNION SWITCH CASE DEFAULT STRUCT SEQUENCE

%token <charVal>	SPECIAL_CHAR	"char"
%token <integerVal> 	INTEGERLIT	"integer"
%token <doubleVal> 	DOUBLELIT	"double"
%token <stringVal> 	STRINGLIT	"string literal"
%token <stringVal> 	IDENTIFIER	"identifier"

%type <portVal>		port_decl
%type <taskVal>		task_decl
%type <serviceVal>	service_decl

%destructor { delete $$.stringVal; } STRING
%destructor { delete $$; } port_decl service_decl task_decl

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
    declaration ';'
  | start declaration ';'
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
};

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
    }
}
| IDENTIFIER COLON INTEGERLIT
{}
| IDENTIFIER COLON IDENTIFIER
{
    if(*$1 == "ids") {
	driver.component().IDSStructName = *$3;
    }
};

/*** Inport or outport declaration ***/

port_decl:
  INPORT IDENTIFIER IDENTIFIER
{
    IDLType::Ptr type = driver.typeFromName(*$2);
    $$ = new Port(*$3, type.get(), true);
}
| OUTPORT IDENTIFIER IDENTIFIER
{
    IDLType::Ptr type = driver.typeFromName(*$2);
    $$ = new Port(*$3, type.get(), false);
};

/*** Task declaration ***/

task_fields:
  task_field SEMICOLON
  | task_fields task_field SEMICOLON
{}


task_decl:
  TASK IDENTIFIER LBRACE task_fields RBRACE
{
    Task *t = driver.currentTask();
    t->name = *$2;
    driver.setCurrentTask(new Task());
    $$ = t;
};

task_field:
  CODEL IDENTIFIER COLON codel_prototype
{}
| IDENTIFIER COLON INTEGERLIT 
{
    Task *t = driver.currentTask();
    if(!t) {
      t = new Task();
      driver.setCurrentTask(t);
    }

    if(*$1 == "priority")
      t->priority = $3;
    else if(*$1 == "period")
      t->period = $3;
    else if(*$1 == "staskSize")
      t->stackSize = $3;
};

/*** Service Declaration ***/

service_decl:
    SERVICE IDENTIFIER LBRACE RBRACE
{};

/*** Codel Declaration ***/

codel_prototype:
  IDENTIFIER
{};

/*** Type Declaration ***/

type_decl:
  IDENTIFIER
{};

%% /*** Additional Code ***/

void G3nom::Parser::error(const Parser::location_type& l,
			    const std::string& m)
{
    driver.error(l, m);
}


