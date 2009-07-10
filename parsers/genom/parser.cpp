
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
#line 260 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 13:

/* Line 678 of lalr1.cc  */
#line 267 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.component().setName((yysemantic_stack_[(5) - (2)].stringVal));
}
    break;

  case 15:

/* Line 678 of lalr1.cc  */
#line 274 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 16:

/* Line 678 of lalr1.cc  */
#line 279 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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
#line 290 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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
#line 299 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    if((yysemantic_stack_[(3) - (1)].stringVal) == "ids") {
	Idl::IdlType::Ptr p = driver.component().typeFromName((yysemantic_stack_[(3) - (3)].stringVal));
	if(!p.get()) {
	    error(yyloc, std::string("Unknown type: ") + (yysemantic_stack_[(3) - (3)].stringVal));
	    YYERROR;
	}
	driver.component().IDSType = p;
    } else {
      error(yyloc, std::string("Unknown component field: ") + (yysemantic_stack_[(3) - (1)].stringVal));
      YYERROR;
    }
}
    break;

  case 19:

/* Line 678 of lalr1.cc  */
#line 313 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    if((yysemantic_stack_[(3) - (1)].stringVal) == "requires") {
      std::vector<std::string> v;
      driver.split((yysemantic_stack_[(3) - (3)].stringVal), v);
      driver.component().addImportedComponents(v);
    } else {
      error(yyloc, std::string("Unknown syntax for component field: ") + (yysemantic_stack_[(3) - (1)].stringVal));
      YYERROR;
    }
}
    break;

  case 20:

/* Line 678 of lalr1.cc  */
#line 328 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 21:

/* Line 678 of lalr1.cc  */
#line 338 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 22:

/* Line 678 of lalr1.cc  */
#line 348 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 23:

/* Line 678 of lalr1.cc  */
#line 368 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Task::Ptr t(new Task((yysemantic_stack_[(2) - (2)].stringVal)));
    driver.setCurrentTask(t);
}
    break;

  case 24:

/* Line 678 of lalr1.cc  */
#line 373 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.component().addTask(driver.currentTask());
}
    break;

  case 26:

/* Line 678 of lalr1.cc  */
#line 380 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 27:

/* Line 678 of lalr1.cc  */
#line 384 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.currentTask()->addCodel((yysemantic_stack_[(4) - (2)].stringVal), (yysemantic_stack_[(4) - (4)].codelVal));
}
    break;

  case 28:

/* Line 678 of lalr1.cc  */
#line 388 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 29:

/* Line 678 of lalr1.cc  */
#line 405 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    if((yysemantic_stack_[(3) - (1)].stringVal) != "errors") {
      error(yyloc, std::string("Wrong arguments for field: ") + (yysemantic_stack_[(3) - (1)].stringVal));
      YYERROR;
    }
}
    break;

  case 30:

/* Line 678 of lalr1.cc  */
#line 414 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.currentTask()->addErrorMessage((yysemantic_stack_[(1) - (1)].stringVal));
}
    break;

  case 31:

/* Line 678 of lalr1.cc  */
#line 418 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.currentTask()->addErrorMessage((yysemantic_stack_[(2) - (2)].stringVal));
}
    break;

  case 32:

/* Line 678 of lalr1.cc  */
#line 426 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Service::Ptr s(new Service((yysemantic_stack_[(2) - (2)].stringVal)));
    driver.setCurrentService(s);
}
    break;

  case 33:

/* Line 678 of lalr1.cc  */
#line 431 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.component().addService(driver.currentService());
}
    break;

  case 35:

/* Line 678 of lalr1.cc  */
#line 438 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 36:

/* Line 678 of lalr1.cc  */
#line 442 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.currentService()->addCodel((yysemantic_stack_[(4) - (2)].stringVal), (yysemantic_stack_[(4) - (4)].codelVal));
}
    break;

  case 37:

/* Line 678 of lalr1.cc  */
#line 446 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.currentService()->addCodel((yysemantic_stack_[(4) - (2)].eventVal)->identifier(), (yysemantic_stack_[(4) - (4)].codelVal));
    driver.currentService()->addEvent((yysemantic_stack_[(4) - (2)].eventVal), (yysemantic_stack_[(4) - (2)].eventVal)->identifier());
}
    break;

  case 38:

/* Line 678 of lalr1.cc  */
#line 451 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
}
    break;

  case 39:

/* Line 678 of lalr1.cc  */
#line 454 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.currentService()->output = (yysemantic_stack_[(3) - (3)].serviceInputVal);
}
    break;

  case 40:

/* Line 678 of lalr1.cc  */
#line 458 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 41:

/* Line 678 of lalr1.cc  */
#line 503 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 42:

/* Line 678 of lalr1.cc  */
#line 513 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 43:

/* Line 678 of lalr1.cc  */
#line 517 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.stringVal) = "";
}
    break;

  case 44:

/* Line 678 of lalr1.cc  */
#line 521 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.stringVal) = (yysemantic_stack_[(1) - (1)].stringVal);
}
    break;

  case 45:

/* Line 678 of lalr1.cc  */
#line 527 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
   (yyval.stringVal) = (yysemantic_stack_[(1) - (1)].stringVal);
}
    break;

  case 46:

/* Line 678 of lalr1.cc  */
#line 531 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.stringVal) = (yysemantic_stack_[(3) - (1)].stringVal) + " " + (yysemantic_stack_[(3) - (3)].stringVal);
}
    break;

  case 47:

/* Line 678 of lalr1.cc  */
#line 537 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 48:

/* Line 678 of lalr1.cc  */
#line 539 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 49:

/* Line 678 of lalr1.cc  */
#line 543 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yysemantic_stack_[(3) - (1)].serviceInputVal).defaultValue = (yysemantic_stack_[(3) - (3)].literalVal);
    driver.currentService()->addInput((yysemantic_stack_[(3) - (1)].serviceInputVal));
}
    break;

  case 50:

/* Line 678 of lalr1.cc  */
#line 548 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.currentService()->addInput((yysemantic_stack_[(1) - (1)].serviceInputVal));
}
    break;

  case 51:

/* Line 678 of lalr1.cc  */
#line 554 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    // check if the name given is in the ids
    Idl::IdlType::Ptr t = driver.component().typeFromIdsName((yysemantic_stack_[(3) - (3)].stringVal));
    if(!t.get()) {
      error(yyloc, std::string("Input is not in the IDS: ") + (yysemantic_stack_[(3) - (3)].stringVal));
      YYERROR;
    }

    ServiceInput i;
    i.kind = ServiceInput::IDSMember;
    i.identifier = (yysemantic_stack_[(3) - (3)].stringVal);
    (yyval.serviceInputVal) = i;
}
    break;

  case 52:

/* Line 678 of lalr1.cc  */
#line 568 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    ServiceInput i;
    i.kind = ServiceInput::Type;
    i.identifier = (yysemantic_stack_[(2) - (2)].stringVal);
    i.type = (yysemantic_stack_[(2) - (1)].typeVal);
    (yyval.serviceInputVal) = i;
}
    break;

  case 53:

/* Line 678 of lalr1.cc  */
#line 579 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Event::Ptr ev = driver.component().event((yysemantic_stack_[(1) - (1)].stringVal));
    if(ev.get()) { // external event
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

  case 54:

/* Line 678 of lalr1.cc  */
#line 600 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    //try to find what type of event this is
    Port::Ptr p = driver.component().port((yysemantic_stack_[(3) - (1)].stringVal));
    Service::Ptr s = driver.component().service((yysemantic_stack_[(3) - (1)].stringVal));
    std::cout << "Reading " << (yysemantic_stack_[(3) - (1)].stringVal) << " and " << (yysemantic_stack_[(3) - (3)].stringVal) << std::endl;
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
    } else if(s.get()) {
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

  case 55:

/* Line 678 of lalr1.cc  */
#line 637 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 56:

/* Line 678 of lalr1.cc  */
#line 656 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Event::Ptr ev(new NamedEvent((yysemantic_stack_[(2) - (2)].stringVal)));
    driver.component().addEvent(ev);
}
    break;

  case 57:

/* Line 678 of lalr1.cc  */
#line 661 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Event::Ptr ev(new NamedEvent((yysemantic_stack_[(4) - (2)].stringVal), (yysemantic_stack_[(4) - (4)].eventVal)));
    driver.component().addEvent(ev);
}
    break;

  case 58:

/* Line 678 of lalr1.cc  */
#line 671 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Codel::Ptr c(new Codel((yysemantic_stack_[(1) - (1)].stringVal)));
    driver.setCurrentCodel(c);
}
    break;

  case 59:

/* Line 678 of lalr1.cc  */
#line 676 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.codelVal) = driver.currentCodel();
}
    break;

  case 60:

/* Line 678 of lalr1.cc  */
#line 682 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 61:

/* Line 678 of lalr1.cc  */
#line 684 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.split((yysemantic_stack_[(2) - (2)].stringVal), driver.currentCodel()->nextCodels); 
}
    break;

  case 62:

/* Line 678 of lalr1.cc  */
#line 690 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 63:

/* Line 678 of lalr1.cc  */
#line 692 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 64:

/* Line 678 of lalr1.cc  */
#line 694 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 65:

/* Line 678 of lalr1.cc  */
#line 698 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 66:

/* Line 678 of lalr1.cc  */
#line 709 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 67:

/* Line 678 of lalr1.cc  */
#line 720 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.currentCodel()->addInPort((yysemantic_stack_[(2) - (2)].stringVal));
}
    break;

  case 68:

/* Line 678 of lalr1.cc  */
#line 724 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.currentCodel()->addOutPort((yysemantic_stack_[(2) - (2)].stringVal));
}
    break;

  case 69:

/* Line 678 of lalr1.cc  */
#line 732 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    ConstValue c((yysemantic_stack_[(5) - (3)].stringVal), (yysemantic_stack_[(5) - (2)].typeVal), (yysemantic_stack_[(5) - (5)].literalVal));
    driver.component().addConstValue(c);
}
    break;

  case 79:

/* Line 678 of lalr1.cc  */
#line 752 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = Literal((yysemantic_stack_[(1) - (1)].integerVal));
}
    break;

  case 80:

/* Line 678 of lalr1.cc  */
#line 756 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = Literal((yysemantic_stack_[(1) - (1)].doubleVal));
}
    break;

  case 81:

/* Line 678 of lalr1.cc  */
#line 760 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = Literal("\"" + (yysemantic_stack_[(1) - (1)].stringVal) + "\"");
}
    break;

  case 82:

/* Line 678 of lalr1.cc  */
#line 764 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = Literal((yysemantic_stack_[(1) - (1)].charVal));
}
    break;

  case 83:

/* Line 678 of lalr1.cc  */
#line 768 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = (yysemantic_stack_[(1) - (1)].literalVal);
}
    break;

  case 84:

/* Line 678 of lalr1.cc  */
#line 772 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    /*enum value*/
    (yyval.literalVal) = Literal((yysemantic_stack_[(1) - (1)].stringVal));
}
    break;

  case 85:

/* Line 678 of lalr1.cc  */
#line 777 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = (yysemantic_stack_[(3) - (2)].literalVal);
}
    break;

  case 86:

/* Line 678 of lalr1.cc  */
#line 781 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = (yysemantic_stack_[(3) - (2)].literalVal);
}
    break;

  case 87:

/* Line 678 of lalr1.cc  */
#line 785 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 88:

/* Line 678 of lalr1.cc  */
#line 789 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = Literal(true);
}
    break;

  case 89:

/* Line 678 of lalr1.cc  */
#line 793 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = Literal(false);
}
    break;

  case 90:

/* Line 678 of lalr1.cc  */
#line 800 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Neg);
    l.addMember((yysemantic_stack_[(2) - (2)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 91:

/* Line 678 of lalr1.cc  */
#line 806 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = (yysemantic_stack_[(2) - (2)].literalVal);
}
    break;

  case 92:

/* Line 678 of lalr1.cc  */
#line 810 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::LNeg);
    l.addMember((yysemantic_stack_[(2) - (2)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 93:

/* Line 678 of lalr1.cc  */
#line 817 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Plus);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 94:

/* Line 678 of lalr1.cc  */
#line 824 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Minus);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 95:

/* Line 678 of lalr1.cc  */
#line 831 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Times);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 96:

/* Line 678 of lalr1.cc  */
#line 838 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Divide);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 97:

/* Line 678 of lalr1.cc  */
#line 845 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Mod);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 98:

/* Line 678 of lalr1.cc  */
#line 852 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::And);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 99:

/* Line 678 of lalr1.cc  */
#line 859 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Or);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 100:

/* Line 678 of lalr1.cc  */
#line 866 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Xor);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 101:

/* Line 678 of lalr1.cc  */
#line 873 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::LShift);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 102:

/* Line 678 of lalr1.cc  */
#line 880 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::RShift);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 103:

/* Line 678 of lalr1.cc  */
#line 889 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Struct);
    l.addMember((yysemantic_stack_[(1) - (1)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 104:

/* Line 678 of lalr1.cc  */
#line 895 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yysemantic_stack_[(3) - (1)].literalVal).addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = (yysemantic_stack_[(3) - (1)].literalVal);
}
    break;

  case 105:

/* Line 678 of lalr1.cc  */
#line 904 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.component().addNativeTypeInclude((yysemantic_stack_[(6) - (3)].stringVal));
}
    break;

  case 106:

/* Line 678 of lalr1.cc  */
#line 910 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yysemantic_stack_[(2) - (1)].typeVal)->setNative(true);
    driver.component().addType((yysemantic_stack_[(2) - (1)].typeVal));
}
    break;

  case 107:

/* Line 678 of lalr1.cc  */
#line 915 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yysemantic_stack_[(4) - (3)].typeVal)->setNative(true);
    driver.component().addType((yysemantic_stack_[(4) - (3)].typeVal));
}
    break;

  case 108:

/* Line 678 of lalr1.cc  */
#line 922 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    IdlType::Ptr p(new TypedefType((yysemantic_stack_[(3) - (2)].typeVal), (yysemantic_stack_[(3) - (3)].declaratorVectVal)));
    (yyval.typeVal) = p;
}
    break;

  case 109:

/* Line 678 of lalr1.cc  */
#line 926 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 110:

/* Line 678 of lalr1.cc  */
#line 928 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 111:

/* Line 678 of lalr1.cc  */
#line 941 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal);
}
    break;

  case 112:

/* Line 678 of lalr1.cc  */
#line 945 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { 
    (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal);
}
    break;

  case 113:

/* Line 678 of lalr1.cc  */
#line 950 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 114:

/* Line 678 of lalr1.cc  */
#line 951 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 115:

/* Line 678 of lalr1.cc  */
#line 953 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    IdlType::Ptr type = driver.component().typeFromName((yysemantic_stack_[(1) - (1)].stringVal));
    if(!type.get()) {
	error(yyloc, std::string("Unknown type: ") + (yysemantic_stack_[(1) - (1)].stringVal));
	YYERROR;
    }
    (yyval.typeVal) = type;
}
    break;

  case 116:

/* Line 678 of lalr1.cc  */
#line 963 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 117:

/* Line 678 of lalr1.cc  */
#line 964 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 118:

/* Line 678 of lalr1.cc  */
#line 965 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 119:

/* Line 678 of lalr1.cc  */
#line 966 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 120:

/* Line 678 of lalr1.cc  */
#line 967 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 121:

/* Line 678 of lalr1.cc  */
#line 968 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 122:

/* Line 678 of lalr1.cc  */
#line 969 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 123:

/* Line 678 of lalr1.cc  */
#line 975 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 124:

/* Line 678 of lalr1.cc  */
#line 976 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 125:

/* Line 678 of lalr1.cc  */
#line 977 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 126:

/* Line 678 of lalr1.cc  */
#line 978 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 127:

/* Line 678 of lalr1.cc  */
#line 982 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 128:

/* Line 678 of lalr1.cc  */
#line 984 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 129:

/* Line 678 of lalr1.cc  */
#line 989 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { 
    Declarator::VectPtr v(new Declarator::Vect());
    v->push_back((yysemantic_stack_[(1) - (1)].declaratorVal));
    (yyval.declaratorVectVal) = v;
}
    break;

  case 130:

/* Line 678 of lalr1.cc  */
#line 995 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 131:

/* Line 678 of lalr1.cc  */
#line 1006 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.declaratorVal) = (yysemantic_stack_[(1) - (1)].declaratorVal); }
    break;

  case 132:

/* Line 678 of lalr1.cc  */
#line 1007 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.declaratorVal) = (yysemantic_stack_[(1) - (1)].declaratorVal); }
    break;

  case 133:

/* Line 678 of lalr1.cc  */
#line 1012 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Declarator::Ptr p(new Declarator((yysemantic_stack_[(1) - (1)].stringVal)));
    (yyval.declaratorVal) = p;
}
    break;

  case 134:

/* Line 678 of lalr1.cc  */
#line 1019 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yysemantic_stack_[(2) - (1)].declaratorVal)->addBound((yysemantic_stack_[(2) - (2)].integerVal));
    (yyval.declaratorVal) = (yysemantic_stack_[(2) - (1)].declaratorVal);
}
    break;

  case 135:

/* Line 678 of lalr1.cc  */
#line 1024 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yysemantic_stack_[(2) - (1)].declaratorVal)->addBound((yysemantic_stack_[(2) - (2)].integerVal));
    (yyval.declaratorVal) = (yysemantic_stack_[(2) - (1)].declaratorVal);
}
    break;

  case 136:

/* Line 678 of lalr1.cc  */
#line 1031 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.integerVal) = (yysemantic_stack_[(3) - (2)].integerVal);
}
    break;

  case 137:

/* Line 678 of lalr1.cc  */
#line 1039 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::floatType; }
    break;

  case 138:

/* Line 678 of lalr1.cc  */
#line 1040 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::doubleType; }
    break;

  case 139:

/* Line 678 of lalr1.cc  */
#line 1041 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::longdoubleType; }
    break;

  case 140:

/* Line 678 of lalr1.cc  */
#line 1046 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    IdlType::Ptr p(new FixedType((yysemantic_stack_[(6) - (3)].integerVal), (yysemantic_stack_[(6) - (5)].integerVal)));
    (yyval.typeVal) = p;
}
    break;

  case 141:

/* Line 678 of lalr1.cc  */
#line 1051 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 142:

/* Line 678 of lalr1.cc  */
#line 1054 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 143:

/* Line 678 of lalr1.cc  */
#line 1055 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 144:

/* Line 678 of lalr1.cc  */
#line 1059 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 145:

/* Line 678 of lalr1.cc  */
#line 1060 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 146:

/* Line 678 of lalr1.cc  */
#line 1061 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 147:

/* Line 678 of lalr1.cc  */
#line 1065 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::shortType; }
    break;

  case 148:

/* Line 678 of lalr1.cc  */
#line 1069 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::longType; }
    break;

  case 149:

/* Line 678 of lalr1.cc  */
#line 1073 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::longlongType; }
    break;

  case 150:

/* Line 678 of lalr1.cc  */
#line 1077 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 151:

/* Line 678 of lalr1.cc  */
#line 1078 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 152:

/* Line 678 of lalr1.cc  */
#line 1079 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 153:

/* Line 678 of lalr1.cc  */
#line 1083 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::ushortType; }
    break;

  case 154:

/* Line 678 of lalr1.cc  */
#line 1087 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::ulongType; }
    break;

  case 155:

/* Line 678 of lalr1.cc  */
#line 1091 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::ulonglongType; }
    break;

  case 156:

/* Line 678 of lalr1.cc  */
#line 1095 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::charType; }
    break;

  case 157:

/* Line 678 of lalr1.cc  */
#line 1099 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::wcharType; }
    break;

  case 158:

/* Line 678 of lalr1.cc  */
#line 1103 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::booleanType; }
    break;

  case 159:

/* Line 678 of lalr1.cc  */
#line 1107 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::octetType; }
    break;

  case 160:

/* Line 678 of lalr1.cc  */
#line 1111 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::anyType; }
    break;

  case 161:

/* Line 678 of lalr1.cc  */
#line 1120 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    IdlType::Ptr p(new StringType((yysemantic_stack_[(4) - (3)].integerVal)));
    (yyval.typeVal) = p; 
}
    break;

  case 162:

/* Line 678 of lalr1.cc  */
#line 1125 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.typeVal) = StringType::unboundedStringType;
}
    break;

  case 163:

/* Line 678 of lalr1.cc  */
#line 1131 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { 
    IdlType::Ptr p(new WStringType((yysemantic_stack_[(4) - (3)].integerVal)));
    (yyval.typeVal) = p; 
}
    break;

  case 164:

/* Line 678 of lalr1.cc  */
#line 1136 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.typeVal) = WStringType::unboundedWStringType;
}
    break;

  case 165:

/* Line 678 of lalr1.cc  */
#line 1146 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 166:

/* Line 678 of lalr1.cc  */
#line 1160 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 167:

/* Line 678 of lalr1.cc  */
#line 1162 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 168:

/* Line 678 of lalr1.cc  */
#line 1166 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 169:

/* Line 678 of lalr1.cc  */
#line 1252 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 170:

/* Line 678 of lalr1.cc  */
#line 1265 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 171:

/* Line 678 of lalr1.cc  */
#line 1267 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 172:

/* Line 678 of lalr1.cc  */
#line 1271 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 173:

/* Line 678 of lalr1.cc  */
#line 1285 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    IdlType::Ptr p(new SequenceType((yysemantic_stack_[(6) - (3)].typeVal), (yysemantic_stack_[(6) - (5)].integerVal)));
    (yyval.typeVal) = p;
}
    break;

  case 174:

/* Line 678 of lalr1.cc  */
#line 1290 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    IdlType::Ptr p(new SequenceType((yysemantic_stack_[(4) - (3)].typeVal), 0));  
    (yyval.typeVal) = p;
}
    break;



/* Line 678 of lalr1.cc  */
#line 2057 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.cpp"
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
  const short int Parser::yypact_ninf_ = -205;
  const short int
  Parser::yypact_[] =
  {
       153,   -39,   -26,    25,    34,    46,    55,    59,   154,   221,
      71,   -16,    92,    86,   127,  -205,  -205,  -205,  -205,  -205,
    -205,  -205,  -205,  -205,  -205,   152,  -205,  -205,   103,   113,
     155,   170,  -205,    14,  -205,  -205,   167,  -205,  -205,   179,
     180,  -205,  -205,    97,   133,  -205,  -205,  -205,  -205,  -205,
    -205,  -205,  -205,  -205,  -205,  -205,  -205,  -205,  -205,  -205,
    -205,  -205,  -205,   183,  -205,   135,  -205,  -205,  -205,  -205,
    -205,  -205,  -205,  -205,  -205,  -205,  -205,  -205,  -205,  -205,
    -205,  -205,  -205,   182,   137,  -205,  -205,   192,  -205,   143,
     197,   198,  -205,   201,   149,   221,  -205,  -205,   165,   166,
     168,  -205,   200,   215,   257,  -205,   219,  -205,   160,   160,
     176,   230,  -205,   228,    -4,   231,    22,    27,   233,  -205,
      -3,  -205,   135,   116,  -205,   229,   232,   246,  -205,    12,
     114,   135,   184,  -205,  -205,   226,  -205,   -24,   -35,  -205,
     250,  -205,   202,   251,    21,   254,   204,   258,   270,   176,
     284,    24,   286,   207,  -205,   149,    28,  -205,  -205,   239,
    -205,  -205,    12,    12,  -205,  -205,  -205,  -205,  -205,  -205,
    -205,    12,    12,    12,   209,  -205,  -205,  -205,   240,  -205,
     223,   241,   120,   290,  -205,  -205,   287,  -205,   291,  -205,
     293,   -48,  -205,   297,  -205,   298,   185,   185,   299,    61,
    -205,   301,  -205,   302,  -205,  -205,   295,   209,    -2,    35,
    -205,  -205,  -205,    12,    12,    12,    12,    12,    12,    12,
      12,    12,    12,   303,  -205,   294,  -205,   -24,  -205,   253,
     256,  -205,  -205,   259,  -205,   256,   304,   305,  -205,   306,
     260,  -205,   256,  -205,  -205,   263,  -205,   256,  -205,  -205,
      12,  -205,   102,   102,   -21,   -21,    44,    65,    65,    65,
      84,    84,  -205,   264,   316,  -205,  -205,  -205,  -205,  -205,
     266,   185,    12,  -205,  -205,  -205,  -205,   318,   209,  -205,
    -205,   300,  -205,  -205,   209,   320,    32,  -205,   269,   271,
     272,   273,    42,  -205,  -205,  -205,  -205,  -205,    32,   312,
    -205,   263,  -205,   291
  };

  /* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
     doesn't specify something else to do.  Zero means the default is an
     error.  */
  const unsigned char
  Parser::yydefact_[] =
  {
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     6,     5,     7,     8,    12,
      11,    10,     9,   109,   110,     0,    23,    32,     0,     0,
       0,     0,   147,   148,   137,   138,   141,   156,   157,   162,
     164,   158,   159,     0,     0,    74,    77,    70,   142,   144,
     145,   146,   143,   150,   151,   152,    71,    72,    73,    78,
      75,    76,   160,     0,   115,     0,   111,   113,   114,   112,
     116,   126,   117,   118,   119,   120,   121,   122,   124,   125,
     127,   128,   123,    56,     0,     1,     2,     0,     3,     0,
       0,     0,    20,    21,     0,     0,   149,   139,     0,     0,
       0,   153,   154,     0,     0,   133,   108,   129,   131,   132,
       0,     0,     4,     0,     0,     0,     0,     0,     0,   172,
       0,   170,     0,     0,   166,     0,     0,     0,   155,     0,
       0,     0,     0,   134,   135,    53,    57,     0,    43,    13,
       0,    14,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   169,     0,     0,   165,   167,     0,
     161,   163,     0,     0,    88,    89,    82,    79,    80,    81,
      84,     0,     0,     0,    69,    83,    87,   174,     0,   130,
       0,     0,     0,     0,    17,    16,    18,    19,    44,    15,
       0,     0,    24,     0,    25,     0,     0,     0,     0,     0,
      33,     0,    34,     0,   171,   168,     0,   103,     0,     0,
      90,    91,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   136,    54,   105,     0,   106,     0,
       0,    28,    30,    29,    26,     0,     0,    38,    47,    50,
       0,    39,     0,    42,    41,    43,    35,     0,   140,    85,
       0,    86,    94,    93,    96,    95,    97,   100,    99,    98,
     102,   101,   173,     0,     0,    46,    58,    27,    31,    36,
       0,     0,     0,    52,    37,    45,    40,     0,   104,    55,
     107,     0,    51,    48,    49,     0,    62,    22,     0,     0,
       0,     0,     0,    63,    65,    66,    67,    68,     0,    60,
      64,     0,    59,    61
  };

  /* YYPGOTO[NTERM-NUM].  */
  const short int
  Parser::yypgoto_[] =
  {
      -205,  -205,  -205,   321,  -205,  -205,   222,  -205,  -205,  -205,
    -205,   191,  -205,  -205,  -205,  -205,   186,    93,    38,  -205,
      69,   144,   193,  -205,  -204,  -205,  -205,  -205,    45,  -205,
    -205,  -155,  -205,  -205,  -205,  -205,  -205,  -134,    -7,   242,
    -205,  -205,  -205,   225,   213,  -205,  -205,   236,   340,   341,
     342,  -205,  -205,  -205,  -205,  -205,  -205,  -205,  -205,   343,
     344,   345,   346,  -205,   347,   348,     0,  -205,   234,     1,
    -205,   203,  -205
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const short int
  Parser::yydefgoto_[] =
  {
        -1,    12,    13,    14,    15,   114,   115,    16,    17,    90,
     144,   145,   233,    18,    91,   151,   152,   187,   188,   237,
     238,   239,   136,    19,   267,   281,   302,   292,   293,    20,
      44,   174,   175,   176,   208,    21,   182,    22,   240,    66,
      67,    68,    69,   106,   107,   108,   109,   133,    70,    71,
      72,    48,    49,    50,    51,    52,    53,    54,    55,    73,
      74,    75,    76,    77,    78,    79,    80,   123,   124,    81,
     120,   121,    82
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If zero, do what YYDEFACT says.  */
  const signed char Parser::yytable_ninf_ = -46;
  const short int
  Parser::yytable_[] =
  {
        23,    24,    65,   183,   139,   154,   249,   207,   209,   231,
     155,   250,   232,    23,    24,     6,   210,   211,   212,   162,
       7,    25,   184,     9,   185,   186,   163,   142,   142,   192,
     146,   269,   200,   146,    26,   164,   165,   205,   274,    84,
      96,   131,    97,   277,   217,   218,   219,   220,   221,   222,
     251,   288,   289,   290,   291,   298,   113,   299,   252,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   166,   167,
     168,   169,   170,   171,   172,   147,   148,   149,   147,   148,
     149,   143,   143,   173,   150,    27,    86,   150,   122,     1,
       2,     3,    85,   264,    28,   278,   213,   214,   215,   216,
     217,   218,   219,   220,   221,   222,    29,     4,     5,   -46,
     218,   219,   220,   221,   222,    30,   122,   284,   243,    31,
     244,   245,   101,   102,   157,     6,   177,   178,   226,   227,
       7,    83,     8,     9,   221,   222,    88,    23,    24,    10,
      11,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    62,   -46,   -46,     6,     1,     2,     3,    89,
       7,    63,    94,    92,    43,   215,   216,   217,   218,   219,
     220,   221,   222,    93,     4,     5,    64,    95,    98,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      99,   100,     6,   103,   104,   105,   111,     7,   110,     8,
       9,   112,    43,   113,   116,   117,    10,    11,   118,   119,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    62,   125,   126,     6,   127,   128,    23,    24,     7,
      63,   129,   131,    43,   132,   236,   135,   137,   138,   153,
     141,   180,   159,   181,   160,    64,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    62,   161,   189,
       6,   191,   190,   194,   195,     7,    63,   203,   196,    43,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     197,    64,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    62,   199,   202,   206,   223,   224,   228,
     -45,   225,    63,   230,   229,    43,   234,   248,   235,   242,
     246,   263,   247,   265,   286,   262,   266,    64,   271,   268,
     273,   270,   272,   275,   279,   280,   282,   285,   287,   294,
     301,   295,   296,   297,    87,   193,   140,   201,   276,   303,
     283,   241,   198,   300,   179,   134,   130,   156,    45,    46,
      47,    56,    57,    58,    59,    60,    61,   158,   204
  };

  /* YYCHECK.  */
  const unsigned short int
  Parser::yycheck_[] =
  {
         0,     0,     9,   137,     8,     8,     8,   162,   163,    57,
      13,    13,    60,    13,    13,    39,   171,   172,   173,     7,
      44,    60,    57,    47,    59,    60,    14,     6,     6,     8,
       6,   235,     8,     6,    60,    23,    24,     9,   242,    55,
      26,    13,    28,   247,    65,    66,    67,    68,    69,    70,
      15,    19,    20,    21,    22,    13,    60,    15,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,    56,    57,
      58,    59,    60,    61,    62,    51,    52,    53,    51,    52,
      53,    60,    60,    71,    60,    60,     0,    60,    95,     3,
       4,     5,     0,   227,    60,   250,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    60,    21,    22,    65,
      66,    67,    68,    69,    70,    60,   123,   272,    57,    60,
      59,    60,    25,    26,     8,    39,    12,    13,     8,     9,
      44,    60,    46,    47,    69,    70,     9,   137,   137,    53,
      54,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    69,    70,    39,     3,     4,     5,     7,
      44,    45,     7,    60,    48,    63,    64,    65,    66,    67,
      68,    69,    70,    60,    21,    22,    60,     7,    11,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      11,    11,    39,    60,    11,    60,    59,    44,    16,    46,
      47,     9,    48,    60,     7,     7,    53,    54,     7,    60,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    57,    57,    39,    57,    26,   227,   227,    44,
      45,    16,    13,    48,    74,    50,    60,     7,    10,     6,
       9,    57,    13,    17,    12,    60,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    12,     9,
      39,    10,    60,     9,    60,    44,    45,    60,    10,    48,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      10,    60,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    10,     9,    57,    57,    75,     9,
      13,    60,    45,    10,    13,    48,     9,    12,    10,    10,
       9,    17,    10,    60,    14,    12,    60,    60,    13,    60,
      60,    17,    16,    60,    60,     9,    60,     9,     8,    60,
      18,    60,    60,    60,    13,   144,   114,   151,   245,   301,
     271,   197,   149,   298,   131,   109,   104,   122,     8,     8,
       8,     8,     8,     8,     8,     8,     8,   123,   155
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  Parser::yystos_[] =
  {
         0,     3,     4,     5,    21,    22,    39,    44,    46,    47,
      53,    54,    77,    78,    79,    80,    83,    84,    89,    99,
     105,   111,   113,   142,   145,    60,    60,    60,    60,    60,
      60,    60,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    48,   106,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     140,   141,    36,    45,    60,   114,   115,   116,   117,   118,
     124,   125,   126,   135,   136,   137,   138,   139,   140,   141,
     142,   145,   148,    60,    55,     0,     0,    79,     9,     7,
      85,    90,    60,    60,     7,     7,    26,    28,    11,    11,
      11,    25,    26,    60,    11,    60,   119,   120,   121,   122,
      16,    59,     9,    60,    81,    82,     7,     7,     7,    60,
     146,   147,   114,   143,   144,    57,    57,    57,    26,    16,
     115,    13,    74,   123,   123,    60,    98,     7,    10,     8,
      82,     9,     6,    60,    86,    87,     6,    51,    52,    53,
      60,    91,    92,     6,     8,    13,   119,     8,   144,    13,
      12,    12,     7,    14,    23,    24,    56,    57,    58,    59,
      60,    61,    62,    71,   107,   108,   109,    12,    13,   120,
      57,    17,   112,   113,    57,    59,    60,    93,    94,     9,
      60,    10,     8,    87,     9,    60,    10,    10,    98,    10,
       8,    92,     9,    60,   147,     9,    57,   107,   110,   107,
     107,   107,   107,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    57,    75,    60,     8,     9,     9,    13,
      10,    57,    60,    88,     9,    10,    50,    95,    96,    97,
     114,    97,    10,    57,    59,    60,     9,    10,    12,     8,
      13,    15,   107,   107,   107,   107,   107,   107,   107,   107,
     107,   107,    12,    17,   113,    60,    60,   100,    60,   100,
      17,    13,    16,    60,   100,    60,    93,   100,   107,    60,
       9,   101,    60,    96,   107,     9,    14,     8,    19,    20,
      21,    22,   103,   104,    60,    60,    60,    60,    13,    15,
     104,    18,   102,    94
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
      79,    79,    79,    80,    81,    81,    82,    82,    82,    82,
      83,    83,    83,    85,    84,    86,    86,    87,    87,    87,
      88,    88,    90,    89,    91,    91,    92,    92,    92,    92,
      92,    92,    92,    93,    93,    94,    94,    95,    95,    96,
      96,    97,    97,    98,    98,    98,    99,    99,   101,   100,
     102,   102,   103,   103,   103,   104,   104,   104,   104,   105,
     106,   106,   106,   106,   106,   106,   106,   106,   106,   107,
     107,   107,   107,   107,   107,   107,   107,   107,   108,   108,
     109,   109,   109,   109,   109,   109,   109,   109,   109,   109,
     109,   109,   109,   110,   110,   111,   112,   112,   113,   113,
     113,   114,   114,   115,   115,   115,   116,   116,   116,   116,
     116,   116,   116,   117,   117,   117,   117,   118,   118,   119,
     119,   120,   120,   121,   122,   122,   123,   124,   124,   124,
     125,   125,   126,   126,   127,   127,   127,   128,   129,   130,
     131,   131,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   140,   141,   141,   142,   143,   143,   144,   145,
     146,   146,   147,   148,   148
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  Parser::yyr2_[] =
  {
         0,     2,     2,     2,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     5,     2,     3,     3,     3,     3,     3,
       3,     3,    10,     0,     6,     2,     3,     4,     3,     3,
       1,     2,     0,     6,     2,     3,     4,     4,     3,     3,
       4,     3,     3,     0,     1,     1,     3,     1,     3,     3,
       1,     3,     2,     1,     3,     5,     2,     4,     0,     6,
       0,     2,     0,     1,     3,     2,     2,     2,     2,     5,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     3,     1,     1,     1,
       2,     2,     2,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     1,     3,     6,     2,     4,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     1,     1,     1,     2,     2,     3,     1,     1,     2,
       6,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       1,     1,     1,     2,     2,     3,     1,     1,     1,     1,
       1,     4,     1,     4,     1,     5,     1,     2,     3,     5,
       1,     3,     1,     6,     4
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
  "task_fields", "task_field", "task_errors_list", "service_decl", "$@2",
  "service_fields", "service_field", "identifier_list", "identifiers",
  "inputs", "input", "input_type", "event_spec", "event_decl",
  "codel_prototype", "$@3", "codel_next", "codel_fields", "codel_field",
  "const_decl", "const_type", "literal", "boolean_literal",
  "composed_literal", "literals", "native_type_decl", "type_decl_list",
  "type_decl", "type_spec", "simple_type_spec", "base_type_spec",
  "template_type_spec", "constr_type_spec", "declarators", "declarator",
  "simple_declarator", "array_declarator", "fixed_array_size",
  "floating_pt_type", "fixed_pt_type", "integer_type", "signed_int",
  "signed_short_int", "signed_long_int", "signed_long_long_int",
  "unsigned_int", "unsigned_short_int", "unsigned_long_int",
  "unsigned_long_long_int", "char_type", "wide_char_type", "boolean_type",
  "octet_type", "any_type", "string_type", "wide_string_type",
  "struct_type", "members", "member", "enum_type", "enumerators",
  "enumerator", "sequence_type", 0
  };
#endif

#if YYDEBUG
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const Parser::rhs_number_type
  Parser::yyrhs_[] =
  {
        77,     0,    -1,    78,     0,    -1,    79,     9,    -1,    78,
      79,     9,    -1,    83,    -1,    80,    -1,    84,    -1,    89,
      -1,   113,    -1,   111,    -1,   105,    -1,    99,    -1,     3,
      60,     7,    81,     8,    -1,    82,     9,    -1,    81,    82,
       9,    -1,    60,    10,    59,    -1,    60,    10,    57,    -1,
      60,    10,    60,    -1,    60,    10,    93,    -1,    21,    60,
      60,    -1,    22,    60,    60,    -1,    22,    60,    60,     7,
       6,    60,    10,   100,     9,     8,    -1,    -1,     4,    60,
      85,     7,    86,     8,    -1,    87,     9,    -1,    86,    87,
       9,    -1,     6,    60,    10,   100,    -1,    60,    10,    57,
      -1,    60,    10,    88,    -1,    60,    -1,    88,    60,    -1,
      -1,     5,    60,    90,     7,    91,     8,    -1,    92,     9,
      -1,    91,    92,     9,    -1,     6,    60,    10,   100,    -1,
      53,    98,    10,   100,    -1,    51,    10,    95,    -1,    52,
      10,    97,    -1,    60,    10,    60,    93,    -1,    60,    10,
      59,    -1,    60,    10,    57,    -1,    -1,    94,    -1,    60,
      -1,    94,    13,    60,    -1,    96,    -1,    95,    13,    96,
      -1,    97,    16,   107,    -1,    97,    -1,    50,    17,    60,
      -1,   114,    60,    -1,    60,    -1,    60,    17,    60,    -1,
      60,    17,    60,    17,    60,    -1,    53,    60,    -1,    53,
      60,    16,    98,    -1,    -1,    60,   101,    14,   103,    15,
     102,    -1,    -1,    18,    94,    -1,    -1,   104,    -1,   103,
      13,   104,    -1,    19,    60,    -1,    20,    60,    -1,    21,
      60,    -1,    22,    60,    -1,    46,   106,    60,    16,   107,
      -1,   126,    -1,   135,    -1,   136,    -1,   137,    -1,   124,
      -1,   140,    -1,   141,    -1,   125,    -1,   138,    -1,    57,
      -1,    58,    -1,    59,    -1,    56,    -1,   108,    -1,    60,
      -1,     7,   110,     8,    -1,    14,   107,    15,    -1,   109,
      -1,    23,    -1,    24,    -1,    61,   107,    -1,    62,   107,
      -1,    71,   107,    -1,   107,    62,   107,    -1,   107,    61,
     107,    -1,   107,    64,   107,    -1,   107,    63,   107,    -1,
     107,    65,   107,    -1,   107,    68,   107,    -1,   107,    67,
     107,    -1,   107,    66,   107,    -1,   107,    70,   107,    -1,
     107,    69,   107,    -1,   107,    -1,   110,    13,   107,    -1,
      54,    55,    59,     7,   112,     8,    -1,   113,     9,    -1,
     112,     9,   113,     9,    -1,    47,   114,   119,    -1,   142,
      -1,   145,    -1,   115,    -1,   118,    -1,   116,    -1,   117,
      -1,    60,    -1,   124,    -1,   126,    -1,   135,    -1,   136,
      -1,   137,    -1,   138,    -1,   139,    -1,   148,    -1,   140,
      -1,   141,    -1,   125,    -1,   142,    -1,   145,    -1,   120,
      -1,   119,    13,   120,    -1,   121,    -1,   122,    -1,    60,
      -1,   121,   123,    -1,   122,   123,    -1,    74,    57,    75,
      -1,    27,    -1,    28,    -1,    26,    28,    -1,    29,    11,
      57,    13,    57,    12,    -1,    29,    -1,   127,    -1,   131,
      -1,   128,    -1,   129,    -1,   130,    -1,    25,    -1,    26,
      -1,    26,    26,    -1,   132,    -1,   133,    -1,   134,    -1,
      48,    25,    -1,    48,    26,    -1,    48,    26,    26,    -1,
      30,    -1,    31,    -1,    34,    -1,    35,    -1,    36,    -1,
      32,    11,    57,    12,    -1,    32,    -1,    33,    11,    57,
      12,    -1,    33,    -1,    44,    60,     7,   143,     8,    -1,
     144,    -1,   143,   144,    -1,   114,   119,     9,    -1,    39,
      60,     7,   146,     8,    -1,   147,    -1,   146,    13,   147,
      -1,    60,    -1,    45,    11,   115,    13,    57,    12,    -1,
      45,    11,   115,    12,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned short int
  Parser::yyprhs_[] =
  {
         0,     0,     3,     6,     9,    13,    15,    17,    19,    21,
      23,    25,    27,    29,    35,    38,    42,    46,    50,    54,
      58,    62,    66,    77,    78,    85,    88,    92,    97,   101,
     105,   107,   110,   111,   118,   121,   125,   130,   135,   139,
     143,   148,   152,   156,   157,   159,   161,   165,   167,   171,
     175,   177,   181,   184,   186,   190,   196,   199,   204,   205,
     212,   213,   216,   217,   219,   223,   226,   229,   232,   235,
     241,   243,   245,   247,   249,   251,   253,   255,   257,   259,
     261,   263,   265,   267,   269,   271,   275,   279,   281,   283,
     285,   288,   291,   294,   298,   302,   306,   310,   314,   318,
     322,   326,   330,   334,   336,   340,   347,   350,   355,   359,
     361,   363,   365,   367,   369,   371,   373,   375,   377,   379,
     381,   383,   385,   387,   389,   391,   393,   395,   397,   399,
     401,   405,   407,   409,   411,   414,   417,   421,   423,   425,
     428,   435,   437,   439,   441,   443,   445,   447,   449,   451,
     454,   456,   458,   460,   463,   466,   470,   472,   474,   476,
     478,   480,   485,   487,   492,   494,   500,   502,   505,   509,
     515,   517,   521,   523,   530
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned short int
  Parser::yyrline_[] =
  {
         0,   234,   234,   237,   238,   243,   245,   247,   249,   251,
     255,   257,   259,   266,   272,   273,   278,   289,   298,   312,
     327,   337,   347,   368,   367,   378,   379,   383,   387,   404,
     413,   417,   426,   425,   436,   437,   441,   445,   450,   453,
     457,   502,   512,   517,   520,   526,   530,   536,   538,   542,
     547,   553,   567,   578,   599,   636,   655,   660,   671,   670,
     682,   683,   690,   691,   693,   697,   708,   719,   723,   731,
     738,   739,   740,   741,   742,   743,   744,   745,   746,   751,
     755,   759,   763,   767,   771,   776,   780,   784,   788,   792,
     799,   805,   809,   816,   823,   830,   837,   844,   851,   858,
     865,   872,   879,   888,   894,   903,   909,   914,   921,   926,
     928,   940,   944,   950,   951,   952,   963,   964,   965,   966,
     967,   968,   969,   975,   976,   977,   978,   982,   984,   988,
     994,  1006,  1007,  1011,  1018,  1023,  1030,  1039,  1040,  1041,
    1045,  1050,  1054,  1055,  1059,  1060,  1061,  1065,  1069,  1073,
    1077,  1078,  1079,  1083,  1087,  1091,  1095,  1099,  1103,  1107,
    1111,  1119,  1124,  1130,  1135,  1145,  1159,  1161,  1165,  1251,
    1265,  1266,  1270,  1284,  1289
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
  const int Parser::yylast_ = 358;
  const int Parser::yynnts_ = 73;
  const int Parser::yyempty_ = -2;
  const int Parser::yyfinal_ = 85;
  const int Parser::yyterror_ = 1;
  const int Parser::yyerrcode_ = 256;
  const int Parser::yyntokens_ = 76;

  const unsigned int Parser::yyuser_token_number_max_ = 328;
  const Parser::token_number_type Parser::yyundef_token_ = 2;


/* Line 1054 of lalr1.cc  */
#line 1 "[Bison:b4_percent_define_default]"

} // G3nom

/* Line 1054 of lalr1.cc  */
#line 2800 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.cpp"


/* Line 1056 of lalr1.cc  */
#line 1295 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
 /*** Additional Code ***/

void G3nom::Parser::error(const Parser::location_type& l,
			    const std::string& m)
{
    driver.error(l, m);
}


