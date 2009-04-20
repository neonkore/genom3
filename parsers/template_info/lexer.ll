
%{ /*** C/C++ Declarations ***/

#include <string>
// #include <iostream>

#include "lexer.h"

/* import the parser's token type into a local typedef */
typedef G3nom::TemplateInfoParser::token token;
typedef G3nom::TemplateInfoParser::token_type token_type;

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
%option prefix="TemplateInfo"

/* the manual says "somewhat more optimized" */
%option batch

/* enable scanner to generate debug output. disable this for release
 * versions. */
%option debug

/* no support for include files is planned */
%option yywrap nounput 

/* enables the use of start condition stacks */
%option stack

stringtext				([^\"])|(\\.)

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

 /* comments*/
<INITIAL>"/*"              { BEGIN(IN_COMMENT); }
     
<IN_COMMENT>{
     "*/"      BEGIN(INITIAL);
     [^*\n]+   // eat comment in chunks
     "*"       // eat the lone star
     \n        //
}

 /* One char keywords */

";"			{ return token::SEMICOLON; }
":"			{ return token::COLON; }
"="			{ return token::EQUAL; }
"->"			{ return token::RARROW; }

 /* string literals */
"\""{stringtext}*"\"" {
   /* remove quotes */
   yytext[yyleng-1] = '\0';
   *yylval = std::string(yytext + 1);
   return token::STRINGLIT;
}

 /* identifiers */

[A-Za-z][A-Za-z0-9]* {
    *yylval = std::string(yytext, yyleng);
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

TemplateInfoLexer::TemplateInfoLexer(std::istream* in,
		 std::ostream* out)
    : TemplateInfoFlexLexer(in, out)
{
}

TemplateInfoLexer::~TemplateInfoLexer()
{
}

void TemplateInfoLexer::setDebug(bool b)
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

int TemplateInfoFlexLexer::yylex()
{
    std::cerr << "in ExampleFlexLexer::yylex() !" << std::endl;
    return 0;
}

/* When the scanner receives an end-of-file indication from YY_INPUT, it then
 * checks the yywrap() function. If yywrap() returns false (zero), then it is
 * assumed that the function has gone ahead and set up `yyin' to point to
 * another input file, and scanning continues. If it returns true (non-zero),
 * then the scanner terminates, returning 0 to its caller. */

int TemplateInfoFlexLexer::yywrap()
{
    return 1;
}
