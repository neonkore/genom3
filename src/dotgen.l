/*
 * Copyright (c) 2009 LAAS/CNRS
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

%option prefix="dotgen"
%option outfile="lex.yy.c"
%option noyywrap nounput
%option bison-locations

%{
#include "acgenom.h"

#include <string.h>
#include <err.h>
#include <sys/wait.h>

#include "genom.h"

#define YY_INPUT(buf, result, max)					\
   do {									\
     extern int dotgenfd;						\
     result = read(dotgenfd, buf, max);					\
     if (result < 0 && errno != EAGAIN) {				\
       warnx("input failed");						\
       result = 0;							\
     }									\
   } while(result < 0)

#define YYLTYPE	tloc

#include "genom3-dotgen.h"

   YYLTYPE curloc = { .file = NULL, .line = 1, .col = 0 };

#define YY_USER_ACTION  curloc.col += yyleng;
%}

intsuffix	([uU][lL]?)|([lL][uU]?)
fixedsuffix	([dD])
fracconst	([0-9]*\.[0-9]+)|([0-9]+\.)
exppart		[eE][-+]?[0-9]+
floatsuffix	[fFlL]
stringtext	([^\"])|(\\.)
chartext	([^\'])|(\\.)

%s HASH

%%

%{
  *yylloc_param = curloc;
%}

 /* ignore whitespace, as in any decent language. */
<INITIAL>"\n"	{ curloc.line++; curloc.col = 0; *yylloc_param = curloc; }
[\t\f\v\r ]+	{ *yylloc_param = curloc;  }

 /* special characters */
[\{\}\[\]\(\)\;\:\?\.\+\-\*\/\%\^\&\|\~\!\=\<\>\,] { return yytext[0]; }
"<%"		{ return '{'; }
"%>"		{ return '}'; }
"<:"		{ return '['; }
":>"		{ return ']'; }
"::"		{ return COLONCOLON; }
"xor"		{ return '^'; }
"bitand"	{ return '&'; }
"bitor"		{ return '|'; }
"compl"		{ return '~'; }
"not"		{ return '!'; }
"<<"		{ return SL; }
">>"		{ return SR; }

 /* C preprocessor output */
"#"		{ BEGIN(HASH); return '#'; }
<HASH>"\n"	{ BEGIN(INITIAL); curloc.line++; curloc.col = 0; return '\n'; }
<HASH>"pragma"	{ return PRAGMA; }

 /* IDL related keywords */
"module"	{ return MODULE; }

"unsigned"	{ return UNSIGNED; }
"short"		{ return SHORT; }
"long"		{ return LONG; }
"fixed"		{ return FIXED; }
"float"		{ return FLOAT; }
"double"	{ return DOUBLE; }
"char"		{ return CHAR; }
"wchar"		{ return WCHAR; }
"string"	{ return STRING; }
"wstring"	{ return WSTRING; }
"boolean"	{ return BOOLEAN; }
"octet"		{ return OCTET; }
"object"	{ return OBJECT; }
"any"		{ return ANY; }
"void"		{ return VOID; }
"property"	{ return PROPERTY; }

"const"		{ return CONST; }
"native"	{ return NATIVE; }
"enum"		{ return ENUM; }
"union"		{ return UNION; }
"switch"	{ return SWITCH; }
"case"		{ return CASE; }
"default"	{ return DEFAULT; }
"struct"	{ return STRUCT; }
"sequence"	{ return SEQUENCE; }
"typedef"	{ return TYPEDEF; }

 /* GenoM keywords */
"component"	{ return COMPONENT; }
"task"		{ return TASK; }
"service"	{ return SERVICE; }
"codel"		{ return CODEL; }
"inport"	{ return INPORT; }
"outport"	{ return OUTPORT; }
"in"		{ return IN; }
"out"		{ return OUT; }
"inout"		{ return INOUT; }
"ids"		{ return IDS; }
"input"		{ return INPUT; }
"output"	{ return OUTPUT; }
"event"		{ return EVENT; }
"import"	{ return IMPORT; }
"from"		{ return FROM; }

 /* integers literals */
"0"[xX][0-9a-fA-F]+{intsuffix}? {
  char *end;
  yylval->i = strtol(yytext, &end, 0);
  return INTEGER_LIT;
}
"0"[0-7]+{intsuffix}? {
  char *end;
  yylval->i = strtol(yytext, &end, 0);
  return INTEGER_LIT;
}
[0-9]+{intsuffix}? {
  char *end;
  yylval->i = strtol(yytext, &end, 0);
  return INTEGER_LIT;
}

 /* floating point literals */
{fracconst}{exppart}?{floatsuffix}? {
  char *end;
  yylval->d = strtod(yytext, &end);
  return FLOAT_LIT;
}
[0-9]+{exppart}{floatsuffix}? {
  char *end;
  yylval->d = strtod(yytext, &end);
  return FLOAT_LIT;
}

 /* char literals */
"'"{chartext}*"'" {
  /* XXX handle escape sequences */
  yylval->c = yytext[1];
  return CHAR_LIT;
}

 /* string literals */
"\""{stringtext}*"\"" {
  /* remove quotes */
  yytext[yyleng-1] = '\0';
  yylval->s = string(yytext + 1);
  return STRING_LIT;
}

 /* identifiers */
[A-Za-z_][A-Za-z0-9_]* {
  yylval->s = string(yytext);
  return IDENTIFIER;
}

 /* errors */
. {
  parserror(curloc, "unexpected character `%c', column %d",
	    yytext[0], curloc.col);
  *yylloc_param = curloc;
}

%%
