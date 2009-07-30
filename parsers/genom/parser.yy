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

#include "utils/ast.h"
#include "utils/idltype.h"

using namespace G3nom;
using namespace Idl;
%}

/* this code will be placed in parser.hpp before the definition of the parser class*/
%code requires {
#include "utils/ast.h"
#include "utils/idltype.h"
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
    G3nom::Event::Ptr		eventVal;
    G3nom::Idl::IdlType::Ptr		typeVal;
    G3nom::Idl::Declarator::VectPtr	declaratorVectVal;
    G3nom::Idl::Declarator::Ptr		declaratorVal;
    G3nom::Idl::Literal		literalVal;
    G3nom::ServiceInput		serviceInputVal;
    G3nom::InputDoc		inputDocVal;
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

%token LBRACE RBRACE SEMICOLON COLON LESS_THAN GREATER_THAN COMMA LPAREN RPAREN EQUAL DOT LARROW

%token IN OUT INPORT OUTPORT PROPERTY
// type tokens
%token TRUE FALSE
%token SHORT LONG FLOAT DOUBLE FIXED CHAR WCHAR STRING WSTRING BOOLEAN OCTET ANY VOID NATIVE
%token ENUM UNION SWITCH CASE DEFAULT STRUCT SEQUENCE CONST
%token TYPEDEF UNSIGNED OBJECT IDS INPUT OUTPUT EVENT IMPORT FROM

/*token SPECIAL_CHAR	"char"
%token INTEGERLIT	"integer"
%token DOUBLELIT	"double"
%token STRINGLIT	"string literal"
%token IDENTIFIER	"identifier"*/
/* %token <charVal>	SPECIAL_CHAR	"char" */
%token <charVal>	CHARLIT		"char"
%token <integerVal> 	INTEGERLIT	"integer"
%token <doubleVal> 	DOUBLELIT	"double"
%token <stringVal> 	STRINGLIT	"string literal"
%token <stringVal> 	IDENTIFIER	"identifier"

%type <literalVal>		literal;
%type <literalVal>		basic_literal;
%type <literalVal>		literals;
%type <literalVal>		boolean_literal;
%type <literalVal>		composed_literal;

%type <serviceInputVal>		input_type;
%type <inputDocVal>		input_doc;
%type <inputDocVal>		input_doc_list;

%type <stringVal>		identifiers
%type <codelVal>		codel_prototype
%type <eventVal>		event_spec
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
%type <typeVal> 		const_type

/*%type <union_val> 		union_type
%type <union_val> 		union_header
%type <type_spec_val> 		switch_type_spec
%type <union_case_val> 		switch_body
%type <union_case_val> 		case_plus
%type <union_case_val> 		case
%type <case_label_val> 		case_labels
%type <case_label_val> 		case_label
%type <union_case_val> 		element_spec*/

%left PLUS MINUS
%left TIMES SLASH
%nonassoc MOD
%left  AND OR XOR 
%nonassoc LSHIFT RSHIFT
%left NEG POS TILDE

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
| native_type_decl
{}
| const_decl
{}
| event_decl
{}
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
    } else if($1 == "email") {
	driver.component().email = $3;
    } else {
      error(yyloc, std::string("Unknown component field: ") + $1);
      YYERROR;
    }
}
| IDENTIFIER COLON INTEGERLIT
{
    if($1 == "uniqueId") {
	driver.component().uniqueId = $3;
    } else {
      error(yyloc, std::string("Unknown component field: ") + $1);
      YYERROR;
    }
}
| IDENTIFIER COLON identifiers
{
    std::vector<std::string> v;
    driver.split($3, v);

    if(v.size() > 1) {
	if($1 == "requires") {
	  for(std::vector<std::string>::const_iterator it = v.begin(); it != v.end(); ++it)
	      driver.component().addImportedComponent(*it);
	} else {
	  error(yyloc, std::string("Unknown syntax for component field: ") + $1);
	  YYERROR;
	}
    } else {
      if($1 == "ids") {
	  Idl::IdlType::Ptr p = driver.component().typeFromName($3);
	  if(!p) {
	      error(yyloc, std::string("Unknown type: ") + $3);
	      YYERROR;
	  }
	  driver.component().IDSType = p;
      } else if($1 == "requires") {
	  driver.component().addImportedComponent($3);
      } else {
	error(yyloc, std::string("Unknown component field: ") + $1);
	YYERROR;
      }
    }
}
| PROPERTY IDENTIFIER COLON basic_literal
{
    driver.component().addProperty($2, $4);
}
;

/*** Inport or outport declaration ***/

port_decl:
  INPORT type_spec IDENTIFIER
{
    Port::Ptr p(new Port($3, $2, true));
    driver.component().addPort(p);
}
| INPORT type_spec IDENTIFIER 
{
    Port::Ptr p(new Port($3, $2, true));
    driver.setCurrentPort(p);
}
LBRACE inport_fields RBRACE
{
    driver.component().addPort(driver.currentPort());
}
| OUTPORT type_spec IDENTIFIER
{
    Port::Ptr p(new Port($3, $2, false));
    driver.component().addPort(p);
}
| OUTPORT type_spec IDENTIFIER
{
    Port::Ptr p(new Port($3, $2, false));
    driver.setCurrentPort(p);
} 
 LBRACE outport_fields RBRACE
{
    driver.component().addPort(driver.currentPort());
}
;
  
inport_fields:
  port_field SEMICOLON
  | inport_fields port_field SEMICOLON
{};

outport_fields:
  outport_field SEMICOLON
  | outport_fields outport_field SEMICOLON
{};

outport_field:
  port_field
| CODEL IDENTIFIER COLON codel_prototype
{
  if($2 == "size")
    driver.currentPort()->sizeCodel = $4;
  else {
      error(yyloc, std::string("Unknown codel for an outport : ") + $2);
      YYERROR;
  }
};

port_field:
 PROPERTY IDENTIFIER COLON basic_literal
{
    driver.currentPort()->addProperty($2, $4);
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
{};

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
    else if($1 == "delay")
      t->delay = $3;
    else if($1 == "stackSize")
      t->stackSize = $3;
    else {
      error(yyloc, std::string("Unknown task field: ") + $1);
      YYERROR;
    }
} 
| IDENTIFIER COLON identifiers
{
    if($1 != "errors") {
      error(yyloc, std::string("Wrong arguments for field: ") + $1);
      YYERROR;
    }

    std::vector<std::string> ids;
    driver.split($3, ids);
    std::vector<std::string>::const_iterator it = ids.begin();
    for(; it != ids.end(); ++it)
      driver.currentTask()->addErrorMessage(*it);
}
| PROPERTY IDENTIFIER COLON basic_literal
{
    driver.currentTask()->addProperty($2, $4);
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
| EVENT event_spec COLON codel_prototype
{
    driver.currentService()->addCodel($2->identifier(), $4);
    driver.currentService()->addEvent($2, $2->identifier());
}
| INPUT COLON inputs
{
}
| OUTPUT COLON input_type
{
    driver.currentService()->output = $3;
}
| IDENTIFIER COLON identifiers
{
    Service::Ptr s = driver.currentService();
    std::vector<std::string> v;
    driver.split($3, v);

    if(v.size() == 1) {
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
	} else if($1 == "errors") {
	    driver.currentService()->addErrorMessage($3);
	} else if($1 == "interrupts") {
	    driver.currentService()->addIncompatibleService($3);
	} else {
	  error(yyloc, std::string("Unknown service field: ") + $1);
	  YYERROR;
	}
    } else {
	std::vector<std::string>::const_iterator it = v.begin();

	if($1 == "errors") {
	    for(; it != v.end(); ++it)
		driver.currentService()->addErrorMessage(*it);
	} else if ($1 == "interrupts") {
	    for(; it != v.end(); ++it)
		driver.currentService()->addIncompatibleService(*it);
	} else {
	  error(yyloc, std::string("Unknown service field (or wrong number of arguments): ") + $1);
	  YYERROR;
	}
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
| PROPERTY IDENTIFIER COLON basic_literal
{
    driver.currentService()->addProperty($2, $4);
}
;

identifiers:
 IDENTIFIER 
{
   $$ = $1;
}
| identifiers COMMA IDENTIFIER
{
    $$ = $1 + " " + $3;
};

inputs:
  input
{}
| inputs COMMA input
{};

input:
  input_type COLON input_doc 
{
    $1.doc = $3;
    driver.currentService()->addInput($1);
}
| input_type EQUAL literal COLON input_doc 
{
    $1.defaultValue = $3;
    $1.doc = $5;
    driver.currentService()->addInput($1);
}
| input_type COLON input_doc EQUAL literal 
{
    $1.defaultValue = $5;
    $1.doc = $3;
    driver.currentService()->addInput($1);
}
| input_type EQUAL literal
{
    $1.defaultValue = $3;
    driver.currentService()->addInput($1);
}
| input_type 
{
    driver.currentService()->addInput($1);
};

input_doc:
  STRINGLIT
{
  InputDoc d($1);
  $$ = d;
}
| LBRACE input_doc_list RBRACE
{
  $$ = $2;
};

input_doc_list:
  input_doc
{
  InputDoc d;
  d.members.push_back($1);
  $$ = d;
}
| input_doc_list COMMA input_doc
{
  $1.members.push_back($3);
  $$ = $1;
}

input_type:
  IDS DOT IDENTIFIER
{
    // check if the name given is in the ids
    Idl::IdlType::Ptr t = driver.component().typeFromIdsName($3);
    if(!t) {
      error(yyloc, std::string("Input is not in the IDS: ") + $3);
      YYERROR;
    }

    ServiceInput i;
    i.kind = ServiceInput::IDSMember;
    i.identifier = $3;
    $$ = i;
}
| type_spec IDENTIFIER
{
    ServiceInput i;
    i.kind = ServiceInput::Type;
    i.identifier = $2;
    i.type = $1;
    $$ = i;
};

/*** Events declaration and use ***/
event_spec:
  IDENTIFIER
{
    Event::Ptr ev = driver.component().event($1);
    if(ev) { // external event
      Event::Ptr e(new NamedEvent($1, ev));
      $$ = e;
    } else { // service event
      if($1 == "onCalled") 
	$$ = Event::Ptr(new ServiceEvent(std::string(), ServiceEvent::OnCalled));
      else if($1 == "onStart") 
	$$ = Event::Ptr(new ServiceEvent(std::string(), ServiceEvent::OnStart));
      else if($1 == "onEnd") 
	$$ = Event::Ptr(new ServiceEvent(std::string(), ServiceEvent::OnEnd));
      else if($1 == "onInter") 
	$$ = Event::Ptr(new ServiceEvent(std::string(), ServiceEvent::OnInter));
      else {
	  error(yyloc, std::string("Unknown service internal event: ") + $1);
	  YYERROR;
      }
    }
}
| IDENTIFIER DOT IDENTIFIER
{
    //try to find what type of event this is
    Port::Ptr p = driver.component().port($1);
    Service::Ptr s = driver.component().service($1);

    if(p) { // port event
      if($3 == "onUpdate") 
	$$ = Event::Ptr(new PortEvent(p->name, PortEvent::OnUpdate));
      else if($3 == "onRead") 
	$$ = Event::Ptr(new PortEvent(p->name, PortEvent::OnRead));
      else if($3 == "onWrite") 
	$$ = Event::Ptr(new PortEvent(p->name, PortEvent::OnWrite));
      else if($3 == "onInitialize") 
	$$ = Event::Ptr(new PortEvent(p->name, PortEvent::OnInitialize));
      else {
	  error(yyloc, std::string("Unknown port event: ") + $3);
	  YYERROR;
      }
    } else if(s) {
      if($3 == "onCalled") 
	$$ = Event::Ptr(new ServiceEvent(s->name, ServiceEvent::OnCalled));
      else if($3 == "onStart") 
	$$ = Event::Ptr(new ServiceEvent(s->name, ServiceEvent::OnStart));
      else if($3 == "onEnd") 
	$$ = Event::Ptr(new ServiceEvent(s->name, ServiceEvent::OnEnd));
      else if($3 == "onInter") 
	$$ = Event::Ptr(new ServiceEvent(s->name, ServiceEvent::OnInter));
      else {
	  error(yyloc, std::string("Unknown service event: ") + $3);
	  YYERROR;
      }
    } else {
      error(yyloc, std::string("Unknown object: ") + $1);
      YYERROR;
    }
}
| IDENTIFIER DOT IDENTIFIER DOT IDENTIFIER
{
    if($3 != "onCodel") {
	  error(yyloc, std::string("Unknwon port syntax") );
	  YYERROR;
    }

    Service::Ptr s = driver.component().service($1);
    if(!s) {
	  error(yyloc, std::string("Unknwon service : ") + $1 );
	  YYERROR;
    }  

    Event::Ptr e(new ServiceEvent(s->name, $5));
    $$ = e;
}
;

event_decl:
  EVENT event_declarators

event_declarators:
  event_declarator
{}
| event_declarators COMMA event_declarator
{};

event_declarator:
  IDENTIFIER 
{
    Event::Ptr ev(new NamedEvent($1));
    driver.component().addEvent(ev);
}
| IDENTIFIER EQUAL event_spec
{
    Event::Ptr ev(new NamedEvent($1, $3));
    driver.component().addEvent(ev);
}
;

/*** Codel Declaration ***/

codel_prototype:
  IDENTIFIER 
{
    Codel::Ptr c(new Codel($1));
    driver.setCurrentCodel(c);
}
  LPAREN codel_fields RPAREN codel_next
{
    $$ = driver.currentCodel();
};

codel_next:
/*empty*/
{}
| LARROW identifiers
{
    driver.split($2, driver.currentCodel()->nextCodels); 
};

codel_fields:
/*empty prototype */
{}
| codel_field
{}
| codel_fields COMMA codel_field
{};

codel_field:
  IN IDENTIFIER
{
    // check if the name given is in the ids
    Idl::IdlType::Ptr t = driver.component().typeFromIdsName($2);
    if(!t) {
      error(yyloc, std::string("Input is not in the IDS: ") + $2);
      YYERROR;
    }

    driver.currentCodel()->addInType($2);
}
| OUT IDENTIFIER
{
    // check if the name given is in the ids
    Idl::IdlType::Ptr t = driver.component().typeFromIdsName($2);
    if(!t) {
      error(yyloc, std::string("Output is not in the IDS: ") + $2);
      YYERROR;
    }

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

/*** Const value declaration ***/

const_decl:
  CONST const_type IDENTIFIER EQUAL literal
{
    ConstValue c($3, $2, $5);
    driver.component().addConstValue(c);
};

const_type:
 integer_type
| char_type
| wide_char_type
| boolean_type
| floating_pt_type
| string_type
| wide_string_type
| fixed_pt_type
| octet_type;

/*** IDL Literal Declaration ***/

basic_literal:
  INTEGERLIT
{
    $$ = Literal($1);
}
| DOUBLELIT
{
    $$ = Literal($1);
}
| STRINGLIT
{
    $$ = Literal("\"" + $1 + "\"");
}
| CHARLIT
{
    $$ = Literal($1);
}
| boolean_literal
{
    $$ = $1;
}
| IDENTIFIER 
{
    /*enum value*/
    $$ = Literal($1);
}

literal:
  basic_literal
{
    $$= $1;
}
| LBRACE literals RBRACE
{
    $$ = $2;
}
| LPAREN literal RPAREN
{
    $$ = $2;
}
| composed_literal
{};

boolean_literal:
  TRUE
{
    $$ = Literal(true);
}
| FALSE
{
    $$ = Literal(false);
};

composed_literal:
  /* unary operators */
  MINUS literal %prec NEG 
{
    Literal l(Literal::Neg);
    l.addMember($2);
    $$ = l;
}
| PLUS literal %prec POS 
{
    $$ = $2;
}
| TILDE literal %prec TILDE
{
    Literal l(Literal::LNeg);
    l.addMember($2);
    $$ = l;
} 
  /* binary operators */
|  literal PLUS literal
{
    Literal l(Literal::Plus);
    l.addMember($1);
    l.addMember($3);
    $$ = l;
}
| literal MINUS literal
{
    Literal l(Literal::Minus);
    l.addMember($1);
    l.addMember($3);
    $$ = l;
}
| literal TIMES literal
{
    Literal l(Literal::Times);
    l.addMember($1);
    l.addMember($3);
    $$ = l;
}
| literal SLASH literal
{
    Literal l(Literal::Divide);
    l.addMember($1);
    l.addMember($3);
    $$ = l;
}
| literal MOD literal
{
    Literal l(Literal::Mod);
    l.addMember($1);
    l.addMember($3);
    $$ = l;
}
| literal AND literal
{
    Literal l(Literal::And);
    l.addMember($1);
    l.addMember($3);
    $$ = l;
}
| literal OR literal
{
    Literal l(Literal::Or);
    l.addMember($1);
    l.addMember($3);
    $$ = l;
}
| literal XOR literal
{
    Literal l(Literal::Xor);
    l.addMember($1);
    l.addMember($3);
    $$ = l;
}
| literal LSHIFT literal
{
    Literal l(Literal::LShift);
    l.addMember($1);
    l.addMember($3);
    $$ = l;
}
| literal RSHIFT literal
{
    Literal l(Literal::RShift);
    l.addMember($1);
    l.addMember($3);
    $$ = l;
};

literals:
  literal
{
    Literal l(Literal::Struct);
    l.addMember($1);
    $$ = l;
}
| literals COMMA literal
{
    $1.addMember($3);
    $$ = $1;
}

/*** IDL Type Declaration ***/

native_type_decl:
  IMPORT FROM STRINGLIT LBRACE type_decl_list RBRACE
{
    driver.component().addNativeTypeInclude($3);
};

type_decl_list:
  type_decl SEMICOLON
{
    $1->setNative(true);
    driver.component().addType($1);
}
  | type_decl_list type_decl SEMICOLON
{
    $2->setNative(true);
    driver.component().addType($2);
};

type_decl:
TYPEDEF type_spec declarators
{
    IdlType::Ptr p(new TypedefType($2, $3));
    $$ = p;
}
| struct_type              { $$ = $1; }
/*| union_type               { $$ = $1; } */
| enum_type                { $$ = $1; }
;
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
    IdlType::Ptr type = driver.component().typeFromName($1);
    if(!type) {
	error(yyloc, std::string("Unknown type: ") + $1);
	YYERROR;
    }
    $$ = type;
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

