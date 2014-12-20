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
#line 28 "flex-bison-scripts/parser.y" // lalr1.cc:408

#include "generated_scanner.h"
#undef yylex
#define yylex lexer_ptr->lex

#include "parser_proxy.h"
#include "tokenizer.h"
#include "utils.h"

using namespace clidoc::tokenizer;

// Error report function.
void clidoc::BisonGeneratedParser::error(const std::string &msg) {
  std::cerr << msg << std::endl;
}


#line 73 "generated_parser.cc" // lalr1.cc:408


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
#line 140 "generated_parser.cc" // lalr1.cc:474

  /// Build a parser object.
  BisonGeneratedParser::BisonGeneratedParser (clidoc::FlexGeneratedScanner *lexer_ptr_yyarg, clidoc::Doc::SharedPtr *doc_ptr_yyarg, clidoc::OptionBindingRecorder *option_binding_recorder_ptr_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      lexer_ptr (lexer_ptr_yyarg),
      doc_ptr (doc_ptr_yyarg),
      option_binding_recorder_ptr (option_binding_recorder_ptr_yyarg)
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
      case 39: // default_value
        value.move< DefaultValue::WeakPtr > (that.value);
        break;

      case 26: // doc
        value.move< Doc::WeakPtr > (that.value);
        break;

      case 31: // seqs
      case 37: // descriptions
      case 40: // comments
        value.move< LogicAnd::WeakPtr > (that.value);
        break;

      case 28: // utilities
      case 30: // or_exprs
        value.move< LogicXor::WeakPtr > (that.value);
        break;

      case 43: // single_binding
        value.move< OptionBinding::WeakPtr > (that.value);
        break;

      case 42: // bindings
        value.move< OptionBindingContainer::WeakPtr > (that.value);
        break;

      case 27: // usage_section
      case 29: // single_utility
      case 32: // single_seq
      case 33: // atom
      case 34: // posix_option_unit
      case 35: // gnu_option_unit
      case 36: // options_section
      case 38: // single_description
      case 41: // single_comment
        value.move< WeakPtrNode > (that.value);
        break;

      case 3: // OPTION_ARGUEMENT
      case 4: // OPERAND
      case 5: // OPTION_DEFAULT_VALUE
      case 6: // COMMENT
      case 7: // ARGUMENT
      case 8: // POSIX_OPTION
      case 9: // GROUPED_OPTIONS
      case 10: // GNU_OPTION
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
      case 39: // default_value
        value.copy< DefaultValue::WeakPtr > (that.value);
        break;

      case 26: // doc
        value.copy< Doc::WeakPtr > (that.value);
        break;

      case 31: // seqs
      case 37: // descriptions
      case 40: // comments
        value.copy< LogicAnd::WeakPtr > (that.value);
        break;

      case 28: // utilities
      case 30: // or_exprs
        value.copy< LogicXor::WeakPtr > (that.value);
        break;

      case 43: // single_binding
        value.copy< OptionBinding::WeakPtr > (that.value);
        break;

      case 42: // bindings
        value.copy< OptionBindingContainer::WeakPtr > (that.value);
        break;

      case 27: // usage_section
      case 29: // single_utility
      case 32: // single_seq
      case 33: // atom
      case 34: // posix_option_unit
      case 35: // gnu_option_unit
      case 36: // options_section
      case 38: // single_description
      case 41: // single_comment
        value.copy< WeakPtrNode > (that.value);
        break;

      case 3: // OPTION_ARGUEMENT
      case 4: // OPERAND
      case 5: // OPTION_DEFAULT_VALUE
      case 6: // COMMENT
      case 7: // ARGUMENT
      case 8: // POSIX_OPTION
      case 9: // GROUPED_OPTIONS
      case 10: // GNU_OPTION
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
      case 39: // default_value
        yylhs.value.build< DefaultValue::WeakPtr > ();
        break;

      case 26: // doc
        yylhs.value.build< Doc::WeakPtr > ();
        break;

      case 31: // seqs
      case 37: // descriptions
      case 40: // comments
        yylhs.value.build< LogicAnd::WeakPtr > ();
        break;

      case 28: // utilities
      case 30: // or_exprs
        yylhs.value.build< LogicXor::WeakPtr > ();
        break;

      case 43: // single_binding
        yylhs.value.build< OptionBinding::WeakPtr > ();
        break;

      case 42: // bindings
        yylhs.value.build< OptionBindingContainer::WeakPtr > ();
        break;

      case 27: // usage_section
      case 29: // single_utility
      case 32: // single_seq
      case 33: // atom
      case 34: // posix_option_unit
      case 35: // gnu_option_unit
      case 36: // options_section
      case 38: // single_description
      case 41: // single_comment
        yylhs.value.build< WeakPtrNode > ();
        break;

      case 3: // OPTION_ARGUEMENT
      case 4: // OPERAND
      case 5: // OPTION_DEFAULT_VALUE
      case 6: // COMMENT
      case 7: // ARGUMENT
      case 8: // POSIX_OPTION
      case 9: // GROUPED_OPTIONS
      case 10: // GNU_OPTION
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
#line 120 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  auto doc = Doc::Init();
  doc->children_.push_back(yystack_[1].value.as< WeakPtrNode > ().lock());
  *doc_ptr = doc;
  yylhs.value.as< Doc::WeakPtr > () = doc;
}
#line 617 "generated_parser.cc" // lalr1.cc:847
    break;

  case 3:
#line 131 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  yylhs.value.as< WeakPtrNode > () = yystack_[0].value.as< LogicXor::WeakPtr > ();
}
#line 625 "generated_parser.cc" // lalr1.cc:847
    break;

  case 4:
#line 140 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  yystack_[1].value.as< LogicXor::WeakPtr > ().lock()->children_.push_back(yystack_[0].value.as< WeakPtrNode > ().lock());
  yylhs.value.as< LogicXor::WeakPtr > () = yystack_[1].value.as< LogicXor::WeakPtr > ();
}
#line 634 "generated_parser.cc" // lalr1.cc:847
    break;

  case 5:
#line 144 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  auto logic_xor = LogicXor::Init();
  logic_xor->children_.push_back(yystack_[0].value.as< WeakPtrNode > ().lock());
  yylhs.value.as< LogicXor::WeakPtr > () = logic_xor;
}
#line 644 "generated_parser.cc" // lalr1.cc:847
    break;

  case 6:
#line 154 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  yylhs.value.as< WeakPtrNode > () = yystack_[0].value.as< LogicXor::WeakPtr > ();
}
#line 652 "generated_parser.cc" // lalr1.cc:847
    break;

  case 7:
#line 163 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  yystack_[2].value.as< LogicXor::WeakPtr > ().lock()->children_.push_back(yystack_[0].value.as< LogicAnd::WeakPtr > ().lock());
  yylhs.value.as< LogicXor::WeakPtr > () = yystack_[2].value.as< LogicXor::WeakPtr > ();
}
#line 661 "generated_parser.cc" // lalr1.cc:847
    break;

  case 8:
#line 167 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  auto logic_xor = LogicXor::Init();
  logic_xor->children_.push_back(yystack_[0].value.as< LogicAnd::WeakPtr > ().lock());
  yylhs.value.as< LogicXor::WeakPtr > () = logic_xor;
}
#line 671 "generated_parser.cc" // lalr1.cc:847
    break;

  case 9:
#line 178 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  yystack_[1].value.as< LogicAnd::WeakPtr > ().lock()->children_.push_back(yystack_[0].value.as< WeakPtrNode > ().lock());
  yylhs.value.as< LogicAnd::WeakPtr > () = yystack_[1].value.as< LogicAnd::WeakPtr > ();
}
#line 680 "generated_parser.cc" // lalr1.cc:847
    break;

  case 10:
#line 182 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  auto logic_and = LogicAnd::Init();
  logic_and->children_.push_back(yystack_[0].value.as< WeakPtrNode > ().lock());
  yylhs.value.as< LogicAnd::WeakPtr > () = logic_and;
}
#line 690 "generated_parser.cc" // lalr1.cc:847
    break;

  case 11:
#line 193 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  yylhs.value.as< WeakPtrNode > () = yystack_[0].value.as< WeakPtrNode > ();
}
#line 698 "generated_parser.cc" // lalr1.cc:847
    break;

  case 12:
#line 196 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  auto logic_one_or_more = LogicOneOrMore::Init();
  logic_one_or_more->children_.push_back(yystack_[1].value.as< WeakPtrNode > ().lock());
  yylhs.value.as< WeakPtrNode > () = logic_one_or_more;
}
#line 708 "generated_parser.cc" // lalr1.cc:847
    break;

  case 13:
#line 212 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  auto logic_and = LogicAnd::Init();
  logic_and->children_.push_back(yystack_[1].value.as< LogicXor::WeakPtr > ().lock());
  yylhs.value.as< WeakPtrNode > () = logic_and;
}
#line 718 "generated_parser.cc" // lalr1.cc:847
    break;

  case 14:
#line 217 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  auto logic_optional = LogicOptional::Init();
  logic_optional->children_.push_back(yystack_[1].value.as< LogicXor::WeakPtr > ().lock());
  yylhs.value.as< WeakPtrNode > () = logic_optional;
}
#line 728 "generated_parser.cc" // lalr1.cc:847
    break;

  case 15:
#line 222 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  yylhs.value.as< WeakPtrNode > () = yystack_[0].value.as< WeakPtrNode > ();
}
#line 736 "generated_parser.cc" // lalr1.cc:847
    break;

  case 16:
#line 225 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  yylhs.value.as< WeakPtrNode > () = yystack_[0].value.as< WeakPtrNode > ();
}
#line 744 "generated_parser.cc" // lalr1.cc:847
    break;

  case 17:
#line 228 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  auto option_argument =
      OptionArguement::Init(InitToken(TypeID::OPTION_ARGUEMENT, yystack_[0].value.as< std::string > ()));
  auto logic_and = LogicAnd::Init();
  logic_and->children_.push_back(option_argument);
  yylhs.value.as< WeakPtrNode > () = logic_and;
}
#line 756 "generated_parser.cc" // lalr1.cc:847
    break;

  case 18:
#line 235 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  auto operand =
      Operand::Init(InitToken(TypeID::OPERAND, yystack_[0].value.as< std::string > ()));
  auto logic_and = LogicAnd::Init();
  logic_and->children_.push_back(operand);
  yylhs.value.as< WeakPtrNode > () = logic_and;
}
#line 768 "generated_parser.cc" // lalr1.cc:847
    break;

  case 19:
#line 242 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  auto k_options =
      KOptions::Init(InitToken(TypeID::K_OPTIONS, ""));
  auto logic_and = LogicAnd::Init();
  logic_and->children_.push_back(k_options);
  yylhs.value.as< WeakPtrNode > () = logic_and;
}
#line 780 "generated_parser.cc" // lalr1.cc:847
    break;

  case 20:
#line 255 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  auto posix_option =
      PosixOption::Init(InitToken(TypeID::POSIX_OPTION, yystack_[0].value.as< std::string > ()));
  auto logic_and = LogicAnd::Init();
  logic_and->children_.push_back(posix_option);
  yylhs.value.as< WeakPtrNode > () = logic_and;
}
#line 792 "generated_parser.cc" // lalr1.cc:847
    break;

  case 21:
#line 262 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  auto grouped_option =
      GroupedOptions::Init(InitToken(TypeID::GROUPED_OPTIONS, yystack_[0].value.as< std::string > ()));
  auto logic_and = LogicAnd::Init();
  logic_and->children_.push_back(grouped_option);
  yylhs.value.as< WeakPtrNode > () = logic_and;
}
#line 804 "generated_parser.cc" // lalr1.cc:847
    break;

  case 22:
#line 275 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  auto gnu_option = GnuOption::Init(InitToken(TypeID::GNU_OPTION, yystack_[0].value.as< std::string > ()));
  auto logic_and = LogicAnd::Init();
  logic_and->children_.push_back(gnu_option);
  yylhs.value.as< WeakPtrNode > () = logic_and;
}
#line 815 "generated_parser.cc" // lalr1.cc:847
    break;

  case 23:
#line 281 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  // recording binding.
  option_binding_recorder_ptr->RecordBinding(
      InitToken(TypeID::GNU_OPTION, yystack_[2].value.as< std::string > ()),
      InitToken(TypeID::OPTION_ARGUEMENT, yystack_[0].value.as< std::string > ()));

  // normal works.
  auto gnu_option = GnuOption::Init(InitToken(TypeID::GNU_OPTION, yystack_[2].value.as< std::string > ()));
  auto option_argument =
      OptionArguement::Init(InitToken(TypeID::OPTION_ARGUEMENT, yystack_[0].value.as< std::string > ()));
  auto logic_and = LogicAnd::Init();
  logic_and->children_.push_back(gnu_option);
  logic_and->children_.push_back(option_argument);
  yylhs.value.as< WeakPtrNode > () = logic_and;
}
#line 835 "generated_parser.cc" // lalr1.cc:847
    break;

  case 24:
#line 301 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 841 "generated_parser.cc" // lalr1.cc:847
    break;

  case 25:
#line 302 "flex-bison-scripts/parser.y" // lalr1.cc:847
    { }
#line 847 "generated_parser.cc" // lalr1.cc:847
    break;

  case 26:
#line 306 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 853 "generated_parser.cc" // lalr1.cc:847
    break;

  case 27:
#line 307 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 859 "generated_parser.cc" // lalr1.cc:847
    break;

  case 28:
#line 313 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  option_binding_recorder_ptr->RecordBinding(
      yystack_[2].value.as< OptionBindingContainer::WeakPtr > ().lock(), yystack_[1].value.as< DefaultValue::WeakPtr > ().lock());
}
#line 868 "generated_parser.cc" // lalr1.cc:847
    break;

  case 29:
#line 320 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  
  auto default_value =
      DefaultValue::Init(InitToken(TypeID::OPTION_DEFAULT_VALUE, yystack_[1].value.as< std::string > ()));
  yylhs.value.as< DefaultValue::WeakPtr > () = default_value;
}
#line 878 "generated_parser.cc" // lalr1.cc:847
    break;

  case 30:
#line 325 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  yylhs.value.as< DefaultValue::WeakPtr > () = DefaultValue::Init();
}
#line 886 "generated_parser.cc" // lalr1.cc:847
    break;

  case 31:
#line 331 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 892 "generated_parser.cc" // lalr1.cc:847
    break;

  case 32:
#line 332 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 898 "generated_parser.cc" // lalr1.cc:847
    break;

  case 33:
#line 336 "flex-bison-scripts/parser.y" // lalr1.cc:847
    { }
#line 904 "generated_parser.cc" // lalr1.cc:847
    break;

  case 34:
#line 337 "flex-bison-scripts/parser.y" // lalr1.cc:847
    { }
#line 910 "generated_parser.cc" // lalr1.cc:847
    break;

  case 35:
#line 344 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  yystack_[1].value.as< OptionBindingContainer::WeakPtr > ().lock()->children_.push_back(yystack_[0].value.as< OptionBinding::WeakPtr > ().lock());
  yylhs.value.as< OptionBindingContainer::WeakPtr > () = yystack_[1].value.as< OptionBindingContainer::WeakPtr > ();

}
#line 920 "generated_parser.cc" // lalr1.cc:847
    break;

  case 36:
#line 349 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  auto bindings = OptionBindingContainer::Init();
  bindings->children_.push_back(yystack_[0].value.as< OptionBinding::WeakPtr > ().lock());
  yylhs.value.as< OptionBindingContainer::WeakPtr > () = bindings;
}
#line 930 "generated_parser.cc" // lalr1.cc:847
    break;

  case 37:
#line 363 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  auto binding =
      OptionBinding::Init(InitToken(TypeID::POSIX_OPTION, yystack_[0].value.as< std::string > ()));
  yylhs.value.as< OptionBinding::WeakPtr > () = binding;
}
#line 940 "generated_parser.cc" // lalr1.cc:847
    break;

  case 38:
#line 368 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  auto binding =
      OptionBinding::Init(InitToken(TypeID::GNU_OPTION, yystack_[0].value.as< std::string > ()));
  yylhs.value.as< OptionBinding::WeakPtr > () = binding;
}
#line 950 "generated_parser.cc" // lalr1.cc:847
    break;

  case 39:
#line 373 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  auto binding =
      OptionBinding::Init(InitToken(TypeID::POSIX_OPTION, yystack_[1].value.as< std::string > ()),
                          InitToken(TypeID::OPTION_ARGUEMENT, yystack_[0].value.as< std::string > ()));
  yylhs.value.as< OptionBinding::WeakPtr > () = binding;
}
#line 961 "generated_parser.cc" // lalr1.cc:847
    break;

  case 40:
#line 379 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  auto binding =
      OptionBinding::Init(InitToken(TypeID::GNU_OPTION, yystack_[1].value.as< std::string > ()),
                          InitToken(TypeID::OPTION_ARGUEMENT, yystack_[0].value.as< std::string > ()));
  yylhs.value.as< OptionBinding::WeakPtr > () = binding;
}
#line 972 "generated_parser.cc" // lalr1.cc:847
    break;

  case 41:
#line 385 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {
  auto binding =
      OptionBinding::Init(InitToken(TypeID::GNU_OPTION, yystack_[2].value.as< std::string > ()),
                          InitToken(TypeID::OPTION_ARGUEMENT, yystack_[0].value.as< std::string > ()));
  yylhs.value.as< OptionBinding::WeakPtr > () = binding;
}
#line 983 "generated_parser.cc" // lalr1.cc:847
    break;


#line 987 "generated_parser.cc" // lalr1.cc:847
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


  const signed char BisonGeneratedParser::yypact_ninf_ = -20;

  const signed char BisonGeneratedParser::yytable_ninf_ = -1;

  const signed char
  BisonGeneratedParser::yypact_[] =
  {
     -12,    -7,    18,     3,    -1,    -7,   -20,   -20,    -4,   -20,
     -20,   -20,   -20,   -20,     6,    -1,    -1,   -20,    14,    -1,
     -20,    15,   -20,   -20,   -20,    29,    -2,    -4,   -20,     7,
     -20,    30,     1,     9,    -1,   -20,   -20,   -20,   -20,    31,
     -20,    16,     5,   -20,   -20,   -20,   -20,    -1,   -20,    32,
      17,   -20,     5,   -20,    22,   -20,   -20,   -20
  };

  const unsigned char
  BisonGeneratedParser::yydefact_[] =
  {
       0,     0,     0,    25,     0,     3,     5,     1,     0,     2,
      17,    18,    20,    21,    22,     0,     0,    19,     6,     8,
      10,    11,    15,    16,     4,    37,    38,    24,    27,    30,
      36,     0,     0,     0,     0,     9,    12,    39,    40,     0,
      26,     0,     0,    35,    23,    13,    14,     7,    41,     0,
       0,    34,    28,    32,     0,    33,    31,    29
  };

  const signed char
  BisonGeneratedParser::yypgoto_[] =
  {
     -20,   -20,   -20,   -20,    33,    10,     8,   -19,   -20,   -20,
     -20,   -20,   -20,    13,   -20,   -20,   -11,   -20,    19
  };

  const signed char
  BisonGeneratedParser::yydefgoto_[] =
  {
      -1,     2,     3,     5,     6,    18,    19,    20,    21,    22,
      23,     9,    27,    28,    42,    52,    53,    29,    30
  };

  const unsigned char
  BisonGeneratedParser::yytable_[] =
  {
      35,    38,    10,    11,    25,     1,    26,    12,    13,    14,
      15,    50,    16,    45,     4,    25,    34,    26,     7,    17,
      41,     8,    39,    46,    34,    32,    33,    51,    35,    34,
      31,    36,    37,    44,    48,    49,    57,    54,    24,    55,
      40,    56,    47,     0,     0,     0,     0,     0,    43
  };

  const signed char
  BisonGeneratedParser::yycheck_[] =
  {
      19,     3,     3,     4,     8,    17,    10,     8,     9,    10,
      11,     6,    13,    12,    21,     8,    15,    10,     0,    20,
      13,    18,    24,    14,    15,    15,    16,    22,    47,    15,
      24,    16,     3,     3,     3,    19,    14,     5,     5,    22,
      27,    52,    34,    -1,    -1,    -1,    -1,    -1,    29
  };

  const unsigned char
  BisonGeneratedParser::yystos_[] =
  {
       0,    17,    26,    27,    21,    28,    29,     0,    18,    36,
       3,     4,     8,     9,    10,    11,    13,    20,    30,    31,
      32,    33,    34,    35,    29,     8,    10,    37,    38,    42,
      43,    24,    30,    30,    15,    32,    16,     3,     3,    24,
      38,    13,    39,    43,     3,    12,    14,    31,     3,    19,
       6,    22,    40,    41,     5,    22,    41,    14
  };

  const unsigned char
  BisonGeneratedParser::yyr1_[] =
  {
       0,    25,    26,    27,    28,    28,    29,    30,    30,    31,
      31,    32,    32,    33,    33,    33,    33,    33,    33,    33,
      34,    34,    35,    35,    36,    36,    37,    37,    38,    39,
      39,    40,    40,    41,    41,    42,    42,    43,    43,    43,
      43,    43
  };

  const unsigned char
  BisonGeneratedParser::yyr2_[] =
  {
       0,     2,     2,     2,     2,     1,     2,     3,     1,     2,
       1,     1,     2,     3,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     2,     0,     2,     1,     3,     4,
       0,     2,     1,     2,     1,     2,     1,     1,     1,     2,
       2,     3
  };


#if YYDEBUG
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const BisonGeneratedParser::yytname_[] =
  {
  "END", "error", "$undefined", "OPTION_ARGUEMENT", "OPERAND",
  "OPTION_DEFAULT_VALUE", "COMMENT", "ARGUMENT", "POSIX_OPTION",
  "GROUPED_OPTIONS", "GNU_OPTION", "\"(\"", "\")\"", "\"[\"", "\"]\"",
  "\"|\"", "\"...\"", "\"usage:\"", "\"options:\"", "\"default:\"",
  "\"options\"", "\"*UTILITY_DELIMITER*\"", "\"*DESC_DELIMITER*\"",
  "\"--\"", "\"=\"", "$accept", "doc", "usage_section", "utilities",
  "single_utility", "or_exprs", "seqs", "single_seq", "atom",
  "posix_option_unit", "gnu_option_unit", "options_section",
  "descriptions", "single_description", "default_value", "comments",
  "single_comment", "bindings", "single_binding", YY_NULLPTR
  };


  const unsigned short int
  BisonGeneratedParser::yyrline_[] =
  {
       0,   120,   120,   131,   140,   144,   154,   163,   167,   178,
     182,   193,   196,   212,   217,   222,   225,   228,   235,   242,
     255,   262,   275,   281,   301,   302,   306,   307,   313,   320,
     325,   331,   332,   336,   337,   344,   349,   363,   368,   373,
     379,   385
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
#line 1302 "generated_parser.cc" // lalr1.cc:1155
#line 393 "flex-bison-scripts/parser.y" // lalr1.cc:1156

