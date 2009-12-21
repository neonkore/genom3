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

#include "genom.h"

#define YYLTYPE	dotgenloc

#include "genom3-dotgen.h"

   YYLTYPE curloc = { .file = NULL, .line = 1, .col = 1 };

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
<INITIAL>"\n"	{ curloc.line++; curloc.col = 1; }
[\t\f\v\r ]+	{ *yylloc_param = curloc;  }

 /* C preprocessor output */
"#"		{ BEGIN(HASH); return '#'; }
<HASH>"\n"	{ BEGIN(INITIAL); curloc.line++; curloc.col = 1; return '\n'; }
<HASH>"pragma"	{ return PRAGMA; }

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

%%
