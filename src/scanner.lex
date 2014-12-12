%{
// Use parser defined tokens.
#include "generated_scanner.h"
// `generated_parser.h` must be included after `generated_scanner.h`.
#include "generated_parser.h"

#define	YY_DECL                         \
  yy::BisonGeneratedParser::symbol_type \
  FlexGeneratedScanner::lex()
%}

%option c++
%option noyywrap nounput batch debug noinput
%option outfile="generated_scanner.cc"

DIGIT                [0-9]
LOWERCASE            [a-z]
UPPERCASE            [A-Z]
LOWERCASE_DIGIT      {LOWERCASE}|{DIGIT}
UPPERCASE_DIGIT      {UPPERCASE}|{DIGIT}
ALNUM                {LOWERCASE}|{UPPERCASE}|{DIGIT}

POSIX_OPTION         -{ALNUM}
GROUPED_OPTIONS      -{ALNUM}+
GNU_OPTION           --{ALNUM}+
ARGUEMENT            (<{LOWERCASE_DIGIT}+>)|({UPPERCASE_DIGIT}+)
OPERAND              {ALNUM}+
OPTION_DEFAULT_VALUE "{ALNUM}+"
COMMENT              #.*

L_PARENTHESIS        \(
R_PARENTHESIS        \)
L_BRACKET            \[
R_BRACKET            \]
EXCLUSIVE_OR         \|
EQUAL_SIGN           =
ELLIPSES             \.\.\.
K_USAGE_COLON        usage:
K_OPTIONS_COLON      options:
K_DEFAULT_COLON      default:
K_OPTIONS            options
K_UTILITY_DELIMITER  \*UTILITY_DELIMITER\*
K_DESC_DELIMITER     \*DESC_DELIMITER\*

%%

{POSIX_OPTION} return yy::BisonGeneratedParser::make_POSIX_OPTION(YYText());

<<EOF>> return yy::BisonGeneratedParser::make_END();

%%
// Bug fix.
int yyFlexLexer::yylex() {/* empty */ }
