/* $Id: scanner.ll 44 2008-10-23 09:03:19Z tb $ -*- mode: c++ -*- */
/** \file scanner.ll Define the example Flex lexical scanner */

%{ /*** C/C++ Declarations ***/

#include <string>

#include "lexer.h"

/* import the parser's token type into a local typedef */
typedef G3nom::Parser::token token;
typedef G3nom::Parser::token_type token_type;

/* By default yylex returns int, we use token_type. Unfortunately yyterminate
 * by default returns 0, which is not of token_type. */
#define yyterminate() return token::END

/* This disables inclusion of unistd.h, which is not available under Visual C++
 * on Win32. The C++ scanner uses STL streams instead. */
#define YY_NO_UNISTD_H

%}

/*** Flex Declarations and Options ***/

/* enable c++ scanner class generation */
%option c++

/* change the name of the scanner class. results in "ExampleFlexLexer" */
%option prefix="G3nom"

/* the manual says "somewhat more optimized" */
%option batch

/* enable scanner to generate debug output. disable this for release
 * versions. */
%option debug

/* no support for include files is planned */
%option yywrap nounput 

/* enables the use of start condition stacks */
%option stack

intsuffix				([uU][lL]?)|([lL][uU]?)
fixedsuffix				([dD])
fracconst				([0-9]*\.[0-9]+)|([0-9]+\.)
exppart					[eE][-+]?[0-9]+
floatsuffix				[fFlL]
stringtext				([^\"])|(\\.)

/* The following paragraph suffices to track locations accurately. Each time
 * yylex is invoked, the begin position is moved onto the end position. */
%{
#define YY_USER_ACTION  yylloc->columns(yyleng);
%}

%% /*** Regular Expressions Part ***/

 /* code to place at the beginning of yylex() */
%{
    // reset location
    yylloc->step();
%}

 /*** BEGIN EXAMPLE - Change the example lexer rules below ***/

 /* gobble up white-spaces */
[ \n\t\r]+ {
    yylloc->step();
}

 /* special characters */
[\{\}\[\]\(\)\;\:\?\.\+\-\*\/\%\^\&\|\~\!\=\<\>\,] {
    yylval->charVal = *yytext;
    return token::SPECIAL_CHAR; // use keywords as token types
}

 /* type related keywords */
"short"			{ return token::SHORT; }
"long"			{ return token::LONG; }
"fixed"			{ return token::FIXED; }
"float"			{ return token::FLOAT; }
"double"		{ return token::DOUBLE; }
"char"			{ return token::CHAR; }
"wchar"			{ return token::WCHAR; }
"string"		{ return token::STRING; }
"wstring"		{ return token::WSTRING; }
"boolean"		{ return token::BOOLEAN; }
"octet"			{ return token::OCTET; }
"any"			{ return token::ANY; }
"void"			{ return token::VOID; }

"native"		{ return token::NATIVE; }
"enum"			{ return token::ENUM; }
"union"			{ return token::UNION; }
"switch"		{ return token::SWITCH; }
"case"			{ return token::CASE; }
"default"		{ return token::DEFAULT; }
"struct"		{ return token::STRUCT; }
"sequence"		{ return token::SEQUENCE; }

 /*other keywords  */
"component"		{ return token::COMPONENT; }
"task"			{ return token::TASK; }
"service"		{ return token::SERVICE; }
"codel"			{ return token::CODEL; }
"inport"		{ return token::INPORT; }
"outport"		{ return token::OUTPORT; }
"in"			{ return token::IN; }
"out"			{ return token::OUT; }

 /* ints */
"0"[xX][0-9a-fA-F]+{intsuffix}? { 
   char *end;
   yylval->integerVal = strtol(yytext, &end, 0);
   return token::INTEGERLIT;
}
"0"[0-7]+{intsuffix}? {
   char *end;
   yylval->integerVal = strtol(yytext, &end, 0);
   return token::INTEGERLIT;
}
[0-9]+{intsuffix}? {
   char *end;
   yylval->integerVal = strtol(yytext, &end, 0);
   return token::INTEGERLIT;
}

 /* doubles */

{fracconst}{exppart}?{floatsuffix}? {
   char *end;
   yylval->doubleVal = strtod(yytext, &end);
   return token::DOUBLELIT;
}
[0-9]+{exppart}{floatsuffix}? {
   char *end;
   yylval->doubleVal = strtod(yytext, &end);
   return token::DOUBLELIT;
}

 /* string literals */
"\""{stringtext}*"\"" {
   /* remove quotes */
   yytext[yyleng-1] = '\0';
   yylval->stringVal = new std::string(yytext + 1);
   return token::STRINGLIT;
}

 /* identifiers */

[A-Za-z][A-Za-z0-9_,.-]* {
    yylval->stringVal = new std::string(yytext, yyleng);
    return token::IDENTIFIER;
}

 /* pass all other characters up to bison */
. {
    return static_cast<token_type>(*yytext);
}

 /*** END EXAMPLE - Change the example lexer rules above ***/

%% /*** Additional Code ***/

namespace G3nom {

Lexer::Lexer(std::istream* in,
		 std::ostream* out)
    : G3nomFlexLexer(in, out)
{
}

Lexer::~Lexer()
{
}

void Lexer::setDebug(bool b)
{
    yy_flex_debug = b;
}

}

/* This implementation of ExampleFlexLexer::yylex() is required to fill the
 * vtable of the class ExampleFlexLexer. We define the scanner's main yylex
 * function via YY_DECL to reside in the Scanner class instead. */

#ifdef yylex
#undef yylex
#endif

int G3nomFlexLexer::yylex()
{
    std::cerr << "in ExampleFlexLexer::yylex() !" << std::endl;
    return 0;
}

/* When the scanner receives an end-of-file indication from YY_INPUT, it then
 * checks the yywrap() function. If yywrap() returns false (zero), then it is
 * assumed that the function has gone ahead and set up `yyin' to point to
 * another input file, and scanning continues. If it returns true (non-zero),
 * then the scanner terminates, returning 0 to its caller. */

int G3nomFlexLexer::yywrap()
{
    return 1;
}
