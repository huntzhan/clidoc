%{
// `generated_parser.h` use macro YY_NULLPTR, which is not defined.
# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

// Use parser defined tokens.
#include "generated_parser.h"
#include "generated_scanner.h"

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

L_PARENTHESIS        (
R_PARENTHESIS        )
L_BRACKET            [
R_BRACKET            ]
EXCLUSIVE_OR         |
EQUAL_SIGN           =
ELLIPSES             \.\.\.
K_USAGE_COLON        usage:
K_OPTIONS_COLON      options:
K_DEFAULT_COLON      default:
K_OPTIONS            options
K_UTILITY_DELIMITER  \*UTILITY_DELIMITER\*
K_DESC_DELIMITER     \*DESC_DELIMITER\*

%%

<<EOF>> {  }
