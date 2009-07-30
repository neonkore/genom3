
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison LALR(1) parsers in C++
   
      Copyright (C) 2002, 2003, 2004, 2005, 2006, 2007, 2008 Free Software
   Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

// Take the name prefix into account.
#define yylex   G3nomlex

/* First part of user declarations.  */

/* Line 311 of lalr1.cc  */
#line 30 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
 /*** C/C++ Declarations ***/

#include <cstdio>
#include <string>
#include <vector>
// #include <boost/variant.hpp>

#include "utils/ast.h"
#include "utils/idltype.h"

using namespace G3nom;
using namespace Idl;

/* Line 311 of lalr1.cc  */
#line 221 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"


#include "driver.h"
#include "lexer.h"

/* this "connects" the bison parser in the driver to the flex scanner class
 * object. it defines the yylex() function call to pull the next token from the
 * current lexer object of the driver context. */
#undef yylex
#define yylex driver.lexer()->lex



/* Line 311 of lalr1.cc  */
#line 72 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.cpp"


#include "parser.hpp"

/* User implementation prologue.  */


/* Line 317 of lalr1.cc  */
#line 81 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.cpp"

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* FIXME: INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#define YYUSE(e) ((void) (e))

/* Enable debugging if requested.  */
#if YYDEBUG

/* A pseudo ostream that takes yydebug_ into account.  */
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)	\
do {							\
  if (yydebug_)						\
    {							\
      *yycdebug_ << Title << ' ';			\
      yy_symbol_print_ ((Type), (Value), (Location));	\
      *yycdebug_ << std::endl;				\
    }							\
} while (false)

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug_)				\
    yy_reduce_print_ (Rule);		\
} while (false)

# define YY_STACK_PRINT()		\
do {					\
  if (yydebug_)				\
    yystack_print_ ();			\
} while (false)

#else /* !YYDEBUG */

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_REDUCE_PRINT(Rule)
# define YY_STACK_PRINT()

#endif /* !YYDEBUG */

#define yyerrok		(yyerrstatus_ = 0)
#define yyclearin	(yychar = yyempty_)

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)


/* Line 380 of lalr1.cc  */
#line 1 "[Bison:b4_percent_define_default]"

namespace G3nom {

/* Line 380 of lalr1.cc  */
#line 150 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.cpp"
#if YYERROR_VERBOSE

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  Parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr = "";
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              /* Fall through.  */
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }

#endif

  /// Build a parser object.
  Parser::Parser (class Driver& driver_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      driver (driver_yyarg)
  {
  }

  Parser::~Parser ()
  {
  }

#if YYDEBUG
  /*--------------------------------.
  | Print this symbol on YYOUTPUT.  |
  `--------------------------------*/

  inline void
  Parser::yy_symbol_value_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yyvaluep);
    switch (yytype)
      {
         default:
	  break;
      }
  }


  void
  Parser::yy_symbol_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    *yycdebug_ << (yytype < yyntokens_ ? "token" : "nterm")
	       << ' ' << yytname_[yytype] << " ("
	       << *yylocationp << ": ";
    yy_symbol_value_print_ (yytype, yyvaluep, yylocationp);
    *yycdebug_ << ')';
  }
#endif

  void
  Parser::yydestruct_ (const char* yymsg,
			   int yytype, semantic_type* yyvaluep, location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yymsg);
    YYUSE (yyvaluep);

    YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

    switch (yytype)
      {
  
	default:
	  break;
      }
  }

  void
  Parser::yypop_ (unsigned int n)
  {
    yystate_stack_.pop (n);
    yysemantic_stack_.pop (n);
    yylocation_stack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  Parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  Parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  Parser::debug_level_type
  Parser::debug_level () const
  {
    return yydebug_;
  }

  void
  Parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif

  int
  Parser::parse ()
  {
    /// Lookahead and lookahead in internal form.
    int yychar = yyempty_;
    int yytoken = 0;

    /* State.  */
    int yyn;
    int yylen = 0;
    int yystate = 0;

    /* Error handling.  */
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// Semantic value of the lookahead.
    semantic_type yylval;
    /// Location of the lookahead.
    location_type yylloc;
    /// The locations where the error started and ended.
    location_type yyerror_range[2];

    /// $$.
    semantic_type yyval;
    /// @$.
    location_type yyloc;

    int yyresult;

    YYCDEBUG << "Starting parse" << std::endl;


    /* User initialization code.  */
    
/* Line 553 of lalr1.cc  */
#line 97 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
{
    // initialize the initial location object
    yylloc.begin.filename = yylloc.end.filename = driver.streamNamePtr();
}

/* Line 553 of lalr1.cc  */
#line 334 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.cpp"

    /* Initialize the stacks.  The initial state will be pushed in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystate_stack_ = state_stack_type (0);
    yysemantic_stack_ = semantic_stack_type (0);
    yylocation_stack_ = location_stack_type (0);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* New state.  */
  yynewstate:
    yystate_stack_.push (yystate);
    YYCDEBUG << "Entering state " << yystate << std::endl;

    /* Accept?  */
    if (yystate == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    /* Backup.  */
  yybackup:

    /* Try to take a decision without lookahead.  */
    yyn = yypact_[yystate];
    if (yyn == yypact_ninf_)
      goto yydefault;

    /* Read a lookahead token.  */
    if (yychar == yyempty_)
      {
	YYCDEBUG << "Reading a token: ";
	yychar = yylex (&yylval, &yylloc);
      }


    /* Convert token to internal form.  */
    if (yychar <= yyeof_)
      {
	yychar = yytoken = yyeof_;
	YYCDEBUG << "Now at end of input." << std::endl;
      }
    else
      {
	yytoken = yytranslate_ (yychar);
	YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
      }

    /* If the proper action on seeing token YYTOKEN is to reduce or to
       detect an error, take that action.  */
    yyn += yytoken;
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yytoken)
      goto yydefault;

    /* Reduce or error.  */
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
	if (yyn == 0 || yyn == yytable_ninf_)
	goto yyerrlab;
	yyn = -yyn;
	goto yyreduce;
      }

    /* Shift the lookahead token.  */
    YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

    /* Discard the token being shifted.  */
    yychar = yyempty_;

    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* Count tokens shifted since error; after three, turn off error
       status.  */
    if (yyerrstatus_)
      --yyerrstatus_;

    yystate = yyn;
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystate];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    /* If YYLEN is nonzero, implement the default value of the action:
       `$$ = $1'.  Otherwise, use the top of the stack.

       Otherwise, the following line sets YYVAL to garbage.
       This behavior is undocumented and Bison
       users should not rely upon it.  */
    if (yylen)
      yyval = yysemantic_stack_[yylen - 1];
    else
      yyval = yysemantic_stack_[0];

    {
      slice<location_type, location_stack_type> slice (yylocation_stack_, yylen);
      YYLLOC_DEFAULT (yyloc, slice, yylen);
    }
    YY_REDUCE_PRINT (yyn);
    switch (yyn)
      {
	  case 4:

/* Line 678 of lalr1.cc  */
#line 242 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 5:

/* Line 678 of lalr1.cc  */
#line 247 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 6:

/* Line 678 of lalr1.cc  */
#line 249 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 7:

/* Line 678 of lalr1.cc  */
#line 251 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 8:

/* Line 678 of lalr1.cc  */
#line 253 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 9:

/* Line 678 of lalr1.cc  */
#line 255 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.component().addType((yysemantic_stack_[(1) - (1)].typeVal));
}
    break;

  case 10:

/* Line 678 of lalr1.cc  */
#line 259 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 11:

/* Line 678 of lalr1.cc  */
#line 261 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 12:

/* Line 678 of lalr1.cc  */
#line 263 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 13:

/* Line 678 of lalr1.cc  */
#line 270 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.component().setName((yysemantic_stack_[(5) - (2)].stringVal));
}
    break;

  case 15:

/* Line 678 of lalr1.cc  */
#line 277 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 16:

/* Line 678 of lalr1.cc  */
#line 282 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    if((yysemantic_stack_[(3) - (1)].stringVal) == "language") {
	driver.component().pluginLanguage = (yysemantic_stack_[(3) - (3)].stringVal);
    } else if((yysemantic_stack_[(3) - (1)].stringVal) == "version") {
	driver.component().version = (yysemantic_stack_[(3) - (3)].stringVal);
    } else if((yysemantic_stack_[(3) - (1)].stringVal) == "email") {
	driver.component().email = (yysemantic_stack_[(3) - (3)].stringVal);
    } else {
      error(yyloc, std::string("Unknown component field: ") + (yysemantic_stack_[(3) - (1)].stringVal));
      YYERROR;
    }
}
    break;

  case 17:

/* Line 678 of lalr1.cc  */
#line 295 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    if((yysemantic_stack_[(3) - (1)].stringVal) == "uniqueId") {
	driver.component().uniqueId = (yysemantic_stack_[(3) - (3)].integerVal);
    } else {
      error(yyloc, std::string("Unknown component field: ") + (yysemantic_stack_[(3) - (1)].stringVal));
      YYERROR;
    }
}
    break;

  case 18:

/* Line 678 of lalr1.cc  */
#line 304 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    std::vector<std::string> v;
    driver.split((yysemantic_stack_[(3) - (3)].stringVal), v);

    if(v.size() > 1) {
	if((yysemantic_stack_[(3) - (1)].stringVal) == "requires") {
	  for(std::vector<std::string>::const_iterator it = v.begin(); it != v.end(); ++it)
	      driver.component().addImportedComponent(*it);
	} else {
	  error(yyloc, std::string("Unknown syntax for component field: ") + (yysemantic_stack_[(3) - (1)].stringVal));
	  YYERROR;
	}
    } else {
      if((yysemantic_stack_[(3) - (1)].stringVal) == "ids") {
	  Idl::IdlType::Ptr p = driver.component().typeFromName((yysemantic_stack_[(3) - (3)].stringVal));
	  if(!p) {
	      error(yyloc, std::string("Unknown type: ") + (yysemantic_stack_[(3) - (3)].stringVal));
	      YYERROR;
	  }
	  driver.component().IDSType = p;
      } else if((yysemantic_stack_[(3) - (1)].stringVal) == "requires") {
	  driver.component().addImportedComponent((yysemantic_stack_[(3) - (3)].stringVal));
      } else {
	error(yyloc, std::string("Unknown component field: ") + (yysemantic_stack_[(3) - (1)].stringVal));
	YYERROR;
      }
    }
}
    break;

  case 19:

/* Line 678 of lalr1.cc  */
#line 333 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.component().addProperty((yysemantic_stack_[(4) - (2)].stringVal), (yysemantic_stack_[(4) - (4)].literalVal));
}
    break;

  case 20:

/* Line 678 of lalr1.cc  */
#line 342 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Port::Ptr p(new Port((yysemantic_stack_[(3) - (3)].stringVal), (yysemantic_stack_[(3) - (2)].typeVal), true));
    driver.component().addPort(p);
}
    break;

  case 21:

/* Line 678 of lalr1.cc  */
#line 347 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Port::Ptr p(new Port((yysemantic_stack_[(3) - (3)].stringVal), (yysemantic_stack_[(3) - (2)].typeVal), true));
    driver.setCurrentPort(p);
}
    break;

  case 22:

/* Line 678 of lalr1.cc  */
#line 352 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.component().addPort(driver.currentPort());
}
    break;

  case 23:

/* Line 678 of lalr1.cc  */
#line 356 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Port::Ptr p(new Port((yysemantic_stack_[(3) - (3)].stringVal), (yysemantic_stack_[(3) - (2)].typeVal), false));
    driver.component().addPort(p);
}
    break;

  case 24:

/* Line 678 of lalr1.cc  */
#line 361 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Port::Ptr p(new Port((yysemantic_stack_[(3) - (3)].stringVal), (yysemantic_stack_[(3) - (2)].typeVal), false));
    driver.setCurrentPort(p);
}
    break;

  case 25:

/* Line 678 of lalr1.cc  */
#line 366 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.component().addPort(driver.currentPort());
}
    break;

  case 27:

/* Line 678 of lalr1.cc  */
#line 374 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 29:

/* Line 678 of lalr1.cc  */
#line 379 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 31:

/* Line 678 of lalr1.cc  */
#line 384 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
  if((yysemantic_stack_[(4) - (2)].stringVal) == "size")
    driver.currentPort()->sizeCodel = (yysemantic_stack_[(4) - (4)].codelVal);
  else {
      error(yyloc, std::string("Unknown codel for an outport : ") + (yysemantic_stack_[(4) - (2)].stringVal));
      YYERROR;
  }
}
    break;

  case 32:

/* Line 678 of lalr1.cc  */
#line 395 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.currentPort()->addProperty((yysemantic_stack_[(4) - (2)].stringVal), (yysemantic_stack_[(4) - (4)].literalVal));
}
    break;

  case 33:

/* Line 678 of lalr1.cc  */
#line 403 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Task::Ptr t(new Task((yysemantic_stack_[(2) - (2)].stringVal)));
    driver.setCurrentTask(t);
}
    break;

  case 34:

/* Line 678 of lalr1.cc  */
#line 408 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.component().addTask(driver.currentTask());
}
    break;

  case 36:

/* Line 678 of lalr1.cc  */
#line 415 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 37:

/* Line 678 of lalr1.cc  */
#line 419 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.currentTask()->addCodel((yysemantic_stack_[(4) - (2)].stringVal), (yysemantic_stack_[(4) - (4)].codelVal));
}
    break;

  case 38:

/* Line 678 of lalr1.cc  */
#line 423 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Task::Ptr t = driver.currentTask();

    if((yysemantic_stack_[(3) - (1)].stringVal) == "priority")
      t->priority = (yysemantic_stack_[(3) - (3)].integerVal);
    else if((yysemantic_stack_[(3) - (1)].stringVal) == "period")
      t->period = (yysemantic_stack_[(3) - (3)].integerVal);
    else if((yysemantic_stack_[(3) - (1)].stringVal) == "delay")
      t->delay = (yysemantic_stack_[(3) - (3)].integerVal);
    else if((yysemantic_stack_[(3) - (1)].stringVal) == "stackSize")
      t->stackSize = (yysemantic_stack_[(3) - (3)].integerVal);
    else {
      error(yyloc, std::string("Unknown task field: ") + (yysemantic_stack_[(3) - (1)].stringVal));
      YYERROR;
    }
}
    break;

  case 39:

/* Line 678 of lalr1.cc  */
#line 440 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    if((yysemantic_stack_[(3) - (1)].stringVal) != "errors") {
      error(yyloc, std::string("Wrong arguments for field: ") + (yysemantic_stack_[(3) - (1)].stringVal));
      YYERROR;
    }

    std::vector<std::string> ids;
    driver.split((yysemantic_stack_[(3) - (3)].stringVal), ids);
    std::vector<std::string>::const_iterator it = ids.begin();
    for(; it != ids.end(); ++it)
      driver.currentTask()->addErrorMessage(*it);
}
    break;

  case 40:

/* Line 678 of lalr1.cc  */
#line 453 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.currentTask()->addProperty((yysemantic_stack_[(4) - (2)].stringVal), (yysemantic_stack_[(4) - (4)].literalVal));
}
    break;

  case 41:

/* Line 678 of lalr1.cc  */
#line 461 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Service::Ptr s(new Service((yysemantic_stack_[(2) - (2)].stringVal)));
    driver.setCurrentService(s);
}
    break;

  case 42:

/* Line 678 of lalr1.cc  */
#line 466 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.component().addService(driver.currentService());
}
    break;

  case 44:

/* Line 678 of lalr1.cc  */
#line 473 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 45:

/* Line 678 of lalr1.cc  */
#line 477 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.currentService()->addCodel((yysemantic_stack_[(4) - (2)].stringVal), (yysemantic_stack_[(4) - (4)].codelVal));
}
    break;

  case 46:

/* Line 678 of lalr1.cc  */
#line 481 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.currentService()->addCodel((yysemantic_stack_[(4) - (2)].eventVal)->identifier(), (yysemantic_stack_[(4) - (4)].codelVal));
    driver.currentService()->addEvent((yysemantic_stack_[(4) - (2)].eventVal), (yysemantic_stack_[(4) - (2)].eventVal)->identifier());
}
    break;

  case 47:

/* Line 678 of lalr1.cc  */
#line 486 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
}
    break;

  case 48:

/* Line 678 of lalr1.cc  */
#line 489 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.currentService()->output = (yysemantic_stack_[(3) - (3)].serviceInputVal);
}
    break;

  case 49:

/* Line 678 of lalr1.cc  */
#line 493 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Service::Ptr s = driver.currentService();
    std::vector<std::string> v;
    driver.split((yysemantic_stack_[(3) - (3)].stringVal), v);

    if(v.size() == 1) {
	if((yysemantic_stack_[(3) - (1)].stringVal) == "type") {
	    if((yysemantic_stack_[(3) - (3)].stringVal) == "init")
	      s->type = Service::Init;
	    else if((yysemantic_stack_[(3) - (3)].stringVal) == "control")
	      s->type = Service::Control;
	    else if((yysemantic_stack_[(3) - (3)].stringVal) == "exec")
	      s->type = Service::Exec;
	    else {
	      error(yyloc, std::string("Unknown service type: ") + (yysemantic_stack_[(3) - (3)].stringVal));
	      YYERROR;
	    }
	} else if((yysemantic_stack_[(3) - (1)].stringVal) == "taskName") {
	    s->taskName = (yysemantic_stack_[(3) - (3)].stringVal);
	} else if((yysemantic_stack_[(3) - (1)].stringVal) == "errors") {
	    driver.currentService()->addErrorMessage((yysemantic_stack_[(3) - (3)].stringVal));
	} else if((yysemantic_stack_[(3) - (1)].stringVal) == "interrupts") {
	    driver.currentService()->addIncompatibleService((yysemantic_stack_[(3) - (3)].stringVal));
	} else {
	  error(yyloc, std::string("Unknown service field: ") + (yysemantic_stack_[(3) - (1)].stringVal));
	  YYERROR;
	}
    } else {
	std::vector<std::string>::const_iterator it = v.begin();

	if((yysemantic_stack_[(3) - (1)].stringVal) == "errors") {
	    for(; it != v.end(); ++it)
		driver.currentService()->addErrorMessage(*it);
	} else if ((yysemantic_stack_[(3) - (1)].stringVal) == "interrupts") {
	    for(; it != v.end(); ++it)
		driver.currentService()->addIncompatibleService(*it);
	} else {
	  error(yyloc, std::string("Unknown service field (or wrong number of arguments): ") + (yysemantic_stack_[(3) - (1)].stringVal));
	  YYERROR;
	}
    }
}
    break;

  case 50:

/* Line 678 of lalr1.cc  */
#line 536 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Service::Ptr s = driver.currentService();
    if((yysemantic_stack_[(3) - (1)].stringVal) == "doc") {
      s->doc = (yysemantic_stack_[(3) - (3)].stringVal);
    } else {
      error(yyloc, std::string("Unknown service field: ") + (yysemantic_stack_[(3) - (1)].stringVal));
      YYERROR;
    }
}
    break;

  case 51:

/* Line 678 of lalr1.cc  */
#line 546 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.currentService()->addProperty((yysemantic_stack_[(4) - (2)].stringVal), (yysemantic_stack_[(4) - (4)].literalVal));
}
    break;

  case 52:

/* Line 678 of lalr1.cc  */
#line 553 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
   (yyval.stringVal) = (yysemantic_stack_[(1) - (1)].stringVal);
}
    break;

  case 53:

/* Line 678 of lalr1.cc  */
#line 557 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.stringVal) = (yysemantic_stack_[(3) - (1)].stringVal) + " " + (yysemantic_stack_[(3) - (3)].stringVal);
}
    break;

  case 54:

/* Line 678 of lalr1.cc  */
#line 563 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 55:

/* Line 678 of lalr1.cc  */
#line 565 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 56:

/* Line 678 of lalr1.cc  */
#line 569 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yysemantic_stack_[(3) - (1)].serviceInputVal).doc = (yysemantic_stack_[(3) - (3)].inputDocVal);
    driver.currentService()->addInput((yysemantic_stack_[(3) - (1)].serviceInputVal));
}
    break;

  case 57:

/* Line 678 of lalr1.cc  */
#line 574 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yysemantic_stack_[(5) - (1)].serviceInputVal).defaultValue = (yysemantic_stack_[(5) - (3)].literalVal);
    (yysemantic_stack_[(5) - (1)].serviceInputVal).doc = (yysemantic_stack_[(5) - (5)].inputDocVal);
    driver.currentService()->addInput((yysemantic_stack_[(5) - (1)].serviceInputVal));
}
    break;

  case 58:

/* Line 678 of lalr1.cc  */
#line 580 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yysemantic_stack_[(5) - (1)].serviceInputVal).defaultValue = (yysemantic_stack_[(5) - (5)].literalVal);
    (yysemantic_stack_[(5) - (1)].serviceInputVal).doc = (yysemantic_stack_[(5) - (3)].inputDocVal);
    driver.currentService()->addInput((yysemantic_stack_[(5) - (1)].serviceInputVal));
}
    break;

  case 59:

/* Line 678 of lalr1.cc  */
#line 586 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yysemantic_stack_[(3) - (1)].serviceInputVal).defaultValue = (yysemantic_stack_[(3) - (3)].literalVal);
    driver.currentService()->addInput((yysemantic_stack_[(3) - (1)].serviceInputVal));
}
    break;

  case 60:

/* Line 678 of lalr1.cc  */
#line 591 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.currentService()->addInput((yysemantic_stack_[(1) - (1)].serviceInputVal));
}
    break;

  case 61:

/* Line 678 of lalr1.cc  */
#line 597 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
  InputDoc d((yysemantic_stack_[(1) - (1)].stringVal));
  (yyval.inputDocVal) = d;
}
    break;

  case 62:

/* Line 678 of lalr1.cc  */
#line 602 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
  (yyval.inputDocVal) = (yysemantic_stack_[(3) - (2)].inputDocVal);
}
    break;

  case 63:

/* Line 678 of lalr1.cc  */
#line 608 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
  InputDoc d;
  d.members.push_back((yysemantic_stack_[(1) - (1)].inputDocVal));
  (yyval.inputDocVal) = d;
}
    break;

  case 64:

/* Line 678 of lalr1.cc  */
#line 614 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
  (yysemantic_stack_[(3) - (1)].inputDocVal).members.push_back((yysemantic_stack_[(3) - (3)].inputDocVal));
  (yyval.inputDocVal) = (yysemantic_stack_[(3) - (1)].inputDocVal);
}
    break;

  case 65:

/* Line 678 of lalr1.cc  */
#line 621 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    // check if the name given is in the ids
    Idl::IdlType::Ptr t = driver.component().typeFromIdsName((yysemantic_stack_[(3) - (3)].stringVal));
    if(!t) {
      error(yyloc, std::string("Input is not in the IDS: ") + (yysemantic_stack_[(3) - (3)].stringVal));
      YYERROR;
    }

    ServiceInput i;
    i.kind = ServiceInput::IDSMember;
    i.identifier = (yysemantic_stack_[(3) - (3)].stringVal);
    (yyval.serviceInputVal) = i;
}
    break;

  case 66:

/* Line 678 of lalr1.cc  */
#line 635 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    ServiceInput i;
    i.kind = ServiceInput::Type;
    i.identifier = (yysemantic_stack_[(2) - (2)].stringVal);
    i.type = (yysemantic_stack_[(2) - (1)].typeVal);
    (yyval.serviceInputVal) = i;
}
    break;

  case 67:

/* Line 678 of lalr1.cc  */
#line 646 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Event::Ptr ev = driver.component().event((yysemantic_stack_[(1) - (1)].stringVal));
    if(ev) { // external event
      Event::Ptr e(new NamedEvent((yysemantic_stack_[(1) - (1)].stringVal), ev));
      (yyval.eventVal) = e;
    } else { // service event
      if((yysemantic_stack_[(1) - (1)].stringVal) == "onCalled") 
	(yyval.eventVal) = Event::Ptr(new ServiceEvent(std::string(), ServiceEvent::OnCalled));
      else if((yysemantic_stack_[(1) - (1)].stringVal) == "onStart") 
	(yyval.eventVal) = Event::Ptr(new ServiceEvent(std::string(), ServiceEvent::OnStart));
      else if((yysemantic_stack_[(1) - (1)].stringVal) == "onEnd") 
	(yyval.eventVal) = Event::Ptr(new ServiceEvent(std::string(), ServiceEvent::OnEnd));
      else if((yysemantic_stack_[(1) - (1)].stringVal) == "onInter") 
	(yyval.eventVal) = Event::Ptr(new ServiceEvent(std::string(), ServiceEvent::OnInter));
      else {
	  error(yyloc, std::string("Unknown service internal event: ") + (yysemantic_stack_[(1) - (1)].stringVal));
	  YYERROR;
      }
    }
}
    break;

  case 68:

/* Line 678 of lalr1.cc  */
#line 667 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    //try to find what type of event this is
    Port::Ptr p = driver.component().port((yysemantic_stack_[(3) - (1)].stringVal));
    Service::Ptr s = driver.component().service((yysemantic_stack_[(3) - (1)].stringVal));

    if(p) { // port event
      if((yysemantic_stack_[(3) - (3)].stringVal) == "onUpdate") 
	(yyval.eventVal) = Event::Ptr(new PortEvent(p->name, PortEvent::OnUpdate));
      else if((yysemantic_stack_[(3) - (3)].stringVal) == "onRead") 
	(yyval.eventVal) = Event::Ptr(new PortEvent(p->name, PortEvent::OnRead));
      else if((yysemantic_stack_[(3) - (3)].stringVal) == "onWrite") 
	(yyval.eventVal) = Event::Ptr(new PortEvent(p->name, PortEvent::OnWrite));
      else if((yysemantic_stack_[(3) - (3)].stringVal) == "onInitialize") 
	(yyval.eventVal) = Event::Ptr(new PortEvent(p->name, PortEvent::OnInitialize));
      else {
	  error(yyloc, std::string("Unknown port event: ") + (yysemantic_stack_[(3) - (3)].stringVal));
	  YYERROR;
      }
    } else if(s) {
      if((yysemantic_stack_[(3) - (3)].stringVal) == "onCalled") 
	(yyval.eventVal) = Event::Ptr(new ServiceEvent(s->name, ServiceEvent::OnCalled));
      else if((yysemantic_stack_[(3) - (3)].stringVal) == "onStart") 
	(yyval.eventVal) = Event::Ptr(new ServiceEvent(s->name, ServiceEvent::OnStart));
      else if((yysemantic_stack_[(3) - (3)].stringVal) == "onEnd") 
	(yyval.eventVal) = Event::Ptr(new ServiceEvent(s->name, ServiceEvent::OnEnd));
      else if((yysemantic_stack_[(3) - (3)].stringVal) == "onInter") 
	(yyval.eventVal) = Event::Ptr(new ServiceEvent(s->name, ServiceEvent::OnInter));
      else {
	  error(yyloc, std::string("Unknown service event: ") + (yysemantic_stack_[(3) - (3)].stringVal));
	  YYERROR;
      }
    } else {
      error(yyloc, std::string("Unknown object: ") + (yysemantic_stack_[(3) - (1)].stringVal));
      YYERROR;
    }
}
    break;

  case 69:

/* Line 678 of lalr1.cc  */
#line 704 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    if((yysemantic_stack_[(5) - (3)].stringVal) != "onCodel") {
	  error(yyloc, std::string("Unknwon port syntax") );
	  YYERROR;
    }

    Service::Ptr s = driver.component().service((yysemantic_stack_[(5) - (1)].stringVal));
    if(!s) {
	  error(yyloc, std::string("Unknwon service : ") + (yysemantic_stack_[(5) - (1)].stringVal) );
	  YYERROR;
    }  

    Event::Ptr e(new ServiceEvent(s->name, (yysemantic_stack_[(5) - (5)].stringVal)));
    (yyval.eventVal) = e;
}
    break;

  case 71:

/* Line 678 of lalr1.cc  */
#line 726 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 72:

/* Line 678 of lalr1.cc  */
#line 728 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 73:

/* Line 678 of lalr1.cc  */
#line 732 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Event::Ptr ev(new NamedEvent((yysemantic_stack_[(1) - (1)].stringVal)));
    driver.component().addEvent(ev);
}
    break;

  case 74:

/* Line 678 of lalr1.cc  */
#line 737 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Event::Ptr ev(new NamedEvent((yysemantic_stack_[(3) - (1)].stringVal), (yysemantic_stack_[(3) - (3)].eventVal)));
    driver.component().addEvent(ev);
}
    break;

  case 75:

/* Line 678 of lalr1.cc  */
#line 747 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Codel::Ptr c(new Codel((yysemantic_stack_[(1) - (1)].stringVal)));
    driver.setCurrentCodel(c);
}
    break;

  case 76:

/* Line 678 of lalr1.cc  */
#line 752 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.codelVal) = driver.currentCodel();
}
    break;

  case 77:

/* Line 678 of lalr1.cc  */
#line 758 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 78:

/* Line 678 of lalr1.cc  */
#line 760 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.split((yysemantic_stack_[(2) - (2)].stringVal), driver.currentCodel()->nextCodels); 
}
    break;

  case 79:

/* Line 678 of lalr1.cc  */
#line 766 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 80:

/* Line 678 of lalr1.cc  */
#line 768 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 81:

/* Line 678 of lalr1.cc  */
#line 770 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 82:

/* Line 678 of lalr1.cc  */
#line 774 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    // check if the name given is in the ids
    Idl::IdlType::Ptr t = driver.component().typeFromIdsName((yysemantic_stack_[(2) - (2)].stringVal));
    if(!t) {
      error(yyloc, std::string("Input is not in the IDS: ") + (yysemantic_stack_[(2) - (2)].stringVal));
      YYERROR;
    }

    driver.currentCodel()->addInType((yysemantic_stack_[(2) - (2)].stringVal));
}
    break;

  case 83:

/* Line 678 of lalr1.cc  */
#line 785 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    // check if the name given is in the ids
    Idl::IdlType::Ptr t = driver.component().typeFromIdsName((yysemantic_stack_[(2) - (2)].stringVal));
    if(!t) {
      error(yyloc, std::string("Output is not in the IDS: ") + (yysemantic_stack_[(2) - (2)].stringVal));
      YYERROR;
    }

    driver.currentCodel()->addOutType((yysemantic_stack_[(2) - (2)].stringVal));
}
    break;

  case 84:

/* Line 678 of lalr1.cc  */
#line 796 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.currentCodel()->addInPort((yysemantic_stack_[(2) - (2)].stringVal));
}
    break;

  case 85:

/* Line 678 of lalr1.cc  */
#line 800 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.currentCodel()->addOutPort((yysemantic_stack_[(2) - (2)].stringVal));
}
    break;

  case 86:

/* Line 678 of lalr1.cc  */
#line 808 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    ConstValue c((yysemantic_stack_[(5) - (3)].stringVal), (yysemantic_stack_[(5) - (2)].typeVal), (yysemantic_stack_[(5) - (5)].literalVal));
    driver.component().addConstValue(c);
}
    break;

  case 96:

/* Line 678 of lalr1.cc  */
#line 828 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = Literal((yysemantic_stack_[(1) - (1)].integerVal));
}
    break;

  case 97:

/* Line 678 of lalr1.cc  */
#line 832 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = Literal((yysemantic_stack_[(1) - (1)].doubleVal));
}
    break;

  case 98:

/* Line 678 of lalr1.cc  */
#line 836 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = Literal("\"" + (yysemantic_stack_[(1) - (1)].stringVal) + "\"");
}
    break;

  case 99:

/* Line 678 of lalr1.cc  */
#line 840 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = Literal((yysemantic_stack_[(1) - (1)].charVal));
}
    break;

  case 100:

/* Line 678 of lalr1.cc  */
#line 844 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = (yysemantic_stack_[(1) - (1)].literalVal);
}
    break;

  case 101:

/* Line 678 of lalr1.cc  */
#line 848 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    /*enum value*/
    (yyval.literalVal) = Literal((yysemantic_stack_[(1) - (1)].stringVal));
}
    break;

  case 102:

/* Line 678 of lalr1.cc  */
#line 855 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal)= (yysemantic_stack_[(1) - (1)].literalVal);
}
    break;

  case 103:

/* Line 678 of lalr1.cc  */
#line 859 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = (yysemantic_stack_[(3) - (2)].literalVal);
}
    break;

  case 104:

/* Line 678 of lalr1.cc  */
#line 863 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = (yysemantic_stack_[(3) - (2)].literalVal);
}
    break;

  case 105:

/* Line 678 of lalr1.cc  */
#line 867 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 106:

/* Line 678 of lalr1.cc  */
#line 871 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = Literal(true);
}
    break;

  case 107:

/* Line 678 of lalr1.cc  */
#line 875 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = Literal(false);
}
    break;

  case 108:

/* Line 678 of lalr1.cc  */
#line 882 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Neg);
    l.addMember((yysemantic_stack_[(2) - (2)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 109:

/* Line 678 of lalr1.cc  */
#line 888 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = (yysemantic_stack_[(2) - (2)].literalVal);
}
    break;

  case 110:

/* Line 678 of lalr1.cc  */
#line 892 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::LNeg);
    l.addMember((yysemantic_stack_[(2) - (2)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 111:

/* Line 678 of lalr1.cc  */
#line 899 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Plus);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 112:

/* Line 678 of lalr1.cc  */
#line 906 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Minus);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 113:

/* Line 678 of lalr1.cc  */
#line 913 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Times);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 114:

/* Line 678 of lalr1.cc  */
#line 920 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Divide);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 115:

/* Line 678 of lalr1.cc  */
#line 927 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Mod);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 116:

/* Line 678 of lalr1.cc  */
#line 934 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::And);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 117:

/* Line 678 of lalr1.cc  */
#line 941 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Or);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 118:

/* Line 678 of lalr1.cc  */
#line 948 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Xor);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 119:

/* Line 678 of lalr1.cc  */
#line 955 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::LShift);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 120:

/* Line 678 of lalr1.cc  */
#line 962 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::RShift);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 121:

/* Line 678 of lalr1.cc  */
#line 971 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Struct);
    l.addMember((yysemantic_stack_[(1) - (1)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 122:

/* Line 678 of lalr1.cc  */
#line 977 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yysemantic_stack_[(3) - (1)].literalVal).addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = (yysemantic_stack_[(3) - (1)].literalVal);
}
    break;

  case 123:

/* Line 678 of lalr1.cc  */
#line 986 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.component().addNativeTypeInclude((yysemantic_stack_[(6) - (3)].stringVal));
}
    break;

  case 124:

/* Line 678 of lalr1.cc  */
#line 992 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yysemantic_stack_[(2) - (1)].typeVal)->setNative(true);
    driver.component().addType((yysemantic_stack_[(2) - (1)].typeVal));
}
    break;

  case 125:

/* Line 678 of lalr1.cc  */
#line 997 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yysemantic_stack_[(3) - (2)].typeVal)->setNative(true);
    driver.component().addType((yysemantic_stack_[(3) - (2)].typeVal));
}
    break;

  case 126:

/* Line 678 of lalr1.cc  */
#line 1004 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    IdlType::Ptr p(new TypedefType((yysemantic_stack_[(3) - (2)].typeVal), (yysemantic_stack_[(3) - (3)].declaratorVectVal)));
    (yyval.typeVal) = p;
}
    break;

  case 127:

/* Line 678 of lalr1.cc  */
#line 1008 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 128:

/* Line 678 of lalr1.cc  */
#line 1010 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 129:

/* Line 678 of lalr1.cc  */
#line 1023 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal);
}
    break;

  case 130:

/* Line 678 of lalr1.cc  */
#line 1027 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { 
    (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal);
}
    break;

  case 131:

/* Line 678 of lalr1.cc  */
#line 1032 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 132:

/* Line 678 of lalr1.cc  */
#line 1033 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 133:

/* Line 678 of lalr1.cc  */
#line 1035 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    IdlType::Ptr type = driver.component().typeFromName((yysemantic_stack_[(1) - (1)].stringVal));
    if(!type) {
	error(yyloc, std::string("Unknown type: ") + (yysemantic_stack_[(1) - (1)].stringVal));
	YYERROR;
    }
    (yyval.typeVal) = type;
}
    break;

  case 134:

/* Line 678 of lalr1.cc  */
#line 1045 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 135:

/* Line 678 of lalr1.cc  */
#line 1046 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 136:

/* Line 678 of lalr1.cc  */
#line 1047 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 137:

/* Line 678 of lalr1.cc  */
#line 1048 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 138:

/* Line 678 of lalr1.cc  */
#line 1049 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 139:

/* Line 678 of lalr1.cc  */
#line 1050 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 140:

/* Line 678 of lalr1.cc  */
#line 1051 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 141:

/* Line 678 of lalr1.cc  */
#line 1057 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 142:

/* Line 678 of lalr1.cc  */
#line 1058 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 143:

/* Line 678 of lalr1.cc  */
#line 1059 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 144:

/* Line 678 of lalr1.cc  */
#line 1060 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 145:

/* Line 678 of lalr1.cc  */
#line 1064 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 146:

/* Line 678 of lalr1.cc  */
#line 1066 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 147:

/* Line 678 of lalr1.cc  */
#line 1071 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { 
    Declarator::VectPtr v(new Declarator::Vect());
    v->push_back((yysemantic_stack_[(1) - (1)].declaratorVal));
    (yyval.declaratorVectVal) = v;
}
    break;

  case 148:

/* Line 678 of lalr1.cc  */
#line 1077 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
      if((yysemantic_stack_[(3) - (1)].declaratorVectVal)) {
	  (yysemantic_stack_[(3) - (1)].declaratorVectVal)->push_back((yysemantic_stack_[(3) - (3)].declaratorVal));
	  (yyval.declaratorVectVal) = (yysemantic_stack_[(3) - (1)].declaratorVectVal); 
      } else {
	  error(yyloc, "Problem with declarator map");
	  YYERROR;
      }  
}
    break;

  case 149:

/* Line 678 of lalr1.cc  */
#line 1088 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.declaratorVal) = (yysemantic_stack_[(1) - (1)].declaratorVal); }
    break;

  case 150:

/* Line 678 of lalr1.cc  */
#line 1089 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.declaratorVal) = (yysemantic_stack_[(1) - (1)].declaratorVal); }
    break;

  case 151:

/* Line 678 of lalr1.cc  */
#line 1094 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Declarator::Ptr p(new Declarator((yysemantic_stack_[(1) - (1)].stringVal)));
    (yyval.declaratorVal) = p;
}
    break;

  case 152:

/* Line 678 of lalr1.cc  */
#line 1101 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yysemantic_stack_[(2) - (1)].declaratorVal)->addBound((yysemantic_stack_[(2) - (2)].integerVal));
    (yyval.declaratorVal) = (yysemantic_stack_[(2) - (1)].declaratorVal);
}
    break;

  case 153:

/* Line 678 of lalr1.cc  */
#line 1106 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yysemantic_stack_[(2) - (1)].declaratorVal)->addBound((yysemantic_stack_[(2) - (2)].integerVal));
    (yyval.declaratorVal) = (yysemantic_stack_[(2) - (1)].declaratorVal);
}
    break;

  case 154:

/* Line 678 of lalr1.cc  */
#line 1113 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.integerVal) = (yysemantic_stack_[(3) - (2)].integerVal);
}
    break;

  case 155:

/* Line 678 of lalr1.cc  */
#line 1121 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::floatType; }
    break;

  case 156:

/* Line 678 of lalr1.cc  */
#line 1122 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::doubleType; }
    break;

  case 157:

/* Line 678 of lalr1.cc  */
#line 1123 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::longdoubleType; }
    break;

  case 158:

/* Line 678 of lalr1.cc  */
#line 1128 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    IdlType::Ptr p(new FixedType((yysemantic_stack_[(6) - (3)].integerVal), (yysemantic_stack_[(6) - (5)].integerVal)));
    (yyval.typeVal) = p;
}
    break;

  case 159:

/* Line 678 of lalr1.cc  */
#line 1133 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 160:

/* Line 678 of lalr1.cc  */
#line 1136 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 161:

/* Line 678 of lalr1.cc  */
#line 1137 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 162:

/* Line 678 of lalr1.cc  */
#line 1141 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 163:

/* Line 678 of lalr1.cc  */
#line 1142 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 164:

/* Line 678 of lalr1.cc  */
#line 1143 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 165:

/* Line 678 of lalr1.cc  */
#line 1147 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::shortType; }
    break;

  case 166:

/* Line 678 of lalr1.cc  */
#line 1151 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::longType; }
    break;

  case 167:

/* Line 678 of lalr1.cc  */
#line 1155 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::longlongType; }
    break;

  case 168:

/* Line 678 of lalr1.cc  */
#line 1159 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 169:

/* Line 678 of lalr1.cc  */
#line 1160 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 170:

/* Line 678 of lalr1.cc  */
#line 1161 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 171:

/* Line 678 of lalr1.cc  */
#line 1165 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::ushortType; }
    break;

  case 172:

/* Line 678 of lalr1.cc  */
#line 1169 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::ulongType; }
    break;

  case 173:

/* Line 678 of lalr1.cc  */
#line 1173 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::ulonglongType; }
    break;

  case 174:

/* Line 678 of lalr1.cc  */
#line 1177 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::charType; }
    break;

  case 175:

/* Line 678 of lalr1.cc  */
#line 1181 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::wcharType; }
    break;

  case 176:

/* Line 678 of lalr1.cc  */
#line 1185 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::booleanType; }
    break;

  case 177:

/* Line 678 of lalr1.cc  */
#line 1189 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::octetType; }
    break;

  case 178:

/* Line 678 of lalr1.cc  */
#line 1193 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::anyType; }
    break;

  case 179:

/* Line 678 of lalr1.cc  */
#line 1202 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    IdlType::Ptr p(new StringType((yysemantic_stack_[(4) - (3)].integerVal)));
    (yyval.typeVal) = p; 
}
    break;

  case 180:

/* Line 678 of lalr1.cc  */
#line 1207 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.typeVal) = StringType::unboundedStringType;
}
    break;

  case 181:

/* Line 678 of lalr1.cc  */
#line 1213 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { 
    IdlType::Ptr p(new WStringType((yysemantic_stack_[(4) - (3)].integerVal)));
    (yyval.typeVal) = p; 
}
    break;

  case 182:

/* Line 678 of lalr1.cc  */
#line 1218 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.typeVal) = WStringType::unboundedWStringType;
}
    break;

  case 183:

/* Line 678 of lalr1.cc  */
#line 1228 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    IdlType::Ptr p = driver.currentType();
    StructType *s = static_cast<StructType*>(p.get());
    if(!s) {
	error(yyloc, "Empty struct ??");
	YYERROR;
    }
    s->setIdentifier((yysemantic_stack_[(5) - (2)].stringVal));
    driver.setCurrentType(IdlType::Ptr());
    (yyval.typeVal) = p;
}
    break;

  case 184:

/* Line 678 of lalr1.cc  */
#line 1242 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 185:

/* Line 678 of lalr1.cc  */
#line 1244 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 186:

/* Line 678 of lalr1.cc  */
#line 1248 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    StructType *s = dynamic_cast<StructType*>(driver.currentType().get());
    if(!s) {
	IdlType::Ptr p(new StructType());
	s = dynamic_cast<StructType*>(p.get());
	driver.setCurrentType(p);
    }
    s->addMember((yysemantic_stack_[(3) - (1)].typeVal), (yysemantic_stack_[(3) - (2)].declaratorVectVal));
}
    break;

  case 187:

/* Line 678 of lalr1.cc  */
#line 1334 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    IdlType::Ptr p = driver.currentType();
    EnumType *s = dynamic_cast<EnumType*>(p.get());
    if(!s) {
	error(yyloc, "Empty enum ??");
	YYERROR;
    }
    s->setIdentifier((yysemantic_stack_[(5) - (2)].stringVal));
    driver.setCurrentType(IdlType::Ptr());
    (yyval.typeVal) = p;
}
    break;

  case 188:

/* Line 678 of lalr1.cc  */
#line 1347 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 189:

/* Line 678 of lalr1.cc  */
#line 1349 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 190:

/* Line 678 of lalr1.cc  */
#line 1353 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    EnumType *s = dynamic_cast<EnumType*>(driver.currentType().get());
    if(!s) {
	IdlType::Ptr e(new EnumType());
	s =  dynamic_cast<EnumType*>(e.get());
	driver.setCurrentType(e);
    }
    s->addEnumerator((yysemantic_stack_[(1) - (1)].stringVal));
}
    break;

  case 191:

/* Line 678 of lalr1.cc  */
#line 1367 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    IdlType::Ptr p(new SequenceType((yysemantic_stack_[(6) - (3)].typeVal), (yysemantic_stack_[(6) - (5)].integerVal)));
    (yyval.typeVal) = p;
}
    break;

  case 192:

/* Line 678 of lalr1.cc  */
#line 1372 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    IdlType::Ptr p(new SequenceType((yysemantic_stack_[(4) - (3)].typeVal), 0));  
    (yyval.typeVal) = p;
}
    break;



/* Line 678 of lalr1.cc  */
#line 2185 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.cpp"
	default:
          break;
      }
    YY_SYMBOL_PRINT ("-> $$ =", yyr1_[yyn], &yyval, &yyloc);

    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();

    yysemantic_stack_.push (yyval);
    yylocation_stack_.push (yyloc);

    /* Shift the result of the reduction.  */
    yyn = yyr1_[yyn];
    yystate = yypgoto_[yyn - yyntokens_] + yystate_stack_[0];
    if (0 <= yystate && yystate <= yylast_
	&& yycheck_[yystate] == yystate_stack_[0])
      yystate = yytable_[yystate];
    else
      yystate = yydefgoto_[yyn - yyntokens_];
    goto yynewstate;

  /*------------------------------------.
  | yyerrlab -- here on detecting error |
  `------------------------------------*/
  yyerrlab:
    /* If not already recovering from an error, report this error.  */
    if (!yyerrstatus_)
      {
	++yynerrs_;
	error (yylloc, yysyntax_error_ (yystate, yytoken));
      }

    yyerror_range[0] = yylloc;
    if (yyerrstatus_ == 3)
      {
	/* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

	if (yychar <= yyeof_)
	  {
	  /* Return failure if at end of input.  */
	  if (yychar == yyeof_)
	    YYABORT;
	  }
	else
	  {
	    yydestruct_ ("Error: discarding", yytoken, &yylval, &yylloc);
	    yychar = yyempty_;
	  }
      }

    /* Else will try to reuse lookahead token after shifting the error
       token.  */
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;

    yyerror_range[0] = yylocation_stack_[yylen - 1];
    /* Do not reclaim the symbols of the rule which action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    yystate = yystate_stack_[0];
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;	/* Each real token shifted decrements this.  */

    for (;;)
      {
	yyn = yypact_[yystate];
	if (yyn != yypact_ninf_)
	{
	  yyn += yyterror_;
	  if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
	    {
	      yyn = yytable_[yyn];
	      if (0 < yyn)
		break;
	    }
	}

	/* Pop the current state because it cannot handle the error token.  */
	if (yystate_stack_.height () == 1)
	YYABORT;

	yyerror_range[0] = yylocation_stack_[0];
	yydestruct_ ("Error: popping",
		     yystos_[yystate],
		     &yysemantic_stack_[0], &yylocation_stack_[0]);
	yypop_ ();
	yystate = yystate_stack_[0];
	YY_STACK_PRINT ();
      }

    yyerror_range[1] = yylloc;
    // Using YYLLOC is tempting, but would change the location of
    // the lookahead.  YYLOC is available though.
    YYLLOC_DEFAULT (yyloc, (yyerror_range - 1), 2);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yyloc);

    /* Shift the error token.  */
    YY_SYMBOL_PRINT ("Shifting", yystos_[yyn],
		     &yysemantic_stack_[0], &yylocation_stack_[0]);

    yystate = yyn;
    goto yynewstate;

    /* Accept.  */
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    /* Abort.  */
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (yychar != yyempty_)
      yydestruct_ ("Cleanup: discarding lookahead", yytoken, &yylval, &yylloc);

    /* Do not reclaim the symbols of the rule which action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (yystate_stack_.height () != 1)
      {
	yydestruct_ ("Cleanup: popping",
		   yystos_[yystate_stack_[0]],
		   &yysemantic_stack_[0],
		   &yylocation_stack_[0]);
	yypop_ ();
      }

    return yyresult;
  }

  // Generate an error message.
  std::string
  Parser::yysyntax_error_ (int yystate, int tok)
  {
    std::string res;
    YYUSE (yystate);
#if YYERROR_VERBOSE
    int yyn = yypact_[yystate];
    if (yypact_ninf_ < yyn && yyn <= yylast_)
      {
	/* Start YYX at -YYN if negative to avoid negative indexes in
	   YYCHECK.  */
	int yyxbegin = yyn < 0 ? -yyn : 0;

	/* Stay within bounds of both yycheck and yytname.  */
	int yychecklim = yylast_ - yyn + 1;
	int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
	int count = 0;
	for (int x = yyxbegin; x < yyxend; ++x)
	  if (yycheck_[x + yyn] == x && x != yyterror_)
	    ++count;

	// FIXME: This method of building the message is not compatible
	// with internationalization.  It should work like yacc.c does it.
	// That is, first build a string that looks like this:
	// "syntax error, unexpected %s or %s or %s"
	// Then, invoke YY_ on this string.
	// Finally, use the string as a format to output
	// yytname_[tok], etc.
	// Until this gets fixed, this message appears in English only.
	res = "syntax error, unexpected ";
	res += yytnamerr_ (yytname_[tok]);
	if (count < 5)
	  {
	    count = 0;
	    for (int x = yyxbegin; x < yyxend; ++x)
	      if (yycheck_[x + yyn] == x && x != yyterror_)
		{
		  res += (!count++) ? ", expecting " : " or ";
		  res += yytnamerr_ (yytname_[x]);
		}
	  }
      }
    else
#endif
      res = YY_("syntax error");
    return res;
  }


  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
  const short int Parser::yypact_ninf_ = -296;
  const short int
  Parser::yypact_[] =
  {
       186,   -40,   -29,   -20,   252,   252,   -10,    -8,   312,   252,
      33,    69,   139,   126,   137,  -296,  -296,  -296,  -296,  -296,
    -296,  -296,  -296,  -296,  -296,   158,  -296,  -296,  -296,    78,
    -296,  -296,   138,  -296,  -296,   161,   177,  -296,  -296,  -296,
     181,   125,  -296,   155,  -296,  -296,  -296,  -296,  -296,  -296,
    -296,  -296,  -296,  -296,  -296,  -296,  -296,  -296,  -296,  -296,
    -296,  -296,  -296,  -296,  -296,  -296,  -296,  -296,  -296,   156,
     218,   220,   167,  -296,  -296,  -296,  -296,  -296,  -296,  -296,
    -296,  -296,   168,   219,   223,  -296,   172,  -296,  -296,   228,
    -296,   -11,   231,   232,  -296,  -296,   196,   197,   199,   288,
    -296,   233,   251,   256,   198,   252,   248,  -296,   253,  -296,
     201,   201,   229,    33,   284,  -296,   234,   283,    -3,   285,
      17,     3,   286,   300,   314,   163,  -296,   289,   293,  -296,
      39,  -296,   168,   169,  -296,    24,   168,   269,  -296,  -296,
     311,  -296,  -296,   105,   319,    63,  -296,   321,  -296,   270,
     271,   323,    16,   326,   275,   290,   340,   342,   229,   343,
      37,   345,   297,  -296,  -296,  -296,   298,   334,    31,  -296,
     198,    21,  -296,  -296,    24,    24,  -296,  -296,  -296,  -296,
    -296,  -296,  -296,    24,    24,    24,  -296,   240,  -296,  -296,
    -296,   282,   299,    55,   350,    77,  -296,  -296,  -296,   349,
    -296,   353,   354,    48,  -296,   356,  -296,   357,   358,   216,
     216,   359,   118,  -296,   361,  -296,   360,   362,   305,    19,
     364,   310,    38,   366,  -296,  -296,  -296,   240,    84,    93,
    -296,  -296,  -296,    24,    24,    24,    24,    24,    24,    24,
      24,    24,    24,  -296,   363,  -296,   367,  -296,  -296,   316,
     317,    77,  -296,   349,  -296,   317,    77,   365,   368,  -296,
      83,   318,  -296,   317,  -296,   349,  -296,  -296,  -296,   373,
    -296,   375,  -296,   376,  -296,   378,  -296,  -296,    24,  -296,
     204,   204,   116,   116,   153,   140,   140,   140,   142,   142,
     324,  -296,  -296,  -296,  -296,  -296,  -296,  -296,   327,   216,
      28,    24,  -296,  -296,    77,  -296,   317,  -296,   240,  -296,
     377,  -296,  -296,    28,  -296,   374,    49,  -296,  -296,   148,
    -296,   114,    24,    28,   328,   331,   332,   333,   127,  -296,
    -296,    28,   240,  -296,  -296,  -296,  -296,  -296,   148,   379,
    -296,  -296,   335,  -296,   349
  };

  /* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
     doesn't specify something else to do.  Zero means the default is an
     error.  */
  const unsigned char
  Parser::yydefact_[] =
  {
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     6,     5,     7,     8,    12,
      11,    10,     9,   127,   128,     0,    33,    41,   165,   166,
     155,   156,   159,   174,   175,   180,   182,   176,   177,   178,
       0,     0,   133,     0,   129,   131,   132,   130,   134,   144,
     135,   160,   162,   163,   164,   161,   168,   169,   170,   136,
     137,   138,   139,   140,   142,   143,   145,   146,   141,     0,
       0,     0,     0,    91,    94,    87,    88,    89,    90,    95,
      92,    93,     0,    73,    70,    71,     0,     1,     2,     0,
       3,     0,     0,     0,   167,   157,     0,     0,     0,     0,
     171,   172,    20,    23,     0,     0,     0,   151,   126,   147,
     149,   150,     0,     0,     0,     4,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   173,     0,     0,   190,
       0,   188,     0,     0,   184,     0,     0,     0,   152,   153,
      67,    74,    72,     0,     0,     0,    13,     0,    14,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   179,   181,   192,     0,     0,     0,   187,
       0,     0,   183,   185,     0,     0,   106,   107,    99,    96,
      97,    98,   101,     0,     0,     0,   102,    86,   100,   105,
     148,     0,     0,     0,     0,     0,    17,    16,    52,    18,
      15,     0,     0,     0,    34,     0,    35,     0,     0,     0,
       0,     0,     0,    42,     0,    43,     0,     0,     0,     0,
       0,     0,     0,     0,    30,   189,   186,   121,     0,     0,
     108,   109,   110,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   154,    68,   123,     0,   124,    19,     0,
       0,     0,    38,    39,    36,     0,     0,     0,    47,    54,
      60,     0,    48,     0,    50,    49,    44,   158,   191,     0,
      22,     0,    26,     0,    25,     0,    28,   103,     0,   104,
     112,   111,   114,   113,   115,   118,   117,   116,   120,   119,
       0,   125,    53,    75,    37,    40,    45,    51,     0,     0,
       0,     0,    66,    46,     0,    27,     0,    29,   122,    69,
       0,    65,    55,     0,    61,    56,    59,    32,    31,    79,
      63,     0,     0,     0,     0,     0,     0,     0,     0,    80,
      62,     0,    58,    57,    82,    83,    84,    85,     0,    77,
      64,    81,     0,    76,    78
  };

  /* YYPGOTO[NTERM-NUM].  */
  const short int
  Parser::yypgoto_[] =
  {
      -296,  -296,  -296,   382,  -296,  -296,   280,  -296,  -296,  -296,
    -296,  -296,   178,  -157,  -296,  -296,  -296,   247,  -296,  -296,
    -296,   241,  -201,  -296,   103,  -295,  -296,   193,   246,  -296,
    -296,   292,  -230,  -296,  -296,  -296,    68,  -296,  -296,  -176,
    -168,  -296,  -296,  -296,  -296,  -296,  -114,    -1,   308,  -296,
    -296,  -296,   276,   273,  -296,  -296,   301,   402,   403,   405,
    -296,  -296,  -296,  -296,  -296,  -296,  -296,  -296,   406,   407,
     408,   409,  -296,   410,   411,     0,  -296,   287,     1,  -296,
     254,  -296
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const short int
  Parser::yydefgoto_[] =
  {
        -1,    12,    13,    14,    15,   118,   119,    16,   127,   128,
     219,   222,   223,   224,    17,    92,   152,   153,    18,    93,
     160,   161,   199,   258,   259,   315,   321,   260,   141,    19,
      84,    85,   294,   310,   343,   328,   329,    20,    72,   186,
     187,   188,   189,   228,    21,   193,    22,   261,    44,    45,
      46,    47,   108,   109,   110,   111,   138,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,   133,   134,    67,   130,
     131,    68
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If zero, do what YYDEFACT says.  */
  const signed char Parser::yytable_ninf_ = -25;
  const short int
  Parser::yytable_[] =
  {
        23,    24,   253,    43,    69,   146,   227,   229,    82,   154,
     220,   265,   116,    23,    24,   230,   231,   232,   320,   248,
     116,    25,   149,   149,   204,   296,   155,   270,   333,   194,
     226,   174,    26,   303,   136,   313,   340,   221,   175,   150,
     150,    27,   218,   154,   221,   213,   274,   169,   176,   177,
     117,    70,   170,    71,   218,   156,   157,   158,   117,   323,
     155,   218,   271,   245,   159,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   295,   318,   151,   151,   246,
     297,   178,   179,   180,   181,   182,   183,   184,   314,   156,
     157,   158,   277,   300,    83,     6,   185,   278,   159,   301,
       7,   176,   177,     9,   132,    94,   252,    95,   279,   198,
     308,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   196,   330,   197,   198,    86,    88,   331,   317,     1,
       2,     3,   132,   316,   178,   179,   180,   181,   182,    87,
     338,   344,   339,    23,    24,     6,    90,     4,     5,    96,
       7,   100,   101,     9,   332,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   242,    91,     6,   324,   325,   326,
     327,     7,    97,     8,     9,   165,   166,   172,   264,   198,
      10,    11,   237,   238,   239,   240,   241,   242,    98,     1,
       2,     3,    99,    23,    24,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,     4,     5,     6,
     241,   242,   -25,   -25,     7,    40,   102,   103,    41,   -25,
     238,   239,   240,   241,   242,   104,     6,   105,   106,   107,
      42,     7,   114,     8,     9,   112,   113,   115,   120,   121,
      10,    11,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,   122,   123,     6,   124,   -21,   129,
     126,     7,    40,   -24,   135,    41,   136,   257,   235,   236,
     237,   238,   239,   240,   241,   242,   137,    42,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
     140,   143,     6,   145,   148,   144,   167,     7,    40,   162,
     168,    41,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   163,    42,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,   164,   191,   192,   195,
     200,   201,   202,   203,    40,   206,   207,    41,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    42,
     209,   208,   210,   212,   215,   216,   217,   218,   243,   247,
     244,    41,   249,   250,   251,   254,   269,   255,   256,   263,
     266,   273,   267,   272,   268,   276,   291,   292,   293,   302,
     290,   299,   298,   304,   305,   309,   306,   307,   311,   334,
     322,   319,   335,   336,   337,    89,   198,   342,   147,   205,
     275,   214,   312,   262,   211,   142,   341,   125,   171,   190,
      73,    74,   139,    75,    76,    77,    78,    79,    80,    81,
     173,     0,     0,     0,   225
  };

  /* YYCHECK.  */
  const short int
  Parser::yycheck_[] =
  {
         0,     0,   203,     4,     5,     8,   174,   175,     9,     6,
     167,   212,    23,    13,    13,   183,   184,   185,   313,   195,
      23,    61,     6,     6,     8,   255,    23,     8,   323,   143,
       9,     7,    61,   263,    13,     7,   331,     6,    14,    23,
      23,    61,    23,     6,     6,     8,     8,     8,    24,    25,
      61,    61,    13,    61,    23,    52,    53,    54,    61,    10,
      23,    23,   219,     8,    61,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   242,   251,   306,    61,    61,   193,
     256,    57,    58,    59,    60,    61,    62,    63,    60,    52,
      53,    54,     8,    10,    61,    40,    72,    13,    61,    16,
      45,    24,    25,    48,   105,    27,    58,    29,    15,    61,
     278,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    58,     8,    60,    61,    56,     0,    13,   304,     3,
       4,     5,   133,   301,    57,    58,    59,    60,    61,     0,
      13,   342,    15,   143,   143,    40,     9,    21,    22,    11,
      45,    26,    27,    48,   322,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,     7,    40,    19,    20,    21,
      22,    45,    11,    47,    48,    12,    13,     8,    60,    61,
      54,    55,    66,    67,    68,    69,    70,    71,    11,     3,
       4,     5,    11,   193,   193,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    21,    22,    40,
      70,    71,    70,    71,    45,    46,    61,    61,    49,    66,
      67,    68,    69,    70,    71,     7,    40,     7,    61,    61,
      61,    45,    60,    47,    48,    16,    13,     9,     7,     7,
      54,    55,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    58,    58,    40,    58,     7,    61,
      27,    45,    46,     7,    16,    49,    13,    51,    64,    65,
      66,    67,    68,    69,    70,    71,    75,    61,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      61,     7,    40,    10,     9,    61,     7,    45,    46,    13,
       7,    49,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    12,    61,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    12,    58,    17,    10,
       9,    61,    61,    10,    46,     9,    61,    49,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    61,
      10,    61,    10,    10,     9,    58,    58,    23,    76,     9,
      61,    49,    13,    10,    10,     9,    61,    10,    10,    10,
       9,    61,    12,     9,    12,     9,     9,    61,    61,    61,
      17,    13,    17,    10,     9,    61,    10,     9,    61,    61,
      16,    14,    61,    61,    61,    13,    61,    18,   118,   152,
     222,   160,   299,   210,   158,   113,   338,    99,   132,   136,
       8,     8,   111,     8,     8,     8,     8,     8,     8,     8,
     133,    -1,    -1,    -1,   170
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  Parser::yystos_[] =
  {
         0,     3,     4,     5,    21,    22,    40,    45,    47,    48,
      54,    55,    78,    79,    80,    81,    84,    91,    95,   106,
     114,   121,   123,   152,   155,    61,    61,    61,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      46,    49,    61,   124,   125,   126,   127,   128,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   155,   158,   124,
      61,    61,   115,   134,   135,   136,   145,   146,   147,   148,
     150,   151,   124,    61,   107,   108,    56,     0,     0,    80,
       9,     7,    92,    96,    27,    29,    11,    11,    11,    11,
      26,    27,    61,    61,     7,     7,    61,    61,   129,   130,
     131,   132,    16,    13,    60,     9,    23,    61,    82,    83,
       7,     7,    58,    58,    58,   125,    27,    85,    86,    61,
     156,   157,   124,   153,   154,    16,    13,    75,   133,   133,
      61,   105,   108,     7,    61,    10,     8,    83,     9,     6,
      23,    61,    93,    94,     6,    23,    52,    53,    54,    61,
      97,    98,    13,    12,    12,    12,    13,     7,     7,     8,
      13,   129,     8,   154,     7,    14,    24,    25,    57,    58,
      59,    60,    61,    62,    63,    72,   116,   117,   118,   119,
     130,    58,    17,   122,   123,    10,    58,    60,    61,    99,
       9,    61,    61,    10,     8,    94,     9,    61,    61,    10,
      10,   105,    10,     8,    98,     9,    58,    58,    23,    87,
      90,     6,    88,    89,    90,   157,     9,   117,   120,   117,
     117,   117,   117,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    76,    61,     8,   123,     9,   116,    13,
      10,    10,    58,    99,     9,    10,    10,    51,   100,   101,
     104,   124,   104,    10,    60,    99,     9,    12,    12,    61,
       8,    90,     9,    61,     8,    89,     9,     8,    13,    15,
     117,   117,   117,   117,   117,   117,   117,   117,   117,   117,
      17,     9,    61,    61,   109,   116,   109,   116,    17,    13,
      10,    16,    61,   109,    10,     9,    10,     9,   117,    61,
     110,    61,   101,     7,    60,   102,   117,   116,   109,    14,
     102,   103,    16,    10,    19,    20,    21,    22,   112,   113,
       8,    13,   117,   102,    61,    61,    61,    61,    13,    15,
     102,   113,    18,   111,    99
  };

#if YYDEBUG
  /* TOKEN_NUMBER_[YYLEX-NUM] -- Internal symbol number corresponding
     to YYLEX-NUM.  */
  const unsigned short int
  Parser::yytoken_number_[] =
  {
         0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,    91,    93
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  Parser::yyr1_[] =
  {
         0,    77,    78,    79,    79,    80,    80,    80,    80,    80,
      80,    80,    80,    81,    82,    82,    83,    83,    83,    83,
      84,    85,    84,    84,    86,    84,    87,    87,    88,    88,
      89,    89,    90,    92,    91,    93,    93,    94,    94,    94,
      94,    96,    95,    97,    97,    98,    98,    98,    98,    98,
      98,    98,    99,    99,   100,   100,   101,   101,   101,   101,
     101,   102,   102,   103,   103,   104,   104,   105,   105,   105,
     106,   107,   107,   108,   108,   110,   109,   111,   111,   112,
     112,   112,   113,   113,   113,   113,   114,   115,   115,   115,
     115,   115,   115,   115,   115,   115,   116,   116,   116,   116,
     116,   116,   117,   117,   117,   117,   118,   118,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   119,   119,
     119,   120,   120,   121,   122,   122,   123,   123,   123,   124,
     124,   125,   125,   125,   126,   126,   126,   126,   126,   126,
     126,   127,   127,   127,   127,   128,   128,   129,   129,   130,
     130,   131,   132,   132,   133,   134,   134,   134,   135,   135,
     136,   136,   137,   137,   137,   138,   139,   140,   141,   141,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     150,   151,   151,   152,   153,   153,   154,   155,   156,   156,
     157,   158,   158
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  Parser::yyr2_[] =
  {
         0,     2,     2,     2,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     5,     2,     3,     3,     3,     3,     4,
       3,     0,     7,     3,     0,     7,     2,     3,     2,     3,
       1,     4,     4,     0,     6,     2,     3,     4,     3,     3,
       4,     0,     6,     2,     3,     4,     4,     3,     3,     3,
       3,     4,     1,     3,     1,     3,     3,     5,     5,     3,
       1,     1,     3,     1,     3,     3,     2,     1,     3,     5,
       2,     1,     3,     1,     3,     0,     6,     0,     2,     0,
       1,     3,     2,     2,     2,     2,     5,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     3,     1,     1,     1,     2,     2,
       2,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     3,     6,     2,     3,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     1,
       1,     1,     2,     2,     3,     1,     1,     2,     6,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     1,     1,
       1,     2,     2,     3,     1,     1,     1,     1,     1,     4,
       1,     4,     1,     5,     1,     2,     3,     5,     1,     3,
       1,     6,     4
  };

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
  /* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
     First, the terminals, then, starting at \a yyntokens_, nonterminals.  */
  const char*
  const Parser::yytname_[] =
  {
    "\"end of file\"", "error", "$undefined", "\"component\"", "\"task\"",
  "\"service\"", "\"codel\"", "LBRACE", "RBRACE", "SEMICOLON", "COLON",
  "LESS_THAN", "GREATER_THAN", "COMMA", "LPAREN", "RPAREN", "EQUAL", "DOT",
  "LARROW", "IN", "OUT", "INPORT", "OUTPORT", "PROPERTY", "TRUE", "FALSE",
  "SHORT", "LONG", "FLOAT", "DOUBLE", "FIXED", "CHAR", "WCHAR", "STRING",
  "WSTRING", "BOOLEAN", "OCTET", "ANY", "VOID", "NATIVE", "ENUM", "UNION",
  "SWITCH", "CASE", "DEFAULT", "STRUCT", "SEQUENCE", "CONST", "TYPEDEF",
  "UNSIGNED", "OBJECT", "IDS", "INPUT", "OUTPUT", "EVENT", "IMPORT",
  "FROM", "\"char\"", "\"integer\"", "\"double\"", "\"string literal\"",
  "\"identifier\"", "MINUS", "PLUS", "SLASH", "TIMES", "MOD", "XOR", "OR",
  "AND", "RSHIFT", "LSHIFT", "TILDE", "POS", "NEG", "'['", "']'",
  "$accept", "start", "declarations", "declaration", "component_decl",
  "component_fields", "component_field", "port_decl", "$@1", "$@2",
  "inport_fields", "outport_fields", "outport_field", "port_field",
  "task_decl", "$@3", "task_fields", "task_field", "service_decl", "$@4",
  "service_fields", "service_field", "identifiers", "inputs", "input",
  "input_doc", "input_doc_list", "input_type", "event_spec", "event_decl",
  "event_declarators", "event_declarator", "codel_prototype", "$@5",
  "codel_next", "codel_fields", "codel_field", "const_decl", "const_type",
  "basic_literal", "literal", "boolean_literal", "composed_literal",
  "literals", "native_type_decl", "type_decl_list", "type_decl",
  "type_spec", "simple_type_spec", "base_type_spec", "template_type_spec",
  "constr_type_spec", "declarators", "declarator", "simple_declarator",
  "array_declarator", "fixed_array_size", "floating_pt_type",
  "fixed_pt_type", "integer_type", "signed_int", "signed_short_int",
  "signed_long_int", "signed_long_long_int", "unsigned_int",
  "unsigned_short_int", "unsigned_long_int", "unsigned_long_long_int",
  "char_type", "wide_char_type", "boolean_type", "octet_type", "any_type",
  "string_type", "wide_string_type", "struct_type", "members", "member",
  "enum_type", "enumerators", "enumerator", "sequence_type", 0
  };
#endif

#if YYDEBUG
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const Parser::rhs_number_type
  Parser::yyrhs_[] =
  {
        78,     0,    -1,    79,     0,    -1,    80,     9,    -1,    79,
      80,     9,    -1,    84,    -1,    81,    -1,    91,    -1,    95,
      -1,   123,    -1,   121,    -1,   114,    -1,   106,    -1,     3,
      61,     7,    82,     8,    -1,    83,     9,    -1,    82,    83,
       9,    -1,    61,    10,    60,    -1,    61,    10,    58,    -1,
      61,    10,    99,    -1,    23,    61,    10,   116,    -1,    21,
     124,    61,    -1,    -1,    21,   124,    61,    85,     7,    87,
       8,    -1,    22,   124,    61,    -1,    -1,    22,   124,    61,
      86,     7,    88,     8,    -1,    90,     9,    -1,    87,    90,
       9,    -1,    89,     9,    -1,    88,    89,     9,    -1,    90,
      -1,     6,    61,    10,   109,    -1,    23,    61,    10,   116,
      -1,    -1,     4,    61,    92,     7,    93,     8,    -1,    94,
       9,    -1,    93,    94,     9,    -1,     6,    61,    10,   109,
      -1,    61,    10,    58,    -1,    61,    10,    99,    -1,    23,
      61,    10,   116,    -1,    -1,     5,    61,    96,     7,    97,
       8,    -1,    98,     9,    -1,    97,    98,     9,    -1,     6,
      61,    10,   109,    -1,    54,   105,    10,   109,    -1,    52,
      10,   100,    -1,    53,    10,   104,    -1,    61,    10,    99,
      -1,    61,    10,    60,    -1,    23,    61,    10,   116,    -1,
      61,    -1,    99,    13,    61,    -1,   101,    -1,   100,    13,
     101,    -1,   104,    10,   102,    -1,   104,    16,   117,    10,
     102,    -1,   104,    10,   102,    16,   117,    -1,   104,    16,
     117,    -1,   104,    -1,    60,    -1,     7,   103,     8,    -1,
     102,    -1,   103,    13,   102,    -1,    51,    17,    61,    -1,
     124,    61,    -1,    61,    -1,    61,    17,    61,    -1,    61,
      17,    61,    17,    61,    -1,    54,   107,    -1,   108,    -1,
     107,    13,   108,    -1,    61,    -1,    61,    16,   105,    -1,
      -1,    61,   110,    14,   112,    15,   111,    -1,    -1,    18,
      99,    -1,    -1,   113,    -1,   112,    13,   113,    -1,    19,
      61,    -1,    20,    61,    -1,    21,    61,    -1,    22,    61,
      -1,    47,   115,    61,    16,   117,    -1,   136,    -1,   145,
      -1,   146,    -1,   147,    -1,   134,    -1,   150,    -1,   151,
      -1,   135,    -1,   148,    -1,    58,    -1,    59,    -1,    60,
      -1,    57,    -1,   118,    -1,    61,    -1,   116,    -1,     7,
     120,     8,    -1,    14,   117,    15,    -1,   119,    -1,    24,
      -1,    25,    -1,    62,   117,    -1,    63,   117,    -1,    72,
     117,    -1,   117,    63,   117,    -1,   117,    62,   117,    -1,
     117,    65,   117,    -1,   117,    64,   117,    -1,   117,    66,
     117,    -1,   117,    69,   117,    -1,   117,    68,   117,    -1,
     117,    67,   117,    -1,   117,    71,   117,    -1,   117,    70,
     117,    -1,   117,    -1,   120,    13,   117,    -1,    55,    56,
      60,     7,   122,     8,    -1,   123,     9,    -1,   122,   123,
       9,    -1,    48,   124,   129,    -1,   152,    -1,   155,    -1,
     125,    -1,   128,    -1,   126,    -1,   127,    -1,    61,    -1,
     134,    -1,   136,    -1,   145,    -1,   146,    -1,   147,    -1,
     148,    -1,   149,    -1,   158,    -1,   150,    -1,   151,    -1,
     135,    -1,   152,    -1,   155,    -1,   130,    -1,   129,    13,
     130,    -1,   131,    -1,   132,    -1,    61,    -1,   131,   133,
      -1,   132,   133,    -1,    75,    58,    76,    -1,    28,    -1,
      29,    -1,    27,    29,    -1,    30,    11,    58,    13,    58,
      12,    -1,    30,    -1,   137,    -1,   141,    -1,   138,    -1,
     139,    -1,   140,    -1,    26,    -1,    27,    -1,    27,    27,
      -1,   142,    -1,   143,    -1,   144,    -1,    49,    26,    -1,
      49,    27,    -1,    49,    27,    27,    -1,    31,    -1,    32,
      -1,    35,    -1,    36,    -1,    37,    -1,    33,    11,    58,
      12,    -1,    33,    -1,    34,    11,    58,    12,    -1,    34,
      -1,    45,    61,     7,   153,     8,    -1,   154,    -1,   153,
     154,    -1,   124,   129,     9,    -1,    40,    61,     7,   156,
       8,    -1,   157,    -1,   156,    13,   157,    -1,    61,    -1,
      46,    11,   125,    13,    58,    12,    -1,    46,    11,   125,
      12,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned short int
  Parser::yyprhs_[] =
  {
         0,     0,     3,     6,     9,    13,    15,    17,    19,    21,
      23,    25,    27,    29,    35,    38,    42,    46,    50,    54,
      59,    63,    64,    72,    76,    77,    85,    88,    92,    95,
      99,   101,   106,   111,   112,   119,   122,   126,   131,   135,
     139,   144,   145,   152,   155,   159,   164,   169,   173,   177,
     181,   185,   190,   192,   196,   198,   202,   206,   212,   218,
     222,   224,   226,   230,   232,   236,   240,   243,   245,   249,
     255,   258,   260,   264,   266,   270,   271,   278,   279,   282,
     283,   285,   289,   292,   295,   298,   301,   307,   309,   311,
     313,   315,   317,   319,   321,   323,   325,   327,   329,   331,
     333,   335,   337,   339,   343,   347,   349,   351,   353,   356,
     359,   362,   366,   370,   374,   378,   382,   386,   390,   394,
     398,   402,   404,   408,   415,   418,   422,   426,   428,   430,
     432,   434,   436,   438,   440,   442,   444,   446,   448,   450,
     452,   454,   456,   458,   460,   462,   464,   466,   468,   472,
     474,   476,   478,   481,   484,   488,   490,   492,   495,   502,
     504,   506,   508,   510,   512,   514,   516,   518,   521,   523,
     525,   527,   530,   533,   537,   539,   541,   543,   545,   547,
     552,   554,   559,   561,   567,   569,   572,   576,   582,   584,
     588,   590,   597
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned short int
  Parser::yyrline_[] =
  {
         0,   237,   237,   240,   241,   246,   248,   250,   252,   254,
     258,   260,   262,   269,   275,   276,   281,   294,   303,   332,
     341,   347,   346,   355,   361,   360,   372,   373,   377,   378,
     382,   383,   394,   403,   402,   413,   414,   418,   422,   439,
     452,   461,   460,   471,   472,   476,   480,   485,   488,   492,
     535,   545,   552,   556,   562,   564,   568,   573,   579,   585,
     590,   596,   601,   607,   613,   620,   634,   645,   666,   703,
     722,   725,   727,   731,   736,   747,   746,   758,   759,   766,
     767,   769,   773,   784,   795,   799,   807,   814,   815,   816,
     817,   818,   819,   820,   821,   822,   827,   831,   835,   839,
     843,   847,   854,   858,   862,   866,   870,   874,   881,   887,
     891,   898,   905,   912,   919,   926,   933,   940,   947,   954,
     961,   970,   976,   985,   991,   996,  1003,  1008,  1010,  1022,
    1026,  1032,  1033,  1034,  1045,  1046,  1047,  1048,  1049,  1050,
    1051,  1057,  1058,  1059,  1060,  1064,  1066,  1070,  1076,  1088,
    1089,  1093,  1100,  1105,  1112,  1121,  1122,  1123,  1127,  1132,
    1136,  1137,  1141,  1142,  1143,  1147,  1151,  1155,  1159,  1160,
    1161,  1165,  1169,  1173,  1177,  1181,  1185,  1189,  1193,  1201,
    1206,  1212,  1217,  1227,  1241,  1243,  1247,  1333,  1347,  1348,
    1352,  1366,  1371
  };

  // Print the state stack on the debug stream.
  void
  Parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (state_stack_type::const_iterator i = yystate_stack_.begin ();
	 i != yystate_stack_.end (); ++i)
      *yycdebug_ << ' ' << *i;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  Parser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    /* Print the symbols being reduced, and their result.  */
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
	       << " (line " << yylno << "):" << std::endl;
    /* The symbols being reduced.  */
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
		       yyrhs_[yyprhs_[yyrule] + yyi],
		       &(yysemantic_stack_[(yynrhs) - (yyi + 1)]),
		       &(yylocation_stack_[(yynrhs) - (yyi + 1)]));
  }
#endif // YYDEBUG

  /* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
  Parser::token_number_type
  Parser::yytranslate_ (int t)
  {
    static
    const token_number_type
    translate_table[] =
    {
           0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    75,     2,    76,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int Parser::yyeof_ = 0;
  const int Parser::yylast_ = 424;
  const int Parser::yynnts_ = 82;
  const int Parser::yyempty_ = -2;
  const int Parser::yyfinal_ = 87;
  const int Parser::yyterror_ = 1;
  const int Parser::yyerrcode_ = 256;
  const int Parser::yyntokens_ = 77;

  const unsigned int Parser::yyuser_token_number_max_ = 329;
  const Parser::token_number_type Parser::yyundef_token_ = 2;


/* Line 1054 of lalr1.cc  */
#line 1 "[Bison:b4_percent_define_default]"

} // G3nom

/* Line 1054 of lalr1.cc  */
#line 2972 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.cpp"


/* Line 1056 of lalr1.cc  */
#line 1377 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
 /*** Additional Code ***/

void G3nom::Parser::error(const Parser::location_type& l,
			    const std::string& m)
{
    driver.error(l, m);
}


