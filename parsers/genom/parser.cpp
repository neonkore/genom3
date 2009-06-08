
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
#line 216 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"


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
#line 95 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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
#line 237 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 5:

/* Line 678 of lalr1.cc  */
#line 242 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 6:

/* Line 678 of lalr1.cc  */
#line 244 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 7:

/* Line 678 of lalr1.cc  */
#line 246 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 8:

/* Line 678 of lalr1.cc  */
#line 248 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 9:

/* Line 678 of lalr1.cc  */
#line 250 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.component().addType((yysemantic_stack_[(1) - (1)].typeVal));
}
    break;

  case 10:

/* Line 678 of lalr1.cc  */
#line 254 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 11:

/* Line 678 of lalr1.cc  */
#line 261 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.component().setName((yysemantic_stack_[(5) - (2)].stringVal));
}
    break;

  case 13:

/* Line 678 of lalr1.cc  */
#line 268 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 14:

/* Line 678 of lalr1.cc  */
#line 273 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 15:

/* Line 678 of lalr1.cc  */
#line 284 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    if((yysemantic_stack_[(3) - (1)].stringVal) == "uniqueId") {
	driver.component().uniqueId = (yysemantic_stack_[(3) - (3)].integerVal);
    } else {
      error(yyloc, std::string("Unknown component field: ") + (yysemantic_stack_[(3) - (1)].stringVal));
      YYERROR;
    }}
    break;

  case 16:

/* Line 678 of lalr1.cc  */
#line 292 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 17:

/* Line 678 of lalr1.cc  */
#line 312 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 18:

/* Line 678 of lalr1.cc  */
#line 322 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 19:

/* Line 678 of lalr1.cc  */
#line 336 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Task::Ptr t(new Task((yysemantic_stack_[(2) - (2)].stringVal)));
    driver.setCurrentTask(t);
}
    break;

  case 20:

/* Line 678 of lalr1.cc  */
#line 341 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.component().addTask(driver.currentTask());
}
    break;

  case 22:

/* Line 678 of lalr1.cc  */
#line 348 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 23:

/* Line 678 of lalr1.cc  */
#line 352 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.currentTask()->addCodel((yysemantic_stack_[(4) - (2)].stringVal), (yysemantic_stack_[(4) - (4)].codelVal));
}
    break;

  case 24:

/* Line 678 of lalr1.cc  */
#line 356 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 25:

/* Line 678 of lalr1.cc  */
#line 373 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    if((yysemantic_stack_[(3) - (1)].stringVal) != "errors") {
      error(yyloc, std::string("Wrong arguments for field: ") + (yysemantic_stack_[(3) - (1)].stringVal));
      YYERROR;
    }
}
    break;

  case 26:

/* Line 678 of lalr1.cc  */
#line 382 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.currentTask()->addErrorMessage((yysemantic_stack_[(1) - (1)].stringVal));
}
    break;

  case 27:

/* Line 678 of lalr1.cc  */
#line 386 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.currentTask()->addErrorMessage((yysemantic_stack_[(2) - (2)].stringVal));
}
    break;

  case 28:

/* Line 678 of lalr1.cc  */
#line 394 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Service::Ptr s(new Service((yysemantic_stack_[(2) - (2)].stringVal)));
    driver.setCurrentService(s);
}
    break;

  case 29:

/* Line 678 of lalr1.cc  */
#line 399 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.component().addService(driver.currentService());
}
    break;

  case 31:

/* Line 678 of lalr1.cc  */
#line 406 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 32:

/* Line 678 of lalr1.cc  */
#line 410 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.currentService()->addCodel((yysemantic_stack_[(4) - (2)].stringVal), (yysemantic_stack_[(4) - (4)].codelVal));
}
    break;

  case 33:

/* Line 678 of lalr1.cc  */
#line 414 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
}
    break;

  case 34:

/* Line 678 of lalr1.cc  */
#line 417 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.currentService()->output = (yysemantic_stack_[(3) - (3)].serviceInputVal);
}
    break;

  case 35:

/* Line 678 of lalr1.cc  */
#line 421 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 36:

/* Line 678 of lalr1.cc  */
#line 466 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 37:

/* Line 678 of lalr1.cc  */
#line 476 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 38:

/* Line 678 of lalr1.cc  */
#line 480 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.stringVal) = "";
}
    break;

  case 39:

/* Line 678 of lalr1.cc  */
#line 484 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.stringVal) = (yysemantic_stack_[(1) - (1)].stringVal);
}
    break;

  case 40:

/* Line 678 of lalr1.cc  */
#line 490 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
   (yyval.stringVal) = (yysemantic_stack_[(1) - (1)].stringVal);
}
    break;

  case 41:

/* Line 678 of lalr1.cc  */
#line 494 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.stringVal) = (yysemantic_stack_[(2) - (1)].stringVal) + " " + (yysemantic_stack_[(2) - (2)].stringVal);
}
    break;

  case 42:

/* Line 678 of lalr1.cc  */
#line 500 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 43:

/* Line 678 of lalr1.cc  */
#line 502 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 44:

/* Line 678 of lalr1.cc  */
#line 506 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yysemantic_stack_[(3) - (1)].serviceInputVal).defaultValue = (yysemantic_stack_[(3) - (3)].literalVal);
    driver.currentService()->addInput((yysemantic_stack_[(3) - (1)].serviceInputVal));
}
    break;

  case 45:

/* Line 678 of lalr1.cc  */
#line 511 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.currentService()->addInput((yysemantic_stack_[(1) - (1)].serviceInputVal));
}
    break;

  case 46:

/* Line 678 of lalr1.cc  */
#line 517 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 47:

/* Line 678 of lalr1.cc  */
#line 531 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Service::Input i;
    i.kind = Service::Input::Type;
    i.identifier = (yysemantic_stack_[(2) - (2)].stringVal);
    i.type = (yysemantic_stack_[(2) - (1)].typeVal);
    (yyval.serviceInputVal) = i;
}
    break;

  case 48:

/* Line 678 of lalr1.cc  */
#line 543 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Codel::Ptr c(new Codel((yysemantic_stack_[(1) - (1)].stringVal)));
    driver.setCurrentCodel(c);
}
    break;

  case 49:

/* Line 678 of lalr1.cc  */
#line 548 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.codelVal) = driver.currentCodel();
}
    break;

  case 50:

/* Line 678 of lalr1.cc  */
#line 554 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 51:

/* Line 678 of lalr1.cc  */
#line 556 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 52:

/* Line 678 of lalr1.cc  */
#line 558 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 53:

/* Line 678 of lalr1.cc  */
#line 562 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 54:

/* Line 678 of lalr1.cc  */
#line 573 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 55:

/* Line 678 of lalr1.cc  */
#line 584 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.currentCodel()->addInPort((yysemantic_stack_[(2) - (2)].stringVal));
}
    break;

  case 56:

/* Line 678 of lalr1.cc  */
#line 588 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.currentCodel()->addOutPort((yysemantic_stack_[(2) - (2)].stringVal));
}
    break;

  case 57:

/* Line 678 of lalr1.cc  */
#line 596 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    ConstValue c((yysemantic_stack_[(5) - (3)].stringVal), (yysemantic_stack_[(5) - (2)].typeVal), (yysemantic_stack_[(5) - (5)].literalVal));
    driver.component().addConstValue(c);
}
    break;

  case 67:

/* Line 678 of lalr1.cc  */
#line 616 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = Literal((yysemantic_stack_[(1) - (1)].integerVal));
}
    break;

  case 68:

/* Line 678 of lalr1.cc  */
#line 620 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = Literal((yysemantic_stack_[(1) - (1)].doubleVal));
}
    break;

  case 69:

/* Line 678 of lalr1.cc  */
#line 624 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = Literal("\"" + (yysemantic_stack_[(1) - (1)].stringVal) + "\"");
}
    break;

  case 70:

/* Line 678 of lalr1.cc  */
#line 628 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = Literal((yysemantic_stack_[(1) - (1)].charVal));
}
    break;

  case 71:

/* Line 678 of lalr1.cc  */
#line 632 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = (yysemantic_stack_[(1) - (1)].literalVal);
}
    break;

  case 72:

/* Line 678 of lalr1.cc  */
#line 636 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    /*enum value*/
    (yyval.literalVal) = Literal((yysemantic_stack_[(1) - (1)].stringVal));
}
    break;

  case 73:

/* Line 678 of lalr1.cc  */
#line 641 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = (yysemantic_stack_[(3) - (2)].literalVal);
}
    break;

  case 74:

/* Line 678 of lalr1.cc  */
#line 645 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = (yysemantic_stack_[(3) - (2)].literalVal);
}
    break;

  case 75:

/* Line 678 of lalr1.cc  */
#line 649 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 76:

/* Line 678 of lalr1.cc  */
#line 653 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = Literal(true);
}
    break;

  case 77:

/* Line 678 of lalr1.cc  */
#line 657 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = Literal(false);
}
    break;

  case 78:

/* Line 678 of lalr1.cc  */
#line 664 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Neg);
    l.addMember((yysemantic_stack_[(2) - (2)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 79:

/* Line 678 of lalr1.cc  */
#line 670 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = (yysemantic_stack_[(2) - (2)].literalVal);
}
    break;

  case 80:

/* Line 678 of lalr1.cc  */
#line 674 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::LNeg);
    l.addMember((yysemantic_stack_[(2) - (2)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 81:

/* Line 678 of lalr1.cc  */
#line 681 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Plus);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 82:

/* Line 678 of lalr1.cc  */
#line 688 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Minus);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 83:

/* Line 678 of lalr1.cc  */
#line 695 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Times);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 84:

/* Line 678 of lalr1.cc  */
#line 702 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Divide);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 85:

/* Line 678 of lalr1.cc  */
#line 709 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Mod);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 86:

/* Line 678 of lalr1.cc  */
#line 716 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::And);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 87:

/* Line 678 of lalr1.cc  */
#line 723 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Or);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 88:

/* Line 678 of lalr1.cc  */
#line 730 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Xor);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 89:

/* Line 678 of lalr1.cc  */
#line 737 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::LShift);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 90:

/* Line 678 of lalr1.cc  */
#line 744 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::RShift);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 91:

/* Line 678 of lalr1.cc  */
#line 753 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Struct);
    l.addMember((yysemantic_stack_[(1) - (1)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 92:

/* Line 678 of lalr1.cc  */
#line 759 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yysemantic_stack_[(3) - (1)].literalVal).addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = (yysemantic_stack_[(3) - (1)].literalVal);
}
    break;

  case 93:

/* Line 678 of lalr1.cc  */
#line 768 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    IdlType::Ptr p(new TypedefType((yysemantic_stack_[(3) - (2)].typeVal), (yysemantic_stack_[(3) - (3)].declaratorVectVal)));
    (yyval.typeVal) = p;
}
    break;

  case 94:

/* Line 678 of lalr1.cc  */
#line 772 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 95:

/* Line 678 of lalr1.cc  */
#line 774 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 96:

/* Line 678 of lalr1.cc  */
#line 789 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal);
}
    break;

  case 97:

/* Line 678 of lalr1.cc  */
#line 793 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { 
    (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal);
}
    break;

  case 98:

/* Line 678 of lalr1.cc  */
#line 798 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 99:

/* Line 678 of lalr1.cc  */
#line 799 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 100:

/* Line 678 of lalr1.cc  */
#line 801 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    IdlType::Ptr type = driver.component().typeFromName((yysemantic_stack_[(1) - (1)].stringVal));
    if(!type.get()) {
	error(yyloc, std::string("Unknown type: ") + (yysemantic_stack_[(1) - (1)].stringVal));
	YYERROR;
    }
    (yyval.typeVal) = type;
}
    break;

  case 101:

/* Line 678 of lalr1.cc  */
#line 811 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 102:

/* Line 678 of lalr1.cc  */
#line 812 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 103:

/* Line 678 of lalr1.cc  */
#line 813 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 104:

/* Line 678 of lalr1.cc  */
#line 814 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 105:

/* Line 678 of lalr1.cc  */
#line 815 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 106:

/* Line 678 of lalr1.cc  */
#line 816 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 107:

/* Line 678 of lalr1.cc  */
#line 817 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 108:

/* Line 678 of lalr1.cc  */
#line 823 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 109:

/* Line 678 of lalr1.cc  */
#line 824 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 110:

/* Line 678 of lalr1.cc  */
#line 825 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 111:

/* Line 678 of lalr1.cc  */
#line 826 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 112:

/* Line 678 of lalr1.cc  */
#line 830 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 113:

/* Line 678 of lalr1.cc  */
#line 832 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 114:

/* Line 678 of lalr1.cc  */
#line 837 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { 
    Declarator::VectPtr v(new Declarator::Vect());
    v->push_back((yysemantic_stack_[(1) - (1)].declaratorVal));
    (yyval.declaratorVectVal) = v;
}
    break;

  case 115:

/* Line 678 of lalr1.cc  */
#line 843 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 116:

/* Line 678 of lalr1.cc  */
#line 854 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.declaratorVal) = (yysemantic_stack_[(1) - (1)].declaratorVal); }
    break;

  case 117:

/* Line 678 of lalr1.cc  */
#line 855 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.declaratorVal) = (yysemantic_stack_[(1) - (1)].declaratorVal); }
    break;

  case 118:

/* Line 678 of lalr1.cc  */
#line 860 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Declarator::Ptr p(new Declarator((yysemantic_stack_[(1) - (1)].stringVal)));
    (yyval.declaratorVal) = p;
}
    break;

  case 119:

/* Line 678 of lalr1.cc  */
#line 867 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yysemantic_stack_[(2) - (1)].declaratorVal)->addBound((yysemantic_stack_[(2) - (2)].integerVal));
    (yyval.declaratorVal) = (yysemantic_stack_[(2) - (1)].declaratorVal);
}
    break;

  case 120:

/* Line 678 of lalr1.cc  */
#line 872 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yysemantic_stack_[(2) - (1)].declaratorVal)->addBound((yysemantic_stack_[(2) - (2)].integerVal));
    (yyval.declaratorVal) = (yysemantic_stack_[(2) - (1)].declaratorVal);
}
    break;

  case 121:

/* Line 678 of lalr1.cc  */
#line 879 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.integerVal) = (yysemantic_stack_[(3) - (2)].integerVal);
}
    break;

  case 122:

/* Line 678 of lalr1.cc  */
#line 887 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::floatType; }
    break;

  case 123:

/* Line 678 of lalr1.cc  */
#line 888 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::doubleType; }
    break;

  case 124:

/* Line 678 of lalr1.cc  */
#line 889 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::longdoubleType; }
    break;

  case 125:

/* Line 678 of lalr1.cc  */
#line 894 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    IdlType::Ptr p(new FixedType((yysemantic_stack_[(6) - (3)].integerVal), (yysemantic_stack_[(6) - (5)].integerVal)));
    (yyval.typeVal) = p;
}
    break;

  case 126:

/* Line 678 of lalr1.cc  */
#line 899 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 127:

/* Line 678 of lalr1.cc  */
#line 902 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 128:

/* Line 678 of lalr1.cc  */
#line 903 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 129:

/* Line 678 of lalr1.cc  */
#line 907 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 130:

/* Line 678 of lalr1.cc  */
#line 908 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 131:

/* Line 678 of lalr1.cc  */
#line 909 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 132:

/* Line 678 of lalr1.cc  */
#line 913 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::shortType; }
    break;

  case 133:

/* Line 678 of lalr1.cc  */
#line 917 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::longType; }
    break;

  case 134:

/* Line 678 of lalr1.cc  */
#line 921 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::longlongType; }
    break;

  case 135:

/* Line 678 of lalr1.cc  */
#line 925 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 136:

/* Line 678 of lalr1.cc  */
#line 926 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 137:

/* Line 678 of lalr1.cc  */
#line 927 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 138:

/* Line 678 of lalr1.cc  */
#line 931 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::ushortType; }
    break;

  case 139:

/* Line 678 of lalr1.cc  */
#line 935 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::ulongType; }
    break;

  case 140:

/* Line 678 of lalr1.cc  */
#line 939 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::ulonglongType; }
    break;

  case 141:

/* Line 678 of lalr1.cc  */
#line 943 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::charType; }
    break;

  case 142:

/* Line 678 of lalr1.cc  */
#line 947 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::wcharType; }
    break;

  case 143:

/* Line 678 of lalr1.cc  */
#line 951 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::booleanType; }
    break;

  case 144:

/* Line 678 of lalr1.cc  */
#line 955 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::octetType; }
    break;

  case 145:

/* Line 678 of lalr1.cc  */
#line 959 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::anyType; }
    break;

  case 146:

/* Line 678 of lalr1.cc  */
#line 968 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    IdlType::Ptr p(new StringType((yysemantic_stack_[(4) - (3)].integerVal)));
    (yyval.typeVal) = p; 
}
    break;

  case 147:

/* Line 678 of lalr1.cc  */
#line 973 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.typeVal) = StringType::unboundedStringType;
}
    break;

  case 148:

/* Line 678 of lalr1.cc  */
#line 979 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { 
    IdlType::Ptr p(new WStringType((yysemantic_stack_[(4) - (3)].integerVal)));
    (yyval.typeVal) = p; 
}
    break;

  case 149:

/* Line 678 of lalr1.cc  */
#line 984 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.typeVal) = WStringType::unboundedWStringType;
}
    break;

  case 150:

/* Line 678 of lalr1.cc  */
#line 994 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 151:

/* Line 678 of lalr1.cc  */
#line 1008 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 152:

/* Line 678 of lalr1.cc  */
#line 1010 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 153:

/* Line 678 of lalr1.cc  */
#line 1014 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 154:

/* Line 678 of lalr1.cc  */
#line 1100 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 155:

/* Line 678 of lalr1.cc  */
#line 1113 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 156:

/* Line 678 of lalr1.cc  */
#line 1115 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 157:

/* Line 678 of lalr1.cc  */
#line 1119 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 158:

/* Line 678 of lalr1.cc  */
#line 1133 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    IdlType::Ptr p(new SequenceType((yysemantic_stack_[(6) - (3)].typeVal), (yysemantic_stack_[(6) - (5)].integerVal)));
    (yyval.typeVal) = p;
}
    break;

  case 159:

/* Line 678 of lalr1.cc  */
#line 1138 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    IdlType::Ptr p(new SequenceType((yysemantic_stack_[(4) - (3)].typeVal), 0));  
    (yyval.typeVal) = p;
}
    break;



/* Line 678 of lalr1.cc  */
#line 1843 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.cpp"
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
  const short int Parser::yypact_ninf_ = -148;
  const short int
  Parser::yypact_[] =
  {
       127,   -13,    14,    37,    42,    68,    80,    82,   249,   183,
     124,    99,   130,  -148,  -148,  -148,  -148,  -148,  -148,  -148,
    -148,   133,  -148,  -148,    87,   107,   138,   156,  -148,    -8,
    -148,  -148,   154,  -148,  -148,   155,   157,  -148,  -148,     2,
     112,  -148,  -148,  -148,  -148,  -148,  -148,  -148,  -148,  -148,
    -148,  -148,  -148,  -148,  -148,  -148,  -148,  -148,  -148,   159,
    -148,   131,  -148,  -148,  -148,  -148,  -148,  -148,  -148,  -148,
    -148,  -148,  -148,  -148,  -148,  -148,  -148,  -148,  -148,  -148,
    -148,   176,  -148,   134,   181,   184,  -148,  -148,   135,   183,
    -148,  -148,   142,   143,   145,  -148,   194,   203,   216,  -148,
     208,  -148,   153,   153,  -148,   213,    11,   215,     1,    18,
    -148,     9,  -148,   131,    83,  -148,   214,   218,   219,  -148,
       6,   109,   131,   180,  -148,  -148,    41,  -148,   224,  -148,
     173,   225,     0,   227,   179,   241,   242,   243,    15,   228,
    -148,   135,    -4,  -148,  -148,   202,  -148,  -148,     6,     6,
    -148,  -148,  -148,  -148,  -148,  -148,  -148,     6,     6,     6,
     -27,  -148,  -148,  -148,   204,  -148,   185,  -148,  -148,  -148,
    -148,   247,    19,  -148,   251,  -148,   248,   150,   150,    46,
    -148,   252,  -148,  -148,  -148,   253,   -27,    31,    25,  -148,
    -148,  -148,     6,     6,     6,     6,     6,     6,     6,     6,
       6,     6,   254,  -148,   209,  -148,  -148,   212,  -148,   209,
     258,   250,  -148,   267,   229,  -148,  -148,  -148,   230,  -148,
    -148,  -148,     6,  -148,    90,    90,    96,    96,   139,    63,
      63,    63,    69,    69,  -148,  -148,  -148,  -148,  -148,   231,
     150,     6,  -148,  -148,  -148,   232,   -27,   255,  -148,  -148,
     -27,  -148,    59,   233,   234,   235,   236,    28,  -148,  -148,
    -148,  -148,  -148,    59,  -148,  -148
  };

  /* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
     doesn't specify something else to do.  Zero means the default is an
     error.  */
  const unsigned char
  Parser::yydefact_[] =
  {
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     6,     5,     7,     8,    10,     9,    94,
      95,     0,    19,    28,     0,     0,     0,     0,   132,   133,
     122,   123,   126,   141,   142,   147,   149,   143,   144,     0,
       0,    62,    65,    58,   127,   129,   130,   131,   128,   135,
     136,   137,    59,    60,    61,    66,    63,    64,   145,     0,
     100,     0,    96,    98,    99,    97,   101,   111,   102,   103,
     104,   105,   106,   107,   109,   110,   112,   113,   108,     1,
       2,     0,     3,     0,     0,     0,    17,    18,     0,     0,
     134,   124,     0,     0,     0,   138,   139,     0,     0,   118,
      93,   114,   116,   117,     4,     0,     0,     0,     0,     0,
     157,     0,   155,     0,     0,   151,     0,     0,     0,   140,
       0,     0,     0,     0,   119,   120,     0,    11,     0,    12,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     154,     0,     0,   150,   152,     0,   146,   148,     0,     0,
      76,    77,    70,    67,    68,    69,    72,     0,     0,     0,
      57,    71,    75,   159,     0,   115,     0,    15,    14,    16,
      13,     0,     0,    20,     0,    21,     0,     0,     0,     0,
      29,     0,    30,   156,   153,     0,    91,     0,     0,    78,
      79,    80,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   121,     0,    24,    26,    25,    22,     0,
       0,    33,    42,    45,     0,    34,    37,    36,    38,    31,
     125,    73,     0,    74,    82,    81,    84,    83,    85,    88,
      87,    86,    90,    89,   158,    48,    23,    27,    32,     0,
       0,     0,    47,    40,    35,    39,    92,     0,    46,    43,
      44,    41,    50,     0,     0,     0,     0,     0,    51,    53,
      54,    55,    56,     0,    49,    52
  };

  /* YYPGOTO[NTERM-NUM].  */
  const short int
  Parser::yypgoto_[] =
  {
      -148,  -148,  -148,   259,  -148,  -148,   186,  -148,  -148,  -148,
    -148,   161,  -148,  -148,  -148,  -148,   158,  -148,  -148,  -148,
      54,   119,    89,  -148,  -148,    36,  -148,  -148,  -147,  -148,
    -148,  -148,  -148,    -9,   205,  -148,  -148,  -148,   187,   182,
    -148,  -148,   198,   294,   297,   298,  -148,  -148,  -148,  -148,
    -148,  -148,  -148,  -148,   299,   300,   301,   302,  -148,   303,
     304,     3,  -148,   199,     4,  -148,   174,  -148
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const short int
  Parser::yydefgoto_[] =
  {
        -1,    10,    11,    12,    13,   106,   107,    14,    15,    84,
     132,   133,   207,    16,    85,   138,   139,   244,   245,   211,
     212,   213,   236,   247,   257,   258,    17,    40,   160,   161,
     162,   187,    18,   214,    62,    63,    64,    65,   100,   101,
     102,   103,   124,    66,    67,    68,    44,    45,    46,    47,
      48,    49,    50,    51,    69,    70,    71,    72,    73,    74,
      75,    76,   114,   115,    77,   111,   112,    78
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If zero, do what YYDEFACT says.  */
  const signed char Parser::yytable_ninf_ = -1;
  const short int
  Parser::yytable_[] =
  {
        61,   186,   188,    19,    20,   184,   130,   130,   173,   122,
     189,   190,   191,   148,    19,    20,    90,   140,    91,   127,
     149,   134,   141,   180,   134,    95,    96,   150,   151,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   221,
     223,   263,    21,   264,   222,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   131,   131,   152,   153,   154,
     155,   156,   157,   158,   135,   136,   105,   135,   136,    22,
     137,   205,   159,   137,   206,   246,   253,   254,   255,   256,
     113,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   143,    23,   167,   250,   168,   169,    24,   216,    80,
     217,   218,     1,     2,     3,   113,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    58,     4,     5,
       6,   163,   164,    25,    79,     7,    59,   200,   201,    39,
       1,     2,     3,    -1,    -1,    26,     6,    27,    60,    82,
      83,     7,    86,     8,     9,    88,     4,     5,   194,   195,
     196,   197,   198,   199,   200,   201,   196,   197,   198,   199,
     200,   201,    87,    89,     6,    92,    93,    97,    94,     7,
      98,     8,     9,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    58,   104,    99,     6,   108,   105,
     110,   109,     7,    59,   116,   117,    39,   118,   210,    -1,
     197,   198,   199,   200,   201,    60,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    58,   119,   120,
       6,   122,   123,   126,   129,     7,    59,   145,   171,    39,
     146,   147,   166,   170,   176,   172,   175,   182,    60,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      58,   177,   178,   179,   185,   203,   202,   204,   209,    59,
     208,   219,    39,   240,   235,   220,   234,   237,   239,   252,
      81,    60,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,   241,   242,   243,   248,   251,   259,   260,
     261,   262,   128,   174,   249,    39,   181,   215,   238,   265,
     142,   125,    41,   121,   165,    42,    43,    52,    53,    54,
      55,    56,    57,   144,     0,   183
  };

  /* YYCHECK.  */
  const short int
  Parser::yycheck_[] =
  {
         9,   148,   149,     0,     0,     9,     6,     6,     8,    13,
     157,   158,   159,     7,    11,    11,    24,     8,    26,     8,
      14,     6,    13,     8,     6,    23,    24,    21,    22,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,     8,
      15,    13,    55,    15,    13,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,    55,    55,    51,    52,    53,
      54,    55,    56,    57,    49,    50,    55,    49,    50,    55,
      55,    52,    66,    55,    55,   222,    17,    18,    19,    20,
      89,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,     8,    55,    52,   241,    54,    55,    55,    52,     0,
      54,    55,     3,     4,     5,   114,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    19,    20,
      37,    12,    13,    55,     0,    42,    43,    64,    65,    46,
       3,     4,     5,    64,    65,    55,    37,    55,    55,     9,
       7,    42,    55,    44,    45,     7,    19,    20,    58,    59,
      60,    61,    62,    63,    64,    65,    60,    61,    62,    63,
      64,    65,    55,     7,    37,    11,    11,    55,    11,    42,
      11,    44,    45,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,     9,    55,    37,     7,    55,
      55,     7,    42,    43,    52,    52,    46,    52,    48,    60,
      61,    62,    63,    64,    65,    55,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    24,    16,
      37,    13,    69,    10,     9,    42,    43,    13,    55,    46,
      12,    12,    52,     9,    55,    10,     9,     9,    55,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    10,    10,    10,    52,    70,    52,    10,    10,    43,
       9,     9,    46,    13,    55,    12,    12,    55,    10,    14,
      11,    55,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    16,    55,    55,    55,    55,    55,    55,
      55,    55,   106,   132,   240,    46,   138,   178,   209,   263,
     113,   103,     8,    98,   122,     8,     8,     8,     8,     8,
       8,     8,     8,   114,    -1,   141
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  Parser::yystos_[] =
  {
         0,     3,     4,     5,    19,    20,    37,    42,    44,    45,
      72,    73,    74,    75,    78,    79,    84,    97,   103,   132,
     135,    55,    55,    55,    55,    55,    55,    55,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    46,
      98,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   130,   131,    34,    43,
      55,   104,   105,   106,   107,   108,   114,   115,   116,   125,
     126,   127,   128,   129,   130,   131,   132,   135,   138,     0,
       0,    74,     9,     7,    80,    85,    55,    55,     7,     7,
      24,    26,    11,    11,    11,    23,    24,    55,    11,    55,
     109,   110,   111,   112,     9,    55,    76,    77,     7,     7,
      55,   136,   137,   104,   133,   134,    52,    52,    52,    24,
      16,   105,    13,    69,   113,   113,    10,     8,    77,     9,
       6,    55,    81,    82,     6,    49,    50,    55,    86,    87,
       8,    13,   109,     8,   134,    13,    12,    12,     7,    14,
      21,    22,    51,    52,    53,    54,    55,    56,    57,    66,
      99,   100,   101,    12,    13,   110,    52,    52,    54,    55,
       9,    55,    10,     8,    82,     9,    55,    10,    10,    10,
       8,    87,     9,   137,     9,    52,    99,   102,    99,    99,
      99,    99,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    52,    70,    10,    52,    55,    83,     9,    10,
      48,    90,    91,    92,   104,    92,    52,    54,    55,     9,
      12,     8,    13,    15,    99,    99,    99,    99,    99,    99,
      99,    99,    99,    99,    12,    55,    93,    55,    93,    10,
      13,    16,    55,    55,    88,    89,    99,    94,    55,    91,
      99,    55,    14,    17,    18,    19,    20,    95,    96,    55,
      55,    55,    55,    13,    15,    96
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
     315,   316,   317,   318,   319,   320,   321,   322,   323,    91,
      93
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  Parser::yyr1_[] =
  {
         0,    71,    72,    73,    73,    74,    74,    74,    74,    74,
      74,    75,    76,    76,    77,    77,    77,    78,    78,    80,
      79,    81,    81,    82,    82,    82,    83,    83,    85,    84,
      86,    86,    87,    87,    87,    87,    87,    87,    88,    88,
      89,    89,    90,    90,    91,    91,    92,    92,    94,    93,
      95,    95,    95,    96,    96,    96,    96,    97,    98,    98,
      98,    98,    98,    98,    98,    98,    98,    99,    99,    99,
      99,    99,    99,    99,    99,    99,   100,   100,   101,   101,
     101,   101,   101,   101,   101,   101,   101,   101,   101,   101,
     101,   102,   102,   103,   103,   103,   104,   104,   105,   105,
     105,   106,   106,   106,   106,   106,   106,   106,   107,   107,
     107,   107,   108,   108,   109,   109,   110,   110,   111,   112,
     112,   113,   114,   114,   114,   115,   115,   116,   116,   117,
     117,   117,   118,   119,   120,   121,   121,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   130,   131,   131,
     132,   133,   133,   134,   135,   136,   136,   137,   138,   138
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  Parser::yyr2_[] =
  {
         0,     2,     2,     2,     3,     1,     1,     1,     1,     1,
       1,     5,     2,     3,     3,     3,     3,     3,     3,     0,
       6,     2,     3,     4,     3,     3,     1,     2,     0,     6,
       2,     3,     4,     3,     3,     4,     3,     3,     0,     1,
       1,     2,     1,     3,     3,     1,     3,     2,     0,     5,
       0,     1,     3,     2,     2,     2,     2,     5,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     3,     1,     1,     1,     2,     2,
       2,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     3,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     1,     1,     1,     2,
       2,     3,     1,     1,     2,     6,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     1,     1,     1,     2,     2,
       3,     1,     1,     1,     1,     1,     4,     1,     4,     1,
       5,     1,     2,     3,     5,     1,     3,     1,     6,     4
  };

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
  /* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
     First, the terminals, then, starting at \a yyntokens_, nonterminals.  */
  const char*
  const Parser::yytname_[] =
  {
    "\"end of file\"", "error", "$undefined", "\"component\"", "\"task\"",
  "\"service\"", "\"codel\"", "LBRACE", "RBRACE", "SEMICOLON", "COLON",
  "LESS_THAN", "GREATER_THAN", "COMMA", "LPAREN", "RPAREN", "EQUAL", "IN",
  "OUT", "INPORT", "OUTPORT", "TRUE", "FALSE", "SHORT", "LONG", "FLOAT",
  "DOUBLE", "FIXED", "CHAR", "WCHAR", "STRING", "WSTRING", "BOOLEAN",
  "OCTET", "ANY", "VOID", "NATIVE", "ENUM", "UNION", "SWITCH", "CASE",
  "DEFAULT", "STRUCT", "SEQUENCE", "CONST", "TYPEDEF", "UNSIGNED",
  "OBJECT", "IDS", "INPUT", "OUTPUT", "\"char\"", "\"integer\"",
  "\"double\"", "\"string literal\"", "\"identifier\"", "MINUS", "PLUS",
  "SLASH", "TIMES", "MOD", "XOR", "OR", "AND", "RSHIFT", "LSHIFT", "TILDE",
  "POS", "NEG", "'['", "']'", "$accept", "start", "declarations",
  "declaration", "component_decl", "component_fields", "component_field",
  "port_decl", "task_decl", "$@1", "task_fields", "task_field",
  "task_errors_list", "service_decl", "$@2", "service_fields",
  "service_field", "identifier_list", "identifiers", "inputs", "input",
  "input_type", "codel_prototype", "$@3", "codel_fields", "codel_field",
  "const_decl", "const_type", "literal", "boolean_literal",
  "composed_literal", "literals", "type_decl", "type_spec",
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
        72,     0,    -1,    73,     0,    -1,    74,     9,    -1,    73,
      74,     9,    -1,    78,    -1,    75,    -1,    79,    -1,    84,
      -1,   103,    -1,    97,    -1,     3,    55,     7,    76,     8,
      -1,    77,     9,    -1,    76,    77,     9,    -1,    55,    10,
      54,    -1,    55,    10,    52,    -1,    55,    10,    55,    -1,
      19,    55,    55,    -1,    20,    55,    55,    -1,    -1,     4,
      55,    80,     7,    81,     8,    -1,    82,     9,    -1,    81,
      82,     9,    -1,     6,    55,    10,    93,    -1,    55,    10,
      52,    -1,    55,    10,    83,    -1,    55,    -1,    83,    55,
      -1,    -1,     5,    55,    85,     7,    86,     8,    -1,    87,
       9,    -1,    86,    87,     9,    -1,     6,    55,    10,    93,
      -1,    49,    10,    90,    -1,    50,    10,    92,    -1,    55,
      10,    55,    88,    -1,    55,    10,    54,    -1,    55,    10,
      52,    -1,    -1,    89,    -1,    55,    -1,    89,    55,    -1,
      91,    -1,    90,    13,    91,    -1,    92,    16,    99,    -1,
      92,    -1,    48,    10,    55,    -1,   104,    55,    -1,    -1,
      55,    94,    14,    95,    15,    -1,    -1,    96,    -1,    95,
      13,    96,    -1,    17,    55,    -1,    18,    55,    -1,    19,
      55,    -1,    20,    55,    -1,    44,    98,    55,    16,    99,
      -1,   116,    -1,   125,    -1,   126,    -1,   127,    -1,   114,
      -1,   130,    -1,   131,    -1,   115,    -1,   128,    -1,    52,
      -1,    53,    -1,    54,    -1,    51,    -1,   100,    -1,    55,
      -1,     7,   102,     8,    -1,    14,    99,    15,    -1,   101,
      -1,    21,    -1,    22,    -1,    56,    99,    -1,    57,    99,
      -1,    66,    99,    -1,    99,    57,    99,    -1,    99,    56,
      99,    -1,    99,    59,    99,    -1,    99,    58,    99,    -1,
      99,    60,    99,    -1,    99,    63,    99,    -1,    99,    62,
      99,    -1,    99,    61,    99,    -1,    99,    65,    99,    -1,
      99,    64,    99,    -1,    99,    -1,   102,    13,    99,    -1,
      45,   104,   109,    -1,   132,    -1,   135,    -1,   105,    -1,
     108,    -1,   106,    -1,   107,    -1,    55,    -1,   114,    -1,
     116,    -1,   125,    -1,   126,    -1,   127,    -1,   128,    -1,
     129,    -1,   138,    -1,   130,    -1,   131,    -1,   115,    -1,
     132,    -1,   135,    -1,   110,    -1,   109,    13,   110,    -1,
     111,    -1,   112,    -1,    55,    -1,   111,   113,    -1,   112,
     113,    -1,    69,    52,    70,    -1,    25,    -1,    26,    -1,
      24,    26,    -1,    27,    11,    52,    13,    52,    12,    -1,
      27,    -1,   117,    -1,   121,    -1,   118,    -1,   119,    -1,
     120,    -1,    23,    -1,    24,    -1,    24,    24,    -1,   122,
      -1,   123,    -1,   124,    -1,    46,    23,    -1,    46,    24,
      -1,    46,    24,    24,    -1,    28,    -1,    29,    -1,    32,
      -1,    33,    -1,    34,    -1,    30,    11,    52,    12,    -1,
      30,    -1,    31,    11,    52,    12,    -1,    31,    -1,    42,
      55,     7,   133,     8,    -1,   134,    -1,   133,   134,    -1,
     104,   109,     9,    -1,    37,    55,     7,   136,     8,    -1,
     137,    -1,   136,    13,   137,    -1,    55,    -1,    43,    11,
     105,    13,    52,    12,    -1,    43,    11,   105,    12,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned short int
  Parser::yyprhs_[] =
  {
         0,     0,     3,     6,     9,    13,    15,    17,    19,    21,
      23,    25,    31,    34,    38,    42,    46,    50,    54,    58,
      59,    66,    69,    73,    78,    82,    86,    88,    91,    92,
      99,   102,   106,   111,   115,   119,   124,   128,   132,   133,
     135,   137,   140,   142,   146,   150,   152,   156,   159,   160,
     166,   167,   169,   173,   176,   179,   182,   185,   191,   193,
     195,   197,   199,   201,   203,   205,   207,   209,   211,   213,
     215,   217,   219,   221,   225,   229,   231,   233,   235,   238,
     241,   244,   248,   252,   256,   260,   264,   268,   272,   276,
     280,   284,   286,   290,   294,   296,   298,   300,   302,   304,
     306,   308,   310,   312,   314,   316,   318,   320,   322,   324,
     326,   328,   330,   332,   334,   336,   340,   342,   344,   346,
     349,   352,   356,   358,   360,   363,   370,   372,   374,   376,
     378,   380,   382,   384,   386,   389,   391,   393,   395,   398,
     401,   405,   407,   409,   411,   413,   415,   420,   422,   427,
     429,   435,   437,   440,   444,   450,   452,   456,   458,   465
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned short int
  Parser::yyrline_[] =
  {
         0,   232,   232,   235,   236,   241,   243,   245,   247,   249,
     253,   260,   266,   267,   272,   283,   291,   311,   321,   336,
     335,   346,   347,   351,   355,   372,   381,   385,   394,   393,
     404,   405,   409,   413,   416,   420,   465,   475,   480,   483,
     489,   493,   499,   501,   505,   510,   516,   530,   543,   542,
     554,   555,   557,   561,   572,   583,   587,   595,   602,   603,
     604,   605,   606,   607,   608,   609,   610,   615,   619,   623,
     627,   631,   635,   640,   644,   648,   652,   656,   663,   669,
     673,   680,   687,   694,   701,   708,   715,   722,   729,   736,
     743,   752,   758,   767,   772,   774,   788,   792,   798,   799,
     800,   811,   812,   813,   814,   815,   816,   817,   823,   824,
     825,   826,   830,   832,   836,   842,   854,   855,   859,   866,
     871,   878,   887,   888,   889,   893,   898,   902,   903,   907,
     908,   909,   913,   917,   921,   925,   926,   927,   931,   935,
     939,   943,   947,   951,   955,   959,   967,   972,   978,   983,
     993,  1007,  1009,  1013,  1099,  1113,  1114,  1118,  1132,  1137
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
       2,    69,     2,    70,     2,     2,     2,     2,     2,     2,
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
      65,    66,    67,    68
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int Parser::yyeof_ = 0;
  const int Parser::yylast_ = 315;
  const int Parser::yynnts_ = 68;
  const int Parser::yyempty_ = -2;
  const int Parser::yyfinal_ = 79;
  const int Parser::yyterror_ = 1;
  const int Parser::yyerrcode_ = 256;
  const int Parser::yyntokens_ = 71;

  const unsigned int Parser::yyuser_token_number_max_ = 323;
  const Parser::token_number_type Parser::yyundef_token_ = 2;


/* Line 1054 of lalr1.cc  */
#line 1 "[Bison:b4_percent_define_default]"

} // G3nom

/* Line 1054 of lalr1.cc  */
#line 2547 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.cpp"


/* Line 1056 of lalr1.cc  */
#line 1143 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
 /*** Additional Code ***/

void G3nom::Parser::error(const Parser::location_type& l,
			    const std::string& m)
{
    driver.error(l, m);
}


