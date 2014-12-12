#ifndef SCANNER_H_
#define SCANNER_H_

// Macro `#if defined(yyFlexLexer) || ! defined(yyFlexLexerOnce)` in
// `FlexLexer.h` would cause the the definition of class yyFlexLexer being
// included more than once, since the flex generated file would always define
// `yyFlexLexer`. Following macro could this behavior.
#ifndef __FLEX_LEXER_H
#include "FlexLexer.h"
#endif

#include "generated_parser.h"

class FlexGeneratedScanner : public yyFlexLexer {
 public:
  yy::BisonGeneratedParser::symbol_type lex();
};

#endif // SCANNER_H_
