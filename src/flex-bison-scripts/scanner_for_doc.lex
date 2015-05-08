%{
// Use parser defined tokens.
#include "clidoc/ast/generated_scanner.h"
#include "clidoc/ast/generated_parser.h"

#define YY_DECL                             \
  clidoc::BisonGeneratedParser::symbol_type \
  clidoc::FlexGeneratedScanner::lex()
%}

%option c++
%option noyywrap nounput batch debug noinput
%option outfile="generated_scanner_for_doc.cc"

DIGIT                [0-9]
LOWERCASE            [a-z]
UPPERCASE            [A-Z]
NON_SPACE            [^ \t\r\n\v\f]
HYPHEN               "-"
LOWERCASE_DIGIT      {LOWERCASE}|{DIGIT}
UPPERCASE_DIGIT      {UPPERCASE}|{DIGIT}
ALNUM                {LOWERCASE}|{UPPERCASE}|{DIGIT}

K_L_PARENTHESIS      "("
K_R_PARENTHESIS      ")"
K_L_BRACKET          "["
K_R_BRACKET          "]"
K_EXCLUSIVE_OR       "|"
K_ELLIPSES           "..."
K_USAGE_COLON        (?i:usage:)
K_OPTIONS_COLON      (?i:options:)
K_DEFAULT_COLON      (?i:default:)
K_OPTIONS            "options"
K_UTILITY_DELIMITER  "*UTILITY_DELIMITER*"
K_DESC_DELIMITER     "*DESC_DELIMITER*"

K_DOUBLE_HYPHEN      "--"
K_EQUAL_SIGN         "="
K_COMMA              ","

POSIX_OPTION         {HYPHEN}{ALNUM}
GROUPED_OPTIONS      {HYPHEN}{ALNUM}{2,}
GNU_OPTION           {HYPHEN}{2}{ALNUM}({ALNUM}|{HYPHEN})+

ARGUMENT_FORM_ONE    <[^>\n]+>
ARGUMENT_FORM_TWO    {UPPERCASE_DIGIT}({UPPERCASE_DIGIT}|{HYPHEN})*
ARGUMENT             {ARGUMENT_FORM_ONE}|{ARGUMENT_FORM_TWO}

DEFAULT_VALUE        \"[^\n\"]*\"

COMMAND              {NON_SPACE}+

%%
{K_L_PARENTHESIS}     return clidoc::BisonGeneratedParser::make_K_L_PARENTHESIS();
{K_R_PARENTHESIS}     return clidoc::BisonGeneratedParser::make_K_R_PARENTHESIS();
{K_L_BRACKET}         return clidoc::BisonGeneratedParser::make_K_L_BRACKET();
{K_R_BRACKET}         return clidoc::BisonGeneratedParser::make_K_R_BRACKET();
{K_EXCLUSIVE_OR}      return clidoc::BisonGeneratedParser::make_K_EXCLUSIVE_OR();
{K_ELLIPSES}          return clidoc::BisonGeneratedParser::make_K_ELLIPSES();
{K_USAGE_COLON}       return clidoc::BisonGeneratedParser::make_K_USAGE_COLON();
{K_OPTIONS_COLON}     return clidoc::BisonGeneratedParser::make_K_OPTIONS_COLON();
{K_DEFAULT_COLON}     return clidoc::BisonGeneratedParser::make_K_DEFAULT_COLON();
{K_OPTIONS}           return clidoc::BisonGeneratedParser::make_K_OPTIONS();
{K_UTILITY_DELIMITER} return clidoc::BisonGeneratedParser::make_K_UTILITY_DELIMITER();
{K_DESC_DELIMITER}    return clidoc::BisonGeneratedParser::make_K_DESC_DELIMITER();

{K_DOUBLE_HYPHEN}     return clidoc::BisonGeneratedParser::make_K_DOUBLE_HYPHEN();
{K_EQUAL_SIGN}        return clidoc::BisonGeneratedParser::make_K_EQUAL_SIGN();
{K_COMMA}             { /* empty */ }

{POSIX_OPTION}        return clidoc::BisonGeneratedParser::make_POSIX_OPTION(YYText());
{GROUPED_OPTIONS}     return clidoc::BisonGeneratedParser::make_GROUPED_OPTIONS(YYText());
{GNU_OPTION}          return clidoc::BisonGeneratedParser::make_GNU_OPTION(YYText());
{ARGUMENT}            return clidoc::BisonGeneratedParser::make_ARGUMENT(YYText());
{DEFAULT_VALUE}       return clidoc::BisonGeneratedParser::make_DEFAULT_VALUE(YYText());
{COMMAND}             return clidoc::BisonGeneratedParser::make_COMMAND(YYText());

<<EOF>>               return clidoc::BisonGeneratedParser::make_END();

%%
// Bug fix.
int yyFlexLexer::yylex() { return 1; }
