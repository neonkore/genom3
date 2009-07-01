
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
#line 218 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"


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
#line 96 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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
#line 239 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 5:

/* Line 678 of lalr1.cc  */
#line 244 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 6:

/* Line 678 of lalr1.cc  */
#line 246 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 7:

/* Line 678 of lalr1.cc  */
#line 248 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 8:

/* Line 678 of lalr1.cc  */
#line 250 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 9:

/* Line 678 of lalr1.cc  */
#line 252 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.component().addType((yysemantic_stack_[(1) - (1)].typeVal));
}
    break;

  case 10:

/* Line 678 of lalr1.cc  */
#line 256 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 11:

/* Line 678 of lalr1.cc  */
#line 258 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 12:

/* Line 678 of lalr1.cc  */
#line 265 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.component().setName((yysemantic_stack_[(5) - (2)].stringVal));
}
    break;

  case 14:

/* Line 678 of lalr1.cc  */
#line 272 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 15:

/* Line 678 of lalr1.cc  */
#line 277 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 16:

/* Line 678 of lalr1.cc  */
#line 288 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    if((yysemantic_stack_[(3) - (1)].stringVal) == "uniqueId") {
	driver.component().uniqueId = (yysemantic_stack_[(3) - (3)].integerVal);
    } else {
      error(yyloc, std::string("Unknown component field: ") + (yysemantic_stack_[(3) - (1)].stringVal));
      YYERROR;
    }}
    break;

  case 17:

/* Line 678 of lalr1.cc  */
#line 296 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    if((yysemantic_stack_[(3) - (1)].stringVal) == "ids") {
	Idl::IdlType::Ptr p = driver.component().typeFromName((yysemantic_stack_[(3) - (3)].stringVal));
	if(!p.get()) {
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
    break;

  case 18:

/* Line 678 of lalr1.cc  */
#line 316 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Idl::IdlType::Ptr type = driver.component().typeFromName((yysemantic_stack_[(3) - (2)].stringVal));
    if(!type.get()) {
	error(yyloc, std::string("Unknown type: ") + (yysemantic_stack_[(3) - (2)].stringVal));
	YYERROR;
    }
    Port::Ptr p(new Port((yysemantic_stack_[(3) - (3)].stringVal), type, true));
    driver.component().addPort(p);
}
    break;

  case 19:

/* Line 678 of lalr1.cc  */
#line 326 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Idl::IdlType::Ptr type = driver.component().typeFromName((yysemantic_stack_[(3) - (2)].stringVal));
    if(!type.get()) {
	error(yyloc, std::string("Unknown type: ") + (yysemantic_stack_[(3) - (2)].stringVal));
	YYERROR;
    }
    Port::Ptr p(new Port((yysemantic_stack_[(3) - (3)].stringVal), type, false));
    driver.component().addPort(p);
}
    break;

  case 20:

/* Line 678 of lalr1.cc  */
#line 336 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Idl::IdlType::Ptr type = driver.component().typeFromName((yysemantic_stack_[(10) - (2)].stringVal));
    if(!type.get()) {
	error(yyloc, std::string("Unknown type: ") + (yysemantic_stack_[(10) - (2)].stringVal));
	YYERROR;
    }
    Port::Ptr p(new Port((yysemantic_stack_[(10) - (3)].stringVal), type, false));
    if((yysemantic_stack_[(10) - (6)].stringVal) == "size")
      p->sizeCodel = (yysemantic_stack_[(10) - (8)].codelVal);
    else {
	error(yyloc, std::string("Unknown codel for an outport : ") + (yysemantic_stack_[(10) - (6)].stringVal));
	YYERROR;
    }
    driver.component().addPort(p);
}
    break;

  case 21:

/* Line 678 of lalr1.cc  */
#line 356 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Task::Ptr t(new Task((yysemantic_stack_[(2) - (2)].stringVal)));
    driver.setCurrentTask(t);
}
    break;

  case 22:

/* Line 678 of lalr1.cc  */
#line 361 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.component().addTask(driver.currentTask());
}
    break;

  case 24:

/* Line 678 of lalr1.cc  */
#line 368 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 25:

/* Line 678 of lalr1.cc  */
#line 372 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.currentTask()->addCodel((yysemantic_stack_[(4) - (2)].stringVal), (yysemantic_stack_[(4) - (4)].codelVal));
}
    break;

  case 26:

/* Line 678 of lalr1.cc  */
#line 376 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 27:

/* Line 678 of lalr1.cc  */
#line 393 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    if((yysemantic_stack_[(3) - (1)].stringVal) != "errors") {
      error(yyloc, std::string("Wrong arguments for field: ") + (yysemantic_stack_[(3) - (1)].stringVal));
      YYERROR;
    }
}
    break;

  case 28:

/* Line 678 of lalr1.cc  */
#line 402 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.currentTask()->addErrorMessage((yysemantic_stack_[(1) - (1)].stringVal));
}
    break;

  case 29:

/* Line 678 of lalr1.cc  */
#line 406 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.currentTask()->addErrorMessage((yysemantic_stack_[(2) - (2)].stringVal));
}
    break;

  case 30:

/* Line 678 of lalr1.cc  */
#line 414 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Service::Ptr s(new Service((yysemantic_stack_[(2) - (2)].stringVal)));
    driver.setCurrentService(s);
}
    break;

  case 31:

/* Line 678 of lalr1.cc  */
#line 419 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.component().addService(driver.currentService());
}
    break;

  case 33:

/* Line 678 of lalr1.cc  */
#line 426 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 34:

/* Line 678 of lalr1.cc  */
#line 430 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.currentService()->addCodel((yysemantic_stack_[(4) - (2)].stringVal), (yysemantic_stack_[(4) - (4)].codelVal));
}
    break;

  case 35:

/* Line 678 of lalr1.cc  */
#line 434 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.currentService()->addCodel((yysemantic_stack_[(4) - (2)].eventVal)->identifier(), (yysemantic_stack_[(4) - (4)].codelVal));
    driver.currentService()->addEvent((yysemantic_stack_[(4) - (2)].eventVal), (yysemantic_stack_[(4) - (2)].eventVal)->identifier());
}
    break;

  case 36:

/* Line 678 of lalr1.cc  */
#line 439 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
}
    break;

  case 37:

/* Line 678 of lalr1.cc  */
#line 442 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.currentService()->output = (yysemantic_stack_[(3) - (3)].serviceInputVal);
}
    break;

  case 38:

/* Line 678 of lalr1.cc  */
#line 446 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Service::Ptr s = driver.currentService();

    if((yysemantic_stack_[(4) - (4)].stringVal) == "") {
	if((yysemantic_stack_[(4) - (1)].stringVal) == "type") {
	    if((yysemantic_stack_[(4) - (3)].stringVal) == "init")
	      s->type = Service::Init;
	    else if((yysemantic_stack_[(4) - (3)].stringVal) == "control")
	      s->type = Service::Control;
	    else if((yysemantic_stack_[(4) - (3)].stringVal) == "exec")
	      s->type = Service::Exec;
	    else {
	      error(yyloc, std::string("Unknown service type: ") + (yysemantic_stack_[(4) - (3)].stringVal));
	      YYERROR;
	    }
	} else if((yysemantic_stack_[(4) - (1)].stringVal) == "taskName") {
	    s->taskName = (yysemantic_stack_[(4) - (3)].stringVal);
	} else if((yysemantic_stack_[(4) - (1)].stringVal) == "errors") {
	    driver.currentService()->addErrorMessage((yysemantic_stack_[(4) - (3)].stringVal));
	} else if((yysemantic_stack_[(4) - (1)].stringVal) == "interrupts") {
	    driver.currentService()->addIncompatibleService((yysemantic_stack_[(4) - (3)].stringVal));
	} else {
	  error(yyloc, std::string("Unknown service field: ") + (yysemantic_stack_[(4) - (1)].stringVal));
	  YYERROR;
	}
    } else {
	std::vector<std::string> ids;
	driver.split((yysemantic_stack_[(4) - (4)].stringVal), ids);
	std::vector<std::string>::const_iterator it = ids.begin();

	if((yysemantic_stack_[(4) - (1)].stringVal) == "errors") {
	    driver.currentService()->addErrorMessage((yysemantic_stack_[(4) - (3)].stringVal));
	    for(; it != ids.end(); ++it)
		driver.currentService()->addErrorMessage(*it);
	} else if ((yysemantic_stack_[(4) - (1)].stringVal) == "interrupts") {
	    driver.currentService()->addIncompatibleService((yysemantic_stack_[(4) - (3)].stringVal));
	    for(; it != ids.end(); ++it)
		driver.currentService()->addIncompatibleService(*it);
	} else {
	  error(yyloc, std::string("Unknown service field (or wrong number of arguments): ") + (yysemantic_stack_[(4) - (1)].stringVal));
	  YYERROR;
	}
    }
}
    break;

  case 39:

/* Line 678 of lalr1.cc  */
#line 491 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 40:

/* Line 678 of lalr1.cc  */
#line 501 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 41:

/* Line 678 of lalr1.cc  */
#line 505 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.stringVal) = "";
}
    break;

  case 42:

/* Line 678 of lalr1.cc  */
#line 509 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.stringVal) = (yysemantic_stack_[(1) - (1)].stringVal);
}
    break;

  case 43:

/* Line 678 of lalr1.cc  */
#line 515 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
   (yyval.stringVal) = (yysemantic_stack_[(1) - (1)].stringVal);
}
    break;

  case 44:

/* Line 678 of lalr1.cc  */
#line 519 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.stringVal) = (yysemantic_stack_[(2) - (1)].stringVal) + " " + (yysemantic_stack_[(2) - (2)].stringVal);
}
    break;

  case 45:

/* Line 678 of lalr1.cc  */
#line 525 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 46:

/* Line 678 of lalr1.cc  */
#line 527 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 47:

/* Line 678 of lalr1.cc  */
#line 531 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yysemantic_stack_[(3) - (1)].serviceInputVal).defaultValue = (yysemantic_stack_[(3) - (3)].literalVal);
    driver.currentService()->addInput((yysemantic_stack_[(3) - (1)].serviceInputVal));
}
    break;

  case 48:

/* Line 678 of lalr1.cc  */
#line 536 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.currentService()->addInput((yysemantic_stack_[(1) - (1)].serviceInputVal));
}
    break;

  case 49:

/* Line 678 of lalr1.cc  */
#line 542 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    // check if the name given is in the ids
    Idl::IdlType::Ptr t = driver.component().typeFromIdsName((yysemantic_stack_[(3) - (3)].stringVal));
    if(!t.get()) {
      error(yyloc, std::string("Input is not in the IDS: ") + (yysemantic_stack_[(3) - (3)].stringVal));
      YYERROR;
    }

    Service::Input i;
    i.kind = Service::Input::IDSMember;
    i.identifier = (yysemantic_stack_[(3) - (3)].stringVal);
    (yyval.serviceInputVal) = i;
}
    break;

  case 50:

/* Line 678 of lalr1.cc  */
#line 556 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Service::Input i;
    i.kind = Service::Input::Type;
    i.identifier = (yysemantic_stack_[(2) - (2)].stringVal);
    i.type = (yysemantic_stack_[(2) - (1)].typeVal);
    (yyval.serviceInputVal) = i;
}
    break;

  case 51:

/* Line 678 of lalr1.cc  */
#line 567 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Event::Ptr ev = driver.component().event((yysemantic_stack_[(1) - (1)].stringVal));
    if(ev.get()) { // external event
      Event::Ptr e(new NamedEvent((yysemantic_stack_[(1) - (1)].stringVal), ev));
      (yyval.eventVal) = e;
    } else { // service event
      Service::Ptr s = driver.currentService();

      if((yysemantic_stack_[(1) - (1)].stringVal) == "onStart") 
	(yyval.eventVal) = Event::Ptr(new ServiceEvent(s->name, ServiceEvent::OnStart));
      else if((yysemantic_stack_[(1) - (1)].stringVal) == "onEnd") 
	(yyval.eventVal) = Event::Ptr(new ServiceEvent(s->name, ServiceEvent::OnEnd));
      else if((yysemantic_stack_[(1) - (1)].stringVal) == "onInter") 
	(yyval.eventVal) = Event::Ptr(new ServiceEvent(s->name, ServiceEvent::OnInter));
      else {
	  error(yyloc, std::string("Unknown service event: ") + (yysemantic_stack_[(1) - (1)].stringVal));
	  YYERROR;
      }
    }
}
    break;

  case 52:

/* Line 678 of lalr1.cc  */
#line 588 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    //try to find what type of event this is
    Port::Ptr p = driver.component().port((yysemantic_stack_[(3) - (1)].stringVal));
    if(p.get()) { // port event
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
    } else {
    }
}
    break;

  case 53:

/* Line 678 of lalr1.cc  */
#line 608 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    if((yysemantic_stack_[(5) - (3)].stringVal) != "onCodel") {
	  error(yyloc, std::string("Unknwon port syntax") );
	  YYERROR;
    }

    Service::Ptr s = driver.component().service((yysemantic_stack_[(5) - (1)].stringVal));
    if(!s.get()) {
	  error(yyloc, std::string("Unknwon service : ") + (yysemantic_stack_[(5) - (1)].stringVal) );
	  YYERROR;
    }  

    Event::Ptr e(new ServiceEvent(s->name, (yysemantic_stack_[(5) - (5)].stringVal)));
    (yyval.eventVal) = e;
}
    break;

  case 54:

/* Line 678 of lalr1.cc  */
#line 627 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Event::Ptr ev(new NamedEvent((yysemantic_stack_[(2) - (2)].stringVal)));
    driver.component().addEvent(ev);
}
    break;

  case 55:

/* Line 678 of lalr1.cc  */
#line 632 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Event::Ptr ev(new NamedEvent((yysemantic_stack_[(4) - (2)].stringVal), (yysemantic_stack_[(4) - (4)].eventVal)));
    driver.component().addEvent(ev);
}
    break;

  case 56:

/* Line 678 of lalr1.cc  */
#line 642 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Codel::Ptr c(new Codel((yysemantic_stack_[(1) - (1)].stringVal)));
    driver.setCurrentCodel(c);
}
    break;

  case 57:

/* Line 678 of lalr1.cc  */
#line 647 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.codelVal) = driver.currentCodel();
}
    break;

  case 58:

/* Line 678 of lalr1.cc  */
#line 653 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 59:

/* Line 678 of lalr1.cc  */
#line 655 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 60:

/* Line 678 of lalr1.cc  */
#line 657 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 61:

/* Line 678 of lalr1.cc  */
#line 661 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    // check if the name given is in the ids
    Idl::IdlType::Ptr t = driver.component().typeFromIdsName((yysemantic_stack_[(2) - (2)].stringVal));
    if(!t.get()) {
      error(yyloc, std::string("Input is not in the IDS: ") + (yysemantic_stack_[(2) - (2)].stringVal));
      YYERROR;
    }

    driver.currentCodel()->addInType((yysemantic_stack_[(2) - (2)].stringVal));
}
    break;

  case 62:

/* Line 678 of lalr1.cc  */
#line 672 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    // check if the name given is in the ids
    Idl::IdlType::Ptr t = driver.component().typeFromIdsName((yysemantic_stack_[(2) - (2)].stringVal));
    if(!t.get()) {
      error(yyloc, std::string("Output is not in the IDS: ") + (yysemantic_stack_[(2) - (2)].stringVal));
      YYERROR;
    }

    driver.currentCodel()->addOutType((yysemantic_stack_[(2) - (2)].stringVal));
}
    break;

  case 63:

/* Line 678 of lalr1.cc  */
#line 683 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.currentCodel()->addInPort((yysemantic_stack_[(2) - (2)].stringVal));
}
    break;

  case 64:

/* Line 678 of lalr1.cc  */
#line 687 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.currentCodel()->addOutPort((yysemantic_stack_[(2) - (2)].stringVal));
}
    break;

  case 65:

/* Line 678 of lalr1.cc  */
#line 695 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    ConstValue c((yysemantic_stack_[(5) - (3)].stringVal), (yysemantic_stack_[(5) - (2)].typeVal), (yysemantic_stack_[(5) - (5)].literalVal));
    driver.component().addConstValue(c);
}
    break;

  case 75:

/* Line 678 of lalr1.cc  */
#line 715 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = Literal((yysemantic_stack_[(1) - (1)].integerVal));
}
    break;

  case 76:

/* Line 678 of lalr1.cc  */
#line 719 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = Literal((yysemantic_stack_[(1) - (1)].doubleVal));
}
    break;

  case 77:

/* Line 678 of lalr1.cc  */
#line 723 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = Literal("\"" + (yysemantic_stack_[(1) - (1)].stringVal) + "\"");
}
    break;

  case 78:

/* Line 678 of lalr1.cc  */
#line 727 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = Literal((yysemantic_stack_[(1) - (1)].charVal));
}
    break;

  case 79:

/* Line 678 of lalr1.cc  */
#line 731 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = (yysemantic_stack_[(1) - (1)].literalVal);
}
    break;

  case 80:

/* Line 678 of lalr1.cc  */
#line 735 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    /*enum value*/
    (yyval.literalVal) = Literal((yysemantic_stack_[(1) - (1)].stringVal));
}
    break;

  case 81:

/* Line 678 of lalr1.cc  */
#line 740 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = (yysemantic_stack_[(3) - (2)].literalVal);
}
    break;

  case 82:

/* Line 678 of lalr1.cc  */
#line 744 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = (yysemantic_stack_[(3) - (2)].literalVal);
}
    break;

  case 83:

/* Line 678 of lalr1.cc  */
#line 748 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 84:

/* Line 678 of lalr1.cc  */
#line 752 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = Literal(true);
}
    break;

  case 85:

/* Line 678 of lalr1.cc  */
#line 756 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = Literal(false);
}
    break;

  case 86:

/* Line 678 of lalr1.cc  */
#line 763 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Neg);
    l.addMember((yysemantic_stack_[(2) - (2)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 87:

/* Line 678 of lalr1.cc  */
#line 769 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = (yysemantic_stack_[(2) - (2)].literalVal);
}
    break;

  case 88:

/* Line 678 of lalr1.cc  */
#line 773 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::LNeg);
    l.addMember((yysemantic_stack_[(2) - (2)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 89:

/* Line 678 of lalr1.cc  */
#line 780 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Plus);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 90:

/* Line 678 of lalr1.cc  */
#line 787 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Minus);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 91:

/* Line 678 of lalr1.cc  */
#line 794 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Times);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 92:

/* Line 678 of lalr1.cc  */
#line 801 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Divide);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 93:

/* Line 678 of lalr1.cc  */
#line 808 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Mod);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 94:

/* Line 678 of lalr1.cc  */
#line 815 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::And);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 95:

/* Line 678 of lalr1.cc  */
#line 822 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Or);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 96:

/* Line 678 of lalr1.cc  */
#line 829 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Xor);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 97:

/* Line 678 of lalr1.cc  */
#line 836 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::LShift);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 98:

/* Line 678 of lalr1.cc  */
#line 843 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::RShift);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 99:

/* Line 678 of lalr1.cc  */
#line 852 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Struct);
    l.addMember((yysemantic_stack_[(1) - (1)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 100:

/* Line 678 of lalr1.cc  */
#line 858 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yysemantic_stack_[(3) - (1)].literalVal).addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = (yysemantic_stack_[(3) - (1)].literalVal);
}
    break;

  case 101:

/* Line 678 of lalr1.cc  */
#line 867 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    IdlType::Ptr p(new TypedefType((yysemantic_stack_[(3) - (2)].typeVal), (yysemantic_stack_[(3) - (3)].declaratorVectVal)));
    (yyval.typeVal) = p;
}
    break;

  case 102:

/* Line 678 of lalr1.cc  */
#line 871 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 103:

/* Line 678 of lalr1.cc  */
#line 873 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 104:

/* Line 678 of lalr1.cc  */
#line 875 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yysemantic_stack_[(2) - (2)].typeVal)->setNative(true);
    (yyval.typeVal) = (yysemantic_stack_[(2) - (2)].typeVal);
}
    break;

  case 105:

/* Line 678 of lalr1.cc  */
#line 890 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal);
}
    break;

  case 106:

/* Line 678 of lalr1.cc  */
#line 894 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { 
    (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal);
}
    break;

  case 107:

/* Line 678 of lalr1.cc  */
#line 899 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 108:

/* Line 678 of lalr1.cc  */
#line 900 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 109:

/* Line 678 of lalr1.cc  */
#line 902 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    IdlType::Ptr type = driver.component().typeFromName((yysemantic_stack_[(1) - (1)].stringVal));
    if(!type.get()) {
	error(yyloc, std::string("Unknown type: ") + (yysemantic_stack_[(1) - (1)].stringVal));
	YYERROR;
    }
    (yyval.typeVal) = type;
}
    break;

  case 110:

/* Line 678 of lalr1.cc  */
#line 912 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 111:

/* Line 678 of lalr1.cc  */
#line 913 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 112:

/* Line 678 of lalr1.cc  */
#line 914 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 113:

/* Line 678 of lalr1.cc  */
#line 915 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 114:

/* Line 678 of lalr1.cc  */
#line 916 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 115:

/* Line 678 of lalr1.cc  */
#line 917 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 116:

/* Line 678 of lalr1.cc  */
#line 918 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 117:

/* Line 678 of lalr1.cc  */
#line 924 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 118:

/* Line 678 of lalr1.cc  */
#line 925 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 119:

/* Line 678 of lalr1.cc  */
#line 926 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 120:

/* Line 678 of lalr1.cc  */
#line 927 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 121:

/* Line 678 of lalr1.cc  */
#line 931 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 122:

/* Line 678 of lalr1.cc  */
#line 933 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 123:

/* Line 678 of lalr1.cc  */
#line 938 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { 
    Declarator::VectPtr v(new Declarator::Vect());
    v->push_back((yysemantic_stack_[(1) - (1)].declaratorVal));
    (yyval.declaratorVectVal) = v;
}
    break;

  case 124:

/* Line 678 of lalr1.cc  */
#line 944 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 125:

/* Line 678 of lalr1.cc  */
#line 955 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.declaratorVal) = (yysemantic_stack_[(1) - (1)].declaratorVal); }
    break;

  case 126:

/* Line 678 of lalr1.cc  */
#line 956 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.declaratorVal) = (yysemantic_stack_[(1) - (1)].declaratorVal); }
    break;

  case 127:

/* Line 678 of lalr1.cc  */
#line 961 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Declarator::Ptr p(new Declarator((yysemantic_stack_[(1) - (1)].stringVal)));
    (yyval.declaratorVal) = p;
}
    break;

  case 128:

/* Line 678 of lalr1.cc  */
#line 968 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yysemantic_stack_[(2) - (1)].declaratorVal)->addBound((yysemantic_stack_[(2) - (2)].integerVal));
    (yyval.declaratorVal) = (yysemantic_stack_[(2) - (1)].declaratorVal);
}
    break;

  case 129:

/* Line 678 of lalr1.cc  */
#line 973 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yysemantic_stack_[(2) - (1)].declaratorVal)->addBound((yysemantic_stack_[(2) - (2)].integerVal));
    (yyval.declaratorVal) = (yysemantic_stack_[(2) - (1)].declaratorVal);
}
    break;

  case 130:

/* Line 678 of lalr1.cc  */
#line 980 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.integerVal) = (yysemantic_stack_[(3) - (2)].integerVal);
}
    break;

  case 131:

/* Line 678 of lalr1.cc  */
#line 988 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::floatType; }
    break;

  case 132:

/* Line 678 of lalr1.cc  */
#line 989 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::doubleType; }
    break;

  case 133:

/* Line 678 of lalr1.cc  */
#line 990 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::longdoubleType; }
    break;

  case 134:

/* Line 678 of lalr1.cc  */
#line 995 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    IdlType::Ptr p(new FixedType((yysemantic_stack_[(6) - (3)].integerVal), (yysemantic_stack_[(6) - (5)].integerVal)));
    (yyval.typeVal) = p;
}
    break;

  case 135:

/* Line 678 of lalr1.cc  */
#line 1000 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 136:

/* Line 678 of lalr1.cc  */
#line 1003 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 137:

/* Line 678 of lalr1.cc  */
#line 1004 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 138:

/* Line 678 of lalr1.cc  */
#line 1008 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 139:

/* Line 678 of lalr1.cc  */
#line 1009 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 140:

/* Line 678 of lalr1.cc  */
#line 1010 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 141:

/* Line 678 of lalr1.cc  */
#line 1014 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::shortType; }
    break;

  case 142:

/* Line 678 of lalr1.cc  */
#line 1018 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::longType; }
    break;

  case 143:

/* Line 678 of lalr1.cc  */
#line 1022 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::longlongType; }
    break;

  case 144:

/* Line 678 of lalr1.cc  */
#line 1026 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 145:

/* Line 678 of lalr1.cc  */
#line 1027 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 146:

/* Line 678 of lalr1.cc  */
#line 1028 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 147:

/* Line 678 of lalr1.cc  */
#line 1032 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::ushortType; }
    break;

  case 148:

/* Line 678 of lalr1.cc  */
#line 1036 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::ulongType; }
    break;

  case 149:

/* Line 678 of lalr1.cc  */
#line 1040 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::ulonglongType; }
    break;

  case 150:

/* Line 678 of lalr1.cc  */
#line 1044 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::charType; }
    break;

  case 151:

/* Line 678 of lalr1.cc  */
#line 1048 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::wcharType; }
    break;

  case 152:

/* Line 678 of lalr1.cc  */
#line 1052 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::booleanType; }
    break;

  case 153:

/* Line 678 of lalr1.cc  */
#line 1056 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::octetType; }
    break;

  case 154:

/* Line 678 of lalr1.cc  */
#line 1060 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::anyType; }
    break;

  case 155:

/* Line 678 of lalr1.cc  */
#line 1069 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    IdlType::Ptr p(new StringType((yysemantic_stack_[(4) - (3)].integerVal)));
    (yyval.typeVal) = p; 
}
    break;

  case 156:

/* Line 678 of lalr1.cc  */
#line 1074 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.typeVal) = StringType::unboundedStringType;
}
    break;

  case 157:

/* Line 678 of lalr1.cc  */
#line 1080 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { 
    IdlType::Ptr p(new WStringType((yysemantic_stack_[(4) - (3)].integerVal)));
    (yyval.typeVal) = p; 
}
    break;

  case 158:

/* Line 678 of lalr1.cc  */
#line 1085 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.typeVal) = WStringType::unboundedWStringType;
}
    break;

  case 159:

/* Line 678 of lalr1.cc  */
#line 1095 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 160:

/* Line 678 of lalr1.cc  */
#line 1109 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 161:

/* Line 678 of lalr1.cc  */
#line 1111 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 162:

/* Line 678 of lalr1.cc  */
#line 1115 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 163:

/* Line 678 of lalr1.cc  */
#line 1201 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 164:

/* Line 678 of lalr1.cc  */
#line 1214 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 165:

/* Line 678 of lalr1.cc  */
#line 1216 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 166:

/* Line 678 of lalr1.cc  */
#line 1220 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 167:

/* Line 678 of lalr1.cc  */
#line 1234 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    IdlType::Ptr p(new SequenceType((yysemantic_stack_[(6) - (3)].typeVal), (yysemantic_stack_[(6) - (5)].integerVal)));
    (yyval.typeVal) = p;
}
    break;

  case 168:

/* Line 678 of lalr1.cc  */
#line 1239 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    IdlType::Ptr p(new SequenceType((yysemantic_stack_[(4) - (3)].typeVal), 0));  
    (yyval.typeVal) = p;
}
    break;



/* Line 678 of lalr1.cc  */
#line 1983 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.cpp"
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
  const short int Parser::yypact_ninf_ = -200;
  const short int
  Parser::yypact_[] =
  {
       143,   -33,   -31,    26,    29,    42,    -2,    52,    80,   144,
     206,    81,    47,    67,   130,  -200,  -200,  -200,  -200,  -200,
    -200,  -200,  -200,  -200,   133,  -200,  -200,    86,   105,  -200,
     137,   158,  -200,    -9,  -200,  -200,   155,  -200,  -200,   156,
     168,  -200,  -200,    15,   125,  -200,  -200,  -200,  -200,  -200,
    -200,  -200,  -200,  -200,  -200,  -200,  -200,  -200,  -200,  -200,
    -200,  -200,  -200,   173,  -200,   126,  -200,  -200,  -200,  -200,
    -200,  -200,  -200,  -200,  -200,  -200,  -200,  -200,  -200,  -200,
    -200,  -200,  -200,   169,  -200,  -200,   178,  -200,   135,   183,
     186,  -200,   187,   151,   206,  -200,  -200,   157,   159,   160,
    -200,   184,   196,   240,  -200,   204,  -200,   147,   147,   163,
    -200,   212,    11,   214,     9,    28,   218,  -200,    14,  -200,
     126,    98,  -200,   213,   215,   216,  -200,     6,    95,   126,
     171,  -200,  -200,   225,  -200,    46,  -200,   234,  -200,   188,
     236,     1,   238,   191,   242,   252,   163,   266,    25,   268,
     221,  -200,   151,    60,  -200,  -200,   197,  -200,  -200,     6,
       6,  -200,  -200,  -200,  -200,  -200,  -200,  -200,     6,     6,
       6,   241,  -200,  -200,  -200,   226,  -200,   207,   224,  -200,
    -200,  -200,  -200,   272,   -12,  -200,   274,  -200,   275,   172,
     172,   276,    61,  -200,   279,  -200,   280,  -200,  -200,   277,
     241,    30,    31,  -200,  -200,  -200,     6,     6,     6,     6,
       6,     6,     6,     6,     6,     6,   281,  -200,   278,   235,
    -200,  -200,   237,  -200,   235,   286,   285,  -200,   293,   253,
    -200,   235,  -200,  -200,   254,  -200,   235,  -200,  -200,     6,
    -200,   194,   194,    87,    87,    94,    54,    54,    54,    68,
      68,  -200,   255,  -200,  -200,  -200,  -200,   256,   172,     6,
    -200,  -200,  -200,  -200,   257,   282,   241,  -200,   301,  -200,
    -200,   241,  -200,   308,   -15,  -200,   260,   261,   262,   263,
     101,  -200,  -200,  -200,  -200,  -200,   -15,  -200,  -200
  };

  /* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
     doesn't specify something else to do.  Zero means the default is an
     error.  */
  const unsigned char
  Parser::yydefact_[] =
  {
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     6,     5,     7,     8,    11,
      10,     9,   102,   103,     0,    21,    30,     0,     0,   104,
       0,     0,   141,   142,   131,   132,   135,   150,   151,   156,
     158,   152,   153,     0,     0,    70,    73,    66,   136,   138,
     139,   140,   137,   144,   145,   146,    67,    68,    69,    74,
      71,    72,   154,     0,   109,     0,   105,   107,   108,   106,
     110,   120,   111,   112,   113,   114,   115,   116,   118,   119,
     121,   122,   117,    54,     1,     2,     0,     3,     0,     0,
       0,    18,    19,     0,     0,   143,   133,     0,     0,     0,
     147,   148,     0,     0,   127,   101,   123,   125,   126,     0,
       4,     0,     0,     0,     0,     0,     0,   166,     0,   164,
       0,     0,   160,     0,     0,     0,   149,     0,     0,     0,
       0,   128,   129,    51,    55,     0,    12,     0,    13,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   163,     0,     0,   159,   161,     0,   155,   157,     0,
       0,    84,    85,    78,    75,    76,    77,    80,     0,     0,
       0,    65,    79,    83,   168,     0,   124,     0,     0,    16,
      15,    17,    14,     0,     0,    22,     0,    23,     0,     0,
       0,     0,     0,    31,     0,    32,     0,   165,   162,     0,
      99,     0,     0,    86,    87,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   130,    52,     0,
      26,    28,    27,    24,     0,     0,    36,    45,    48,     0,
      37,     0,    40,    39,    41,    33,     0,   134,    81,     0,
      82,    90,    89,    92,    91,    93,    96,    95,    94,    98,
      97,   167,     0,    56,    25,    29,    34,     0,     0,     0,
      50,    35,    43,    38,    42,     0,   100,    53,     0,    49,
      46,    47,    44,     0,    58,    20,     0,     0,     0,     0,
       0,    59,    61,    62,    63,    64,     0,    57,    60
  };

  /* YYPGOTO[NTERM-NUM].  */
  const short int
  Parser::yypgoto_[] =
  {
      -200,  -200,  -200,   309,  -200,  -200,   209,  -200,  -200,  -200,
    -200,   182,  -200,  -200,  -200,  -200,   176,  -200,  -200,  -200,
      69,   136,   179,  -200,  -199,  -200,  -200,    43,  -200,  -200,
    -158,  -200,  -200,  -200,   322,   -10,   227,  -200,  -200,  -200,
     211,   203,  -200,  -200,   228,   324,   325,   326,  -200,  -200,
    -200,  -200,  -200,  -200,  -200,  -200,   328,   329,   330,   331,
    -200,   332,   333,     8,  -200,   222,    17,  -200,   192,  -200
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const short int
  Parser::yydefgoto_[] =
  {
        -1,    12,    13,    14,    15,   112,   113,    16,    17,    89,
     141,   142,   222,    18,    90,   148,   149,   263,   264,   226,
     227,   228,   134,    19,   254,   268,   280,   281,    20,    44,
     171,   172,   173,   201,    21,   229,    66,    67,    68,    69,
     105,   106,   107,   108,   131,    70,    71,    72,    48,    49,
      50,    51,    52,    53,    54,    55,    73,    74,    75,    76,
      77,    78,    79,    80,   121,   122,    81,   118,   119,    82
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If zero, do what YYDEFACT says.  */
  const signed char Parser::yytable_ninf_ = -1;
  const short int
  Parser::yytable_[] =
  {
        65,   200,   202,   276,   277,   278,   279,   139,    22,   185,
     203,   204,   205,   159,    22,   139,    95,    23,    96,   136,
     160,    22,   151,    23,    24,   256,    25,   152,   161,   162,
      23,   143,   261,   193,   143,     6,     7,   265,   238,   100,
     101,     8,   220,   239,    10,   221,   240,    84,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   140,   163,
     164,   165,   166,   167,   168,   169,   140,    85,   111,   198,
       1,     2,     3,   129,   170,   144,   145,   146,   144,   145,
     146,   266,   147,    26,   120,   147,    27,     4,     5,   206,
     207,   208,   209,   210,   211,   212,   213,   214,   215,    28,
     179,   271,   180,   181,     6,     7,   154,   174,   175,    30,
       8,   120,     9,    10,   286,   232,   287,   233,   234,    11,
     214,   215,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    62,    -1,    -1,     7,    31,    83,    87,
      88,     8,    63,    91,    93,    43,     1,     2,     3,   210,
     211,   212,   213,   214,   215,    64,    -1,   211,   212,   213,
     214,   215,    92,     4,     5,    94,    97,    98,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    99,
       6,     7,   102,   104,   103,   109,     8,   110,     9,    10,
     114,    43,   111,   115,   116,    11,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    62,   117,   126,
       7,   123,   127,   124,   125,     8,    63,   129,   130,    43,
     133,   225,   135,   138,   150,   177,   156,   157,   158,    64,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    62,   178,   182,     7,   183,   184,   187,   188,     8,
      63,   199,   189,    43,   208,   209,   210,   211,   212,   213,
     214,   215,   190,    64,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    62,   192,   195,   196,   217,
     216,   218,   219,   223,    63,   224,   231,    43,   235,   237,
     236,   273,   253,   251,   255,   252,   257,    64,   258,   206,
     207,   208,   209,   210,   211,   212,   213,   214,   215,   259,
     260,   262,   267,   269,   272,   274,   275,   282,   283,   284,
     285,   137,    86,   186,   194,   191,   230,   270,    29,   288,
     128,   153,   176,    45,    46,    47,   132,    56,    57,    58,
      59,    60,    61,   155,   197
  };

  /* YYCHECK.  */
  const unsigned short int
  Parser::yycheck_[] =
  {
        10,   159,   160,    18,    19,    20,    21,     6,     0,     8,
     168,   169,   170,     7,     6,     6,    25,     0,    27,     8,
      14,    13,     8,     6,    57,   224,    57,    13,    22,    23,
      13,     6,   231,     8,     6,    37,    38,   236,     8,    24,
      25,    43,    54,    13,    46,    57,    15,     0,   206,   207,
     208,   209,   210,   211,   212,   213,   214,   215,    57,    53,
      54,    55,    56,    57,    58,    59,    57,     0,    57,     9,
       3,     4,     5,    13,    68,    50,    51,    52,    50,    51,
      52,   239,    57,    57,    94,    57,    57,    20,    21,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    57,
      54,   259,    56,    57,    37,    38,     8,    12,    13,    57,
      43,   121,    45,    46,    13,    54,    15,    56,    57,    52,
      66,    67,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    66,    67,    38,    57,    57,     9,
       7,    43,    44,    57,     7,    47,     3,     4,     5,    62,
      63,    64,    65,    66,    67,    57,    62,    63,    64,    65,
      66,    67,    57,    20,    21,     7,    11,    11,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    11,
      37,    38,    57,    57,    11,    16,    43,     9,    45,    46,
       7,    47,    57,     7,     7,    52,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    57,    25,
      38,    54,    16,    54,    54,    43,    44,    13,    71,    47,
      57,    49,    10,     9,     6,    54,    13,    12,    12,    57,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    17,     9,    38,    57,    10,     9,    57,    43,
      44,    54,    10,    47,    60,    61,    62,    63,    64,    65,
      66,    67,    10,    57,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    10,     9,    57,    72,
      54,    57,    10,     9,    44,    10,    10,    47,     9,    12,
      10,     9,    57,    12,    57,    17,    10,    57,    13,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    16,
      57,    57,    57,    57,    57,    14,     8,    57,    57,    57,
      57,   112,    13,   141,   148,   146,   190,   258,     6,   286,
     103,   120,   129,     9,     9,     9,   108,     9,     9,     9,
       9,     9,     9,   121,   152
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  Parser::yystos_[] =
  {
         0,     3,     4,     5,    20,    21,    37,    38,    43,    45,
      46,    52,    74,    75,    76,    77,    80,    81,    86,    96,
     101,   107,   136,   139,    57,    57,    57,    57,    57,   107,
      57,    57,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    47,   102,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     134,   135,    35,    44,    57,   108,   109,   110,   111,   112,
     118,   119,   120,   129,   130,   131,   132,   133,   134,   135,
     136,   139,   142,    57,     0,     0,    76,     9,     7,    82,
      87,    57,    57,     7,     7,    25,    27,    11,    11,    11,
      24,    25,    57,    11,    57,   113,   114,   115,   116,    16,
       9,    57,    78,    79,     7,     7,     7,    57,   140,   141,
     108,   137,   138,    54,    54,    54,    25,    16,   109,    13,
      71,   117,   117,    57,    95,    10,     8,    79,     9,     6,
      57,    83,    84,     6,    50,    51,    52,    57,    88,    89,
       6,     8,    13,   113,     8,   138,    13,    12,    12,     7,
      14,    22,    23,    53,    54,    55,    56,    57,    58,    59,
      68,   103,   104,   105,    12,    13,   114,    54,    17,    54,
      56,    57,     9,    57,    10,     8,    84,     9,    57,    10,
      10,    95,    10,     8,    89,     9,    57,   141,     9,    54,
     103,   106,   103,   103,   103,   103,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    54,    72,    57,    10,
      54,    57,    85,     9,    10,    49,    92,    93,    94,   108,
      94,    10,    54,    56,    57,     9,    10,    12,     8,    13,
      15,   103,   103,   103,   103,   103,   103,   103,   103,   103,
     103,    12,    17,    57,    97,    57,    97,    10,    13,    16,
      57,    97,    57,    90,    91,    97,   103,    57,    98,    57,
      93,   103,    57,     9,    14,     8,    18,    19,    20,    21,
      99,   100,    57,    57,    57,    57,    13,    15,   100
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
     325,    91,    93
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  Parser::yyr1_[] =
  {
         0,    73,    74,    75,    75,    76,    76,    76,    76,    76,
      76,    76,    77,    78,    78,    79,    79,    79,    80,    80,
      80,    82,    81,    83,    83,    84,    84,    84,    85,    85,
      87,    86,    88,    88,    89,    89,    89,    89,    89,    89,
      89,    90,    90,    91,    91,    92,    92,    93,    93,    94,
      94,    95,    95,    95,    96,    96,    98,    97,    99,    99,
      99,   100,   100,   100,   100,   101,   102,   102,   102,   102,
     102,   102,   102,   102,   102,   103,   103,   103,   103,   103,
     103,   103,   103,   103,   104,   104,   105,   105,   105,   105,
     105,   105,   105,   105,   105,   105,   105,   105,   105,   106,
     106,   107,   107,   107,   107,   108,   108,   109,   109,   109,
     110,   110,   110,   110,   110,   110,   110,   111,   111,   111,
     111,   112,   112,   113,   113,   114,   114,   115,   116,   116,
     117,   118,   118,   118,   119,   119,   120,   120,   121,   121,
     121,   122,   123,   124,   125,   125,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   134,   135,   135,   136,
     137,   137,   138,   139,   140,   140,   141,   142,   142
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  Parser::yyr2_[] =
  {
         0,     2,     2,     2,     3,     1,     1,     1,     1,     1,
       1,     1,     5,     2,     3,     3,     3,     3,     3,     3,
      10,     0,     6,     2,     3,     4,     3,     3,     1,     2,
       0,     6,     2,     3,     4,     4,     3,     3,     4,     3,
       3,     0,     1,     1,     2,     1,     3,     3,     1,     3,
       2,     1,     3,     5,     2,     4,     0,     5,     0,     1,
       3,     2,     2,     2,     2,     5,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     3,     1,     1,     1,     2,     2,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     1,
       3,     3,     1,     1,     2,     1,     1,     1,     1,     1,
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
  "IN", "OUT", "INPORT", "OUTPORT", "TRUE", "FALSE", "SHORT", "LONG",
  "FLOAT", "DOUBLE", "FIXED", "CHAR", "WCHAR", "STRING", "WSTRING",
  "BOOLEAN", "OCTET", "ANY", "VOID", "NATIVE", "ENUM", "UNION", "SWITCH",
  "CASE", "DEFAULT", "STRUCT", "SEQUENCE", "CONST", "TYPEDEF", "UNSIGNED",
  "OBJECT", "IDS", "INPUT", "OUTPUT", "EVENT", "\"char\"", "\"integer\"",
  "\"double\"", "\"string literal\"", "\"identifier\"", "MINUS", "PLUS",
  "SLASH", "TIMES", "MOD", "XOR", "OR", "AND", "RSHIFT", "LSHIFT", "TILDE",
  "POS", "NEG", "'['", "']'", "$accept", "start", "declarations",
  "declaration", "component_decl", "component_fields", "component_field",
  "port_decl", "task_decl", "$@1", "task_fields", "task_field",
  "task_errors_list", "service_decl", "$@2", "service_fields",
  "service_field", "identifier_list", "identifiers", "inputs", "input",
  "input_type", "event_spec", "event_decl", "codel_prototype", "$@3",
  "codel_fields", "codel_field", "const_decl", "const_type", "literal",
  "boolean_literal", "composed_literal", "literals", "type_decl",
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
        74,     0,    -1,    75,     0,    -1,    76,     9,    -1,    75,
      76,     9,    -1,    80,    -1,    77,    -1,    81,    -1,    86,
      -1,   107,    -1,   101,    -1,    96,    -1,     3,    57,     7,
      78,     8,    -1,    79,     9,    -1,    78,    79,     9,    -1,
      57,    10,    56,    -1,    57,    10,    54,    -1,    57,    10,
      57,    -1,    20,    57,    57,    -1,    21,    57,    57,    -1,
      21,    57,    57,     7,     6,    57,    10,    97,     9,     8,
      -1,    -1,     4,    57,    82,     7,    83,     8,    -1,    84,
       9,    -1,    83,    84,     9,    -1,     6,    57,    10,    97,
      -1,    57,    10,    54,    -1,    57,    10,    85,    -1,    57,
      -1,    85,    57,    -1,    -1,     5,    57,    87,     7,    88,
       8,    -1,    89,     9,    -1,    88,    89,     9,    -1,     6,
      57,    10,    97,    -1,    52,    95,    10,    97,    -1,    50,
      10,    92,    -1,    51,    10,    94,    -1,    57,    10,    57,
      90,    -1,    57,    10,    56,    -1,    57,    10,    54,    -1,
      -1,    91,    -1,    57,    -1,    91,    57,    -1,    93,    -1,
      92,    13,    93,    -1,    94,    16,   103,    -1,    94,    -1,
      49,    10,    57,    -1,   108,    57,    -1,    57,    -1,    57,
      17,    57,    -1,    57,    17,    57,    17,    57,    -1,    52,
      57,    -1,    52,    57,    16,    95,    -1,    -1,    57,    98,
      14,    99,    15,    -1,    -1,   100,    -1,    99,    13,   100,
      -1,    18,    57,    -1,    19,    57,    -1,    20,    57,    -1,
      21,    57,    -1,    45,   102,    57,    16,   103,    -1,   120,
      -1,   129,    -1,   130,    -1,   131,    -1,   118,    -1,   134,
      -1,   135,    -1,   119,    -1,   132,    -1,    54,    -1,    55,
      -1,    56,    -1,    53,    -1,   104,    -1,    57,    -1,     7,
     106,     8,    -1,    14,   103,    15,    -1,   105,    -1,    22,
      -1,    23,    -1,    58,   103,    -1,    59,   103,    -1,    68,
     103,    -1,   103,    59,   103,    -1,   103,    58,   103,    -1,
     103,    61,   103,    -1,   103,    60,   103,    -1,   103,    62,
     103,    -1,   103,    65,   103,    -1,   103,    64,   103,    -1,
     103,    63,   103,    -1,   103,    67,   103,    -1,   103,    66,
     103,    -1,   103,    -1,   106,    13,   103,    -1,    46,   108,
     113,    -1,   136,    -1,   139,    -1,    37,   107,    -1,   109,
      -1,   112,    -1,   110,    -1,   111,    -1,    57,    -1,   118,
      -1,   120,    -1,   129,    -1,   130,    -1,   131,    -1,   132,
      -1,   133,    -1,   142,    -1,   134,    -1,   135,    -1,   119,
      -1,   136,    -1,   139,    -1,   114,    -1,   113,    13,   114,
      -1,   115,    -1,   116,    -1,    57,    -1,   115,   117,    -1,
     116,   117,    -1,    71,    54,    72,    -1,    26,    -1,    27,
      -1,    25,    27,    -1,    28,    11,    54,    13,    54,    12,
      -1,    28,    -1,   121,    -1,   125,    -1,   122,    -1,   123,
      -1,   124,    -1,    24,    -1,    25,    -1,    25,    25,    -1,
     126,    -1,   127,    -1,   128,    -1,    47,    24,    -1,    47,
      25,    -1,    47,    25,    25,    -1,    29,    -1,    30,    -1,
      33,    -1,    34,    -1,    35,    -1,    31,    11,    54,    12,
      -1,    31,    -1,    32,    11,    54,    12,    -1,    32,    -1,
      43,    57,     7,   137,     8,    -1,   138,    -1,   137,   138,
      -1,   108,   113,     9,    -1,    38,    57,     7,   140,     8,
      -1,   141,    -1,   140,    13,   141,    -1,    57,    -1,    44,
      11,   109,    13,    54,    12,    -1,    44,    11,   109,    12,
      -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned short int
  Parser::yyprhs_[] =
  {
         0,     0,     3,     6,     9,    13,    15,    17,    19,    21,
      23,    25,    27,    33,    36,    40,    44,    48,    52,    56,
      60,    71,    72,    79,    82,    86,    91,    95,    99,   101,
     104,   105,   112,   115,   119,   124,   129,   133,   137,   142,
     146,   150,   151,   153,   155,   158,   160,   164,   168,   170,
     174,   177,   179,   183,   189,   192,   197,   198,   204,   205,
     207,   211,   214,   217,   220,   223,   229,   231,   233,   235,
     237,   239,   241,   243,   245,   247,   249,   251,   253,   255,
     257,   259,   263,   267,   269,   271,   273,   276,   279,   282,
     286,   290,   294,   298,   302,   306,   310,   314,   318,   322,
     324,   328,   332,   334,   336,   339,   341,   343,   345,   347,
     349,   351,   353,   355,   357,   359,   361,   363,   365,   367,
     369,   371,   373,   375,   377,   381,   383,   385,   387,   390,
     393,   397,   399,   401,   404,   411,   413,   415,   417,   419,
     421,   423,   425,   427,   430,   432,   434,   436,   439,   442,
     446,   448,   450,   452,   454,   456,   461,   463,   468,   470,
     476,   478,   481,   485,   491,   493,   497,   499,   506
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned short int
  Parser::yyrline_[] =
  {
         0,   234,   234,   237,   238,   243,   245,   247,   249,   251,
     255,   257,   264,   270,   271,   276,   287,   295,   315,   325,
     335,   356,   355,   366,   367,   371,   375,   392,   401,   405,
     414,   413,   424,   425,   429,   433,   438,   441,   445,   490,
     500,   505,   508,   514,   518,   524,   526,   530,   535,   541,
     555,   566,   587,   607,   626,   631,   642,   641,   653,   654,
     656,   660,   671,   682,   686,   694,   701,   702,   703,   704,
     705,   706,   707,   708,   709,   714,   718,   722,   726,   730,
     734,   739,   743,   747,   751,   755,   762,   768,   772,   779,
     786,   793,   800,   807,   814,   821,   828,   835,   842,   851,
     857,   866,   871,   873,   874,   889,   893,   899,   900,   901,
     912,   913,   914,   915,   916,   917,   918,   924,   925,   926,
     927,   931,   933,   937,   943,   955,   956,   960,   967,   972,
     979,   988,   989,   990,   994,   999,  1003,  1004,  1008,  1009,
    1010,  1014,  1018,  1022,  1026,  1027,  1028,  1032,  1036,  1040,
    1044,  1048,  1052,  1056,  1060,  1068,  1073,  1079,  1084,  1094,
    1108,  1110,  1114,  1200,  1214,  1215,  1219,  1233,  1238
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
       2,    71,     2,    72,     2,     2,     2,     2,     2,     2,
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
      65,    66,    67,    68,    69,    70
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int Parser::yyeof_ = 0;
  const int Parser::yylast_ = 344;
  const int Parser::yynnts_ = 70;
  const int Parser::yyempty_ = -2;
  const int Parser::yyfinal_ = 84;
  const int Parser::yyterror_ = 1;
  const int Parser::yyerrcode_ = 256;
  const int Parser::yyntokens_ = 73;

  const unsigned int Parser::yyuser_token_number_max_ = 325;
  const Parser::token_number_type Parser::yyundef_token_ = 2;


/* Line 1054 of lalr1.cc  */
#line 1 "[Bison:b4_percent_define_default]"

} // G3nom

/* Line 1054 of lalr1.cc  */
#line 2708 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.cpp"


/* Line 1056 of lalr1.cc  */
#line 1244 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
 /*** Additional Code ***/

void G3nom::Parser::error(const Parser::location_type& l,
			    const std::string& m)
{
    driver.error(l, m);
}


