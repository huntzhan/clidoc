#ifndef INCLUDE_CLIDOC_AST_GENERATED_SCANNER_H_
#define INCLUDE_CLIDOC_AST_GENERATED_SCANNER_H_

#include <istream>
#include <ostream>

// Macro `#if defined(yyFlexLexer) || ! defined(yyFlexLexerOnce)` in
// `FlexLexer.h` cause the definition of class yyFlexLexer being
// included more than once, since the flex generated file would always define
// the variable `yyFlexLexer`. Following macro could this behavior.
#ifndef __FLEX_LEXER_H
#undef yyFlexLexer
#define yyFlexLexer argvFlexLexer
#include "FlexLexer.h"
#endif

#include "clidoc/ast/generated_parser.h"

namespace clidoc {

class FlexGeneratedScannerForArgv : public yyFlexLexer {
 public:
  FlexGeneratedScannerForArgv(std::istream *arg_yyin, std::ostream *arg_yyout);
  BisonGeneratedParser::symbol_type lex();
};

inline FlexGeneratedScannerForArgv::FlexGeneratedScannerForArgv(
    std::istream *arg_yyin, std::ostream *arg_yyout)
    : yyFlexLexer(arg_yyin, arg_yyout) { /* empty */ }

}  // namespace clidoc

#endif  // INCLUDE_CLIDOC_AST_GENERATED_SCANNER_H_
