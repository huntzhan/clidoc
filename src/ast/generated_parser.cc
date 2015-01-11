// A Bison parser, made by GNU Bison 3.0.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2013 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// Take the name prefix into account.
#define yylex   clidoclex

// First part of user declarations.

#line 39 "generated_parser.cc" // lalr1.cc:399

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "generated_parser.h"

// User implementation prologue.

#line 53 "generated_parser.cc" // lalr1.cc:407
// Unqualified %code blocks.
#line 30 "flex-bison-scripts/parser.y" // lalr1.cc:408


#include <iostream>

#include "ast/parser_proxy.h"
#include "ast/generated_scanner.h"
#undef yylex
#define yylex lexer_ptr->lex

#include "ast/ast_nodes.h"
#include "ast/option_record.h"
#include "ast/token_proxy.h"


// Error report function.
void clidoc::BisonGeneratedParser::error(const std::string &msg) {
  std::cerr << "clidoc::BisonGeneratedParser::error" << std::endl
            << msg << std::endl;
}


#line 77 "generated_parser.cc" // lalr1.cc:408


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif



// Suppress unused-variable warnings by "using" E.
#define YYUSE(E) ((void) (E))

// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << std::endl;                  \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE(Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void>(0)
# define YY_STACK_PRINT()                static_cast<void>(0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyempty = true)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)


namespace clidoc {
#line 144 "generated_parser.cc" // lalr1.cc:474

  /// Build a parser object.
  BisonGeneratedParser::BisonGeneratedParser (clidoc::FlexGeneratedScanner *lexer_ptr_yyarg, clidoc::Doc::SharedPtr *doc_node_ptr_yyarg, clidoc::OptionBindingRecorder *recorder_ptr_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      lexer_ptr (lexer_ptr_yyarg),
      doc_node_ptr (doc_node_ptr_yyarg),
      recorder_ptr (recorder_ptr_yyarg)
  {}

  BisonGeneratedParser::~BisonGeneratedParser ()
  {}


  /*---------------.
  | Symbol types.  |
  `---------------*/



  // by_state.
  inline
  BisonGeneratedParser::by_state::by_state ()
    : state (empty)
  {}

  inline
  BisonGeneratedParser::by_state::by_state (const by_state& other)
    : state (other.state)
  {}

  inline
  void
  BisonGeneratedParser::by_state::move (by_state& that)
  {
    state = that.state;
    that.state = empty;
  }

  inline
  BisonGeneratedParser::by_state::by_state (state_type s)
    : state (s)
  {}

  inline
  BisonGeneratedParser::symbol_number_type
  BisonGeneratedParser::by_state::type_get () const
  {
    return state == empty ? 0 : yystos_[state];
  }

  inline
  BisonGeneratedParser::stack_symbol_type::stack_symbol_type ()
  {}


  inline
  BisonGeneratedParser::stack_symbol_type::stack_symbol_type (state_type s, symbol_type& that)
    : super_type (s)
  {
      switch (that.type_get ())
    {
      case 38: // default_value
        value.move< DefaultValue::WeakPtr > (that.value);
        break;

      case 25: // doc
        value.move< Doc::WeakPtr > (that.value);
        break;

      case 30: // seqs
      case 36: // descriptions
        value.move< LogicAnd::WeakPtr > (that.value);
        break;

      case 27: // utilities
      case 29: // xor_exprs
        value.move< LogicXor::WeakPtr > (that.value);
        break;

      case 40: // single_binding
        value.move< OptionBinding::WeakPtr > (that.value);
        break;

      case 39: // bindings
        value.move< OptionBindingContainer::WeakPtr > (that.value);
        break;

      case 26: // usage_section
      case 28: // single_utility
      case 31: // single_seq
      case 32: // atom
      case 33: // posix_option_unit
      case 34: // gnu_option_unit
      case 35: // options_section
      case 37: // single_description
        value.move< WeakPtrNode > (that.value);
        break;

      case 3: // POSIX_OPTION
      case 4: // GROUPED_OPTIONS
      case 5: // GNU_OPTION
      case 6: // ARGUMENT
      case 7: // DEFAULT_VALUE
      case 8: // COMMAND
      case 9: // GENERAL_ELEMENT
        value.move< std::string > (that.value);
        break;

      default:
        break;
    }

    // that is emptied.
    that.type = empty;
  }

  inline
  BisonGeneratedParser::stack_symbol_type&
  BisonGeneratedParser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
      switch (that.type_get ())
    {
      case 38: // default_value
        value.copy< DefaultValue::WeakPtr > (that.value);
        break;

      case 25: // doc
        value.copy< Doc::WeakPtr > (that.value);
        break;

      case 30: // seqs
      case 36: // descriptions
        value.copy< LogicAnd::WeakPtr > (that.value);
        break;

      case 27: // utilities
      case 29: // xor_exprs
        value.copy< LogicXor::WeakPtr > (that.value);
        break;

      case 40: // single_binding
        value.copy< OptionBinding::WeakPtr > (that.value);
        break;

      case 39: // bindings
        value.copy< OptionBindingContainer::WeakPtr > (that.value);
        break;

      case 26: // usage_section
      case 28: // single_utility
      case 31: // single_seq
      case 32: // atom
      case 33: // posix_option_unit
      case 34: // gnu_option_unit
      case 35: // options_section
      case 37: // single_description
        value.copy< WeakPtrNode > (that.value);
        break;

      case 3: // POSIX_OPTION
      case 4: // GROUPED_OPTIONS
      case 5: // GNU_OPTION
      case 6: // ARGUMENT
      case 7: // DEFAULT_VALUE
      case 8: // COMMAND
      case 9: // GENERAL_ELEMENT
        value.copy< std::string > (that.value);
        break;

      default:
        break;
    }

    return *this;
  }


  template <typename Base>
  inline
  void
  BisonGeneratedParser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  BisonGeneratedParser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " (";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  inline
  void
  BisonGeneratedParser::yypush_ (const char* m, state_type s, symbol_type& sym)
  {
    stack_symbol_type t (s, sym);
    yypush_ (m, t);
  }

  inline
  void
  BisonGeneratedParser::yypush_ (const char* m, stack_symbol_type& s)
  {
    if (m)
      YY_SYMBOL_PRINT (m, s);
    yystack_.push (s);
  }

  inline
  void
  BisonGeneratedParser::yypop_ (unsigned int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  BisonGeneratedParser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  BisonGeneratedParser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  BisonGeneratedParser::debug_level_type
  BisonGeneratedParser::debug_level () const
  {
    return yydebug_;
  }

  void
  BisonGeneratedParser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  inline BisonGeneratedParser::state_type
  BisonGeneratedParser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  inline bool
  BisonGeneratedParser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  BisonGeneratedParser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  BisonGeneratedParser::parse ()
  {
    /// Whether yyla contains a lookahead.
    bool yyempty = true;

    // State.
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The return value of parse ().
    int yyresult;

    // FIXME: This shoud be completely indented.  It is not yet to
    // avoid gratuitous conflicts when merging into the master branch.
    try
      {
    YYCDEBUG << "Starting parse" << std::endl;


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, yyla);

    // A new symbol was pushed on the stack.
  yynewstate:
    YYCDEBUG << "Entering state " << yystack_[0].state << std::endl;

    // Accept?
    if (yystack_[0].state == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    // Backup.
  yybackup:

    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyempty)
      {
        YYCDEBUG << "Reading a token: ";
        try
          {
            symbol_type yylookahead (yylex ());
            yyla.move (yylookahead);
          }
        catch (const syntax_error& yyexc)
          {
            error (yyexc);
            goto yyerrlab1;
          }
        yyempty = false;
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      goto yydefault;

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Discard the token being shifted.
    yyempty = true;

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", yyn, yyla);
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_(yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
        switch (yyr1_[yyn])
    {
      case 38: // default_value
        yylhs.value.build< DefaultValue::WeakPtr > ();
        break;

      case 25: // doc
        yylhs.value.build< Doc::WeakPtr > ();
        break;

      case 30: // seqs
      case 36: // descriptions
        yylhs.value.build< LogicAnd::WeakPtr > ();
        break;

      case 27: // utilities
      case 29: // xor_exprs
        yylhs.value.build< LogicXor::WeakPtr > ();
        break;

      case 40: // single_binding
        yylhs.value.build< OptionBinding::WeakPtr > ();
        break;

      case 39: // bindings
        yylhs.value.build< OptionBindingContainer::WeakPtr > ();
        break;

      case 26: // usage_section
      case 28: // single_utility
      case 31: // single_seq
      case 32: // atom
      case 33: // posix_option_unit
      case 34: // gnu_option_unit
      case 35: // options_section
      case 37: // single_description
        yylhs.value.build< WeakPtrNode > ();
        break;

      case 3: // POSIX_OPTION
      case 4: // GROUPED_OPTIONS
      case 5: // GNU_OPTION
      case 6: // ARGUMENT
      case 7: // DEFAULT_VALUE
      case 8: // COMMAND
      case 9: // GENERAL_ELEMENT
        yylhs.value.build< std::string > ();
        break;

      default:
        break;
    }



      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
      try
        {
          switch (yyn)
            {
  case 2:
#line 121 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  auto doc = Doc::Init();
  doc->AddChild(yystack_[1].value.as< WeakPtrNode > ().lock());
  *doc_node_ptr = doc;
  yylhs.value.as< Doc::WeakPtr > () = doc;
}
#line 612 "generated_parser.cc" // lalr1.cc:847
    break;

  case 3:
#line 132 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  yylhs.value.as< WeakPtrNode > () = yystack_[0].value.as< LogicXor::WeakPtr > ();
}
#line 620 "generated_parser.cc" // lalr1.cc:847
    break;

  case 4:
#line 141 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  yystack_[1].value.as< LogicXor::WeakPtr > ().lock()->AddChild(yystack_[0].value.as< WeakPtrNode > ().lock());
  yylhs.value.as< LogicXor::WeakPtr > () = yystack_[1].value.as< LogicXor::WeakPtr > ();
}
#line 629 "generated_parser.cc" // lalr1.cc:847
    break;

  case 5:
#line 145 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  auto logic_xor = LogicXor::Init();
  logic_xor->AddChild(yystack_[0].value.as< WeakPtrNode > ().lock());
  yylhs.value.as< LogicXor::WeakPtr > () = logic_xor;
}
#line 639 "generated_parser.cc" // lalr1.cc:847
    break;

  case 6:
#line 155 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  yylhs.value.as< WeakPtrNode > () = yystack_[0].value.as< LogicXor::WeakPtr > ();
}
#line 647 "generated_parser.cc" // lalr1.cc:847
    break;

  case 7:
#line 164 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  yystack_[2].value.as< LogicXor::WeakPtr > ().lock()->AddChild(yystack_[0].value.as< LogicAnd::WeakPtr > ().lock());
  yylhs.value.as< LogicXor::WeakPtr > () = yystack_[2].value.as< LogicXor::WeakPtr > ();
}
#line 656 "generated_parser.cc" // lalr1.cc:847
    break;

  case 8:
#line 168 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  auto logic_xor = LogicXor::Init();
  logic_xor->AddChild(yystack_[0].value.as< LogicAnd::WeakPtr > ().lock());
  yylhs.value.as< LogicXor::WeakPtr > () = logic_xor;
}
#line 666 "generated_parser.cc" // lalr1.cc:847
    break;

  case 9:
#line 179 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  yystack_[1].value.as< LogicAnd::WeakPtr > ().lock()->AddChild(yystack_[0].value.as< WeakPtrNode > ().lock());
  yylhs.value.as< LogicAnd::WeakPtr > () = yystack_[1].value.as< LogicAnd::WeakPtr > ();
}
#line 675 "generated_parser.cc" // lalr1.cc:847
    break;

  case 10:
#line 183 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  auto logic_and = LogicAnd::Init();
  logic_and->AddChild(yystack_[0].value.as< WeakPtrNode > ().lock());
  yylhs.value.as< LogicAnd::WeakPtr > () = logic_and;
}
#line 685 "generated_parser.cc" // lalr1.cc:847
    break;

  case 11:
#line 194 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  yylhs.value.as< WeakPtrNode > () = yystack_[0].value.as< WeakPtrNode > ();
}
#line 693 "generated_parser.cc" // lalr1.cc:847
    break;

  case 12:
#line 197 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  auto logic_one_or_more = LogicOneOrMore::Init();
  logic_one_or_more->AddChild(yystack_[1].value.as< WeakPtrNode > ().lock());
  yylhs.value.as< WeakPtrNode > () = logic_one_or_more;
}
#line 703 "generated_parser.cc" // lalr1.cc:847
    break;

  case 13:
#line 214 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  auto logic_and = LogicAnd::Init();
  logic_and->AddChild(yystack_[1].value.as< LogicXor::WeakPtr > ().lock());
  yylhs.value.as< WeakPtrNode > () = logic_and;
}
#line 713 "generated_parser.cc" // lalr1.cc:847
    break;

  case 14:
#line 219 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  auto logic_optional = LogicOptional::Init();
  logic_optional->AddChild(yystack_[1].value.as< LogicXor::WeakPtr > ().lock());
  yylhs.value.as< WeakPtrNode > () = logic_optional;
}
#line 723 "generated_parser.cc" // lalr1.cc:847
    break;

  case 15:
#line 224 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  yylhs.value.as< WeakPtrNode > () = yystack_[0].value.as< WeakPtrNode > ();
}
#line 731 "generated_parser.cc" // lalr1.cc:847
    break;

  case 16:
#line 227 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  yylhs.value.as< WeakPtrNode > () = yystack_[0].value.as< WeakPtrNode > ();
}
#line 739 "generated_parser.cc" // lalr1.cc:847
    break;

  case 17:
#line 230 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  auto argument =
      Argument::Init(InitToken(TypeID::ARGUMENT, yystack_[0].value.as< std::string > ()));
  auto logic_and = LogicAnd::Init();
  logic_and->AddChild(argument);
  yylhs.value.as< WeakPtrNode > () = logic_and;
}
#line 751 "generated_parser.cc" // lalr1.cc:847
    break;

  case 18:
#line 237 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  auto command =
      Command::Init(InitToken(TypeID::COMMAND, yystack_[0].value.as< std::string > ()));
  auto logic_and = LogicAnd::Init();
  logic_and->AddChild(command);
  yylhs.value.as< WeakPtrNode > () = logic_and;
}
#line 763 "generated_parser.cc" // lalr1.cc:847
    break;

  case 19:
#line 244 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  auto k_options =
      KOptions::Init(InitToken(TypeID::K_OPTIONS, ""));
  auto logic_and = LogicAnd::Init();
  logic_and->AddChild(k_options);
  yylhs.value.as< WeakPtrNode > () = logic_and;
}
#line 775 "generated_parser.cc" // lalr1.cc:847
    break;

  case 20:
#line 251 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  auto k_double_hyphen =
      KDoubleHyphen::Init(InitToken(TypeID::K_DOUBLE_HYPHEN, ""));
  auto logic_and = LogicAnd::Init();
  logic_and->AddChild(k_double_hyphen);
  yylhs.value.as< WeakPtrNode > () = logic_and;
}
#line 787 "generated_parser.cc" // lalr1.cc:847
    break;

  case 21:
#line 264 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  auto posix_option =
      PosixOption::Init(InitToken(TypeID::POSIX_OPTION, yystack_[0].value.as< std::string > ()));
  auto logic_and = LogicAnd::Init();
  logic_and->AddChild(posix_option);
  yylhs.value.as< WeakPtrNode > () = logic_and;
}
#line 799 "generated_parser.cc" // lalr1.cc:847
    break;

  case 22:
#line 271 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  auto grouped_option =
      GroupedOptions::Init(InitToken(TypeID::GROUPED_OPTIONS, yystack_[0].value.as< std::string > ()));
  auto logic_and = LogicAnd::Init();
  logic_and->AddChild(grouped_option);
  yylhs.value.as< WeakPtrNode > () = logic_and;
}
#line 811 "generated_parser.cc" // lalr1.cc:847
    break;

  case 23:
#line 284 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  auto gnu_option = GnuOption::Init(InitToken(TypeID::GNU_OPTION, yystack_[0].value.as< std::string > ()));
  auto logic_and = LogicAnd::Init();
  logic_and->AddChild(gnu_option);
  yylhs.value.as< WeakPtrNode > () = logic_and;
}
#line 822 "generated_parser.cc" // lalr1.cc:847
    break;

  case 24:
#line 290 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  // recording binding.
  recorder_ptr->RecordBinding(
      InitToken(TypeID::GNU_OPTION, yystack_[2].value.as< std::string > ()),
      InitToken(TypeID::ARGUMENT, yystack_[0].value.as< std::string > ()));

  // normal works.
  auto gnu_option = GnuOption::Init(InitToken(TypeID::GNU_OPTION, yystack_[2].value.as< std::string > ()));
  auto argument =
      Argument::Init(InitToken(TypeID::ARGUMENT, yystack_[0].value.as< std::string > ()));
  auto logic_and = LogicAnd::Init();
  logic_and->AddChild(gnu_option);
  logic_and->AddChild(argument);
  yylhs.value.as< WeakPtrNode > () = logic_and;
}
#line 842 "generated_parser.cc" // lalr1.cc:847
    break;

  case 25:
#line 308 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 848 "generated_parser.cc" // lalr1.cc:847
    break;

  case 26:
#line 309 "flex-bison-scripts/parser.y" // lalr1.cc:847
    { }
#line 854 "generated_parser.cc" // lalr1.cc:847
    break;

  case 27:
#line 313 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 860 "generated_parser.cc" // lalr1.cc:847
    break;

  case 28:
#line 314 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 866 "generated_parser.cc" // lalr1.cc:847
    break;

  case 29:
#line 320 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  recorder_ptr->RecordBinding(yystack_[2].value.as< OptionBindingContainer::WeakPtr > ().lock(), yystack_[1].value.as< DefaultValue::WeakPtr > ().lock());
}
#line 874 "generated_parser.cc" // lalr1.cc:847
    break;

  case 30:
#line 326 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  
  auto default_value =
      DefaultValue::Init(InitToken(TypeID::DEFAULT_VALUE, yystack_[1].value.as< std::string > ()));
  yylhs.value.as< DefaultValue::WeakPtr > () = default_value;
}
#line 884 "generated_parser.cc" // lalr1.cc:847
    break;

  case 31:
#line 331 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  yylhs.value.as< DefaultValue::WeakPtr > () = DefaultValue::Init();
}
#line 892 "generated_parser.cc" // lalr1.cc:847
    break;

  case 32:
#line 340 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  yystack_[1].value.as< OptionBindingContainer::WeakPtr > ().lock()->AddChild(yystack_[0].value.as< OptionBinding::WeakPtr > ().lock());
  yylhs.value.as< OptionBindingContainer::WeakPtr > () = yystack_[1].value.as< OptionBindingContainer::WeakPtr > ();

}
#line 902 "generated_parser.cc" // lalr1.cc:847
    break;

  case 33:
#line 345 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  auto bindings = OptionBindingContainer::Init();
  bindings->AddChild(yystack_[0].value.as< OptionBinding::WeakPtr > ().lock());
  yylhs.value.as< OptionBindingContainer::WeakPtr > () = bindings;
}
#line 912 "generated_parser.cc" // lalr1.cc:847
    break;

  case 34:
#line 359 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  auto binding =
      OptionBinding::Init(InitToken(TypeID::POSIX_OPTION, yystack_[0].value.as< std::string > ()));
  yylhs.value.as< OptionBinding::WeakPtr > () = binding;
}
#line 922 "generated_parser.cc" // lalr1.cc:847
    break;

  case 35:
#line 364 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  auto binding =
      OptionBinding::Init(InitToken(TypeID::GNU_OPTION, yystack_[0].value.as< std::string > ()));
  yylhs.value.as< OptionBinding::WeakPtr > () = binding;
}
#line 932 "generated_parser.cc" // lalr1.cc:847
    break;

  case 36:
#line 369 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  auto binding =
      OptionBinding::Init(InitToken(TypeID::POSIX_OPTION, yystack_[1].value.as< std::string > ()),
                          InitToken(TypeID::ARGUMENT, yystack_[0].value.as< std::string > ()));
  yylhs.value.as< OptionBinding::WeakPtr > () = binding;
}
#line 943 "generated_parser.cc" // lalr1.cc:847
    break;

  case 37:
#line 375 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  auto binding =
      OptionBinding::Init(InitToken(TypeID::GNU_OPTION, yystack_[1].value.as< std::string > ()),
                          InitToken(TypeID::ARGUMENT, yystack_[0].value.as< std::string > ()));
  yylhs.value.as< OptionBinding::WeakPtr > () = binding;
}
#line 954 "generated_parser.cc" // lalr1.cc:847
    break;

  case 38:
#line 381 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  auto binding =
      OptionBinding::Init(InitToken(TypeID::GNU_OPTION, yystack_[2].value.as< std::string > ()),
                          InitToken(TypeID::ARGUMENT, yystack_[0].value.as< std::string > ()));
  yylhs.value.as< OptionBinding::WeakPtr > () = binding;
}
#line 965 "generated_parser.cc" // lalr1.cc:847
    break;


#line 969 "generated_parser.cc" // lalr1.cc:847
            default:
              break;
            }
        }
      catch (const syntax_error& yyexc)
        {
          error (yyexc);
          YYERROR;
        }
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, yylhs);
    }
    goto yynewstate;

  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yysyntax_error_ (yystack_[0].state,
                                           yyempty ? yyempty_ : yyla.type_get ()));
      }


    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyempty)
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyempty = true;
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
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
    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yyterror_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }


      // Shift the error token.
      error_token.state = yyn;
      yypush_ ("Shifting", error_token);
    }
    goto yynewstate;

    // Accept.
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    // Abort.
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (!yyempty)
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack"
                 << std::endl;
        // Do not try to display the values of the reclaimed symbols,
        // as their printer might throw an exception.
        if (!yyempty)
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
  }

  void
  BisonGeneratedParser::error (const syntax_error& yyexc)
  {
    error (yyexc.what());
  }

  // Generate an error message.
  std::string
  BisonGeneratedParser::yysyntax_error_ (state_type, symbol_number_type) const
  {
    return YY_("syntax error");
  }


  const signed char BisonGeneratedParser::yypact_ninf_ = -21;

  const signed char BisonGeneratedParser::yytable_ninf_ = -1;

  const signed char
  BisonGeneratedParser::yypact_[] =
  {
      -4,     1,    25,     9,    -2,     1,   -21,   -21,    10,   -21,
     -21,   -21,     6,   -21,   -21,    -2,    -2,   -21,   -21,    13,
      -2,   -21,    15,   -21,   -21,   -21,    26,    -1,    10,   -21,
       4,   -21,    27,     0,     5,    -2,   -21,   -21,   -21,   -21,
      28,   -21,    17,    16,   -21,   -21,   -21,   -21,    -2,   -21,
      24,   -21,    23,   -21
  };

  const unsigned char
  BisonGeneratedParser::yydefact_[] =
  {
       0,     0,     0,    26,     0,     3,     5,     1,     0,     2,
      21,    22,    23,    17,    18,     0,     0,    19,    20,     6,
       8,    10,    11,    15,    16,     4,    34,    35,    25,    28,
      31,    33,     0,     0,     0,     0,     9,    12,    36,    37,
       0,    27,     0,     0,    32,    24,    13,    14,     7,    38,
       0,    29,     0,    30
  };

  const signed char
  BisonGeneratedParser::yypgoto_[] =
  {
     -21,   -21,   -21,   -21,    33,     8,     7,   -20,   -21,   -21,
     -21,   -21,   -21,    11,   -21,   -21,    14
  };

  const signed char
  BisonGeneratedParser::yydefgoto_[] =
  {
      -1,     2,     3,     5,     6,    19,    20,    21,    22,    23,
      24,     9,    28,    29,    43,    30,    31
  };

  const unsigned char
  BisonGeneratedParser::yytable_[] =
  {
      36,    10,    11,    12,    13,    39,    14,    26,    15,    27,
      16,    46,     1,    26,    35,    27,    42,    17,    47,    35,
      18,     4,    40,    33,    34,     7,     8,    35,    36,    32,
      37,    52,    38,    45,    49,    50,    53,    51,    25,    41,
       0,     0,    48,     0,    44
  };

  const signed char
  BisonGeneratedParser::yycheck_[] =
  {
      20,     3,     4,     5,     6,     6,     8,     3,    10,     5,
      12,    11,    16,     3,    14,     5,    12,    19,    13,    14,
      22,    20,    23,    15,    16,     0,    17,    14,    48,    23,
      15,     7,     6,     6,     6,    18,    13,    21,     5,    28,
      -1,    -1,    35,    -1,    30
  };

  const unsigned char
  BisonGeneratedParser::yystos_[] =
  {
       0,    16,    25,    26,    20,    27,    28,     0,    17,    35,
       3,     4,     5,     6,     8,    10,    12,    19,    22,    29,
      30,    31,    32,    33,    34,    28,     3,     5,    36,    37,
      39,    40,    23,    29,    29,    14,    31,    15,     6,     6,
      23,    37,    12,    38,    40,     6,    11,    13,    30,     6,
      18,    21,     7,    13
  };

  const unsigned char
  BisonGeneratedParser::yyr1_[] =
  {
       0,    24,    25,    26,    27,    27,    28,    29,    29,    30,
      30,    31,    31,    32,    32,    32,    32,    32,    32,    32,
      32,    33,    33,    34,    34,    35,    35,    36,    36,    37,
      38,    38,    39,    39,    40,    40,    40,    40,    40
  };

  const unsigned char
  BisonGeneratedParser::yyr2_[] =
  {
       0,     2,     2,     2,     2,     1,     2,     3,     1,     2,
       1,     1,     2,     3,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     2,     0,     2,     1,     3,
       4,     0,     2,     1,     1,     1,     2,     2,     3
  };


#if YYDEBUG
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const BisonGeneratedParser::yytname_[] =
  {
  "END", "error", "$undefined", "POSIX_OPTION", "GROUPED_OPTIONS",
  "GNU_OPTION", "ARGUMENT", "DEFAULT_VALUE", "COMMAND", "GENERAL_ELEMENT",
  "K_L_PARENTHESIS", "K_R_PARENTHESIS", "K_L_BRACKET", "K_R_BRACKET",
  "K_EXCLUSIVE_OR", "K_ELLIPSES", "K_USAGE_COLON", "K_OPTIONS_COLON",
  "K_DEFAULT_COLON", "K_OPTIONS", "K_UTILITY_DELIMITER",
  "K_DESC_DELIMITER", "K_DOUBLE_HYPHEN", "K_EQUAL_SIGN", "$accept", "doc",
  "usage_section", "utilities", "single_utility", "xor_exprs", "seqs",
  "single_seq", "atom", "posix_option_unit", "gnu_option_unit",
  "options_section", "descriptions", "single_description", "default_value",
  "bindings", "single_binding", YY_NULLPTR
  };


  const unsigned short int
  BisonGeneratedParser::yyrline_[] =
  {
       0,   121,   121,   132,   141,   145,   155,   164,   168,   179,
     183,   194,   197,   214,   219,   224,   227,   230,   237,   244,
     251,   264,   271,   284,   290,   308,   309,   313,   314,   320,
     326,   331,   340,   345,   359,   364,   369,   375,   381
  };

  // Print the state stack on the debug stream.
  void
  BisonGeneratedParser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  BisonGeneratedParser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):" << std::endl;
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG



} // clidoc
#line 1281 "generated_parser.cc" // lalr1.cc:1155
#line 389 "flex-bison-scripts/parser.y" // lalr1.cc:1156

