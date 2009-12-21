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
%{
#include "acgenom.h"

#define YYLTYPE	dotgenloc

#define YYLLOC_DEFAULT(c, rhs, n)		\
  do {						\
    (c).file = YYRHSLOC(rhs, (n)?1:0).file;	\
    (c).line = YYRHSLOC(rhs, (n)?1:0).line;	\
    (c).col = YYRHSLOC(rhs, (n)?1:0).col;	\
  } while (0)

%}

%name-prefix "dotgen"
%define api.pure
%locations

%code provides {
  typedef struct dotgenloc {
    char *file;
    int line, col;
  } dotgenloc;

  extern YYLTYPE curloc;

  extern int dotgenlex(YYSTYPE *lvalp, YYLTYPE *llocp);
  extern void dotgenerror(const char *msg);
}

%union {
  int	i;
  char *s;
}

%token <i>	PRAGMA
%token <i>	INTEGER_LIT
%token <s>	STRING_LIT IDENTIFIER

%type <i>	spec statement cpphash

%start spec

%%

spec: statement | spec statement;

statement:
  cpphash
  | error
  {
    YYABORT;
  }
;


/* --- # directives from cpp ----------------------------------------------- */

cpphash:
  '#' INTEGER_LIT STRING_LIT '\n'
  {
    curloc.file = $3;
    curloc.line = $2;
    curloc.col = 1;
  }
  | '#' INTEGER_LIT STRING_LIT INTEGER_LIT '\n'
  {
    curloc.file = $3;
    curloc.line = $2;
    curloc.col = 1;
  }
  | '#' PRAGMA IDENTIFIER error '\n'
  {
    /* idlerParseError(spec, &$<noval>1.p, IDLER_E_WARNING, */
    /* 		    "ignoring pragma: %s", $3.v); */
    /* idlerObjectUnref(spec, $3.v); */
    yyerrok;
  }
  | '#' error '\n'
  {
    /* idlerParseError(spec, &$<noval>1.p, IDLER_E_WARNING, */
    /* 		    "unknown # directive"); */
    yyerrok;
  }
;

%%

 /* --- dotgenerror -------------------------------------------------------- */

void
dotgenerror(const char *msg)
{
  ;
}
