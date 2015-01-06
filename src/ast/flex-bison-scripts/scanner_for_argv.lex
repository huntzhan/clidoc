%{
// Use parser defined tokens.
#include "ast/generated_scanner.h"
#include "ast/generated_parser.h"

#define YY_DECL                             \
  clidoc::BisonGeneratedParser::symbol_type \
  clidoc::FlexGeneratedScanner::lex()
%}

%option c++
%option noyywrap nounput batch debug noinput
%option outfile="generated_scanner_for_argv.cc"

DIGIT                [0-9]
LOWERCASE            [a-z]
UPPERCASE            [A-Z]
NON_SPACE            [^ \t\r\n\v\f]
HYPHEN               "-"
LOWERCASE_DIGIT      {LOWERCASE}|{DIGIT}
UPPERCASE_DIGIT      {UPPERCASE}|{DIGIT}
ALNUM                {LOWERCASE}|{UPPERCASE}|{DIGIT}

K_DOUBLE_HYPHEN      "--"
K_EQUAL_SIGN         "="

POSIX_OPTION         {HYPHEN}{ALNUM}
GROUPED_OPTIONS      {HYPHEN}{ALNUM}{2,}
GNU_OPTION           {HYPHEN}{2}{ALNUM}({ALNUM}|{HYPHEN})+

GENERAL_ELEMENT      {NON_SPACE}+

%%

{K_DOUBLE_HYPHEN} return clidoc::BisonGeneratedParser::make_K_DOUBLE_HYPHEN();
{K_EQUAL_SIGN}    return clidoc::BisonGeneratedParser::make_K_EQUAL_SIGN();

{POSIX_OPTION}    return clidoc::BisonGeneratedParser::make_POSIX_OPTION(YYText());
{GROUPED_OPTIONS} return clidoc::BisonGeneratedParser::make_GROUPED_OPTIONS(YYText());
{GNU_OPTION}      return clidoc::BisonGeneratedParser::make_GNU_OPTION(YYText());
{GENERAL_ELEMENT} return clidoc::BisonGeneratedParser::make_GENERAL_ELEMENT(YYText());

<<EOF>>           return clidoc::BisonGeneratedParser::make_END();

%%
// Bug fix.
int yyFlexLexer::yylex() { return 1; }
