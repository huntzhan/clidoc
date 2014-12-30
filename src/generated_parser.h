// A Bison parser, made by GNU Bison 3.0.2.

// Skeleton interface for Bison LALR(1) parsers in C++

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

/**
 ** \file generated_parser.h
 ** Define the clidoc::parser class.
 */

// C++ LALR(1) parser skeleton written by Akim Demaille.

#ifndef YY_CLIDOC_GENERATED_PARSER_H_INCLUDED
# define YY_CLIDOC_GENERATED_PARSER_H_INCLUDED
// //                    "%code requires" blocks.
#line 13 "flex-bison-scripts/parser.y" // lalr1.cc:387

#include <string>
#include "utils.h"

// Forward declarations.
namespace clidoc {

class FlexGeneratedScanner;
class OptionBindingRecorder;

}  // namespace clidoc


#line 58 "generated_parser.h" // lalr1.cc:387


# include <vector>
# include <iostream>
# include <stdexcept>
# include <string>
# include "stack.hh"


#ifndef YYASSERT
# include <cassert>
# define YYASSERT assert
#endif


#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif


namespace clidoc {
#line 134 "generated_parser.h" // lalr1.cc:387



  /// A char[S] buffer to store and retrieve objects.
  ///
  /// Sort of a variant, but does not keep track of the nature
  /// of the stored data, since that knowledge is available
  /// via the current state.
  template <size_t S>
  struct variant
  {
    /// Type of *this.
    typedef variant<S> self_type;

    /// Empty construction.
    variant ()
    {}

    /// Construct and fill.
    template <typename T>
    variant (const T& t)
    {
      YYASSERT (sizeof (T) <= S);
      new (yyas_<T> ()) T (t);
    }

    /// Destruction, allowed only if empty.
    ~variant ()
    {}

    /// Instantiate an empty \a T in here.
    template <typename T>
    T&
    build ()
    {
      return *new (yyas_<T> ()) T;
    }

    /// Instantiate a \a T in here from \a t.
    template <typename T>
    T&
    build (const T& t)
    {
      return *new (yyas_<T> ()) T (t);
    }

    /// Accessor to a built \a T.
    template <typename T>
    T&
    as ()
    {
      return *yyas_<T> ();
    }

    /// Const accessor to a built \a T (for %printer).
    template <typename T>
    const T&
    as () const
    {
      return *yyas_<T> ();
    }

    /// Swap the content with \a other, of same type.
    ///
    /// Both variants must be built beforehand, because swapping the actual
    /// data requires reading it (with as()), and this is not possible on
    /// unconstructed variants: it would require some dynamic testing, which
    /// should not be the variant's responsability.
    /// Swapping between built and (possibly) non-built is done with
    /// variant::move ().
    template <typename T>
    void
    swap (self_type& other)
    {
      std::swap (as<T> (), other.as<T> ());
    }

    /// Move the content of \a other to this.
    ///
    /// Destroys \a other.
    template <typename T>
    void
    move (self_type& other)
    {
      build<T> ();
      swap<T> (other);
      other.destroy<T> ();
    }

    /// Copy the content of \a other to this.
    template <typename T>
    void
    copy (const self_type& other)
    {
      build<T> (other.as<T> ());
    }

    /// Destroy the stored \a T.
    template <typename T>
    void
    destroy ()
    {
      as<T> ().~T ();
    }

  private:
    /// Prohibit blind copies.
    self_type& operator=(const self_type&);
    variant (const self_type&);

    /// Accessor to raw memory as \a T.
    template <typename T>
    T*
    yyas_ ()
    {
      void *yyp = yybuffer_.yyraw;
      return static_cast<T*> (yyp);
     }

    /// Const accessor to raw memory as \a T.
    template <typename T>
    const T*
    yyas_ () const
    {
      const void *yyp = yybuffer_.yyraw;
      return static_cast<const T*> (yyp);
     }

    union
    {
      /// Strongest alignment constraints.
      long double yyalign_me;
      /// A buffer large enough to store any of the semantic values.
      char yyraw[S];
    } yybuffer_;
  };


  /// A Bison parser.
  class BisonGeneratedParser
  {
  public:
#ifndef YYSTYPE
    /// An auxiliary type to compute the largest semantic type.
    union union_type
    {
      // default_value
      char dummy1[sizeof(DefaultValue::WeakPtr)];

      // doc
      char dummy2[sizeof(Doc::WeakPtr)];

      // seqs
      // descriptions
      char dummy3[sizeof(LogicAnd::WeakPtr)];

      // utilities
      // or_exprs
      char dummy4[sizeof(LogicXor::WeakPtr)];

      // single_binding
      char dummy5[sizeof(OptionBinding::WeakPtr)];

      // bindings
      char dummy6[sizeof(OptionBindingContainer::WeakPtr)];

      // usage_section
      // single_utility
      // single_seq
      // atom
      // posix_option_unit
      // gnu_option_unit
      // options_section
      // single_description
      char dummy7[sizeof(WeakPtrNode)];

      // POSIX_OPTION
      // GROUPED_OPTIONS
      // GNU_OPTION
      // ARGUMENT
      // DEFAULT_VALUE
      // COMMAND
      // GENERAL_ELEMENT
      char dummy8[sizeof(std::string)];
};

    /// Symbol semantic values.
    typedef variant<sizeof(union_type)> semantic_type;
#else
    typedef YYSTYPE semantic_type;
#endif

    /// Syntax errors thrown from user actions.
    struct syntax_error : std::runtime_error
    {
      syntax_error (const std::string& m);
    };

    /// Tokens.
    struct token
    {
      enum yytokentype
      {
        END = 0,
        POSIX_OPTION = 258,
        GROUPED_OPTIONS = 259,
        GNU_OPTION = 260,
        ARGUMENT = 261,
        DEFAULT_VALUE = 262,
        COMMAND = 263,
        GENERAL_ELEMENT = 264,
        K_L_PARENTHESIS = 265,
        K_R_PARENTHESIS = 266,
        K_L_BRACKET = 267,
        K_R_BRACKET = 268,
        K_EXCLUSIVE_OR = 269,
        K_ELLIPSES = 270,
        K_USAGE_COLON = 271,
        K_OPTIONS_COLON = 272,
        K_DEFAULT_COLON = 273,
        K_OPTIONS = 274,
        K_UTILITY_DELIMITER = 275,
        K_DESC_DELIMITER = 276,
        K_DOUBLE_HYPHEN = 277,
        K_EQUAL_SIGN = 278
      };
    };

    /// (External) token type, as returned by yylex.
    typedef token::yytokentype token_type;

    /// Internal symbol number.
    typedef int symbol_number_type;

    /// Internal symbol number for tokens (subsumed by symbol_number_type).
    typedef unsigned char token_number_type;

    /// A complete symbol.
    ///
    /// Expects its Base type to provide access to the symbol type
    /// via type_get().
    ///
    /// Provide access to semantic value.
    template <typename Base>
    struct basic_symbol : Base
    {
      /// Alias to Base.
      typedef Base super_type;

      /// Default constructor.
      basic_symbol ();

      /// Copy constructor.
      basic_symbol (const basic_symbol& other);

      /// Constructor for valueless symbols, and symbols from each type.

  basic_symbol (typename Base::kind_type t);

  basic_symbol (typename Base::kind_type t, const DefaultValue::WeakPtr v);

  basic_symbol (typename Base::kind_type t, const Doc::WeakPtr v);

  basic_symbol (typename Base::kind_type t, const LogicAnd::WeakPtr v);

  basic_symbol (typename Base::kind_type t, const LogicXor::WeakPtr v);

  basic_symbol (typename Base::kind_type t, const OptionBinding::WeakPtr v);

  basic_symbol (typename Base::kind_type t, const OptionBindingContainer::WeakPtr v);

  basic_symbol (typename Base::kind_type t, const WeakPtrNode v);

  basic_symbol (typename Base::kind_type t, const std::string v);


      /// Constructor for symbols with semantic value.
      basic_symbol (typename Base::kind_type t,
                    const semantic_type& v);

      ~basic_symbol ();

      /// Destructive move, \a s is emptied into this.
      void move (basic_symbol& s);

      /// The semantic value.
      semantic_type value;

    private:
      /// Assignment operator.
      basic_symbol& operator= (const basic_symbol& other);
    };

    /// Type access provider for token (enum) based symbols.
    struct by_type
    {
      /// Default constructor.
      by_type ();

      /// Copy constructor.
      by_type (const by_type& other);

      /// The symbol type as needed by the constructor.
      typedef token_type kind_type;

      /// Constructor from (external) token numbers.
      by_type (kind_type t);

      /// Steal the symbol type from \a that.
      void move (by_type& that);

      /// The (internal) type number (corresponding to \a type).
      /// -1 when this symbol is empty.
      symbol_number_type type_get () const;

      /// The token.
      token_type token () const;

      enum { empty = 0 };

      /// The symbol type.
      /// -1 when this symbol is empty.
      token_number_type type;
    };

    /// "External" symbols: returned by the scanner.
    typedef basic_symbol<by_type> symbol_type;

    // Symbol constructors declarations.
    static inline
    symbol_type
    make_END ();

    static inline
    symbol_type
    make_POSIX_OPTION (const std::string& v);

    static inline
    symbol_type
    make_GROUPED_OPTIONS (const std::string& v);

    static inline
    symbol_type
    make_GNU_OPTION (const std::string& v);

    static inline
    symbol_type
    make_ARGUMENT (const std::string& v);

    static inline
    symbol_type
    make_DEFAULT_VALUE (const std::string& v);

    static inline
    symbol_type
    make_COMMAND (const std::string& v);

    static inline
    symbol_type
    make_GENERAL_ELEMENT (const std::string& v);

    static inline
    symbol_type
    make_K_L_PARENTHESIS ();

    static inline
    symbol_type
    make_K_R_PARENTHESIS ();

    static inline
    symbol_type
    make_K_L_BRACKET ();

    static inline
    symbol_type
    make_K_R_BRACKET ();

    static inline
    symbol_type
    make_K_EXCLUSIVE_OR ();

    static inline
    symbol_type
    make_K_ELLIPSES ();

    static inline
    symbol_type
    make_K_USAGE_COLON ();

    static inline
    symbol_type
    make_K_OPTIONS_COLON ();

    static inline
    symbol_type
    make_K_DEFAULT_COLON ();

    static inline
    symbol_type
    make_K_OPTIONS ();

    static inline
    symbol_type
    make_K_UTILITY_DELIMITER ();

    static inline
    symbol_type
    make_K_DESC_DELIMITER ();

    static inline
    symbol_type
    make_K_DOUBLE_HYPHEN ();

    static inline
    symbol_type
    make_K_EQUAL_SIGN ();


    /// Build a parser object.
    BisonGeneratedParser (clidoc::FlexGeneratedScanner *lexer_ptr_yyarg, clidoc::Doc::SharedPtr *doc_ptr_yyarg, clidoc::OptionBindingRecorder *option_binding_recorder_ptr_yyarg);
    virtual ~BisonGeneratedParser ();

    /// Parse.
    /// \returns  0 iff parsing succeeded.
    virtual int parse ();

#if YYDEBUG
    /// The current debugging stream.
    std::ostream& debug_stream () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging stream.
    void set_debug_stream (std::ostream &);

    /// Type for debugging levels.
    typedef int debug_level_type;
    /// The current debugging level.
    debug_level_type debug_level () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging level.
    void set_debug_level (debug_level_type l);
#endif

    /// Report a syntax error.
    /// \param msg    a description of the syntax error.
    virtual void error (const std::string& msg);

    /// Report a syntax error.
    void error (const syntax_error& err);

  private:
    /// This class is not copyable.
    BisonGeneratedParser (const BisonGeneratedParser&);
    BisonGeneratedParser& operator= (const BisonGeneratedParser&);

    /// State numbers.
    typedef int state_type;

    /// Generate an error message.
    /// \param yystate   the state where the error occurred.
    /// \param yytoken   the lookahead token type, or yyempty_.
    virtual std::string yysyntax_error_ (state_type yystate,
                                         symbol_number_type yytoken) const;

    /// Compute post-reduction state.
    /// \param yystate   the current state
    /// \param yysym     the nonterminal to push on the stack
    state_type yy_lr_goto_state_ (state_type yystate, int yysym);

    /// Whether the given \c yypact_ value indicates a defaulted state.
    /// \param yyvalue   the value to check
    static bool yy_pact_value_is_default_ (int yyvalue);

    /// Whether the given \c yytable_ value indicates a syntax error.
    /// \param yyvalue   the value to check
    static bool yy_table_value_is_error_ (int yyvalue);

    static const signed char yypact_ninf_;
    static const signed char yytable_ninf_;

    /// Convert a scanner token number \a t to a symbol number.
    static token_number_type yytranslate_ (token_type t);

    // Tables.
  // YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
  // STATE-NUM.
  static const signed char yypact_[];

  // YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
  // Performed when YYTABLE does not specify something else to do.  Zero
  // means the default is an error.
  static const unsigned char yydefact_[];

  // YYPGOTO[NTERM-NUM].
  static const signed char yypgoto_[];

  // YYDEFGOTO[NTERM-NUM].
  static const signed char yydefgoto_[];

  // YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
  // positive, shift that token.  If negative, reduce the rule whose
  // number is the opposite.  If YYTABLE_NINF, syntax error.
  static const unsigned char yytable_[];

  static const signed char yycheck_[];

  // YYSTOS[STATE-NUM] -- The (internal number of the) accessing
  // symbol of state STATE-NUM.
  static const unsigned char yystos_[];

  // YYR1[YYN] -- Symbol number of symbol that rule YYN derives.
  static const unsigned char yyr1_[];

  // YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.
  static const unsigned char yyr2_[];


#if YYDEBUG
    /// For a symbol, its name in clear.
    static const char* const yytname_[];

  // YYRLINE[YYN] -- Source line where rule number YYN was defined.
  static const unsigned short int yyrline_[];
    /// Report on the debug stream that the rule \a r is going to be reduced.
    virtual void yy_reduce_print_ (int r);
    /// Print the state stack on the debug stream.
    virtual void yystack_print_ ();

    // Debugging.
    int yydebug_;
    std::ostream* yycdebug_;

    /// \brief Display a symbol type, value and location.
    /// \param yyo    The output stream.
    /// \param yysym  The symbol.
    template <typename Base>
    void yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const;
#endif

    /// \brief Reclaim the memory associated to a symbol.
    /// \param yymsg     Why this token is reclaimed.
    ///                  If null, print nothing.
    /// \param yysym     The symbol.
    template <typename Base>
    void yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const;

  private:
    /// Type access provider for state based symbols.
    struct by_state
    {
      /// Default constructor.
      by_state ();

      /// The symbol type as needed by the constructor.
      typedef state_type kind_type;

      /// Constructor.
      by_state (kind_type s);

      /// Copy constructor.
      by_state (const by_state& other);

      /// Steal the symbol type from \a that.
      void move (by_state& that);

      /// The (internal) type number (corresponding to \a state).
      /// "empty" when empty.
      symbol_number_type type_get () const;

      enum { empty = 0 };

      /// The state.
      state_type state;
    };

    /// "Internal" symbol: element of the stack.
    struct stack_symbol_type : basic_symbol<by_state>
    {
      /// Superclass.
      typedef basic_symbol<by_state> super_type;
      /// Construct an empty symbol.
      stack_symbol_type ();
      /// Steal the contents from \a sym to build this.
      stack_symbol_type (state_type s, symbol_type& sym);
      /// Assignment, needed by push_back.
      stack_symbol_type& operator= (const stack_symbol_type& that);
    };

    /// Stack type.
    typedef stack<stack_symbol_type> stack_type;

    /// The stack.
    stack_type yystack_;

    /// Push a new state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the symbol
    /// \warning the contents of \a s.value is stolen.
    void yypush_ (const char* m, stack_symbol_type& s);

    /// Push a new look ahead token on the state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the state
    /// \param sym  the symbol (for its value and location).
    /// \warning the contents of \a s.value is stolen.
    void yypush_ (const char* m, state_type s, symbol_type& sym);

    /// Pop \a n symbols the three stacks.
    void yypop_ (unsigned int n = 1);

    // Constants.
    enum
    {
      yyeof_ = 0,
      yylast_ = 46,     ///< Last index in yytable_.
      yynnts_ = 17,  ///< Number of nonterminal symbols.
      yyempty_ = -2,
      yyfinal_ = 7, ///< Termination state number.
      yyterror_ = 1,
      yyerrcode_ = 256,
      yyntokens_ = 24  ///< Number of tokens.
    };


    // User arguments.
    clidoc::FlexGeneratedScanner *lexer_ptr;
    clidoc::Doc::SharedPtr *doc_ptr;
    clidoc::OptionBindingRecorder *option_binding_recorder_ptr;
  };

  // Symbol number corresponding to token number t.
  inline
  BisonGeneratedParser::token_number_type
  BisonGeneratedParser::yytranslate_ (token_type t)
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
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23
    };
    const unsigned int user_token_number_max_ = 278;
    const token_number_type undef_token_ = 2;

    if (static_cast<int>(t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned int> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }

  inline
  BisonGeneratedParser::syntax_error::syntax_error (const std::string& m)
    : std::runtime_error (m)
  {}

  // basic_symbol.
  template <typename Base>
  inline
  BisonGeneratedParser::basic_symbol<Base>::basic_symbol ()
    : value ()
  {}

  template <typename Base>
  inline
  BisonGeneratedParser::basic_symbol<Base>::basic_symbol (const basic_symbol& other)
    : Base (other)
    , value ()
  {
      switch (other.type_get ())
    {
      case 38: // default_value
        value.copy< DefaultValue::WeakPtr > (other.value);
        break;

      case 25: // doc
        value.copy< Doc::WeakPtr > (other.value);
        break;

      case 30: // seqs
      case 36: // descriptions
        value.copy< LogicAnd::WeakPtr > (other.value);
        break;

      case 27: // utilities
      case 29: // or_exprs
        value.copy< LogicXor::WeakPtr > (other.value);
        break;

      case 40: // single_binding
        value.copy< OptionBinding::WeakPtr > (other.value);
        break;

      case 39: // bindings
        value.copy< OptionBindingContainer::WeakPtr > (other.value);
        break;

      case 26: // usage_section
      case 28: // single_utility
      case 31: // single_seq
      case 32: // atom
      case 33: // posix_option_unit
      case 34: // gnu_option_unit
      case 35: // options_section
      case 37: // single_description
        value.copy< WeakPtrNode > (other.value);
        break;

      case 3: // POSIX_OPTION
      case 4: // GROUPED_OPTIONS
      case 5: // GNU_OPTION
      case 6: // ARGUMENT
      case 7: // DEFAULT_VALUE
      case 8: // COMMAND
      case 9: // GENERAL_ELEMENT
        value.copy< std::string > (other.value);
        break;

      default:
        break;
    }

  }


  template <typename Base>
  inline
  BisonGeneratedParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const semantic_type& v)
    : Base (t)
    , value ()
  {
    (void) v;
      switch (this->type_get ())
    {
      case 38: // default_value
        value.copy< DefaultValue::WeakPtr > (v);
        break;

      case 25: // doc
        value.copy< Doc::WeakPtr > (v);
        break;

      case 30: // seqs
      case 36: // descriptions
        value.copy< LogicAnd::WeakPtr > (v);
        break;

      case 27: // utilities
      case 29: // or_exprs
        value.copy< LogicXor::WeakPtr > (v);
        break;

      case 40: // single_binding
        value.copy< OptionBinding::WeakPtr > (v);
        break;

      case 39: // bindings
        value.copy< OptionBindingContainer::WeakPtr > (v);
        break;

      case 26: // usage_section
      case 28: // single_utility
      case 31: // single_seq
      case 32: // atom
      case 33: // posix_option_unit
      case 34: // gnu_option_unit
      case 35: // options_section
      case 37: // single_description
        value.copy< WeakPtrNode > (v);
        break;

      case 3: // POSIX_OPTION
      case 4: // GROUPED_OPTIONS
      case 5: // GNU_OPTION
      case 6: // ARGUMENT
      case 7: // DEFAULT_VALUE
      case 8: // COMMAND
      case 9: // GENERAL_ELEMENT
        value.copy< std::string > (v);
        break;

      default:
        break;
    }
}


  // Implementation of basic_symbol constructor for each type.

  template <typename Base>
  BisonGeneratedParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t)
    : Base (t)
    , value ()
  {}

  template <typename Base>
  BisonGeneratedParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const DefaultValue::WeakPtr v)
    : Base (t)
    , value (v)
  {}

  template <typename Base>
  BisonGeneratedParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const Doc::WeakPtr v)
    : Base (t)
    , value (v)
  {}

  template <typename Base>
  BisonGeneratedParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const LogicAnd::WeakPtr v)
    : Base (t)
    , value (v)
  {}

  template <typename Base>
  BisonGeneratedParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const LogicXor::WeakPtr v)
    : Base (t)
    , value (v)
  {}

  template <typename Base>
  BisonGeneratedParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const OptionBinding::WeakPtr v)
    : Base (t)
    , value (v)
  {}

  template <typename Base>
  BisonGeneratedParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const OptionBindingContainer::WeakPtr v)
    : Base (t)
    , value (v)
  {}

  template <typename Base>
  BisonGeneratedParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const WeakPtrNode v)
    : Base (t)
    , value (v)
  {}

  template <typename Base>
  BisonGeneratedParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::string v)
    : Base (t)
    , value (v)
  {}


  template <typename Base>
  inline
  BisonGeneratedParser::basic_symbol<Base>::~basic_symbol ()
  {
    // User destructor.
    symbol_number_type yytype = this->type_get ();
    switch (yytype)
    {
   default:
      break;
    }

    // Type destructor.
    switch (yytype)
    {
      case 38: // default_value
        value.template destroy< DefaultValue::WeakPtr > ();
        break;

      case 25: // doc
        value.template destroy< Doc::WeakPtr > ();
        break;

      case 30: // seqs
      case 36: // descriptions
        value.template destroy< LogicAnd::WeakPtr > ();
        break;

      case 27: // utilities
      case 29: // or_exprs
        value.template destroy< LogicXor::WeakPtr > ();
        break;

      case 40: // single_binding
        value.template destroy< OptionBinding::WeakPtr > ();
        break;

      case 39: // bindings
        value.template destroy< OptionBindingContainer::WeakPtr > ();
        break;

      case 26: // usage_section
      case 28: // single_utility
      case 31: // single_seq
      case 32: // atom
      case 33: // posix_option_unit
      case 34: // gnu_option_unit
      case 35: // options_section
      case 37: // single_description
        value.template destroy< WeakPtrNode > ();
        break;

      case 3: // POSIX_OPTION
      case 4: // GROUPED_OPTIONS
      case 5: // GNU_OPTION
      case 6: // ARGUMENT
      case 7: // DEFAULT_VALUE
      case 8: // COMMAND
      case 9: // GENERAL_ELEMENT
        value.template destroy< std::string > ();
        break;

      default:
        break;
    }

  }

  template <typename Base>
  inline
  void
  BisonGeneratedParser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move(s);
      switch (this->type_get ())
    {
      case 38: // default_value
        value.move< DefaultValue::WeakPtr > (s.value);
        break;

      case 25: // doc
        value.move< Doc::WeakPtr > (s.value);
        break;

      case 30: // seqs
      case 36: // descriptions
        value.move< LogicAnd::WeakPtr > (s.value);
        break;

      case 27: // utilities
      case 29: // or_exprs
        value.move< LogicXor::WeakPtr > (s.value);
        break;

      case 40: // single_binding
        value.move< OptionBinding::WeakPtr > (s.value);
        break;

      case 39: // bindings
        value.move< OptionBindingContainer::WeakPtr > (s.value);
        break;

      case 26: // usage_section
      case 28: // single_utility
      case 31: // single_seq
      case 32: // atom
      case 33: // posix_option_unit
      case 34: // gnu_option_unit
      case 35: // options_section
      case 37: // single_description
        value.move< WeakPtrNode > (s.value);
        break;

      case 3: // POSIX_OPTION
      case 4: // GROUPED_OPTIONS
      case 5: // GNU_OPTION
      case 6: // ARGUMENT
      case 7: // DEFAULT_VALUE
      case 8: // COMMAND
      case 9: // GENERAL_ELEMENT
        value.move< std::string > (s.value);
        break;

      default:
        break;
    }

  }

  // by_type.
  inline
  BisonGeneratedParser::by_type::by_type ()
     : type (empty)
  {}

  inline
  BisonGeneratedParser::by_type::by_type (const by_type& other)
    : type (other.type)
  {}

  inline
  BisonGeneratedParser::by_type::by_type (token_type t)
    : type (yytranslate_ (t))
  {}

  inline
  void
  BisonGeneratedParser::by_type::move (by_type& that)
  {
    type = that.type;
    that.type = empty;
  }

  inline
  int
  BisonGeneratedParser::by_type::type_get () const
  {
    return type;
  }

  inline
  BisonGeneratedParser::token_type
  BisonGeneratedParser::by_type::token () const
  {
    // YYTOKNUM[NUM] -- (External) token number corresponding to the
    // (internal) symbol number NUM (which must be that of a token).  */
    static
    const unsigned short int
    yytoken_number_[] =
    {
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278
    };
    return static_cast<token_type> (yytoken_number_[type]);
  }
  // Implementation of make_symbol for each symbol type.
  BisonGeneratedParser::symbol_type
  BisonGeneratedParser::make_END ()
  {
    return symbol_type (token::END);
  }

  BisonGeneratedParser::symbol_type
  BisonGeneratedParser::make_POSIX_OPTION (const std::string& v)
  {
    return symbol_type (token::POSIX_OPTION, v);
  }

  BisonGeneratedParser::symbol_type
  BisonGeneratedParser::make_GROUPED_OPTIONS (const std::string& v)
  {
    return symbol_type (token::GROUPED_OPTIONS, v);
  }

  BisonGeneratedParser::symbol_type
  BisonGeneratedParser::make_GNU_OPTION (const std::string& v)
  {
    return symbol_type (token::GNU_OPTION, v);
  }

  BisonGeneratedParser::symbol_type
  BisonGeneratedParser::make_ARGUMENT (const std::string& v)
  {
    return symbol_type (token::ARGUMENT, v);
  }

  BisonGeneratedParser::symbol_type
  BisonGeneratedParser::make_DEFAULT_VALUE (const std::string& v)
  {
    return symbol_type (token::DEFAULT_VALUE, v);
  }

  BisonGeneratedParser::symbol_type
  BisonGeneratedParser::make_COMMAND (const std::string& v)
  {
    return symbol_type (token::COMMAND, v);
  }

  BisonGeneratedParser::symbol_type
  BisonGeneratedParser::make_GENERAL_ELEMENT (const std::string& v)
  {
    return symbol_type (token::GENERAL_ELEMENT, v);
  }

  BisonGeneratedParser::symbol_type
  BisonGeneratedParser::make_K_L_PARENTHESIS ()
  {
    return symbol_type (token::K_L_PARENTHESIS);
  }

  BisonGeneratedParser::symbol_type
  BisonGeneratedParser::make_K_R_PARENTHESIS ()
  {
    return symbol_type (token::K_R_PARENTHESIS);
  }

  BisonGeneratedParser::symbol_type
  BisonGeneratedParser::make_K_L_BRACKET ()
  {
    return symbol_type (token::K_L_BRACKET);
  }

  BisonGeneratedParser::symbol_type
  BisonGeneratedParser::make_K_R_BRACKET ()
  {
    return symbol_type (token::K_R_BRACKET);
  }

  BisonGeneratedParser::symbol_type
  BisonGeneratedParser::make_K_EXCLUSIVE_OR ()
  {
    return symbol_type (token::K_EXCLUSIVE_OR);
  }

  BisonGeneratedParser::symbol_type
  BisonGeneratedParser::make_K_ELLIPSES ()
  {
    return symbol_type (token::K_ELLIPSES);
  }

  BisonGeneratedParser::symbol_type
  BisonGeneratedParser::make_K_USAGE_COLON ()
  {
    return symbol_type (token::K_USAGE_COLON);
  }

  BisonGeneratedParser::symbol_type
  BisonGeneratedParser::make_K_OPTIONS_COLON ()
  {
    return symbol_type (token::K_OPTIONS_COLON);
  }

  BisonGeneratedParser::symbol_type
  BisonGeneratedParser::make_K_DEFAULT_COLON ()
  {
    return symbol_type (token::K_DEFAULT_COLON);
  }

  BisonGeneratedParser::symbol_type
  BisonGeneratedParser::make_K_OPTIONS ()
  {
    return symbol_type (token::K_OPTIONS);
  }

  BisonGeneratedParser::symbol_type
  BisonGeneratedParser::make_K_UTILITY_DELIMITER ()
  {
    return symbol_type (token::K_UTILITY_DELIMITER);
  }

  BisonGeneratedParser::symbol_type
  BisonGeneratedParser::make_K_DESC_DELIMITER ()
  {
    return symbol_type (token::K_DESC_DELIMITER);
  }

  BisonGeneratedParser::symbol_type
  BisonGeneratedParser::make_K_DOUBLE_HYPHEN ()
  {
    return symbol_type (token::K_DOUBLE_HYPHEN);
  }

  BisonGeneratedParser::symbol_type
  BisonGeneratedParser::make_K_EQUAL_SIGN ()
  {
    return symbol_type (token::K_EQUAL_SIGN);
  }



} // clidoc
#line 1318 "generated_parser.h" // lalr1.cc:387




#endif // !YY_CLIDOC_GENERATED_PARSER_H_INCLUDED
