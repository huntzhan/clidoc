#ifndef SRC_GENERATED_SCANNER_H_
#define SRC_GENERATED_SCANNER_H_

#include <istream>
#include <ostream>

// Macro `#if defined(yyFlexLexer) || ! defined(yyFlexLexerOnce)` in
// `FlexLexer.h` cause the definition of class yyFlexLexer being
// included more than once, since the flex generated file would always define
// the variable `yyFlexLexer`. Following macro could this behavior.
#ifndef __FLEX_LEXER_H
#include "FlexLexer.h"
#endif

#include "ast/generated_parser.h"

namespace clidoc {

class FlexGeneratedScanner : public yyFlexLexer {
 public:
  FlexGeneratedScanner(std::istream *arg_yyin = 0,
                       std::ostream *arg_yyout = 0)
      : yyFlexLexer(arg_yyin, arg_yyout) { /* empty */ }

  BisonGeneratedParser::symbol_type lex();
};

}  // namespace clidoc

#endif  // SRC_GENERATED_SCANNER_H_
