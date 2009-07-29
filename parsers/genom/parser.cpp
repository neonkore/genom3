
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
#line 220 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"


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
#line 241 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 5:

/* Line 678 of lalr1.cc  */
#line 246 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 6:

/* Line 678 of lalr1.cc  */
#line 248 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 7:

/* Line 678 of lalr1.cc  */
#line 250 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 8:

/* Line 678 of lalr1.cc  */
#line 252 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 9:

/* Line 678 of lalr1.cc  */
#line 254 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.component().addType((yysemantic_stack_[(1) - (1)].typeVal));
}
    break;

  case 10:

/* Line 678 of lalr1.cc  */
#line 258 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 11:

/* Line 678 of lalr1.cc  */
#line 260 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 12:

/* Line 678 of lalr1.cc  */
#line 262 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 13:

/* Line 678 of lalr1.cc  */
#line 269 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.component().setName((yysemantic_stack_[(5) - (2)].stringVal));
}
    break;

  case 15:

/* Line 678 of lalr1.cc  */
#line 276 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 16:

/* Line 678 of lalr1.cc  */
#line 281 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    if((yysemantic_stack_[(3) - (1)].stringVal) == "language") {
	driver.component().pluginLanguage = (yysemantic_stack_[(3) - (3)].stringVal);
    } else if((yysemantic_stack_[(3) - (1)].stringVal) == "version") {
	driver.component().version = (yysemantic_stack_[(3) - (3)].stringVal);
    } else {
      error(yyloc, std::string("Unknown component field: ") + (yysemantic_stack_[(3) - (1)].stringVal));
      YYERROR;
    }
}
    break;

  case 17:

/* Line 678 of lalr1.cc  */
#line 292 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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
#line 301 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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
#line 334 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Port::Ptr p(new Port((yysemantic_stack_[(3) - (3)].stringVal), (yysemantic_stack_[(3) - (2)].typeVal), true));
    driver.component().addPort(p);
}
    break;

  case 20:

/* Line 678 of lalr1.cc  */
#line 339 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Port::Ptr p(new Port((yysemantic_stack_[(3) - (3)].stringVal), (yysemantic_stack_[(3) - (2)].typeVal), false));
    driver.component().addPort(p);
}
    break;

  case 21:

/* Line 678 of lalr1.cc  */
#line 344 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Port::Ptr p(new Port((yysemantic_stack_[(10) - (3)].stringVal), (yysemantic_stack_[(10) - (2)].typeVal), false));
    if((yysemantic_stack_[(10) - (6)].stringVal) == "size")
      p->sizeCodel = (yysemantic_stack_[(10) - (8)].codelVal);
    else {
	error(yyloc, std::string("Unknown codel for an outport : ") + (yysemantic_stack_[(10) - (6)].stringVal));
	YYERROR;
    }
    driver.component().addPort(p);
}
    break;

  case 22:

/* Line 678 of lalr1.cc  */
#line 359 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Task::Ptr t(new Task((yysemantic_stack_[(2) - (2)].stringVal)));
    driver.setCurrentTask(t);
}
    break;

  case 23:

/* Line 678 of lalr1.cc  */
#line 364 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.component().addTask(driver.currentTask());
}
    break;

  case 25:

/* Line 678 of lalr1.cc  */
#line 371 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 26:

/* Line 678 of lalr1.cc  */
#line 375 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.currentTask()->addCodel((yysemantic_stack_[(4) - (2)].stringVal), (yysemantic_stack_[(4) - (4)].codelVal));
}
    break;

  case 27:

/* Line 678 of lalr1.cc  */
#line 379 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 28:

/* Line 678 of lalr1.cc  */
#line 396 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 29:

/* Line 678 of lalr1.cc  */
#line 413 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Service::Ptr s(new Service((yysemantic_stack_[(2) - (2)].stringVal)));
    driver.setCurrentService(s);
}
    break;

  case 30:

/* Line 678 of lalr1.cc  */
#line 418 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.component().addService(driver.currentService());
}
    break;

  case 32:

/* Line 678 of lalr1.cc  */
#line 425 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 33:

/* Line 678 of lalr1.cc  */
#line 429 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.currentService()->addCodel((yysemantic_stack_[(4) - (2)].stringVal), (yysemantic_stack_[(4) - (4)].codelVal));
}
    break;

  case 34:

/* Line 678 of lalr1.cc  */
#line 433 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.currentService()->addCodel((yysemantic_stack_[(4) - (2)].eventVal)->identifier(), (yysemantic_stack_[(4) - (4)].codelVal));
    driver.currentService()->addEvent((yysemantic_stack_[(4) - (2)].eventVal), (yysemantic_stack_[(4) - (2)].eventVal)->identifier());
}
    break;

  case 35:

/* Line 678 of lalr1.cc  */
#line 438 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
}
    break;

  case 36:

/* Line 678 of lalr1.cc  */
#line 441 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.currentService()->output = (yysemantic_stack_[(3) - (3)].serviceInputVal);
}
    break;

  case 37:

/* Line 678 of lalr1.cc  */
#line 445 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 38:

/* Line 678 of lalr1.cc  */
#line 488 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 39:

/* Line 678 of lalr1.cc  */
#line 500 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
   (yyval.stringVal) = (yysemantic_stack_[(1) - (1)].stringVal);
}
    break;

  case 40:

/* Line 678 of lalr1.cc  */
#line 504 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.stringVal) = (yysemantic_stack_[(3) - (1)].stringVal) + " " + (yysemantic_stack_[(3) - (3)].stringVal);
}
    break;

  case 41:

/* Line 678 of lalr1.cc  */
#line 510 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 42:

/* Line 678 of lalr1.cc  */
#line 512 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 43:

/* Line 678 of lalr1.cc  */
#line 516 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yysemantic_stack_[(3) - (1)].serviceInputVal).doc = (yysemantic_stack_[(3) - (3)].inputDocVal);
    driver.currentService()->addInput((yysemantic_stack_[(3) - (1)].serviceInputVal));
}
    break;

  case 44:

/* Line 678 of lalr1.cc  */
#line 521 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yysemantic_stack_[(5) - (1)].serviceInputVal).defaultValue = (yysemantic_stack_[(5) - (3)].literalVal);
    (yysemantic_stack_[(5) - (1)].serviceInputVal).doc = (yysemantic_stack_[(5) - (5)].inputDocVal);
    driver.currentService()->addInput((yysemantic_stack_[(5) - (1)].serviceInputVal));
}
    break;

  case 45:

/* Line 678 of lalr1.cc  */
#line 527 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yysemantic_stack_[(5) - (1)].serviceInputVal).defaultValue = (yysemantic_stack_[(5) - (5)].literalVal);
    (yysemantic_stack_[(5) - (1)].serviceInputVal).doc = (yysemantic_stack_[(5) - (3)].inputDocVal);
    driver.currentService()->addInput((yysemantic_stack_[(5) - (1)].serviceInputVal));
}
    break;

  case 46:

/* Line 678 of lalr1.cc  */
#line 533 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yysemantic_stack_[(3) - (1)].serviceInputVal).defaultValue = (yysemantic_stack_[(3) - (3)].literalVal);
    driver.currentService()->addInput((yysemantic_stack_[(3) - (1)].serviceInputVal));
}
    break;

  case 47:

/* Line 678 of lalr1.cc  */
#line 538 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.currentService()->addInput((yysemantic_stack_[(1) - (1)].serviceInputVal));
}
    break;

  case 48:

/* Line 678 of lalr1.cc  */
#line 544 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
  InputDoc d((yysemantic_stack_[(1) - (1)].stringVal));
  (yyval.inputDocVal) = d;
}
    break;

  case 49:

/* Line 678 of lalr1.cc  */
#line 549 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
  (yyval.inputDocVal) = (yysemantic_stack_[(3) - (2)].inputDocVal);
}
    break;

  case 50:

/* Line 678 of lalr1.cc  */
#line 555 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
  InputDoc d;
  d.members.push_back((yysemantic_stack_[(1) - (1)].inputDocVal));
  (yyval.inputDocVal) = d;
}
    break;

  case 51:

/* Line 678 of lalr1.cc  */
#line 561 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
  (yysemantic_stack_[(3) - (1)].inputDocVal).members.push_back((yysemantic_stack_[(3) - (3)].inputDocVal));
  (yyval.inputDocVal) = (yysemantic_stack_[(3) - (1)].inputDocVal);
}
    break;

  case 52:

/* Line 678 of lalr1.cc  */
#line 568 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 53:

/* Line 678 of lalr1.cc  */
#line 582 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    ServiceInput i;
    i.kind = ServiceInput::Type;
    i.identifier = (yysemantic_stack_[(2) - (2)].stringVal);
    i.type = (yysemantic_stack_[(2) - (1)].typeVal);
    (yyval.serviceInputVal) = i;
}
    break;

  case 54:

/* Line 678 of lalr1.cc  */
#line 593 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 55:

/* Line 678 of lalr1.cc  */
#line 614 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 56:

/* Line 678 of lalr1.cc  */
#line 651 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 58:

/* Line 678 of lalr1.cc  */
#line 673 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 59:

/* Line 678 of lalr1.cc  */
#line 675 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 60:

/* Line 678 of lalr1.cc  */
#line 679 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Event::Ptr ev(new NamedEvent((yysemantic_stack_[(1) - (1)].stringVal)));
    driver.component().addEvent(ev);
}
    break;

  case 61:

/* Line 678 of lalr1.cc  */
#line 684 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Event::Ptr ev(new NamedEvent((yysemantic_stack_[(3) - (1)].stringVal), (yysemantic_stack_[(3) - (3)].eventVal)));
    driver.component().addEvent(ev);
}
    break;

  case 62:

/* Line 678 of lalr1.cc  */
#line 694 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Codel::Ptr c(new Codel((yysemantic_stack_[(1) - (1)].stringVal)));
    driver.setCurrentCodel(c);
}
    break;

  case 63:

/* Line 678 of lalr1.cc  */
#line 699 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.codelVal) = driver.currentCodel();
}
    break;

  case 64:

/* Line 678 of lalr1.cc  */
#line 705 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 65:

/* Line 678 of lalr1.cc  */
#line 707 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.split((yysemantic_stack_[(2) - (2)].stringVal), driver.currentCodel()->nextCodels); 
}
    break;

  case 66:

/* Line 678 of lalr1.cc  */
#line 713 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 67:

/* Line 678 of lalr1.cc  */
#line 715 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 68:

/* Line 678 of lalr1.cc  */
#line 717 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 69:

/* Line 678 of lalr1.cc  */
#line 721 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 70:

/* Line 678 of lalr1.cc  */
#line 732 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 71:

/* Line 678 of lalr1.cc  */
#line 743 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.currentCodel()->addInPort((yysemantic_stack_[(2) - (2)].stringVal));
}
    break;

  case 72:

/* Line 678 of lalr1.cc  */
#line 747 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.currentCodel()->addOutPort((yysemantic_stack_[(2) - (2)].stringVal));
}
    break;

  case 73:

/* Line 678 of lalr1.cc  */
#line 755 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    ConstValue c((yysemantic_stack_[(5) - (3)].stringVal), (yysemantic_stack_[(5) - (2)].typeVal), (yysemantic_stack_[(5) - (5)].literalVal));
    driver.component().addConstValue(c);
}
    break;

  case 83:

/* Line 678 of lalr1.cc  */
#line 775 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = Literal((yysemantic_stack_[(1) - (1)].integerVal));
}
    break;

  case 84:

/* Line 678 of lalr1.cc  */
#line 779 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = Literal((yysemantic_stack_[(1) - (1)].doubleVal));
}
    break;

  case 85:

/* Line 678 of lalr1.cc  */
#line 783 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = Literal("\"" + (yysemantic_stack_[(1) - (1)].stringVal) + "\"");
}
    break;

  case 86:

/* Line 678 of lalr1.cc  */
#line 787 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = Literal((yysemantic_stack_[(1) - (1)].charVal));
}
    break;

  case 87:

/* Line 678 of lalr1.cc  */
#line 791 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = (yysemantic_stack_[(1) - (1)].literalVal);
}
    break;

  case 88:

/* Line 678 of lalr1.cc  */
#line 795 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    /*enum value*/
    (yyval.literalVal) = Literal((yysemantic_stack_[(1) - (1)].stringVal));
}
    break;

  case 89:

/* Line 678 of lalr1.cc  */
#line 800 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = (yysemantic_stack_[(3) - (2)].literalVal);
}
    break;

  case 90:

/* Line 678 of lalr1.cc  */
#line 804 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = (yysemantic_stack_[(3) - (2)].literalVal);
}
    break;

  case 91:

/* Line 678 of lalr1.cc  */
#line 808 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 92:

/* Line 678 of lalr1.cc  */
#line 812 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = Literal(true);
}
    break;

  case 93:

/* Line 678 of lalr1.cc  */
#line 816 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = Literal(false);
}
    break;

  case 94:

/* Line 678 of lalr1.cc  */
#line 823 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Neg);
    l.addMember((yysemantic_stack_[(2) - (2)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 95:

/* Line 678 of lalr1.cc  */
#line 829 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = (yysemantic_stack_[(2) - (2)].literalVal);
}
    break;

  case 96:

/* Line 678 of lalr1.cc  */
#line 833 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::LNeg);
    l.addMember((yysemantic_stack_[(2) - (2)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 97:

/* Line 678 of lalr1.cc  */
#line 840 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Plus);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 98:

/* Line 678 of lalr1.cc  */
#line 847 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Minus);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 99:

/* Line 678 of lalr1.cc  */
#line 854 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Times);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 100:

/* Line 678 of lalr1.cc  */
#line 861 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Divide);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 101:

/* Line 678 of lalr1.cc  */
#line 868 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Mod);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 102:

/* Line 678 of lalr1.cc  */
#line 875 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::And);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 103:

/* Line 678 of lalr1.cc  */
#line 882 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Or);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 104:

/* Line 678 of lalr1.cc  */
#line 889 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Xor);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 105:

/* Line 678 of lalr1.cc  */
#line 896 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::LShift);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 106:

/* Line 678 of lalr1.cc  */
#line 903 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::RShift);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 107:

/* Line 678 of lalr1.cc  */
#line 912 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Struct);
    l.addMember((yysemantic_stack_[(1) - (1)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 108:

/* Line 678 of lalr1.cc  */
#line 918 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yysemantic_stack_[(3) - (1)].literalVal).addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = (yysemantic_stack_[(3) - (1)].literalVal);
}
    break;

  case 109:

/* Line 678 of lalr1.cc  */
#line 927 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.component().addNativeTypeInclude((yysemantic_stack_[(6) - (3)].stringVal));
}
    break;

  case 110:

/* Line 678 of lalr1.cc  */
#line 933 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yysemantic_stack_[(2) - (1)].typeVal)->setNative(true);
    driver.component().addType((yysemantic_stack_[(2) - (1)].typeVal));
}
    break;

  case 111:

/* Line 678 of lalr1.cc  */
#line 938 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yysemantic_stack_[(3) - (2)].typeVal)->setNative(true);
    driver.component().addType((yysemantic_stack_[(3) - (2)].typeVal));
}
    break;

  case 112:

/* Line 678 of lalr1.cc  */
#line 945 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    IdlType::Ptr p(new TypedefType((yysemantic_stack_[(3) - (2)].typeVal), (yysemantic_stack_[(3) - (3)].declaratorVectVal)));
    (yyval.typeVal) = p;
}
    break;

  case 113:

/* Line 678 of lalr1.cc  */
#line 949 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 114:

/* Line 678 of lalr1.cc  */
#line 951 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 115:

/* Line 678 of lalr1.cc  */
#line 964 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal);
}
    break;

  case 116:

/* Line 678 of lalr1.cc  */
#line 968 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { 
    (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal);
}
    break;

  case 117:

/* Line 678 of lalr1.cc  */
#line 973 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 118:

/* Line 678 of lalr1.cc  */
#line 974 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 119:

/* Line 678 of lalr1.cc  */
#line 976 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    IdlType::Ptr type = driver.component().typeFromName((yysemantic_stack_[(1) - (1)].stringVal));
    if(!type) {
	error(yyloc, std::string("Unknown type: ") + (yysemantic_stack_[(1) - (1)].stringVal));
	YYERROR;
    }
    (yyval.typeVal) = type;
}
    break;

  case 120:

/* Line 678 of lalr1.cc  */
#line 986 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 121:

/* Line 678 of lalr1.cc  */
#line 987 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 122:

/* Line 678 of lalr1.cc  */
#line 988 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 123:

/* Line 678 of lalr1.cc  */
#line 989 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 124:

/* Line 678 of lalr1.cc  */
#line 990 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 125:

/* Line 678 of lalr1.cc  */
#line 991 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 126:

/* Line 678 of lalr1.cc  */
#line 992 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 127:

/* Line 678 of lalr1.cc  */
#line 998 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 128:

/* Line 678 of lalr1.cc  */
#line 999 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 129:

/* Line 678 of lalr1.cc  */
#line 1000 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 130:

/* Line 678 of lalr1.cc  */
#line 1001 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 131:

/* Line 678 of lalr1.cc  */
#line 1005 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 132:

/* Line 678 of lalr1.cc  */
#line 1007 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 133:

/* Line 678 of lalr1.cc  */
#line 1012 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { 
    Declarator::VectPtr v(new Declarator::Vect());
    v->push_back((yysemantic_stack_[(1) - (1)].declaratorVal));
    (yyval.declaratorVectVal) = v;
}
    break;

  case 134:

/* Line 678 of lalr1.cc  */
#line 1018 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 135:

/* Line 678 of lalr1.cc  */
#line 1029 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.declaratorVal) = (yysemantic_stack_[(1) - (1)].declaratorVal); }
    break;

  case 136:

/* Line 678 of lalr1.cc  */
#line 1030 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.declaratorVal) = (yysemantic_stack_[(1) - (1)].declaratorVal); }
    break;

  case 137:

/* Line 678 of lalr1.cc  */
#line 1035 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Declarator::Ptr p(new Declarator((yysemantic_stack_[(1) - (1)].stringVal)));
    (yyval.declaratorVal) = p;
}
    break;

  case 138:

/* Line 678 of lalr1.cc  */
#line 1042 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yysemantic_stack_[(2) - (1)].declaratorVal)->addBound((yysemantic_stack_[(2) - (2)].integerVal));
    (yyval.declaratorVal) = (yysemantic_stack_[(2) - (1)].declaratorVal);
}
    break;

  case 139:

/* Line 678 of lalr1.cc  */
#line 1047 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yysemantic_stack_[(2) - (1)].declaratorVal)->addBound((yysemantic_stack_[(2) - (2)].integerVal));
    (yyval.declaratorVal) = (yysemantic_stack_[(2) - (1)].declaratorVal);
}
    break;

  case 140:

/* Line 678 of lalr1.cc  */
#line 1054 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.integerVal) = (yysemantic_stack_[(3) - (2)].integerVal);
}
    break;

  case 141:

/* Line 678 of lalr1.cc  */
#line 1062 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::floatType; }
    break;

  case 142:

/* Line 678 of lalr1.cc  */
#line 1063 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::doubleType; }
    break;

  case 143:

/* Line 678 of lalr1.cc  */
#line 1064 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::longdoubleType; }
    break;

  case 144:

/* Line 678 of lalr1.cc  */
#line 1069 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    IdlType::Ptr p(new FixedType((yysemantic_stack_[(6) - (3)].integerVal), (yysemantic_stack_[(6) - (5)].integerVal)));
    (yyval.typeVal) = p;
}
    break;

  case 145:

/* Line 678 of lalr1.cc  */
#line 1074 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 146:

/* Line 678 of lalr1.cc  */
#line 1077 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 147:

/* Line 678 of lalr1.cc  */
#line 1078 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 148:

/* Line 678 of lalr1.cc  */
#line 1082 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 149:

/* Line 678 of lalr1.cc  */
#line 1083 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 150:

/* Line 678 of lalr1.cc  */
#line 1084 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 151:

/* Line 678 of lalr1.cc  */
#line 1088 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::shortType; }
    break;

  case 152:

/* Line 678 of lalr1.cc  */
#line 1092 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::longType; }
    break;

  case 153:

/* Line 678 of lalr1.cc  */
#line 1096 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::longlongType; }
    break;

  case 154:

/* Line 678 of lalr1.cc  */
#line 1100 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 155:

/* Line 678 of lalr1.cc  */
#line 1101 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 156:

/* Line 678 of lalr1.cc  */
#line 1102 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 157:

/* Line 678 of lalr1.cc  */
#line 1106 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::ushortType; }
    break;

  case 158:

/* Line 678 of lalr1.cc  */
#line 1110 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::ulongType; }
    break;

  case 159:

/* Line 678 of lalr1.cc  */
#line 1114 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::ulonglongType; }
    break;

  case 160:

/* Line 678 of lalr1.cc  */
#line 1118 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::charType; }
    break;

  case 161:

/* Line 678 of lalr1.cc  */
#line 1122 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::wcharType; }
    break;

  case 162:

/* Line 678 of lalr1.cc  */
#line 1126 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::booleanType; }
    break;

  case 163:

/* Line 678 of lalr1.cc  */
#line 1130 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::octetType; }
    break;

  case 164:

/* Line 678 of lalr1.cc  */
#line 1134 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::anyType; }
    break;

  case 165:

/* Line 678 of lalr1.cc  */
#line 1143 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    IdlType::Ptr p(new StringType((yysemantic_stack_[(4) - (3)].integerVal)));
    (yyval.typeVal) = p; 
}
    break;

  case 166:

/* Line 678 of lalr1.cc  */
#line 1148 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.typeVal) = StringType::unboundedStringType;
}
    break;

  case 167:

/* Line 678 of lalr1.cc  */
#line 1154 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { 
    IdlType::Ptr p(new WStringType((yysemantic_stack_[(4) - (3)].integerVal)));
    (yyval.typeVal) = p; 
}
    break;

  case 168:

/* Line 678 of lalr1.cc  */
#line 1159 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.typeVal) = WStringType::unboundedWStringType;
}
    break;

  case 169:

/* Line 678 of lalr1.cc  */
#line 1169 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 170:

/* Line 678 of lalr1.cc  */
#line 1183 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 171:

/* Line 678 of lalr1.cc  */
#line 1185 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 172:

/* Line 678 of lalr1.cc  */
#line 1189 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 173:

/* Line 678 of lalr1.cc  */
#line 1275 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 174:

/* Line 678 of lalr1.cc  */
#line 1288 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 175:

/* Line 678 of lalr1.cc  */
#line 1290 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 176:

/* Line 678 of lalr1.cc  */
#line 1294 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 177:

/* Line 678 of lalr1.cc  */
#line 1308 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    IdlType::Ptr p(new SequenceType((yysemantic_stack_[(6) - (3)].typeVal), (yysemantic_stack_[(6) - (5)].integerVal)));
    (yyval.typeVal) = p;
}
    break;

  case 178:

/* Line 678 of lalr1.cc  */
#line 1313 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    IdlType::Ptr p(new SequenceType((yysemantic_stack_[(4) - (3)].typeVal), 0));  
    (yyval.typeVal) = p;
}
    break;



/* Line 678 of lalr1.cc  */
#line 2088 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.cpp"
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
  const short int Parser::yypact_ninf_ = -262;
  const short int
  Parser::yypact_[] =
  {
       173,   -54,   -49,   -27,   239,   239,   -20,   -15,   174,   239,
      27,    57,   146,   104,   140,  -262,  -262,  -262,  -262,  -262,
    -262,  -262,  -262,  -262,  -262,   149,  -262,  -262,  -262,    96,
    -262,  -262,   162,  -262,  -262,   163,   168,  -262,  -262,  -262,
     186,   102,  -262,   125,  -262,  -262,  -262,  -262,  -262,  -262,
    -262,  -262,  -262,  -262,  -262,  -262,  -262,  -262,  -262,  -262,
    -262,  -262,  -262,  -262,  -262,  -262,  -262,  -262,  -262,   138,
     204,   206,   150,  -262,  -262,  -262,  -262,  -262,  -262,  -262,
    -262,  -262,   154,   199,   205,  -262,   157,  -262,  -262,   212,
    -262,   164,   216,   218,  -262,  -262,   183,   184,   187,   275,
    -262,   217,  -262,   238,   189,   239,   230,  -262,   237,  -262,
     178,   178,   202,    27,   269,  -262,   267,     7,   270,    23,
      29,   268,   273,   274,    42,  -262,   276,  -262,    15,  -262,
     154,   136,  -262,    13,   154,   223,  -262,  -262,   281,  -262,
    -262,     4,    31,  -262,   303,  -262,   253,   304,    16,   306,
     256,   307,   308,   202,   309,    26,   312,   265,  -262,  -262,
    -262,   271,   264,  -262,   189,    43,  -262,  -262,    13,    13,
    -262,  -262,  -262,  -262,  -262,  -262,  -262,    13,    13,    13,
     227,  -262,  -262,  -262,   250,   266,     2,   318,  -262,  -262,
    -262,   316,  -262,   320,    63,  -262,   322,  -262,   323,   203,
     203,   324,   -34,  -262,   327,  -262,   325,   326,   329,  -262,
    -262,   227,    97,    70,  -262,  -262,  -262,    13,    13,    13,
      13,    13,    13,    13,    13,    13,    13,  -262,   315,  -262,
     331,  -262,   282,   283,  -262,   316,  -262,   283,   328,   333,
    -262,    37,   284,  -262,   283,  -262,   316,  -262,  -262,  -262,
     283,  -262,    13,  -262,   191,   191,    49,    49,   123,    90,
      90,    90,   113,   113,   287,  -262,  -262,  -262,  -262,  -262,
     288,   203,     9,    13,  -262,  -262,   332,   227,  -262,   335,
    -262,  -262,     9,  -262,   334,    32,   343,   133,  -262,    98,
      13,     9,  -262,   292,   293,   294,   295,   132,  -262,  -262,
       9,   227,  -262,  -262,  -262,  -262,  -262,   133,   338,  -262,
    -262,   297,  -262,   316
  };

  /* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
     doesn't specify something else to do.  Zero means the default is an
     error.  */
  const unsigned char
  Parser::yydefact_[] =
  {
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     6,     5,     7,     8,    12,
      11,    10,     9,   113,   114,     0,    22,    29,   151,   152,
     141,   142,   145,   160,   161,   166,   168,   162,   163,   164,
       0,     0,   119,     0,   115,   117,   118,   116,   120,   130,
     121,   146,   148,   149,   150,   147,   154,   155,   156,   122,
     123,   124,   125,   126,   128,   129,   131,   132,   127,     0,
       0,     0,     0,    78,    81,    74,    75,    76,    77,    82,
      79,    80,     0,    60,    57,    58,     0,     1,     2,     0,
       3,     0,     0,     0,   153,   143,     0,     0,     0,     0,
     157,   158,    19,    20,     0,     0,     0,   137,   112,   133,
     135,   136,     0,     0,     0,     4,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   159,     0,   176,     0,   174,
       0,     0,   170,     0,     0,     0,   138,   139,    54,    61,
      59,     0,     0,    13,     0,    14,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   165,   167,
     178,     0,     0,   173,     0,     0,   169,   171,     0,     0,
      92,    93,    86,    83,    84,    85,    88,     0,     0,     0,
      73,    87,    91,   134,     0,     0,     0,     0,    17,    16,
      39,    18,    15,     0,     0,    23,     0,    24,     0,     0,
       0,     0,     0,    30,     0,    31,     0,     0,     0,   175,
     172,   107,     0,     0,    94,    95,    96,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   140,    55,   109,
       0,   110,     0,     0,    27,    28,    25,     0,     0,    35,
      41,    47,     0,    36,     0,    38,    37,    32,   144,   177,
       0,    89,     0,    90,    98,    97,   100,    99,   101,   104,
     103,   102,   106,   105,     0,   111,    40,    62,    26,    33,
       0,     0,     0,     0,    53,    34,     0,   108,    56,     0,
      52,    42,     0,    48,    43,    46,     0,    66,    50,     0,
       0,     0,    21,     0,     0,     0,     0,     0,    67,    49,
       0,    45,    44,    69,    70,    71,    72,     0,    64,    51,
      68,     0,    63,    65
  };

  /* YYPGOTO[NTERM-NUM].  */
  const short int
  Parser::yypgoto_[] =
  {
      -262,  -262,  -262,   345,  -262,  -262,   242,  -262,  -262,  -262,
    -262,   213,  -262,  -262,  -262,   207,  -190,  -262,    89,  -261,
    -262,   165,   210,  -262,  -262,   251,  -206,  -262,  -262,  -262,
      59,  -262,  -262,  -160,  -262,  -262,  -262,  -262,  -262,  -136,
      -2,   272,  -262,  -262,  -262,   240,   233,  -262,  -262,   257,
     361,   364,   365,  -262,  -262,  -262,  -262,  -262,  -262,  -262,
    -262,   366,   367,   368,   369,  -262,   370,   371,     0,  -262,
     249,     1,  -262,   219,  -262
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const short int
  Parser::yydefgoto_[] =
  {
        -1,    12,    13,    14,    15,   117,   118,    16,    17,    92,
     148,   149,    18,    93,   155,   156,   191,   239,   240,   284,
     289,   241,   139,    19,    84,    85,   268,   279,   312,   297,
     298,    20,    72,   180,   181,   182,   212,    21,   186,    22,
     242,    44,    45,    46,    47,   108,   109,   110,   111,   136,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,   131,
     132,    67,   128,   129,    68
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If zero, do what YYDEFACT says.  */
  const signed char Parser::yytable_ninf_ = -1;
  const short int
  Parser::yytable_[] =
  {
        23,    24,    43,    69,   235,   187,    25,    82,   211,   213,
     229,    26,   246,    23,    24,   143,   282,   214,   215,   216,
     168,   288,   146,   163,   195,   245,   190,   169,   164,   146,
     302,   269,   150,    27,   203,   150,   170,   171,   275,   309,
      70,     6,   291,     6,   276,    71,     7,   272,     7,     9,
     230,     9,   210,   273,   160,   161,   134,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   116,   283,   172,
     173,   174,   175,   176,   177,   178,   147,   151,   152,   153,
     151,   152,   153,   147,   179,   253,   154,    83,   188,   154,
     189,   190,   277,   217,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   130,    88,   251,   299,     1,     2,     3,
     252,   300,    86,   285,   221,   222,   223,   224,   225,   226,
     234,   313,    94,   190,    95,     4,     5,   100,   101,   130,
     301,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,    23,    24,     6,   166,   307,    87,   308,     7,    90,
       8,     9,   293,   294,   295,   296,    91,    10,    11,   225,
     226,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    96,    97,     6,     1,     2,     3,    98,
       7,    40,    -1,    -1,    41,   102,    23,    24,    -1,   222,
     223,   224,   225,   226,     4,     5,    42,    99,   103,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
     106,   104,     6,   105,   107,   112,   114,     7,   113,     8,
       9,   115,    41,   119,   116,   120,    10,    11,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
     121,   122,     6,   125,   123,   126,   133,     7,    40,   127,
     134,    41,   135,   238,   219,   220,   221,   222,   223,   224,
     225,   226,   138,    42,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,   141,   142,     6,   145,
     184,   157,   162,     7,    40,   158,   159,    41,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   185,    42,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,   192,   193,   194,   197,   198,   199,   200,   202,
      40,   205,   206,    41,   208,   227,   228,   231,   207,   232,
     233,   236,   264,   237,   244,    42,   247,   248,   249,   250,
     265,   286,   266,   267,   274,   270,   271,   278,   280,   287,
     290,   292,   303,   304,   305,   306,   311,   190,    89,   144,
     281,   196,   204,   201,   140,   243,   310,   183,   137,    73,
     165,   124,    74,    75,    76,    77,    78,    79,    80,    81,
     167,     0,     0,   209
  };

  /* YYCHECK.  */
  const short int
  Parser::yycheck_[] =
  {
         0,     0,     4,     5,   194,   141,    60,     9,   168,   169,
       8,    60,   202,    13,    13,     8,     7,   177,   178,   179,
       7,   282,     6,     8,     8,    59,    60,    14,    13,     6,
     291,   237,     6,    60,     8,     6,    23,    24,   244,   300,
      60,    39,    10,    39,   250,    60,    44,    10,    44,    47,
     186,    47,     9,    16,    12,    13,    13,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,    60,    59,    56,
      57,    58,    59,    60,    61,    62,    60,    51,    52,    53,
      51,    52,    53,    60,    71,    15,    60,    60,    57,    60,
      59,    60,   252,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,   105,     0,     8,     8,     3,     4,     5,
      13,    13,    55,   273,    65,    66,    67,    68,    69,    70,
      57,   311,    26,    60,    28,    21,    22,    25,    26,   131,
     290,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,   141,   141,    39,     8,    13,     0,    15,    44,     9,
      46,    47,    19,    20,    21,    22,     7,    53,    54,    69,
      70,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    11,    11,    39,     3,     4,     5,    11,
      44,    45,    69,    70,    48,    60,   186,   186,    65,    66,
      67,    68,    69,    70,    21,    22,    60,    11,    60,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      60,     7,    39,     7,    60,    16,    59,    44,    13,    46,
      47,     9,    48,     7,    60,     7,    53,    54,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      57,    57,    39,    26,    57,     7,    16,    44,    45,    60,
      13,    48,    74,    50,    63,    64,    65,    66,    67,    68,
      69,    70,    60,    60,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,     7,    10,    39,     9,
      57,    13,     6,    44,    45,    12,    12,    48,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    17,    60,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,     9,    60,    10,     9,    60,    10,    10,    10,
      45,     9,    57,    48,    60,    75,    60,     9,    57,    13,
      10,     9,    17,    10,    10,    60,     9,    12,    12,    10,
       9,     9,    60,    60,    60,    17,    13,    60,    60,    14,
      16,     8,    60,    60,    60,    60,    18,    60,    13,   117,
     271,   148,   155,   153,   113,   200,   307,   134,   111,     8,
     130,    99,     8,     8,     8,     8,     8,     8,     8,     8,
     131,    -1,    -1,   164
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  Parser::yystos_[] =
  {
         0,     3,     4,     5,    21,    22,    39,    44,    46,    47,
      53,    54,    77,    78,    79,    80,    83,    84,    88,    99,
     107,   113,   115,   144,   147,    60,    60,    60,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      45,    48,    60,   116,   117,   118,   119,   120,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   147,   150,   116,
      60,    60,   108,   126,   127,   128,   137,   138,   139,   140,
     142,   143,   116,    60,   100,   101,    55,     0,     0,    79,
       9,     7,    85,    89,    26,    28,    11,    11,    11,    11,
      25,    26,    60,    60,     7,     7,    60,    60,   121,   122,
     123,   124,    16,    13,    59,     9,    60,    81,    82,     7,
       7,    57,    57,    57,   117,    26,     7,    60,   148,   149,
     116,   145,   146,    16,    13,    74,   125,   125,    60,    98,
     101,     7,    10,     8,    82,     9,     6,    60,    86,    87,
       6,    51,    52,    53,    60,    90,    91,    13,    12,    12,
      12,    13,     6,     8,    13,   121,     8,   146,     7,    14,
      23,    24,    56,    57,    58,    59,    60,    61,    62,    71,
     109,   110,   111,   122,    57,    17,   114,   115,    57,    59,
      60,    92,     9,    60,    10,     8,    87,     9,    60,    10,
      10,    98,    10,     8,    91,     9,    57,    57,    60,   149,
       9,   109,   112,   109,   109,   109,   109,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    75,    60,     8,
     115,     9,    13,    10,    57,    92,     9,    10,    50,    93,
      94,    97,   116,    97,    10,    59,    92,     9,    12,    12,
      10,     8,    13,    15,   109,   109,   109,   109,   109,   109,
     109,   109,   109,   109,    17,     9,    60,    60,   102,   102,
      17,    13,    10,    16,    60,   102,   102,   109,    60,   103,
      60,    94,     7,    59,    95,   109,     9,    14,    95,    96,
      16,    10,     8,    19,    20,    21,    22,   105,   106,     8,
      13,   109,    95,    60,    60,    60,    60,    13,    15,    95,
     106,    18,   104,    92
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
     325,   326,   327,   328,    91,    93
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  Parser::yyr1_[] =
  {
         0,    76,    77,    78,    78,    79,    79,    79,    79,    79,
      79,    79,    79,    80,    81,    81,    82,    82,    82,    83,
      83,    83,    85,    84,    86,    86,    87,    87,    87,    89,
      88,    90,    90,    91,    91,    91,    91,    91,    91,    92,
      92,    93,    93,    94,    94,    94,    94,    94,    95,    95,
      96,    96,    97,    97,    98,    98,    98,    99,   100,   100,
     101,   101,   103,   102,   104,   104,   105,   105,   105,   106,
     106,   106,   106,   107,   108,   108,   108,   108,   108,   108,
     108,   108,   108,   109,   109,   109,   109,   109,   109,   109,
     109,   109,   110,   110,   111,   111,   111,   111,   111,   111,
     111,   111,   111,   111,   111,   111,   111,   112,   112,   113,
     114,   114,   115,   115,   115,   116,   116,   117,   117,   117,
     118,   118,   118,   118,   118,   118,   118,   119,   119,   119,
     119,   120,   120,   121,   121,   122,   122,   123,   124,   124,
     125,   126,   126,   126,   127,   127,   128,   128,   129,   129,
     129,   130,   131,   132,   133,   133,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   142,   143,   143,   144,
     145,   145,   146,   147,   148,   148,   149,   150,   150
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  Parser::yyr2_[] =
  {
         0,     2,     2,     2,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     5,     2,     3,     3,     3,     3,     3,
       3,    10,     0,     6,     2,     3,     4,     3,     3,     0,
       6,     2,     3,     4,     4,     3,     3,     3,     3,     1,
       3,     1,     3,     3,     5,     5,     3,     1,     1,     3,
       1,     3,     3,     2,     1,     3,     5,     2,     1,     3,
       1,     3,     0,     6,     0,     2,     0,     1,     3,     2,
       2,     2,     2,     5,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       3,     1,     1,     1,     2,     2,     2,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     1,     3,     6,
       2,     3,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     1,     1,     1,     2,     2,
       3,     1,     1,     2,     6,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     1,     1,     1,     2,     2,     3,
       1,     1,     1,     1,     1,     4,     1,     4,     1,     5,
       1,     2,     3,     5,     1,     3,     1,     6,     4
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
  "LARROW", "IN", "OUT", "INPORT", "OUTPORT", "TRUE", "FALSE", "SHORT",
  "LONG", "FLOAT", "DOUBLE", "FIXED", "CHAR", "WCHAR", "STRING", "WSTRING",
  "BOOLEAN", "OCTET", "ANY", "VOID", "NATIVE", "ENUM", "UNION", "SWITCH",
  "CASE", "DEFAULT", "STRUCT", "SEQUENCE", "CONST", "TYPEDEF", "UNSIGNED",
  "OBJECT", "IDS", "INPUT", "OUTPUT", "EVENT", "IMPORT", "FROM",
  "\"char\"", "\"integer\"", "\"double\"", "\"string literal\"",
  "\"identifier\"", "MINUS", "PLUS", "SLASH", "TIMES", "MOD", "XOR", "OR",
  "AND", "RSHIFT", "LSHIFT", "TILDE", "POS", "NEG", "'['", "']'",
  "$accept", "start", "declarations", "declaration", "component_decl",
  "component_fields", "component_field", "port_decl", "task_decl", "$@1",
  "task_fields", "task_field", "service_decl", "$@2", "service_fields",
  "service_field", "identifiers", "inputs", "input", "input_doc",
  "input_doc_list", "input_type", "event_spec", "event_decl",
  "event_declarators", "event_declarator", "codel_prototype", "$@3",
  "codel_next", "codel_fields", "codel_field", "const_decl", "const_type",
  "literal", "boolean_literal", "composed_literal", "literals",
  "native_type_decl", "type_decl_list", "type_decl", "type_spec",
  "simple_type_spec", "base_type_spec", "template_type_spec",
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
        77,     0,    -1,    78,     0,    -1,    79,     9,    -1,    78,
      79,     9,    -1,    83,    -1,    80,    -1,    84,    -1,    88,
      -1,   115,    -1,   113,    -1,   107,    -1,    99,    -1,     3,
      60,     7,    81,     8,    -1,    82,     9,    -1,    81,    82,
       9,    -1,    60,    10,    59,    -1,    60,    10,    57,    -1,
      60,    10,    92,    -1,    21,   116,    60,    -1,    22,   116,
      60,    -1,    22,   116,    60,     7,     6,    60,    10,   102,
       9,     8,    -1,    -1,     4,    60,    85,     7,    86,     8,
      -1,    87,     9,    -1,    86,    87,     9,    -1,     6,    60,
      10,   102,    -1,    60,    10,    57,    -1,    60,    10,    92,
      -1,    -1,     5,    60,    89,     7,    90,     8,    -1,    91,
       9,    -1,    90,    91,     9,    -1,     6,    60,    10,   102,
      -1,    53,    98,    10,   102,    -1,    51,    10,    93,    -1,
      52,    10,    97,    -1,    60,    10,    92,    -1,    60,    10,
      59,    -1,    60,    -1,    92,    13,    60,    -1,    94,    -1,
      93,    13,    94,    -1,    97,    10,    95,    -1,    97,    16,
     109,    10,    95,    -1,    97,    10,    95,    16,   109,    -1,
      97,    16,   109,    -1,    97,    -1,    59,    -1,     7,    96,
       8,    -1,    95,    -1,    96,    13,    95,    -1,    50,    17,
      60,    -1,   116,    60,    -1,    60,    -1,    60,    17,    60,
      -1,    60,    17,    60,    17,    60,    -1,    53,   100,    -1,
     101,    -1,   100,    13,   101,    -1,    60,    -1,    60,    16,
      98,    -1,    -1,    60,   103,    14,   105,    15,   104,    -1,
      -1,    18,    92,    -1,    -1,   106,    -1,   105,    13,   106,
      -1,    19,    60,    -1,    20,    60,    -1,    21,    60,    -1,
      22,    60,    -1,    46,   108,    60,    16,   109,    -1,   128,
      -1,   137,    -1,   138,    -1,   139,    -1,   126,    -1,   142,
      -1,   143,    -1,   127,    -1,   140,    -1,    57,    -1,    58,
      -1,    59,    -1,    56,    -1,   110,    -1,    60,    -1,     7,
     112,     8,    -1,    14,   109,    15,    -1,   111,    -1,    23,
      -1,    24,    -1,    61,   109,    -1,    62,   109,    -1,    71,
     109,    -1,   109,    62,   109,    -1,   109,    61,   109,    -1,
     109,    64,   109,    -1,   109,    63,   109,    -1,   109,    65,
     109,    -1,   109,    68,   109,    -1,   109,    67,   109,    -1,
     109,    66,   109,    -1,   109,    70,   109,    -1,   109,    69,
     109,    -1,   109,    -1,   112,    13,   109,    -1,    54,    55,
      59,     7,   114,     8,    -1,   115,     9,    -1,   114,   115,
       9,    -1,    47,   116,   121,    -1,   144,    -1,   147,    -1,
     117,    -1,   120,    -1,   118,    -1,   119,    -1,    60,    -1,
     126,    -1,   128,    -1,   137,    -1,   138,    -1,   139,    -1,
     140,    -1,   141,    -1,   150,    -1,   142,    -1,   143,    -1,
     127,    -1,   144,    -1,   147,    -1,   122,    -1,   121,    13,
     122,    -1,   123,    -1,   124,    -1,    60,    -1,   123,   125,
      -1,   124,   125,    -1,    74,    57,    75,    -1,    27,    -1,
      28,    -1,    26,    28,    -1,    29,    11,    57,    13,    57,
      12,    -1,    29,    -1,   129,    -1,   133,    -1,   130,    -1,
     131,    -1,   132,    -1,    25,    -1,    26,    -1,    26,    26,
      -1,   134,    -1,   135,    -1,   136,    -1,    48,    25,    -1,
      48,    26,    -1,    48,    26,    26,    -1,    30,    -1,    31,
      -1,    34,    -1,    35,    -1,    36,    -1,    32,    11,    57,
      12,    -1,    32,    -1,    33,    11,    57,    12,    -1,    33,
      -1,    44,    60,     7,   145,     8,    -1,   146,    -1,   145,
     146,    -1,   116,   121,     9,    -1,    39,    60,     7,   148,
       8,    -1,   149,    -1,   148,    13,   149,    -1,    60,    -1,
      45,    11,   117,    13,    57,    12,    -1,    45,    11,   117,
      12,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned short int
  Parser::yyprhs_[] =
  {
         0,     0,     3,     6,     9,    13,    15,    17,    19,    21,
      23,    25,    27,    29,    35,    38,    42,    46,    50,    54,
      58,    62,    73,    74,    81,    84,    88,    93,    97,   101,
     102,   109,   112,   116,   121,   126,   130,   134,   138,   142,
     144,   148,   150,   154,   158,   164,   170,   174,   176,   178,
     182,   184,   188,   192,   195,   197,   201,   207,   210,   212,
     216,   218,   222,   223,   230,   231,   234,   235,   237,   241,
     244,   247,   250,   253,   259,   261,   263,   265,   267,   269,
     271,   273,   275,   277,   279,   281,   283,   285,   287,   289,
     293,   297,   299,   301,   303,   306,   309,   312,   316,   320,
     324,   328,   332,   336,   340,   344,   348,   352,   354,   358,
     365,   368,   372,   376,   378,   380,   382,   384,   386,   388,
     390,   392,   394,   396,   398,   400,   402,   404,   406,   408,
     410,   412,   414,   416,   418,   422,   424,   426,   428,   431,
     434,   438,   440,   442,   445,   452,   454,   456,   458,   460,
     462,   464,   466,   468,   471,   473,   475,   477,   480,   483,
     487,   489,   491,   493,   495,   497,   502,   504,   509,   511,
     517,   519,   522,   526,   532,   534,   538,   540,   547
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned short int
  Parser::yyrline_[] =
  {
         0,   236,   236,   239,   240,   245,   247,   249,   251,   253,
     257,   259,   261,   268,   274,   275,   280,   291,   300,   333,
     338,   343,   359,   358,   369,   370,   374,   378,   395,   413,
     412,   423,   424,   428,   432,   437,   440,   444,   487,   499,
     503,   509,   511,   515,   520,   526,   532,   537,   543,   548,
     554,   560,   567,   581,   592,   613,   650,   669,   672,   674,
     678,   683,   694,   693,   705,   706,   713,   714,   716,   720,
     731,   742,   746,   754,   761,   762,   763,   764,   765,   766,
     767,   768,   769,   774,   778,   782,   786,   790,   794,   799,
     803,   807,   811,   815,   822,   828,   832,   839,   846,   853,
     860,   867,   874,   881,   888,   895,   902,   911,   917,   926,
     932,   937,   944,   949,   951,   963,   967,   973,   974,   975,
     986,   987,   988,   989,   990,   991,   992,   998,   999,  1000,
    1001,  1005,  1007,  1011,  1017,  1029,  1030,  1034,  1041,  1046,
    1053,  1062,  1063,  1064,  1068,  1073,  1077,  1078,  1082,  1083,
    1084,  1088,  1092,  1096,  1100,  1101,  1102,  1106,  1110,  1114,
    1118,  1122,  1126,  1130,  1134,  1142,  1147,  1153,  1158,  1168,
    1182,  1184,  1188,  1274,  1288,  1289,  1293,  1307,  1312
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
       2,    74,     2,    75,     2,     2,     2,     2,     2,     2,
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
      65,    66,    67,    68,    69,    70,    71,    72,    73
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int Parser::yyeof_ = 0;
  const int Parser::yylast_ = 383;
  const int Parser::yynnts_ = 75;
  const int Parser::yyempty_ = -2;
  const int Parser::yyfinal_ = 87;
  const int Parser::yyterror_ = 1;
  const int Parser::yyerrcode_ = 256;
  const int Parser::yyntokens_ = 76;

  const unsigned int Parser::yyuser_token_number_max_ = 328;
  const Parser::token_number_type Parser::yyundef_token_ = 2;


/* Line 1054 of lalr1.cc  */
#line 1 "[Bison:b4_percent_define_default]"

} // G3nom

/* Line 1054 of lalr1.cc  */
#line 2842 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.cpp"


/* Line 1056 of lalr1.cc  */
#line 1318 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
 /*** Additional Code ***/

void G3nom::Parser::error(const Parser::location_type& l,
			    const std::string& m)
{
    driver.error(l, m);
}


