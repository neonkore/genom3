/* 
 * Copyright (c) 2009 LAAS/CNRS         
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

#include "templateinterpreter.h"

using namespace G3nom;
%}

/* this code will be placed in parser.hpp before the definition of the parser class*/
%code requires {
#define YYSTYPE std::string
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
%define "parser_class_name" "TemplateInfoParser"

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
%parse-param { class TemplateInterpreter& driver }

/* verbose error messages */
%error-verbose

 /*** BEGIN EXAMPLE - Change the example grammar's tokens below ***/

%token			ENDOFFILE 0   	"end of file"
%token IDENTIFIER STRINGLIT

%token SEMICOLON COLON EQUAL RARROW

 /*** END EXAMPLE - Change the example grammar's tokens above ***/

%{

#include "templateinterpreter.h"
#include "lexer.h"

/* this "connects" the bison parser in the driver to the flex scanner class
 * object. it defines the yylex() function call to pull the next token from the
 * current lexer object of the driver context. */
#undef yylex
#define yylex driver.lexer()->lex

%}

%% /*** Grammar Rules ***/

start:
  rules ENDOFFILE
{};

rules:
  rule SEMICOLON
{}
| rules rule SEMICOLON
{};

rule:
  IDENTIFIER COLON
{
    if($1 == "service")
      driver.setState(TemplateInterpreter::InsideService);
    else if($1 == "task")
      driver.setState(TemplateInterpreter::InsideTask);
    else if($1 == "main")
      driver.setState(TemplateInterpreter::InsideMain);
    else if($1 == "language") 
      driver.setState(TemplateInterpreter::InsideLanguage);
    else if($1 == "requires") 
      driver.setState(TemplateInterpreter::InsideRequires);
    else {
      error(yyloc, std::string("Unknown field: ") + $1);
      YYERROR;
    }
} fields {}
;

fields:
  IDENTIFIER 
{
    if(driver.state() != TemplateInterpreter::InsideLanguage) {
      error(yyloc, std::string("Expected string instead of identifier "));
      YYERROR;	
    }
    driver.setLanguage($1);
}
| field
{}
| fields field
{};

field:
 STRINGLIT 
{
    if(driver.state() == TemplateInterpreter::InsideRequires)
	driver.executeFile($1);
    else
	driver.interpretFile($1);
}
| STRINGLIT RARROW STRINGLIT 
{
    switch(driver.state()) {
      case TemplateInterpreter::InsideTask: {
	driver.interpretTaskFile($1, $3);
	break;
      }
      case TemplateInterpreter::InsideService: {
	driver.interpretServiceFile($1,$3);
	break;
    }
    default: {
      error(yyloc, "This construction can only be used in 'task' and 'service' sections.");
      YYERROR;
    }
  }
      
};

%% /*** Additional Code ***/

void G3nom::TemplateInfoParser::error(const TemplateInfoParser::location_type& l,
			    const std::string& m)
{
    driver.error(l, m);
}


