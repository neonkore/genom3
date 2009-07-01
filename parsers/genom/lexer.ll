/* $Id: scanner.ll 44 2008-10-23 09:03:19Z tb $ -*- mode: c++ -*- */
/** \file scanner.ll Define the example Flex lexical scanner */

%{ /*** C/C++ Declarations ***/

#include <string>
// #include <iostream>

#include "lexer.h"

/* import the parser's token type into a local typedef */
typedef G3nom::Parser::token token;
typedef G3nom::Parser::token_type token_type;

/* By default yylex returns int, we use token_type. Unfortunately yyterminate
 * by default returns 0, which is not of token_type. */
#define yyterminate() return token::ENDOFFILE

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
chartext				([^\'])|(\\.)

%s IN_COMMENT

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
\n { 
    yylloc->lines(yyleng); 
    yylloc->step(); 
}

[ \t\r]+ {
    yylloc->step();
}

 /* special characters 
[\{\}\[\]\(\)\;\:\?\.\+\-\*\/\%\^\&\|\~\!\=\<\>\,] {
    yylval->charVal = *yytext;
    return token::SPECIAL_CHAR; // use keywords as token types
}*/

 /* comments*/
<INITIAL>"/*"              { BEGIN(IN_COMMENT); }
     
<IN_COMMENT>{
     "*/"      BEGIN(INITIAL);
     [^*\n]+   // eat comment in chunks
     "*"       // eat the lone star
     \n        //
}

 /* One char keywords */
"->"			{ return token::DOT; }
"{"			{ return token::LBRACE; }
"}"			{ return token::RBRACE; }
";"			{ return token::SEMICOLON; }
":"			{ return token::COLON; }
","			{ return token::COMMA; }
"("			{ return token::LPAREN; }
")"			{ return token::RPAREN; }
"<"			{ return token::LESS_THAN; }
">"			{ return token::GREATER_THAN; }
"="			{ return token::EQUAL; }
"+"			{ return token::PLUS; }
"-"			{ return token::MINUS; }
"*"			{ return token::TIMES; }
"/"			{ return token::SLASH; }
"%"			{ return token::MOD; }
"&"			{ return token::AND; }
"|"			{ return token::OR; }
"^"			{ return token::XOR; }
"<<"			{ return token::LSHIFT; }
">>"			{ return token::RSHIFT; }
"~"			{ return token::TILDE; }

 /* type related keywords */
"unsigned"		{ return token::UNSIGNED; }
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
"object"		{ return token::OBJECT; }
"any"			{ return token::ANY; }
"void"			{ return token::VOID; }

"const"			{ return token::CONST; }
"native"		{ return token::NATIVE; }
"enum"			{ return token::ENUM; }
"union"			{ return token::UNION; }
"switch"		{ return token::SWITCH; }
"case"			{ return token::CASE; }
"default"		{ return token::DEFAULT; }
"struct"		{ return token::STRUCT; }
"sequence"		{ return token::SEQUENCE; }
"typedef"		{ return token::TYPEDEF; }
"TRUE"			{ return token::TRUE; }
"FALSE"			{ return token::FALSE; }

 /*other keywords  */
"component"		{ return token::COMPONENT; }
"task"			{ return token::TASK; }
"service"		{ return token::SERVICE; }
"codel"			{ return token::CODEL; }
"inport"		{ return token::INPORT; }
"outport"		{ return token::OUTPORT; }
"in"			{ return token::IN; }
"out"			{ return token::OUT; }
"IDS"			{ return token::IDS; }
"input"			{ return token::INPUT; }
"output"		{ return token::OUTPUT; }
"event"			{ return token::EVENT; }

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

 /* char literals */
"'"{chartext}*"'" {
   yylval->charVal = yytext[1];
   return token::CHARLIT;
}

 /* string literals */
"\""{stringtext}*"\"" {
   /* remove quotes */
   yytext[yyleng-1] = '\0';
   yylval->stringVal = std::string(yytext + 1);
   return token::STRINGLIT;
}

 /* identifiers */

[A-Za-z][A-Za-z0-9_]* {
    yylval->stringVal = std::string(yytext, yyleng);
/*    std::cout << "read identifier " << *(yylval->stringVal) << std::endl;*/
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
