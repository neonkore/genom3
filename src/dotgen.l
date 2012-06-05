/*
 * Copyright (c) 2009-2012 LAAS/CNRS
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

#include <assert.h>
#include <stdarg.h>
#include <string.h>
#include <err.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include "genom.h"

#define YYLTYPE	tloc

   static size_t	dotgen_read(char *buffer, size_t max_size);
   static void		dotgen_parsecomment(YYLTYPE *yylloc);
   static void		dotgen_parsehash(char *buffer, YYLTYPE *yylloc);
   static void		dotgen_parsepragmarequire(char *buffer,
				YYLTYPE *yylloc);
   static void		dotgen_parsepragmanative(char *buffer, YYLTYPE *yylloc);

#define YY_INPUT(buf, result, max) ((result) = dotgen_read(buf, max))

#include "genom3-dotgen.h"

   YYLTYPE curloc = { .file = NULL, .line = 1, .col = 0 };

#define YY_USER_ACTION  yylloc->col += yyleng; curloc = *yylloc;
%}

intsuffix	([uU][lL]?)|([lL][uU]?)
fixedsuffix	([dD])
fracconst	([0-9]*\.[0-9]+)|([0-9]+\.)
exppart		[eE][-+]?[0-9]+
floatsuffix	[fFlL]
stringtext	([^\"])|(\\.)
chartext	([^\'])|(\\.)

%%

%{
  *yylloc_param = curloc;
%}

 /*/ @node #line
  * @section @code{#line} directives
  *
  * Line directives are inserted by the @code{C} preprocessor. They inform the
  * @genom{} parser about the line number and file name where each token was
  * defined. Line directives can also be inserted manually to achieve special
  * effect on error reporting or similar.
  *
  * A line directive starts with the @code{#line} token, followed by the
  * current line number, the file name of the source file and an optional
  * numeric flag. The numeric flag is never used by @genom{}. Its meaning
  * depends on the @code{C} preprocessor used.
  *
  * @example
  * @      #line linenum [ "filename" [flag] ]
  * @end example
  *
  * The file name and line number replace the current location information.
  * Line number is automatically incremented after each @code{\n}.
  */
^[ \t]*#[ \t]*[0-9]+[ \t]+\"{stringtext}*\"[ \t]+[0-9]+[ \t]*\n {
  dotgen_parsehash(yytext, yylloc);
}

^[ \t]*#[ \t]*[0-9]+[ \t]+\"{stringtext}*\"[ \t]*\n {
  dotgen_parsehash(yytext, yylloc);
}

^[ \t]*#[ \t]*[0-9]+[ \t]*\n {
  dotgen_parsehash(yytext, yylloc);
}

 /*/ @node #pragma require
  * @section @code{#pragma require} directives
  * @cindex @code{#pragma}
  * @cindex @code{#pragma require}
  * @cindex @code{pkg-config}
  * @cindex @env{PKG_CONFIG}
  * @cindex dependency
  * @cindex require
  * @cindex package, dependency
  *
  * @code{#pragma require} is recognized by @emph{both} the @code{genom-pcpp}
  * preprocessor and @genom{}. This can be used to indicate an external
  * dependency on a package using the @code{pkg-config} utility (see
  * @uref{http://www.freedesktop.org/wiki/Software/pkg-config}).
  *
  * @example
  * @      #pragma require "package [ >= version ]"
  * @end example
  *
  * @code{#pragma require} accepts a string argument in the form @code{package
  * [>= version]}. @code{genom-pcpp} interprets it by running @code{pkg-config
  * --cflags} on the string argument. It then adds the resulting @code{-I} and
  * @code{-D} flags as if they had been passed on the command line
  * (@pxref{General options}). Note that the flags are added @emph{at the
  * current processing location}, so they do not influence already preprocessed
  * input. The @code{pkg-config} utility is found in @env{PATH}, or via the
  * @env{PKG_CONFIG} environment variable if defined (@pxref{Environment
  * variables}).
  *
  * @genom{} also recognizes @code{#pragma require}. It handles it by adding
  * the string argument to the @code{require:} property of @emph{all} the
  * components defined in a specification (@pxref{Component declaration}).
  */
^[ \t]*#[ \t]*pragma[ \t]+require[ \t].*\n {
  dotgen_parsepragmarequire(yytext, yylloc);
  yylloc->line++; yylloc->col = 0;
}

 /* pragmas */
^[ \t]*#[ \t]*pragma[ \t]+native[ \t].*\n {
  dotgen_parsepragmanative(yytext, yylloc);
  yylloc->line++; yylloc->col = 0;
}

^[ \t]*#.*\n {
  /* ignore unknown directives */
  yylloc->line++; yylloc->col = 0;
}

 /* ignore whitespace, as in any decent language. */
"\n"		{ yylloc->line++; yylloc->col = 0; }
[\t\f\v\r ]+	{ }

 /* ignore comments (normally eaten by cpp). */
"/*"		{ dotgen_parsecomment(yylloc); }
[/][/].*\n	{ yylloc->line++; yylloc->col = 0; }

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
"enum"		{ return ENUM; }
"union"		{ return UNION; }
"switch"	{ return SWITCH; }
"case"		{ return CASE; }
"default"	{ return DEFAULT; }
"struct"	{ return STRUCT; }
"sequence"	{ return SEQUENCE; }
"typedef"	{ return TYPEDEF; }

 /* GenoM keywords (can be identifiers as well depending on the context) */
"template"	{ yylval->s = string(yytext); return TEMPLATE; }
"component"	{ yylval->s = string(yytext); return COMPONENT; }
"task"		{ yylval->s = string(yytext); return TASK; }
"service"	{ yylval->s = string(yytext); return SERVICE; }
"codel"		{ yylval->s = string(yytext); return CODEL; }
"inport"	{ yylval->s = string(yytext); return INPORT; }
"outport"	{ yylval->s = string(yytext); return OUTPORT; }
"data"		{ yylval->s = string(yytext); return DATA; }
"handle"	{ yylval->s = string(yytext); return HANDLE; }

"doc"		{ yylval->s = string(yytext); return DOC; }
"ids"		{ yylval->s = string(yytext); return IDS; }
"attribute"	{ yylval->s = string(yytext); return ATTRIBUTE; }
"version"	{ yylval->s = string(yytext); return VERSION; }
"lang"		{ yylval->s = string(yytext); return LANG; }
"email"		{ yylval->s = string(yytext); return EMAIL; }
"require"	{ yylval->s = string(yytext); return REQUIRE; }
"codels-require" { yylval->s = string(yytext); return CODELSREQUIRE; }
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
"async"		{ yylval->s = string(yytext); return ASYNC; }

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
  parserror(*yylloc, "stray `%c', column %d",
	    yytext[0], yylloc->col);
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


/* --- dotgen_parsecomment ------------------------------------------------- */

static void
dotgen_parsecomment(YYLTYPE *yylloc)
{
  int c1, c2;

  c1 = input();
  c2 = input();
  while (c2 != EOF && !(c1 == '*' && c2 == '/')) {
    if (c1 == '\n') {
      yylloc->line++; yylloc->col = 0;
    }
    c1 = c2;
    c2 = input();
  }
  if (c2 == EOF) parserror(*yylloc, "unterminated comment");
}


/* --- dotgen_parsehash ---------------------------------------------------- */

static hash_s hdeps;


/** Parse cpp # line directives
 */
static void
dotgen_parsehash(char *buffer, YYLTYPE *yylloc)
{
  char file[PATH_MAX];
  struct stat s, si;
  int line, flag;
  int n;

  n = sscanf(buffer, "# %d \"%[^\"]\" %d", &line, file, &flag);
  if (n < 1) return;
  if (n < 2) { yylloc->line = line; return; }

  if (!runopt.cppdotgen) {
    if (!stat(file, &s) && !stat(runopt.input, &si))
      if (si.st_dev == s.st_dev && si.st_ino == s.st_ino)
        strncpy(file, runopt.input, sizeof(file));
  }

  yylloc->file = string(file);
  yylloc->line = line;
  yylloc->col = 1;

  /* create dependencies hash */
  if (!hdeps) {
    hdeps = hash_create("input files", 2);
    if (!hdeps) {
      parserror(*yylloc, "dropped dependency %s", yylloc->file);
      return;
    }
  }

  /* insert dependency */
  if (hash_insert(hdeps, yylloc->file, yylloc->file, NULL))
    return;
}

hash_s
dotgen_hdeps()
{
  return hdeps;
}


/* --- dotgen_parsepragmarequire ------------------------------------------- */

static hash_s hrequire;


/** Parse #pragma require directives
 */
static void
dotgen_parsepragmarequire(char *buffer, YYLTYPE *yylloc)
{
  const char *delim;
  char *p, *arg;
  prop_s prop;
  clist_s cl;
  cval cv;

  p = strstr(buffer, "require");
  assert(p);
  p+=7;

  cl = NULL;
  while(*p) {
    p += strspn(p, " \t\n");
    if (*p == 0) break;

    if (*p == 'L' && p[1] == '\"') p++;
    if (*p == '\"') { p++; delim = "\""; } else delim = " \t\n";
    arg = p;
    do {
      p += strspn(p, delim);
      p += strcspn(p, delim);
    } while (p[-1] == '\\');
    if (*p) *p++ = 0;

    xwarnx("creating global require property '%s'", arg);
    cv.k = CST_STRING;
    cv.s = string(arg);
    cl = clist_append(cl, cv, 0);
  }

  /* create a new hash of REQUIRE properties */
  if (!hrequire) {
    hrequire = hash_create("pragma require", 1);
    if (!hrequire) {
      parserror(*yylloc, "dropped #pragma require");
      return;
    }
  }

  prop = prop_newrequire(*yylloc, PROP_REQUIRE, cl);
  if (hash_insert(hrequire, prop_name(prop), prop, (hrelease_f)prop_destroy)) {
    if (prop_merge(hrequire, prop)) {
      parserror(*yylloc, "dropped #pragma require");
      return;
    }
    prop_destroy(prop);
  }
}

hash_s
dotgen_hrequire()
{
  return hrequire;
}


/* --- dotgen_parsepragmanative -------------------------------------------- */

static hash_s hnative;

/** Parse #pragma native directives
 */
static void
dotgen_parsepragmanative(char *buffer, YYLTYPE *yylloc)
{
  const char *delim;
  const char *args[3];
  idltype_s type;
  char *p, *arg;
  hash_s h;
  int n, s;

  p = strstr(buffer, "native");
  assert(p);
  p+=6;

  /* split input into 3 args: template type native */
  n = 0;
  while(*p) {
    p += strspn(p, " \t\n");
    if (*p == 0) break;

    if (*p == 'L' && p[1] == '\"') p++;
    if (*p == '\"') { p++; delim = "\""; } else delim = " \t\n";
    arg = p;
    do {
      p += strspn(p, delim);
      p += strcspn(p, delim);
    } while (p[-1] == '\\');
    if (*p) *p++ = 0;

    args[n++] = string(arg);
    if (n > 3) break;
  }

  if (n != 3) {
    parserror(*yylloc,
              "syntax error for #pragma native <template> <type> <string>");
    return;
  }

  /* resolve type */
  type = type_find(args[1]);
  if (!type) {
    parserror(*yylloc, "unknown type '%s' in #pragma native", args[1]);
    return;
  }

  /* create a new hash */
  if (!hnative) {
    hnative = hash_create("pragma native", 1);
    if (!hnative) {
      parserror(*yylloc, "dropped #pragma native");
      return;
    }
  }

  /* get hash for template */
  h = hash_find(hnative, args[0]);
  if (!h) {
    h = hash_create("pragma native", 1);
    if (!h) {
      parserror(*yylloc, "dropped #pragma native");
      return;
    }
    if (hash_insert(hnative, args[0], h, NULL)) {
      parserror(*yylloc, "dropped #pragma native");
      return;
    }
  }

  /* insert native for type */
  s = hash_insert(h, type_fullname(type), (void *)args[2], NULL);
  switch(s) {
    case 0: break;

    case EEXIST:
      parserror(*yylloc, "duplicate #pragma native for type '%s'",
                type_fullname(type));
      /*FALLTHROUGH*/
    default:
      parserror(*yylloc, "dropped #pragma native");
      return;
  }

  xwarnx("#pragma native for '%s' on '%s'", args[0], type_fullname(type));
}

hash_s
dotgen_hnative()
{
  return hnative;
}
