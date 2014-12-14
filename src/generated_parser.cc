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
#line 23 "flex-bison-scripts/parser.y" // lalr1.cc:408

#include "generated_scanner.h"
#undef yylex
#define yylex lexer_ptr->lex

// Error report function.
void clidoc::BisonGeneratedParser::error (const std::string&) { /* empty */ }

#line 64 "generated_parser.cc" // lalr1.cc:408


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
#line 131 "generated_parser.cc" // lalr1.cc:474

  /// Build a parser object.
  BisonGeneratedParser::BisonGeneratedParser (clidoc::FlexGeneratedScanner *lexer_ptr_yyarg)
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
      case 26: // doc
      case 27: // usage_section
      case 28: // utilities
      case 29: // single_utility
      case 30: // or_exprs
      case 31: // seqs
      case 32: // single_seq
      case 33: // atom
      case 34: // posix_option_unit
      case 35: // gnu_option_unit
      case 36: // options_section
      case 37: // descriptions
      case 38: // single_description
      case 39: // default_value
      case 40: // comment
      case 41: // bindings
      case 42: // single_binding
        value.move< Undefine > (that.value);
        break;

      case 3: // OPTION_ARGUEMENT
      case 4: // OPTION_DEFAULT_VALUE
      case 5: // COMMENT
      case 6: // ARGUMENT
      case 7: // POSIX_OPTION
      case 8: // GROUPED_OPTIONS
      case 9: // GNU_OPTION
      case 10: // OPERAND
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
      case 26: // doc
      case 27: // usage_section
      case 28: // utilities
      case 29: // single_utility
      case 30: // or_exprs
      case 31: // seqs
      case 32: // single_seq
      case 33: // atom
      case 34: // posix_option_unit
      case 35: // gnu_option_unit
      case 36: // options_section
      case 37: // descriptions
      case 38: // single_description
      case 39: // default_value
      case 40: // comment
      case 41: // bindings
      case 42: // single_binding
        value.copy< Undefine > (that.value);
        break;

      case 3: // OPTION_ARGUEMENT
      case 4: // OPTION_DEFAULT_VALUE
      case 5: // COMMENT
      case 6: // ARGUMENT
      case 7: // POSIX_OPTION
      case 8: // GROUPED_OPTIONS
      case 9: // GNU_OPTION
      case 10: // OPERAND
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
      case 26: // doc
      case 27: // usage_section
      case 28: // utilities
      case 29: // single_utility
      case 30: // or_exprs
      case 31: // seqs
      case 32: // single_seq
      case 33: // atom
      case 34: // posix_option_unit
      case 35: // gnu_option_unit
      case 36: // options_section
      case 37: // descriptions
      case 38: // single_description
      case 39: // default_value
      case 40: // comment
      case 41: // bindings
      case 42: // single_binding
        yylhs.value.build< Undefine > ();
        break;

      case 3: // OPTION_ARGUEMENT
      case 4: // OPTION_DEFAULT_VALUE
      case 5: // COMMENT
      case 6: // ARGUMENT
      case 7: // POSIX_OPTION
      case 8: // GROUPED_OPTIONS
      case 9: // GNU_OPTION
      case 10: // OPERAND
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
#line 99 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 544 "generated_parser.cc" // lalr1.cc:847
    break;

  case 3:
#line 102 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 550 "generated_parser.cc" // lalr1.cc:847
    break;

  case 4:
#line 105 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 556 "generated_parser.cc" // lalr1.cc:847
    break;

  case 6:
#line 109 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 562 "generated_parser.cc" // lalr1.cc:847
    break;

  case 7:
#line 112 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 568 "generated_parser.cc" // lalr1.cc:847
    break;

  case 8:
#line 113 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 574 "generated_parser.cc" // lalr1.cc:847
    break;

  case 9:
#line 116 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 580 "generated_parser.cc" // lalr1.cc:847
    break;

  case 10:
#line 117 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 586 "generated_parser.cc" // lalr1.cc:847
    break;

  case 11:
#line 120 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 592 "generated_parser.cc" // lalr1.cc:847
    break;

  case 12:
#line 121 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 598 "generated_parser.cc" // lalr1.cc:847
    break;

  case 13:
#line 124 "flex-bison-scripts/parser.y" // lalr1.cc:847
    { }
#line 604 "generated_parser.cc" // lalr1.cc:847
    break;

  case 14:
#line 125 "flex-bison-scripts/parser.y" // lalr1.cc:847
    { }
#line 610 "generated_parser.cc" // lalr1.cc:847
    break;

  case 15:
#line 126 "flex-bison-scripts/parser.y" // lalr1.cc:847
    { }
#line 616 "generated_parser.cc" // lalr1.cc:847
    break;

  case 16:
#line 127 "flex-bison-scripts/parser.y" // lalr1.cc:847
    { }
#line 622 "generated_parser.cc" // lalr1.cc:847
    break;

  case 17:
#line 128 "flex-bison-scripts/parser.y" // lalr1.cc:847
    { }
#line 628 "generated_parser.cc" // lalr1.cc:847
    break;

  case 18:
#line 129 "flex-bison-scripts/parser.y" // lalr1.cc:847
    { }
#line 634 "generated_parser.cc" // lalr1.cc:847
    break;

  case 19:
#line 130 "flex-bison-scripts/parser.y" // lalr1.cc:847
    { }
#line 640 "generated_parser.cc" // lalr1.cc:847
    break;

  case 20:
#line 133 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 646 "generated_parser.cc" // lalr1.cc:847
    break;

  case 21:
#line 134 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 652 "generated_parser.cc" // lalr1.cc:847
    break;

  case 22:
#line 137 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 658 "generated_parser.cc" // lalr1.cc:847
    break;

  case 23:
#line 138 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 664 "generated_parser.cc" // lalr1.cc:847
    break;

  case 24:
#line 141 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 670 "generated_parser.cc" // lalr1.cc:847
    break;

  case 25:
#line 144 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 676 "generated_parser.cc" // lalr1.cc:847
    break;

  case 26:
#line 145 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 682 "generated_parser.cc" // lalr1.cc:847
    break;

  case 27:
#line 148 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 688 "generated_parser.cc" // lalr1.cc:847
    break;

  case 28:
#line 151 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 694 "generated_parser.cc" // lalr1.cc:847
    break;

  case 29:
#line 152 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 700 "generated_parser.cc" // lalr1.cc:847
    break;

  case 30:
#line 155 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 706 "generated_parser.cc" // lalr1.cc:847
    break;

  case 31:
#line 156 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 712 "generated_parser.cc" // lalr1.cc:847
    break;

  case 32:
#line 159 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 718 "generated_parser.cc" // lalr1.cc:847
    break;

  case 33:
#line 160 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 724 "generated_parser.cc" // lalr1.cc:847
    break;

  case 34:
#line 163 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 730 "generated_parser.cc" // lalr1.cc:847
    break;

  case 35:
#line 164 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 736 "generated_parser.cc" // lalr1.cc:847
    break;

  case 36:
#line 165 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 742 "generated_parser.cc" // lalr1.cc:847
    break;

  case 37:
#line 166 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 748 "generated_parser.cc" // lalr1.cc:847
    break;

  case 38:
#line 167 "flex-bison-scripts/parser.y" // lalr1.cc:847
    {  }
#line 754 "generated_parser.cc" // lalr1.cc:847
    break;


#line 758 "generated_parser.cc" // lalr1.cc:847
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
      -4,     6,    15,     7,    -2,     6,   -20,   -20,    -5,   -20,
     -20,   -20,   -20,     1,   -20,    -2,    -2,   -20,    12,    -2,
     -20,    13,   -20,   -20,   -20,    27,     0,    -5,   -20,     3,
     -20,    28,     2,     4,    -2,   -20,   -20,   -20,   -20,    29,
     -20,    14,    30,   -20,   -20,   -20,   -20,    -2,   -20,    32,
     -20,    16,    20,   -20,   -20
  };

  const unsigned char
  BisonGeneratedParser::yydefact_[] =
  {
       0,     0,     0,     0,     0,     3,     5,     1,     0,     2,
      17,    20,    21,    22,    18,     0,     0,    19,     6,     8,
      10,    11,    15,    16,     4,    34,    35,    24,    26,    29,
      33,     0,     0,     0,     0,     9,    12,    36,    37,     0,
      25,     0,    31,    32,    23,    13,    14,     7,    38,     0,
      30,     0,     0,    27,    28
  };

  const signed char
  BisonGeneratedParser::yypgoto_[] =
  {
     -20,   -20,   -20,   -20,    33,     5,     8,   -19,   -20,   -20,
     -20,   -20,   -20,    17,   -20,   -20,   -20,    10
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
      35,    10,    25,    38,    26,    11,    12,    13,    14,    15,
      25,    16,    26,     1,    45,     7,    41,    34,    46,    34,
      32,    33,    17,    39,    31,     8,     4,    34,    35,    36,
      37,    44,    48,    49,    54,    50,    52,    53,    24,    43,
       0,     0,    47,     0,    40
  };

  const signed char
  BisonGeneratedParser::yycheck_[] =
  {
      19,     3,     7,     3,     9,     7,     8,     9,    10,    11,
       7,    13,     9,    17,    12,     0,    13,    15,    14,    15,
      15,    16,    24,    23,    23,    18,    20,    15,    47,    16,
       3,     3,     3,    19,    14,     5,     4,    21,     5,    29,
      -1,    -1,    34,    -1,    27
  };

  const unsigned char
  BisonGeneratedParser::yystos_[] =
  {
       0,    17,    26,    27,    20,    28,    29,     0,    18,    36,
       3,     7,     8,     9,    10,    11,    13,    24,    30,    31,
      32,    33,    34,    35,    29,     7,     9,    37,    38,    41,
      42,    23,    30,    30,    15,    32,    16,     3,     3,    23,
      38,    13,    39,    42,     3,    12,    14,    31,     3,    19,
       5,    40,     4,    21,    14
  };

  const unsigned char
  BisonGeneratedParser::yyr1_[] =
  {
       0,    25,    26,    27,    28,    28,    29,    30,    30,    31,
      31,    32,    32,    33,    33,    33,    33,    33,    33,    33,
      34,    34,    35,    35,    36,    37,    37,    38,    39,    39,
      40,    40,    41,    41,    42,    42,    42,    42,    42
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
  "END", "error", "$undefined", "OPTION_ARGUEMENT",
  "OPTION_DEFAULT_VALUE", "COMMENT", "ARGUMENT", "POSIX_OPTION",
  "GROUPED_OPTIONS", "GNU_OPTION", "OPERAND", "\"(\"", "\")\"", "\"[\"",
  "\"]\"", "\"|\"", "\"...\"", "\"usage:\"", "\"options:\"",
  "\"default:\"", "\"*UTILITY_DELIMITER*\"", "\"*DESC_DELIMITER*\"",
  "\"--\"", "\"=\"", "\"options\"", "$accept", "doc", "usage_section",
  "utilities", "single_utility", "or_exprs", "seqs", "single_seq", "atom",
  "posix_option_unit", "gnu_option_unit", "options_section",
  "descriptions", "single_description", "default_value", "comment",
  "bindings", "single_binding", YY_NULLPTR
  };


  const unsigned char
  BisonGeneratedParser::yyrline_[] =
  {
       0,    99,    99,   102,   105,   106,   109,   112,   113,   116,
     117,   120,   121,   124,   125,   126,   127,   128,   129,   130,
     133,   134,   137,   138,   141,   144,   145,   148,   151,   152,
     155,   156,   159,   160,   163,   164,   165,   166,   167
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
#line 1070 "generated_parser.cc" // lalr1.cc:1155
#line 170 "flex-bison-scripts/parser.y" // lalr1.cc:1156

