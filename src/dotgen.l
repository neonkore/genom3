/*
 * Copyright (c) 2009-2010 LAAS/CNRS
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
%option noyywrap noinput nounput
%option bison-locations

%{
#include "acgenom.h"

#include <stdarg.h>
#include <string.h>
#include <err.h>
#include <sys/wait.h>

#include "genom.h"

   static size_t	dotgen_read(char *buffer, size_t max_size);

#define YY_INPUT(buf, result, max) ((result) = dotgen_read(buf, max))

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

"FALSE"		{ return FALSE; }
"TRUE"		{ return TRUE; }

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

 /* GenoM keywords (can be identifiers as well depending on the context) */
"component"	{ yylval->s = string(yytext); return COMPONENT; }
"task"		{ yylval->s = string(yytext); return TASK; }
"service"	{ yylval->s = string(yytext); return SERVICE; }
"codel"		{ yylval->s = string(yytext); return CODEL; }
"inport"	{ yylval->s = string(yytext); return INPORT; }
"outport"	{ yylval->s = string(yytext); return OUTPORT; }
"event"		{ yylval->s = string(yytext); return EVENT; }
"data"		{ yylval->s = string(yytext); return DATA; }

"doc"		{ yylval->s = string(yytext); return DOC; }
"ids"		{ yylval->s = string(yytext); return IDS; }
"attribute"	{ yylval->s = string(yytext); return ATTRIBUTE; }
"version"	{ yylval->s = string(yytext); return VERSION; }
"lang"		{ yylval->s = string(yytext); return LANG; }
"email"		{ yylval->s = string(yytext); return EMAIL; }
"require"	{ yylval->s = string(yytext); return REQUIRE; }
"build-require"	{ yylval->s = string(yytext); return BUILDREQUIRE; }
"clock-rate"	{ yylval->s = string(yytext); return CLOCKRATE; }
"period"	{ yylval->s = string(yytext); return PERIOD; }
"delay"		{ yylval->s = string(yytext); return DELAY; }
"priority"	{ yylval->s = string(yytext); return PRIORITY; }
"scheduling"	{ yylval->s = string(yytext); return SCHEDULING; }
"stack"		{ yylval->s = string(yytext); return STACK; }
"validate"	{ yylval->s = string(yytext); return VALIDATE; }
"yield"		{ yylval->s = string(yytext); return YIELD; }
"throw"		{ yylval->s = string(yytext); return THROWS; }
"interrupts"	{ yylval->s = string(yytext); return INTERRUPTS; }
"before"	{ yylval->s = string(yytext); return BEFORE; }
"after"		{ yylval->s = string(yytext); return AFTER; }

"in"		{ yylval->s = string(yytext); return IN; }
"out"		{ yylval->s = string(yytext); return OUT; }
"inout"		{ yylval->s = string(yytext); return INOUT; }

"s"		{ yylval->s = string(yytext); return S; }
"ms"		{ yylval->s = string(yytext); return MS; }
"us"		{ yylval->s = string(yytext); return US; }
"k"		{ yylval->s = string(yytext); return K; }
"M"		{ yylval->s = string(yytext); return M; }
"real-time"	{ yylval->s = string(yytext); return REAL_TIME; }

 /* integers literals */
"0"[xX][0-9a-fA-F]+{intsuffix}? {
  char *end;
  yylval->i = strtol(yytext, &end, 0);
  return integer_literal;
}
"0"[0-7]+{intsuffix}? {
  char *end;
  yylval->i = strtol(yytext, &end, 0);
  return integer_literal;
}
[0-9]+{intsuffix}? {
  char *end;
  yylval->i = strtol(yytext, &end, 0);
  return integer_literal;
}

 /* fixed point literal */
{fracconst}{exppart}?{fixedsuffix} {
  return FIXED_LIT;
}
[0-9]+{exppart}{fixedsuffix} {
  return FIXED_LIT;
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
  return string_literal;
}

 /* identifiers */
[A-Za-z_][A-Za-z0-9_]* {
  yylval->s = string(yytext);
  return IDENTIFIER;
}

 /* errors */
. {
  parserror(curloc, "stray `%c', column %d",
	    yytext[0], curloc.col);
  *yylloc_param = curloc;
}

%%

/** current input source */
static struct {
  dotgen_input_kind k;
  union {
    int fd;
    const char *buffer;
  } src;
} dg_input;


/* --- dotgen_input -------------------------------------------------------- */

/** Set current input source
 */
void
dotgen_input(dotgen_input_kind k, ...)
{
  va_list va;

  dg_input.k = k;
  va_start(va, k);
  switch(k) {
    case DG_INPUT_FILE:
      dg_input.src.fd = va_arg(va, int);
      curloc.file = NULL;
      break;
    case DG_INPUT_BUFFER:
      dg_input.src.buffer = va_arg(va, const char *);
      curloc.file = "<inline string>";
      break;
  }
  va_end(va);

  curloc.line = 1;
  curloc.col = 0;
  YY_FLUSH_BUFFER;
}


/* --- dotgen_read --------------------------------------------------------- */

/** Read max_size bytes into buffer from current input
 */
static size_t
dotgen_read(char *buffer, size_t max_size)
{
  ssize_t r;

  switch(dg_input.k) {
    case DG_INPUT_FILE:
      r = read(dg_input.src.fd, buffer, max_size);
      if (r < 0 && errno != EAGAIN) {
	warnx("input failed");
	r = YY_NULL;
      }
      break;

    case DG_INPUT_BUFFER:
      strncpy(buffer, dg_input.src.buffer, max_size);
      for(r = 0; buffer[r] != '\0' && r < max_size; r++) /* empty body */;
      dg_input.src.buffer += r;
      break;
  }

  return r;
}
