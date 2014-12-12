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


// First part of user declarations.

#line 37 "generated_parser.cc" // lalr1.cc:399

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "generated_parser.h"

// User implementation prologue.

#line 51 "generated_parser.cc" // lalr1.cc:407
// Unqualified %code blocks.
#line 21 "flex-bison-scripts/parser.y" // lalr1.cc:408

#include "generated_scanner.h"
#undef yylex
#define yylex lexer_ptr->lex

// Error report function.
void yy::BisonGeneratedParser::error (const std::string&) {}

#line 62 "generated_parser.cc" // lalr1.cc:408


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


namespace yy {
#line 129 "generated_parser.cc" // lalr1.cc:474

  /// Build a parser object.
  BisonGeneratedParser::BisonGeneratedParser (FlexGeneratedScanner *lexer_ptr_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      lexer_ptr (lexer_ptr_yyarg)
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
      case 24: // doc
      case 25: // usage_section
      case 26: // utilities
      case 27: // single_utility
      case 28: // or_exprs
      case 29: // seqs
      case 30: // single_seq
      case 31: // atom
      case 32: // posix_option_unit
      case 33: // gnu_option_unit
      case 34: // options_section
      case 35: // descriptions
      case 36: // single_description
      case 37: // default_value
      case 38: // comment
      case 39: // bindings
      case 40: // single_binding
        value.move< Undefine > (that.value);
        break;

      case 3: // POSIX_OPTION
      case 4: // GROUPED_OPTIONS
      case 5: // GNU_OPTION
      case 6: // ARGUEMENT
      case 7: // OPERAND
      case 8: // OPTION_DEFAULT_VALUE
      case 9: // COMMENT
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
      case 24: // doc
      case 25: // usage_section
      case 26: // utilities
      case 27: // single_utility
      case 28: // or_exprs
      case 29: // seqs
      case 30: // single_seq
      case 31: // atom
      case 32: // posix_option_unit
      case 33: // gnu_option_unit
      case 34: // options_section
      case 35: // descriptions
      case 36: // single_description
      case 37: // default_value
      case 38: // comment
      case 39: // bindings
      case 40: // single_binding
        value.copy< Undefine > (that.value);
        break;

      case 3: // POSIX_OPTION
      case 4: // GROUPED_OPTIONS
      case 5: // GNU_OPTION
      case 6: // ARGUEMENT
      case 7: // OPERAND
      case 8: // OPTION_DEFAULT_VALUE
      case 9: // COMMENT
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
      case 24: // doc
      case 25: // usage_section
      case 26: // utilities
      case 27: // single_utility
      case 28: // or_exprs
      case 29: // seqs
      case 30: // single_seq
      case 31: // atom
      case 32: // posix_option_unit
      case 33: // gnu_option_unit
      case 34: // options_section
      case 35: // descriptions
      case 36: // single_description
      case 37: // default_value
      case 38: // comment
      case 39: // bindings
      case 40: // single_binding
        yylhs.value.build< Undefine > ();
        break;

      case 3: // POSIX_OPTION
      case 4: // GROUPED_OPTIONS
      case 5: // GNU_OPTION
      case 6: // ARGUEMENT
      case 7: // OPERAND
      case 8: // OPTION_DEFAULT_VALUE
      case 9: // COMMENT
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
#line 77 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 539 "generated_parser.cc" // lalr1.cc:847
    break;

  case 3:
#line 80 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 545 "generated_parser.cc" // lalr1.cc:847
    break;

  case 4:
#line 83 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 551 "generated_parser.cc" // lalr1.cc:847
    break;

  case 6:
#line 87 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 557 "generated_parser.cc" // lalr1.cc:847
    break;

  case 7:
#line 90 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 563 "generated_parser.cc" // lalr1.cc:847
    break;

  case 8:
#line 91 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 569 "generated_parser.cc" // lalr1.cc:847
    break;

  case 9:
#line 94 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 575 "generated_parser.cc" // lalr1.cc:847
    break;

  case 10:
#line 95 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 581 "generated_parser.cc" // lalr1.cc:847
    break;

  case 11:
#line 98 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 587 "generated_parser.cc" // lalr1.cc:847
    break;

  case 12:
#line 99 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 593 "generated_parser.cc" // lalr1.cc:847
    break;

  case 13:
#line 102 "flex-bison-scripts/parser.y" // lalr1.cc:847
    { }
#line 599 "generated_parser.cc" // lalr1.cc:847
    break;

  case 14:
#line 103 "flex-bison-scripts/parser.y" // lalr1.cc:847
    { }
#line 605 "generated_parser.cc" // lalr1.cc:847
    break;

  case 15:
#line 104 "flex-bison-scripts/parser.y" // lalr1.cc:847
    { }
#line 611 "generated_parser.cc" // lalr1.cc:847
    break;

  case 16:
#line 105 "flex-bison-scripts/parser.y" // lalr1.cc:847
    { }
#line 617 "generated_parser.cc" // lalr1.cc:847
    break;

  case 17:
#line 106 "flex-bison-scripts/parser.y" // lalr1.cc:847
    { }
#line 623 "generated_parser.cc" // lalr1.cc:847
    break;

  case 18:
#line 107 "flex-bison-scripts/parser.y" // lalr1.cc:847
    { }
#line 629 "generated_parser.cc" // lalr1.cc:847
    break;

  case 19:
#line 108 "flex-bison-scripts/parser.y" // lalr1.cc:847
    { }
#line 635 "generated_parser.cc" // lalr1.cc:847
    break;

  case 20:
#line 111 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 641 "generated_parser.cc" // lalr1.cc:847
    break;

  case 21:
#line 112 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 647 "generated_parser.cc" // lalr1.cc:847
    break;

  case 22:
#line 115 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 653 "generated_parser.cc" // lalr1.cc:847
    break;

  case 23:
#line 116 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 659 "generated_parser.cc" // lalr1.cc:847
    break;

  case 24:
#line 119 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 665 "generated_parser.cc" // lalr1.cc:847
    break;

  case 25:
#line 122 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 671 "generated_parser.cc" // lalr1.cc:847
    break;

  case 26:
#line 123 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 677 "generated_parser.cc" // lalr1.cc:847
    break;

  case 27:
#line 126 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 683 "generated_parser.cc" // lalr1.cc:847
    break;

  case 28:
#line 129 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 689 "generated_parser.cc" // lalr1.cc:847
    break;

  case 29:
#line 130 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 695 "generated_parser.cc" // lalr1.cc:847
    break;

  case 30:
#line 133 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 701 "generated_parser.cc" // lalr1.cc:847
    break;

  case 31:
#line 134 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 707 "generated_parser.cc" // lalr1.cc:847
    break;

  case 32:
#line 137 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 713 "generated_parser.cc" // lalr1.cc:847
    break;

  case 33:
#line 138 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 719 "generated_parser.cc" // lalr1.cc:847
    break;

  case 34:
#line 141 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 725 "generated_parser.cc" // lalr1.cc:847
    break;

  case 35:
#line 142 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 731 "generated_parser.cc" // lalr1.cc:847
    break;

  case 36:
#line 143 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 737 "generated_parser.cc" // lalr1.cc:847
    break;

  case 37:
#line 144 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 743 "generated_parser.cc" // lalr1.cc:847
    break;

  case 38:
#line 145 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 749 "generated_parser.cc" // lalr1.cc:847
    break;


#line 753 "generated_parser.cc" // lalr1.cc:847
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
      -5,     5,    21,     9,    -2,     5,   -20,   -20,     8,   -20,
     -20,   -20,    10,   -20,   -20,    -2,    -2,   -20,    15,    -2,
     -20,    14,   -20,   -20,   -20,    18,     0,     8,   -20,     4,
     -20,    25,     3,     6,    -2,   -20,   -20,   -20,   -20,    26,
     -20,    16,    24,   -20,   -20,   -20,   -20,    -2,   -20,    28,
     -20,    12,    27,   -20,   -20
  };

  const unsigned char
  BisonGeneratedParser::yydefact_[] =
  {
       0,     0,     0,     0,     0,     3,     5,     1,     0,     2,
      20,    21,    22,    17,    18,     0,     0,    19,     6,     8,
      10,    11,    15,    16,     4,    34,    35,    24,    26,    29,
      33,     0,     0,     0,     0,     9,    12,    36,    37,     0,
      25,     0,    31,    32,    23,    13,    14,     7,    38,     0,
      30,     0,     0,    27,    28
  };

  const signed char
  BisonGeneratedParser::yypgoto_[] =
  {
     -20,   -20,   -20,   -20,    32,     7,    11,   -19,   -20,   -20,
     -20,   -20,   -20,    17,   -20,   -20,   -20,    13
  };

  const signed char
  BisonGeneratedParser::yydefgoto_[] =
  {
      -1,     2,     3,     5,     6,    18,    19,    20,    21,    22,
      23,     9,    27,    28,    42,    51,    29,    30
  };

  const unsigned char
  BisonGeneratedParser::yytable_[] =
  {
      35,    10,    11,    12,    13,    14,    38,    25,    15,    26,
      16,    25,     1,    26,    45,    39,    41,    34,    17,    46,
      34,     7,    32,    33,    37,    31,     4,     8,    35,    34,
      36,    44,    48,    50,    53,    49,    52,    24,     0,     0,
      54,     0,    43,     0,    40,    47
  };

  const signed char
  BisonGeneratedParser::yycheck_[] =
  {
      19,     3,     4,     5,     6,     7,     6,     3,    10,     5,
      12,     3,    17,     5,    11,    15,    12,    14,    20,    13,
      14,     0,    15,    16,     6,    15,    21,    18,    47,    14,
      16,     6,     6,     9,    22,    19,     8,     5,    -1,    -1,
      13,    -1,    29,    -1,    27,    34
  };

  const unsigned char
  BisonGeneratedParser::yystos_[] =
  {
       0,    17,    24,    25,    21,    26,    27,     0,    18,    34,
       3,     4,     5,     6,     7,    10,    12,    20,    28,    29,
      30,    31,    32,    33,    27,     3,     5,    35,    36,    39,
      40,    15,    28,    28,    14,    30,    16,     6,     6,    15,
      36,    12,    37,    40,     6,    11,    13,    29,     6,    19,
       9,    38,     8,    22,    13
  };

  const unsigned char
  BisonGeneratedParser::yyr1_[] =
  {
       0,    23,    24,    25,    26,    26,    27,    28,    28,    29,
      29,    30,    30,    31,    31,    31,    31,    31,    31,    31,
      32,    32,    33,    33,    34,    35,    35,    36,    37,    37,
      38,    38,    39,    39,    40,    40,    40,    40,    40
  };

  const unsigned char
  BisonGeneratedParser::yyr2_[] =
  {
       0,     2,     2,     2,     2,     1,     2,     3,     1,     2,
       1,     1,     2,     3,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     2,     2,     1,     4,     4,     0,
       1,     0,     2,     1,     1,     1,     2,     2,     3
  };


#if YYDEBUG
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const BisonGeneratedParser::yytname_[] =
  {
  "END", "error", "$undefined", "POSIX_OPTION", "GROUPED_OPTIONS",
  "GNU_OPTION", "ARGUEMENT", "OPERAND", "OPTION_DEFAULT_VALUE", "COMMENT",
  "\"(\"", "\")\"", "\"[\"", "\"]\"", "\"|\"", "\"=\"", "\"...\"",
  "\"usage:\"", "\"options:\"", "\"default:\"", "\"options\"",
  "\"*UTILITY_DELIMITER*\"", "\"*DESC_DELIMITER*\"", "$accept", "doc",
  "usage_section", "utilities", "single_utility", "or_exprs", "seqs",
  "single_seq", "atom", "posix_option_unit", "gnu_option_unit",
  "options_section", "descriptions", "single_description", "default_value",
  "comment", "bindings", "single_binding", YY_NULLPTR
  };


  const unsigned char
  BisonGeneratedParser::yyrline_[] =
  {
       0,    77,    77,    80,    83,    84,    87,    90,    91,    94,
      95,    98,    99,   102,   103,   104,   105,   106,   107,   108,
     111,   112,   115,   116,   119,   122,   123,   126,   129,   130,
     133,   134,   137,   138,   141,   142,   143,   144,   145
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



} // yy
#line 1064 "generated_parser.cc" // lalr1.cc:1155
#line 148 "flex-bison-scripts/parser.y" // lalr1.cc:1156

