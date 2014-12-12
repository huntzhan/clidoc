#ifndef SCANNER_H_
#define SCANNER_H_

#include "FlexLexer.h"
#include "generated_parser.hh"

class FlexGeneratedScanner : public yyFlexLexer {
 public:
  yy::BisonGeneratedParser::symbol_type lex();
};

#endif // SCANNER_H_
