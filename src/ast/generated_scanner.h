#ifndef SRC_AST_GENERATED_SCANNER_H_
#define SRC_AST_GENERATED_SCANNER_H_

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
  FlexGeneratedScanner(std::istream *arg_yyin, std::ostream *arg_yyout);
  BisonGeneratedParser::symbol_type lex();
};

inline FlexGeneratedScanner::FlexGeneratedScanner(
    std::istream *arg_yyin, std::ostream *arg_yyout)
    : yyFlexLexer(arg_yyin, arg_yyout) { /* empty */ }

}  // namespace clidoc

#endif  // SRC_AST_GENERATED_SCANNER_H_
