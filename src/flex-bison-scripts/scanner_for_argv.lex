%{
// Use parser defined tokens.
#include "generated_scanner.h"
#include "generated_parser.h"

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
LOWERCASE_DIGIT      {LOWERCASE}|{DIGIT}
UPPERCASE_DIGIT      {UPPERCASE}|{DIGIT}
ALNUM                {LOWERCASE}|{UPPERCASE}|{DIGIT}
NON_SPACE            [^ \t\r\n\v\f]

K_DOUBLE_HYPHEN      "--"
EQUAL_SIGN           "="

POSIX_OPTION         -{ALNUM}
GROUPED_OPTIONS      -{ALNUM}+
GNU_OPTION           --{ALNUM}+
ARGUMENT             {NON_SPACE}+

%%

{K_DOUBLE_HYPHEN}      return clidoc::BisonGeneratedParser::make_K_DOUBLE_HYPHEN();
{EQUAL_SIGN}           return clidoc::BisonGeneratedParser::make_EQUAL_SIGN();

{POSIX_OPTION}         return clidoc::BisonGeneratedParser::make_POSIX_OPTION(YYText());
{GROUPED_OPTIONS}      return clidoc::BisonGeneratedParser::make_GROUPED_OPTIONS(YYText());
{GNU_OPTION}           return clidoc::BisonGeneratedParser::make_GNU_OPTION(YYText());
{ARGUMENT}             return clidoc::BisonGeneratedParser::make_ARGUMENT(YYText());

<<EOF>>                return clidoc::BisonGeneratedParser::make_END();

%%
// Bug fix.
int yyFlexLexer::yylex() { return 1; }
