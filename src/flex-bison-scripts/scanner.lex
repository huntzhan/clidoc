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

POSIX_OPTION         -{ALNUM}
GROUPED_OPTIONS      -{ALNUM}+
GNU_OPTION           --{ALNUM}+
ARGUEMENT            (<{LOWERCASE_DIGIT}+>)|({UPPERCASE_DIGIT}+)
OPERAND              {ALNUM}+
OPTION_DEFAULT_VALUE \"{ALNUM}+\"
COMMENT              #.*

%%

{L_PARENTHESIS}        return yy::BisonGeneratedParser::make_L_PARENTHESIS();
{R_PARENTHESIS}        return yy::BisonGeneratedParser::make_R_PARENTHESIS();
{L_BRACKET}            return yy::BisonGeneratedParser::make_L_BRACKET();
{R_BRACKET}            return yy::BisonGeneratedParser::make_R_BRACKET();
{EXCLUSIVE_OR}         return yy::BisonGeneratedParser::make_EXCLUSIVE_OR();
{EQUAL_SIGN}           return yy::BisonGeneratedParser::make_EQUAL_SIGN();
{ELLIPSES}             return yy::BisonGeneratedParser::make_ELLIPSES();
{K_USAGE_COLON}        return yy::BisonGeneratedParser::make_K_USAGE_COLON();
{K_OPTIONS_COLON}      return yy::BisonGeneratedParser::make_K_OPTIONS_COLON();
{K_DEFAULT_COLON}      return yy::BisonGeneratedParser::make_K_DEFAULT_COLON();
{K_OPTIONS}            return yy::BisonGeneratedParser::make_K_OPTIONS();
{K_UTILITY_DELIMITER}  return yy::BisonGeneratedParser::make_K_UTILITY_DELIMITER();
{K_DESC_DELIMITER}     return yy::BisonGeneratedParser::make_K_DESC_DELIMITER();

{POSIX_OPTION}         return yy::BisonGeneratedParser::make_POSIX_OPTION(YYText());
{GROUPED_OPTIONS}      return yy::BisonGeneratedParser::make_GROUPED_OPTIONS(YYText());
{GNU_OPTION}           return yy::BisonGeneratedParser::make_GNU_OPTION(YYText());
{ARGUEMENT}            return yy::BisonGeneratedParser::make_ARGUEMENT(YYText());
{OPERAND}              return yy::BisonGeneratedParser::make_OPERAND(YYText());
{OPTION_DEFAULT_VALUE} return yy::BisonGeneratedParser::make_OPTION_DEFAULT_VALUE(YYText());
{COMMENT}              return yy::BisonGeneratedParser::make_COMMENT(YYText());

<<EOF>>                return yy::BisonGeneratedParser::make_END();

%%
// Bug fix.
int yyFlexLexer::yylex() {/* empty */ }
