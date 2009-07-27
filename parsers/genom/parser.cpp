
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
#line 217 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"


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
#line 238 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 5:

/* Line 678 of lalr1.cc  */
#line 243 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 6:

/* Line 678 of lalr1.cc  */
#line 245 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 7:

/* Line 678 of lalr1.cc  */
#line 247 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 8:

/* Line 678 of lalr1.cc  */
#line 249 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 9:

/* Line 678 of lalr1.cc  */
#line 251 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.component().addType((yysemantic_stack_[(1) - (1)].typeVal));
}
    break;

  case 10:

/* Line 678 of lalr1.cc  */
#line 255 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 11:

/* Line 678 of lalr1.cc  */
#line 257 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 12:

/* Line 678 of lalr1.cc  */
#line 259 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 13:

/* Line 678 of lalr1.cc  */
#line 266 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.component().setName((yysemantic_stack_[(5) - (2)].stringVal));
}
    break;

  case 15:

/* Line 678 of lalr1.cc  */
#line 273 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 16:

/* Line 678 of lalr1.cc  */
#line 278 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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
#line 289 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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
#line 298 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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
#line 331 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Port::Ptr p(new Port((yysemantic_stack_[(3) - (3)].stringVal), (yysemantic_stack_[(3) - (2)].typeVal), true));
    driver.component().addPort(p);
}
    break;

  case 20:

/* Line 678 of lalr1.cc  */
#line 336 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Port::Ptr p(new Port((yysemantic_stack_[(3) - (3)].stringVal), (yysemantic_stack_[(3) - (2)].typeVal), false));
    driver.component().addPort(p);
}
    break;

  case 21:

/* Line 678 of lalr1.cc  */
#line 341 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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
#line 356 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Task::Ptr t(new Task((yysemantic_stack_[(2) - (2)].stringVal)));
    driver.setCurrentTask(t);
}
    break;

  case 23:

/* Line 678 of lalr1.cc  */
#line 361 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.component().addTask(driver.currentTask());
}
    break;

  case 25:

/* Line 678 of lalr1.cc  */
#line 368 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 26:

/* Line 678 of lalr1.cc  */
#line 372 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.currentTask()->addCodel((yysemantic_stack_[(4) - (2)].stringVal), (yysemantic_stack_[(4) - (4)].codelVal));
}
    break;

  case 27:

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

  case 28:

/* Line 678 of lalr1.cc  */
#line 393 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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
#line 410 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Service::Ptr s(new Service((yysemantic_stack_[(2) - (2)].stringVal)));
    driver.setCurrentService(s);
}
    break;

  case 30:

/* Line 678 of lalr1.cc  */
#line 415 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.component().addService(driver.currentService());
}
    break;

  case 32:

/* Line 678 of lalr1.cc  */
#line 422 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 33:

/* Line 678 of lalr1.cc  */
#line 426 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.currentService()->addCodel((yysemantic_stack_[(4) - (2)].stringVal), (yysemantic_stack_[(4) - (4)].codelVal));
}
    break;

  case 34:

/* Line 678 of lalr1.cc  */
#line 430 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.currentService()->addCodel((yysemantic_stack_[(4) - (2)].eventVal)->identifier(), (yysemantic_stack_[(4) - (4)].codelVal));
    driver.currentService()->addEvent((yysemantic_stack_[(4) - (2)].eventVal), (yysemantic_stack_[(4) - (2)].eventVal)->identifier());
}
    break;

  case 35:

/* Line 678 of lalr1.cc  */
#line 435 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
}
    break;

  case 36:

/* Line 678 of lalr1.cc  */
#line 438 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.currentService()->output = (yysemantic_stack_[(3) - (3)].serviceInputVal);
}
    break;

  case 37:

/* Line 678 of lalr1.cc  */
#line 442 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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
#line 485 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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
#line 497 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
   (yyval.stringVal) = (yysemantic_stack_[(1) - (1)].stringVal);
}
    break;

  case 40:

/* Line 678 of lalr1.cc  */
#line 501 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.stringVal) = (yysemantic_stack_[(3) - (1)].stringVal) + " " + (yysemantic_stack_[(3) - (3)].stringVal);
}
    break;

  case 41:

/* Line 678 of lalr1.cc  */
#line 507 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 42:

/* Line 678 of lalr1.cc  */
#line 509 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 43:

/* Line 678 of lalr1.cc  */
#line 513 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yysemantic_stack_[(3) - (1)].serviceInputVal).doc = (yysemantic_stack_[(3) - (3)].stringVal);
    driver.currentService()->addInput((yysemantic_stack_[(3) - (1)].serviceInputVal));
}
    break;

  case 44:

/* Line 678 of lalr1.cc  */
#line 518 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yysemantic_stack_[(5) - (1)].serviceInputVal).defaultValue = (yysemantic_stack_[(5) - (3)].literalVal);
    (yysemantic_stack_[(5) - (1)].serviceInputVal).doc = (yysemantic_stack_[(5) - (5)].stringVal);
    driver.currentService()->addInput((yysemantic_stack_[(5) - (1)].serviceInputVal));
}
    break;

  case 45:

/* Line 678 of lalr1.cc  */
#line 524 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yysemantic_stack_[(5) - (1)].serviceInputVal).defaultValue = (yysemantic_stack_[(5) - (5)].literalVal);
    (yysemantic_stack_[(5) - (1)].serviceInputVal).doc = (yysemantic_stack_[(5) - (3)].stringVal);
    driver.currentService()->addInput((yysemantic_stack_[(5) - (1)].serviceInputVal));
}
    break;

  case 46:

/* Line 678 of lalr1.cc  */
#line 530 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yysemantic_stack_[(3) - (1)].serviceInputVal).defaultValue = (yysemantic_stack_[(3) - (3)].literalVal);
    driver.currentService()->addInput((yysemantic_stack_[(3) - (1)].serviceInputVal));
}
    break;

  case 47:

/* Line 678 of lalr1.cc  */
#line 535 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.currentService()->addInput((yysemantic_stack_[(1) - (1)].serviceInputVal));
}
    break;

  case 48:

/* Line 678 of lalr1.cc  */
#line 541 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 49:

/* Line 678 of lalr1.cc  */
#line 555 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    ServiceInput i;
    i.kind = ServiceInput::Type;
    i.identifier = (yysemantic_stack_[(2) - (2)].stringVal);
    i.type = (yysemantic_stack_[(2) - (1)].typeVal);
    (yyval.serviceInputVal) = i;
}
    break;

  case 50:

/* Line 678 of lalr1.cc  */
#line 566 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 51:

/* Line 678 of lalr1.cc  */
#line 587 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 52:

/* Line 678 of lalr1.cc  */
#line 624 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 54:

/* Line 678 of lalr1.cc  */
#line 646 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 55:

/* Line 678 of lalr1.cc  */
#line 648 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 56:

/* Line 678 of lalr1.cc  */
#line 652 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Event::Ptr ev(new NamedEvent((yysemantic_stack_[(1) - (1)].stringVal)));
    driver.component().addEvent(ev);
}
    break;

  case 57:

/* Line 678 of lalr1.cc  */
#line 657 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Event::Ptr ev(new NamedEvent((yysemantic_stack_[(3) - (1)].stringVal), (yysemantic_stack_[(3) - (3)].eventVal)));
    driver.component().addEvent(ev);
}
    break;

  case 58:

/* Line 678 of lalr1.cc  */
#line 667 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Codel::Ptr c(new Codel((yysemantic_stack_[(1) - (1)].stringVal)));
    driver.setCurrentCodel(c);
}
    break;

  case 59:

/* Line 678 of lalr1.cc  */
#line 672 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.codelVal) = driver.currentCodel();
}
    break;

  case 60:

/* Line 678 of lalr1.cc  */
#line 678 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 61:

/* Line 678 of lalr1.cc  */
#line 680 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.split((yysemantic_stack_[(2) - (2)].stringVal), driver.currentCodel()->nextCodels); 
}
    break;

  case 62:

/* Line 678 of lalr1.cc  */
#line 686 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 63:

/* Line 678 of lalr1.cc  */
#line 688 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 64:

/* Line 678 of lalr1.cc  */
#line 690 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 65:

/* Line 678 of lalr1.cc  */
#line 694 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 66:

/* Line 678 of lalr1.cc  */
#line 705 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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

  case 67:

/* Line 678 of lalr1.cc  */
#line 716 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.currentCodel()->addInPort((yysemantic_stack_[(2) - (2)].stringVal));
}
    break;

  case 68:

/* Line 678 of lalr1.cc  */
#line 720 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.currentCodel()->addOutPort((yysemantic_stack_[(2) - (2)].stringVal));
}
    break;

  case 69:

/* Line 678 of lalr1.cc  */
#line 728 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    ConstValue c((yysemantic_stack_[(5) - (3)].stringVal), (yysemantic_stack_[(5) - (2)].typeVal), (yysemantic_stack_[(5) - (5)].literalVal));
    driver.component().addConstValue(c);
}
    break;

  case 79:

/* Line 678 of lalr1.cc  */
#line 748 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = Literal((yysemantic_stack_[(1) - (1)].integerVal));
}
    break;

  case 80:

/* Line 678 of lalr1.cc  */
#line 752 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = Literal((yysemantic_stack_[(1) - (1)].doubleVal));
}
    break;

  case 81:

/* Line 678 of lalr1.cc  */
#line 756 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = Literal("\"" + (yysemantic_stack_[(1) - (1)].stringVal) + "\"");
}
    break;

  case 82:

/* Line 678 of lalr1.cc  */
#line 760 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = Literal((yysemantic_stack_[(1) - (1)].charVal));
}
    break;

  case 83:

/* Line 678 of lalr1.cc  */
#line 764 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = (yysemantic_stack_[(1) - (1)].literalVal);
}
    break;

  case 84:

/* Line 678 of lalr1.cc  */
#line 768 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    /*enum value*/
    (yyval.literalVal) = Literal((yysemantic_stack_[(1) - (1)].stringVal));
}
    break;

  case 85:

/* Line 678 of lalr1.cc  */
#line 773 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = (yysemantic_stack_[(3) - (2)].literalVal);
}
    break;

  case 86:

/* Line 678 of lalr1.cc  */
#line 777 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = (yysemantic_stack_[(3) - (2)].literalVal);
}
    break;

  case 87:

/* Line 678 of lalr1.cc  */
#line 781 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 88:

/* Line 678 of lalr1.cc  */
#line 785 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = Literal(true);
}
    break;

  case 89:

/* Line 678 of lalr1.cc  */
#line 789 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = Literal(false);
}
    break;

  case 90:

/* Line 678 of lalr1.cc  */
#line 796 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Neg);
    l.addMember((yysemantic_stack_[(2) - (2)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 91:

/* Line 678 of lalr1.cc  */
#line 802 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.literalVal) = (yysemantic_stack_[(2) - (2)].literalVal);
}
    break;

  case 92:

/* Line 678 of lalr1.cc  */
#line 806 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::LNeg);
    l.addMember((yysemantic_stack_[(2) - (2)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 93:

/* Line 678 of lalr1.cc  */
#line 813 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Plus);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 94:

/* Line 678 of lalr1.cc  */
#line 820 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Minus);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 95:

/* Line 678 of lalr1.cc  */
#line 827 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Times);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 96:

/* Line 678 of lalr1.cc  */
#line 834 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Divide);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 97:

/* Line 678 of lalr1.cc  */
#line 841 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Mod);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 98:

/* Line 678 of lalr1.cc  */
#line 848 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::And);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 99:

/* Line 678 of lalr1.cc  */
#line 855 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Or);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 100:

/* Line 678 of lalr1.cc  */
#line 862 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Xor);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 101:

/* Line 678 of lalr1.cc  */
#line 869 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::LShift);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 102:

/* Line 678 of lalr1.cc  */
#line 876 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::RShift);
    l.addMember((yysemantic_stack_[(3) - (1)].literalVal));
    l.addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 103:

/* Line 678 of lalr1.cc  */
#line 885 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Literal l(Literal::Struct);
    l.addMember((yysemantic_stack_[(1) - (1)].literalVal));
    (yyval.literalVal) = l;
}
    break;

  case 104:

/* Line 678 of lalr1.cc  */
#line 891 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yysemantic_stack_[(3) - (1)].literalVal).addMember((yysemantic_stack_[(3) - (3)].literalVal));
    (yyval.literalVal) = (yysemantic_stack_[(3) - (1)].literalVal);
}
    break;

  case 105:

/* Line 678 of lalr1.cc  */
#line 900 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    driver.component().addNativeTypeInclude((yysemantic_stack_[(6) - (3)].stringVal));
}
    break;

  case 106:

/* Line 678 of lalr1.cc  */
#line 906 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yysemantic_stack_[(2) - (1)].typeVal)->setNative(true);
    driver.component().addType((yysemantic_stack_[(2) - (1)].typeVal));
}
    break;

  case 107:

/* Line 678 of lalr1.cc  */
#line 911 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yysemantic_stack_[(3) - (2)].typeVal)->setNative(true);
    driver.component().addType((yysemantic_stack_[(3) - (2)].typeVal));
}
    break;

  case 108:

/* Line 678 of lalr1.cc  */
#line 918 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    IdlType::Ptr p(new TypedefType((yysemantic_stack_[(3) - (2)].typeVal), (yysemantic_stack_[(3) - (3)].declaratorVectVal)));
    (yyval.typeVal) = p;
}
    break;

  case 109:

/* Line 678 of lalr1.cc  */
#line 922 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 110:

/* Line 678 of lalr1.cc  */
#line 924 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 111:

/* Line 678 of lalr1.cc  */
#line 937 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal);
}
    break;

  case 112:

/* Line 678 of lalr1.cc  */
#line 941 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { 
    (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal);
}
    break;

  case 113:

/* Line 678 of lalr1.cc  */
#line 946 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 114:

/* Line 678 of lalr1.cc  */
#line 947 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 115:

/* Line 678 of lalr1.cc  */
#line 949 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    IdlType::Ptr type = driver.component().typeFromName((yysemantic_stack_[(1) - (1)].stringVal));
    if(!type) {
	error(yyloc, std::string("Unknown type: ") + (yysemantic_stack_[(1) - (1)].stringVal));
	YYERROR;
    }
    (yyval.typeVal) = type;
}
    break;

  case 116:

/* Line 678 of lalr1.cc  */
#line 959 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 117:

/* Line 678 of lalr1.cc  */
#line 960 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 118:

/* Line 678 of lalr1.cc  */
#line 961 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 119:

/* Line 678 of lalr1.cc  */
#line 962 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 120:

/* Line 678 of lalr1.cc  */
#line 963 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 121:

/* Line 678 of lalr1.cc  */
#line 964 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 122:

/* Line 678 of lalr1.cc  */
#line 965 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 123:

/* Line 678 of lalr1.cc  */
#line 971 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 124:

/* Line 678 of lalr1.cc  */
#line 972 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 125:

/* Line 678 of lalr1.cc  */
#line 973 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 126:

/* Line 678 of lalr1.cc  */
#line 974 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 127:

/* Line 678 of lalr1.cc  */
#line 978 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 128:

/* Line 678 of lalr1.cc  */
#line 980 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 129:

/* Line 678 of lalr1.cc  */
#line 985 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { 
    Declarator::VectPtr v(new Declarator::Vect());
    v->push_back((yysemantic_stack_[(1) - (1)].declaratorVal));
    (yyval.declaratorVectVal) = v;
}
    break;

  case 130:

/* Line 678 of lalr1.cc  */
#line 991 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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
#line 1002 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.declaratorVal) = (yysemantic_stack_[(1) - (1)].declaratorVal); }
    break;

  case 132:

/* Line 678 of lalr1.cc  */
#line 1003 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.declaratorVal) = (yysemantic_stack_[(1) - (1)].declaratorVal); }
    break;

  case 133:

/* Line 678 of lalr1.cc  */
#line 1008 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    Declarator::Ptr p(new Declarator((yysemantic_stack_[(1) - (1)].stringVal)));
    (yyval.declaratorVal) = p;
}
    break;

  case 134:

/* Line 678 of lalr1.cc  */
#line 1015 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yysemantic_stack_[(2) - (1)].declaratorVal)->addBound((yysemantic_stack_[(2) - (2)].integerVal));
    (yyval.declaratorVal) = (yysemantic_stack_[(2) - (1)].declaratorVal);
}
    break;

  case 135:

/* Line 678 of lalr1.cc  */
#line 1020 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yysemantic_stack_[(2) - (1)].declaratorVal)->addBound((yysemantic_stack_[(2) - (2)].integerVal));
    (yyval.declaratorVal) = (yysemantic_stack_[(2) - (1)].declaratorVal);
}
    break;

  case 136:

/* Line 678 of lalr1.cc  */
#line 1027 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.integerVal) = (yysemantic_stack_[(3) - (2)].integerVal);
}
    break;

  case 137:

/* Line 678 of lalr1.cc  */
#line 1035 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::floatType; }
    break;

  case 138:

/* Line 678 of lalr1.cc  */
#line 1036 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::doubleType; }
    break;

  case 139:

/* Line 678 of lalr1.cc  */
#line 1037 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::longdoubleType; }
    break;

  case 140:

/* Line 678 of lalr1.cc  */
#line 1042 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    IdlType::Ptr p(new FixedType((yysemantic_stack_[(6) - (3)].integerVal), (yysemantic_stack_[(6) - (5)].integerVal)));
    (yyval.typeVal) = p;
}
    break;

  case 141:

/* Line 678 of lalr1.cc  */
#line 1047 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 142:

/* Line 678 of lalr1.cc  */
#line 1050 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 143:

/* Line 678 of lalr1.cc  */
#line 1051 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 144:

/* Line 678 of lalr1.cc  */
#line 1055 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 145:

/* Line 678 of lalr1.cc  */
#line 1056 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 146:

/* Line 678 of lalr1.cc  */
#line 1057 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 147:

/* Line 678 of lalr1.cc  */
#line 1061 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::shortType; }
    break;

  case 148:

/* Line 678 of lalr1.cc  */
#line 1065 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::longType; }
    break;

  case 149:

/* Line 678 of lalr1.cc  */
#line 1069 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::longlongType; }
    break;

  case 150:

/* Line 678 of lalr1.cc  */
#line 1073 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 151:

/* Line 678 of lalr1.cc  */
#line 1074 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 152:

/* Line 678 of lalr1.cc  */
#line 1075 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = (yysemantic_stack_[(1) - (1)].typeVal); }
    break;

  case 153:

/* Line 678 of lalr1.cc  */
#line 1079 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::ushortType; }
    break;

  case 154:

/* Line 678 of lalr1.cc  */
#line 1083 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::ulongType; }
    break;

  case 155:

/* Line 678 of lalr1.cc  */
#line 1087 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::ulonglongType; }
    break;

  case 156:

/* Line 678 of lalr1.cc  */
#line 1091 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::charType; }
    break;

  case 157:

/* Line 678 of lalr1.cc  */
#line 1095 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::wcharType; }
    break;

  case 158:

/* Line 678 of lalr1.cc  */
#line 1099 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::booleanType; }
    break;

  case 159:

/* Line 678 of lalr1.cc  */
#line 1103 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::octetType; }
    break;

  case 160:

/* Line 678 of lalr1.cc  */
#line 1107 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { (yyval.typeVal) = BaseType::anyType; }
    break;

  case 161:

/* Line 678 of lalr1.cc  */
#line 1116 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    IdlType::Ptr p(new StringType((yysemantic_stack_[(4) - (3)].integerVal)));
    (yyval.typeVal) = p; 
}
    break;

  case 162:

/* Line 678 of lalr1.cc  */
#line 1121 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.typeVal) = StringType::unboundedStringType;
}
    break;

  case 163:

/* Line 678 of lalr1.cc  */
#line 1127 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    { 
    IdlType::Ptr p(new WStringType((yysemantic_stack_[(4) - (3)].integerVal)));
    (yyval.typeVal) = p; 
}
    break;

  case 164:

/* Line 678 of lalr1.cc  */
#line 1132 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    (yyval.typeVal) = WStringType::unboundedWStringType;
}
    break;

  case 165:

/* Line 678 of lalr1.cc  */
#line 1142 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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
#line 1156 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 167:

/* Line 678 of lalr1.cc  */
#line 1158 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 168:

/* Line 678 of lalr1.cc  */
#line 1162 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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
#line 1248 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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
#line 1261 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 171:

/* Line 678 of lalr1.cc  */
#line 1263 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {}
    break;

  case 172:

/* Line 678 of lalr1.cc  */
#line 1267 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
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
#line 1281 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    IdlType::Ptr p(new SequenceType((yysemantic_stack_[(6) - (3)].typeVal), (yysemantic_stack_[(6) - (5)].integerVal)));
    (yyval.typeVal) = p;
}
    break;

  case 174:

/* Line 678 of lalr1.cc  */
#line 1286 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
    {
    IdlType::Ptr p(new SequenceType((yysemantic_stack_[(4) - (3)].typeVal), 0));  
    (yyval.typeVal) = p;
}
    break;



/* Line 678 of lalr1.cc  */
#line 2048 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.cpp"
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
  const short int Parser::yypact_ninf_ = -212;
  const short int
  Parser::yypact_[] =
  {
       173,   -54,   -39,   -20,   239,   239,     8,    27,   174,   239,
      44,    53,   112,   106,   116,  -212,  -212,  -212,  -212,  -212,
    -212,  -212,  -212,  -212,  -212,   148,  -212,  -212,  -212,    79,
    -212,  -212,   119,  -212,  -212,   140,   168,  -212,  -212,  -212,
     199,   132,  -212,    83,  -212,  -212,  -212,  -212,  -212,  -212,
    -212,  -212,  -212,  -212,  -212,  -212,  -212,  -212,  -212,  -212,
    -212,  -212,  -212,  -212,  -212,  -212,  -212,  -212,  -212,   125,
     204,   208,   156,  -212,  -212,  -212,  -212,  -212,  -212,  -212,
    -212,  -212,   158,   205,   210,  -212,   165,  -212,  -212,   216,
    -212,   180,   234,   236,  -212,  -212,   187,   188,   189,   275,
    -212,   223,  -212,   243,   192,   239,   246,  -212,   263,  -212,
     206,   206,   217,    44,   272,  -212,   271,     7,   273,    23,
      29,   276,   274,   278,   161,  -212,   279,  -212,     3,  -212,
     158,   136,  -212,    13,   158,   231,  -212,  -212,   277,  -212,
    -212,   -16,    31,  -212,   282,  -212,   232,   283,    16,   286,
     237,   288,   302,   217,   303,    26,   287,   257,  -212,  -212,
    -212,   258,   256,  -212,   192,    34,  -212,  -212,    13,    13,
    -212,  -212,  -212,  -212,  -212,  -212,  -212,    13,    13,    13,
      54,  -212,  -212,  -212,   242,   259,     2,   309,  -212,  -212,
    -212,   308,  -212,   312,   -12,  -212,   315,  -212,   316,   203,
     203,   317,   138,  -212,   319,  -212,   313,   318,   321,  -212,
    -212,    54,    17,    70,  -212,  -212,  -212,    13,    13,    13,
      13,    13,    13,    13,    13,    13,    13,  -212,   320,  -212,
     323,  -212,   269,   280,  -212,   308,  -212,   280,   322,   325,
    -212,    28,   281,  -212,   280,  -212,   308,  -212,  -212,  -212,
     280,  -212,    13,  -212,   191,   191,   -14,   -14,   123,   113,
     113,   113,   144,   144,   284,  -212,  -212,  -212,  -212,  -212,
     285,   203,   289,    13,  -212,  -212,   324,    54,  -212,   328,
    -212,  -212,   327,    32,   326,   127,    13,   290,  -212,   291,
     292,   293,   294,   141,  -212,    54,  -212,  -212,  -212,  -212,
    -212,   127,   329,  -212,   295,  -212,   308
  };

  /* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
     doesn't specify something else to do.  Zero means the default is an
     error.  */
  const unsigned char
  Parser::yydefact_[] =
  {
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     6,     5,     7,     8,    12,
      11,    10,     9,   109,   110,     0,    22,    29,   147,   148,
     137,   138,   141,   156,   157,   162,   164,   158,   159,   160,
       0,     0,   115,     0,   111,   113,   114,   112,   116,   126,
     117,   142,   144,   145,   146,   143,   150,   151,   152,   118,
     119,   120,   121,   122,   124,   125,   127,   128,   123,     0,
       0,     0,     0,    74,    77,    70,    71,    72,    73,    78,
      75,    76,     0,    56,    53,    54,     0,     1,     2,     0,
       3,     0,     0,     0,   149,   139,     0,     0,     0,     0,
     153,   154,    19,    20,     0,     0,     0,   133,   108,   129,
     131,   132,     0,     0,     0,     4,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   155,     0,   172,     0,   170,
       0,     0,   166,     0,     0,     0,   134,   135,    50,    57,
      55,     0,     0,    13,     0,    14,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   161,   163,
     174,     0,     0,   169,     0,     0,   165,   167,     0,     0,
      88,    89,    82,    79,    80,    81,    84,     0,     0,     0,
      69,    83,    87,   130,     0,     0,     0,     0,    17,    16,
      39,    18,    15,     0,     0,    23,     0,    24,     0,     0,
       0,     0,     0,    30,     0,    31,     0,     0,     0,   171,
     168,   103,     0,     0,    90,    91,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   136,    51,   105,
       0,   106,     0,     0,    27,    28,    25,     0,     0,    35,
      41,    47,     0,    36,     0,    38,    37,    32,   140,   173,
       0,    85,     0,    86,    94,    93,    96,    95,    97,   100,
      99,    98,   102,   101,     0,   107,    40,    58,    26,    33,
       0,     0,     0,     0,    49,    34,     0,   104,    52,     0,
      48,    42,    43,    46,     0,    62,     0,     0,    21,     0,
       0,     0,     0,     0,    63,    45,    44,    65,    66,    67,
      68,     0,    60,    64,     0,    59,    61
  };

  /* YYPGOTO[NTERM-NUM].  */
  const short int
  Parser::yypgoto_[] =
  {
      -212,  -212,  -212,   333,  -212,  -212,   219,  -212,  -212,  -212,
    -212,   202,  -212,  -212,  -212,   201,  -190,  -212,    86,   159,
     207,  -212,  -212,   245,  -211,  -212,  -212,  -212,    60,  -212,
    -212,  -160,  -212,  -212,  -212,  -212,  -212,  -136,    -2,   264,
    -212,  -212,  -212,   235,   228,  -212,  -212,   253,   358,   359,
     360,  -212,  -212,  -212,  -212,  -212,  -212,  -212,  -212,   361,
     362,   363,   364,  -212,   365,   366,     0,  -212,   244,     1,
    -212,   212,  -212
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const short int
  Parser::yydefgoto_[] =
  {
        -1,    12,    13,    14,    15,   117,   118,    16,    17,    92,
     148,   149,    18,    93,   155,   156,   191,   239,   240,   241,
     139,    19,    84,    85,   268,   279,   305,   293,   294,    20,
      72,   180,   181,   182,   212,    21,   186,    22,   242,    44,
      45,    46,    47,   108,   109,   110,   111,   136,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,   131,   132,    67,
     128,   129,    68
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If zero, do what YYDEFACT says.  */
  const signed char Parser::yytable_ninf_ = -1;
  const short int
  Parser::yytable_[] =
  {
        23,    24,    43,    69,   235,   187,    25,    82,   211,   213,
     229,   163,   246,    23,    24,   143,   164,   214,   215,   216,
     168,    26,   146,     6,   195,   251,   269,   169,     7,   146,
     252,     9,   150,   275,   203,   150,   170,   171,   272,   276,
      27,     6,   287,   210,   273,   234,     7,   134,   190,     9,
     230,   221,   222,   223,   224,   225,   226,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   116,    70,   172,
     173,   174,   175,   176,   177,   178,   147,   151,   152,   153,
     151,   152,   153,   147,   179,   253,   154,    71,   188,   154,
     189,   190,   277,   217,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   130,    83,    94,    88,    95,    86,     1,
       2,     3,    87,   283,   306,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,    90,   295,     4,     5,   130,
      96,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,    23,    24,   102,   166,     6,   289,   290,   291,   292,
       7,    97,     8,     9,   301,    91,   302,   100,   101,    10,
      11,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,   160,   161,     6,     1,     2,     3,    98,
       7,    40,   225,   226,    41,   103,    23,    24,    -1,   222,
     223,   224,   225,   226,     4,     5,    42,   245,   190,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      99,   104,     6,    -1,    -1,   105,   106,     7,   107,     8,
       9,   112,    41,   113,   114,   115,    10,    11,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
     116,   119,     6,   120,   121,   122,   123,     7,    40,   125,
     126,    41,   127,   238,   219,   220,   221,   222,   223,   224,
     225,   226,   133,    42,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,   134,   138,     6,   141,
     135,   142,   145,     7,    40,   162,   158,    41,   184,   157,
     159,   192,   193,   194,   185,   197,   205,   198,   199,    42,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,   200,   202,   206,   207,   208,   227,   231,   228,
      40,   232,   233,    41,   236,   248,   237,   244,   247,   266,
     249,   250,   265,   284,   288,    42,   144,   264,   271,   270,
     267,   274,   285,   286,   278,   280,    89,   304,   282,   296,
     196,   297,   298,   299,   300,   190,   204,   281,   140,   243,
     201,   303,   183,   124,   137,   165,    73,    74,    75,    76,
      77,    78,    79,    80,    81,   167,   209
  };

  /* YYCHECK.  */
  const unsigned short int
  Parser::yycheck_[] =
  {
         0,     0,     4,     5,   194,   141,    60,     9,   168,   169,
       8,     8,   202,    13,    13,     8,    13,   177,   178,   179,
       7,    60,     6,    39,     8,     8,   237,    14,    44,     6,
      13,    47,     6,   244,     8,     6,    23,    24,    10,   250,
      60,    39,    10,     9,    16,    57,    44,    13,    60,    47,
     186,    65,    66,    67,    68,    69,    70,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,    60,    60,    56,
      57,    58,    59,    60,    61,    62,    60,    51,    52,    53,
      51,    52,    53,    60,    71,    15,    60,    60,    57,    60,
      59,    60,   252,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,   105,    60,    26,     0,    28,    55,     3,
       4,     5,     0,   273,   304,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,     9,   286,    21,    22,   131,
      11,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,   141,   141,    60,     8,    39,    19,    20,    21,    22,
      44,    11,    46,    47,    13,     7,    15,    25,    26,    53,
      54,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    12,    13,    39,     3,     4,     5,    11,
      44,    45,    69,    70,    48,    60,   186,   186,    65,    66,
      67,    68,    69,    70,    21,    22,    60,    59,    60,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      11,     7,    39,    69,    70,     7,    60,    44,    60,    46,
      47,    16,    48,    13,    59,     9,    53,    54,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      60,     7,    39,     7,    57,    57,    57,    44,    45,    26,
       7,    48,    60,    50,    63,    64,    65,    66,    67,    68,
      69,    70,    16,    60,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    13,    60,    39,     7,
      74,    10,     9,    44,    45,     6,    12,    48,    57,    13,
      12,     9,    60,    10,    17,     9,     9,    60,    10,    60,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    10,    10,    57,    57,    60,    75,     9,    60,
      45,    13,    10,    48,     9,    12,    10,    10,     9,    60,
      12,    10,     9,     9,     8,    60,   117,    17,    13,    17,
      60,    60,    14,    16,    60,    60,    13,    18,    59,    59,
     148,    60,    60,    60,    60,    60,   155,   271,   113,   200,
     153,   301,   134,    99,   111,   130,     8,     8,     8,     8,
       8,     8,     8,     8,     8,   131,   164
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  Parser::yystos_[] =
  {
         0,     3,     4,     5,    21,    22,    39,    44,    46,    47,
      53,    54,    77,    78,    79,    80,    83,    84,    88,    97,
     105,   111,   113,   142,   145,    60,    60,    60,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      45,    48,    60,   114,   115,   116,   117,   118,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   145,   148,   114,
      60,    60,   106,   124,   125,   126,   135,   136,   137,   138,
     140,   141,   114,    60,    98,    99,    55,     0,     0,    79,
       9,     7,    85,    89,    26,    28,    11,    11,    11,    11,
      25,    26,    60,    60,     7,     7,    60,    60,   119,   120,
     121,   122,    16,    13,    59,     9,    60,    81,    82,     7,
       7,    57,    57,    57,   115,    26,     7,    60,   146,   147,
     114,   143,   144,    16,    13,    74,   123,   123,    60,    96,
      99,     7,    10,     8,    82,     9,     6,    60,    86,    87,
       6,    51,    52,    53,    60,    90,    91,    13,    12,    12,
      12,    13,     6,     8,    13,   119,     8,   144,     7,    14,
      23,    24,    56,    57,    58,    59,    60,    61,    62,    71,
     107,   108,   109,   120,    57,    17,   112,   113,    57,    59,
      60,    92,     9,    60,    10,     8,    87,     9,    60,    10,
      10,    96,    10,     8,    91,     9,    57,    57,    60,   147,
       9,   107,   110,   107,   107,   107,   107,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    75,    60,     8,
     113,     9,    13,    10,    57,    92,     9,    10,    50,    93,
      94,    95,   114,    95,    10,    59,    92,     9,    12,    12,
      10,     8,    13,    15,   107,   107,   107,   107,   107,   107,
     107,   107,   107,   107,    17,     9,    60,    60,   100,   100,
      17,    13,    10,    16,    60,   100,   100,   107,    60,   101,
      60,    94,    59,   107,     9,    14,    16,    10,     8,    19,
      20,    21,    22,   103,   104,   107,    59,    60,    60,    60,
      60,    13,    15,   104,    18,   102,    92
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
      96,    96,    96,    97,    98,    98,    99,    99,   101,   100,
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
       3,    10,     0,     6,     2,     3,     4,     3,     3,     0,
       6,     2,     3,     4,     4,     3,     3,     3,     3,     1,
       3,     1,     3,     3,     5,     5,     3,     1,     3,     2,
       1,     3,     5,     2,     1,     3,     1,     3,     0,     6,
       0,     2,     0,     1,     3,     2,     2,     2,     2,     5,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     3,     1,     1,     1,
       2,     2,     2,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     1,     3,     6,     2,     3,     3,     1,
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
  "task_fields", "task_field", "service_decl", "$@2", "service_fields",
  "service_field", "identifiers", "inputs", "input", "input_type",
  "event_spec", "event_decl", "event_declarators", "event_declarator",
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
      79,     9,    -1,    83,    -1,    80,    -1,    84,    -1,    88,
      -1,   113,    -1,   111,    -1,   105,    -1,    97,    -1,     3,
      60,     7,    81,     8,    -1,    82,     9,    -1,    81,    82,
       9,    -1,    60,    10,    59,    -1,    60,    10,    57,    -1,
      60,    10,    92,    -1,    21,   114,    60,    -1,    22,   114,
      60,    -1,    22,   114,    60,     7,     6,    60,    10,   100,
       9,     8,    -1,    -1,     4,    60,    85,     7,    86,     8,
      -1,    87,     9,    -1,    86,    87,     9,    -1,     6,    60,
      10,   100,    -1,    60,    10,    57,    -1,    60,    10,    92,
      -1,    -1,     5,    60,    89,     7,    90,     8,    -1,    91,
       9,    -1,    90,    91,     9,    -1,     6,    60,    10,   100,
      -1,    53,    96,    10,   100,    -1,    51,    10,    93,    -1,
      52,    10,    95,    -1,    60,    10,    92,    -1,    60,    10,
      59,    -1,    60,    -1,    92,    13,    60,    -1,    94,    -1,
      93,    13,    94,    -1,    95,    10,    59,    -1,    95,    16,
     107,    10,    59,    -1,    95,    10,    59,    16,   107,    -1,
      95,    16,   107,    -1,    95,    -1,    50,    17,    60,    -1,
     114,    60,    -1,    60,    -1,    60,    17,    60,    -1,    60,
      17,    60,    17,    60,    -1,    53,    98,    -1,    99,    -1,
      98,    13,    99,    -1,    60,    -1,    60,    16,    96,    -1,
      -1,    60,   101,    14,   103,    15,   102,    -1,    -1,    18,
      92,    -1,    -1,   104,    -1,   103,    13,   104,    -1,    19,
      60,    -1,    20,    60,    -1,    21,    60,    -1,    22,    60,
      -1,    46,   106,    60,    16,   107,    -1,   126,    -1,   135,
      -1,   136,    -1,   137,    -1,   124,    -1,   140,    -1,   141,
      -1,   125,    -1,   138,    -1,    57,    -1,    58,    -1,    59,
      -1,    56,    -1,   108,    -1,    60,    -1,     7,   110,     8,
      -1,    14,   107,    15,    -1,   109,    -1,    23,    -1,    24,
      -1,    61,   107,    -1,    62,   107,    -1,    71,   107,    -1,
     107,    62,   107,    -1,   107,    61,   107,    -1,   107,    64,
     107,    -1,   107,    63,   107,    -1,   107,    65,   107,    -1,
     107,    68,   107,    -1,   107,    67,   107,    -1,   107,    66,
     107,    -1,   107,    70,   107,    -1,   107,    69,   107,    -1,
     107,    -1,   110,    13,   107,    -1,    54,    55,    59,     7,
     112,     8,    -1,   113,     9,    -1,   112,   113,     9,    -1,
      47,   114,   119,    -1,   142,    -1,   145,    -1,   115,    -1,
     118,    -1,   116,    -1,   117,    -1,    60,    -1,   124,    -1,
     126,    -1,   135,    -1,   136,    -1,   137,    -1,   138,    -1,
     139,    -1,   148,    -1,   140,    -1,   141,    -1,   125,    -1,
     142,    -1,   145,    -1,   120,    -1,   119,    13,   120,    -1,
     121,    -1,   122,    -1,    60,    -1,   121,   123,    -1,   122,
     123,    -1,    74,    57,    75,    -1,    27,    -1,    28,    -1,
      26,    28,    -1,    29,    11,    57,    13,    57,    12,    -1,
      29,    -1,   127,    -1,   131,    -1,   128,    -1,   129,    -1,
     130,    -1,    25,    -1,    26,    -1,    26,    26,    -1,   132,
      -1,   133,    -1,   134,    -1,    48,    25,    -1,    48,    26,
      -1,    48,    26,    26,    -1,    30,    -1,    31,    -1,    34,
      -1,    35,    -1,    36,    -1,    32,    11,    57,    12,    -1,
      32,    -1,    33,    11,    57,    12,    -1,    33,    -1,    44,
      60,     7,   143,     8,    -1,   144,    -1,   143,   144,    -1,
     114,   119,     9,    -1,    39,    60,     7,   146,     8,    -1,
     147,    -1,   146,    13,   147,    -1,    60,    -1,    45,    11,
     115,    13,    57,    12,    -1,    45,    11,   115,    12,    -1
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
     144,   148,   150,   154,   158,   164,   170,   174,   176,   180,
     183,   185,   189,   195,   198,   200,   204,   206,   210,   211,
     218,   219,   222,   223,   225,   229,   232,   235,   238,   241,
     247,   249,   251,   253,   255,   257,   259,   261,   263,   265,
     267,   269,   271,   273,   275,   277,   281,   285,   287,   289,
     291,   294,   297,   300,   304,   308,   312,   316,   320,   324,
     328,   332,   336,   340,   342,   346,   353,   356,   360,   364,
     366,   368,   370,   372,   374,   376,   378,   380,   382,   384,
     386,   388,   390,   392,   394,   396,   398,   400,   402,   404,
     406,   410,   412,   414,   416,   419,   422,   426,   428,   430,
     433,   440,   442,   444,   446,   448,   450,   452,   454,   456,
     459,   461,   463,   465,   468,   471,   475,   477,   479,   481,
     483,   485,   490,   492,   497,   499,   505,   507,   510,   514,
     520,   522,   526,   528,   535
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned short int
  Parser::yyrline_[] =
  {
         0,   233,   233,   236,   237,   242,   244,   246,   248,   250,
     254,   256,   258,   265,   271,   272,   277,   288,   297,   330,
     335,   340,   356,   355,   366,   367,   371,   375,   392,   410,
     409,   420,   421,   425,   429,   434,   437,   441,   484,   496,
     500,   506,   508,   512,   517,   523,   529,   534,   540,   554,
     565,   586,   623,   642,   645,   647,   651,   656,   667,   666,
     678,   679,   686,   687,   689,   693,   704,   715,   719,   727,
     734,   735,   736,   737,   738,   739,   740,   741,   742,   747,
     751,   755,   759,   763,   767,   772,   776,   780,   784,   788,
     795,   801,   805,   812,   819,   826,   833,   840,   847,   854,
     861,   868,   875,   884,   890,   899,   905,   910,   917,   922,
     924,   936,   940,   946,   947,   948,   959,   960,   961,   962,
     963,   964,   965,   971,   972,   973,   974,   978,   980,   984,
     990,  1002,  1003,  1007,  1014,  1019,  1026,  1035,  1036,  1037,
    1041,  1046,  1050,  1051,  1055,  1056,  1057,  1061,  1065,  1069,
    1073,  1074,  1075,  1079,  1083,  1087,  1091,  1095,  1099,  1103,
    1107,  1115,  1120,  1126,  1131,  1141,  1155,  1157,  1161,  1247,
    1261,  1262,  1266,  1280,  1285
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
  const int Parser::yylast_ = 376;
  const int Parser::yynnts_ = 73;
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
#line 2795 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.cpp"


/* Line 1056 of lalr1.cc  */
#line 1291 "/home/ccpasteur/work/git/g3nom/parsers/genom/parser.yy"
 /*** Additional Code ***/

void G3nom::Parser::error(const Parser::location_type& l,
			    const std::string& m)
{
    driver.error(l, m);
}


