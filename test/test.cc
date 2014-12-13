
#include <vector>
#include <vector>
#include "gtest/gtest.h"
#include "utils.h"
#include "tokenizer.h"
#include "generated_parser.h"

using namespace clidoc;
using std::string;
using std::vector;
using token_type = BisonGeneratedParser::token_type;

#define L_PARENTHESIS        BisonGeneratedParser::token::L_PARENTHESIS
#define R_PARENTHESIS        BisonGeneratedParser::token::R_PARENTHESIS
#define L_BRACKET            BisonGeneratedParser::token::L_BRACKET
#define R_BRACKET            BisonGeneratedParser::token::R_BRACKET
#define EXCLUSIVE_OR         BisonGeneratedParser::token::EXCLUSIVE_OR
#define EQUAL_SIGN           BisonGeneratedParser::token::EQUAL_SIGN
#define ELLIPSES             BisonGeneratedParser::token::ELLIPSES
#define K_USAGE_COLON        BisonGeneratedParser::token::K_USAGE_COLON
#define K_OPTIONS_COLON      BisonGeneratedParser::token::K_OPTIONS_COLON
#define K_DEFAULT_COLON      BisonGeneratedParser::token::K_DEFAULT_COLON
#define K_OPTIONS            BisonGeneratedParser::token::K_OPTIONS
#define K_UTILITY_DELIMITER  BisonGeneratedParser::token::K_UTILITY_DELIMITER
#define K_DESC_DELIMITER     BisonGeneratedParser::token::K_DESC_DELIMITER
#define POSIX_OPTION         BisonGeneratedParser::token::POSIX_OPTION
#define GROUPED_OPTIONS      BisonGeneratedParser::token::GROUPED_OPTIONS
#define GNU_OPTION           BisonGeneratedParser::token::GNU_OPTION
#define ARGUEMENT            BisonGeneratedParser::token::ARGUEMENT
#define OPERAND              BisonGeneratedParser::token::OPERAND
#define OPTION_DEFAULT_VALUE BisonGeneratedParser::token::OPTION_DEFAULT_VALUE
#define COMMENT              BisonGeneratedParser::token::COMMENT

vector<token_type> Extractoken_typeokenType(const vector<Token> &tokens) {
  vector<token_type> types;
  for (const Token &token : tokens) {
    types.push_back(token.type());
  }
  return types;
}

void CheckTokenTypes(const string &text,
                     const vector<token_type> &token_types) {
  auto tokens = Tokenizer::FromString(text);
  EXPECT_EQ(token_types, Extractoken_typeokenType(tokens));
}

TEST(tokenizer, simple_cases) {
  CheckTokenTypes(
      "-c --long <arg> ARG", 
      {POSIX_OPTION, GNU_OPTION, ARGUEMENT, ARGUEMENT});
  CheckTokenTypes(
      "( ) [ ] | = ...", 
      {L_PARENTHESIS, R_PARENTHESIS, L_BRACKET, R_BRACKET, EXCLUSIVE_OR,
       EQUAL_SIGN, ELLIPSES});
  CheckTokenTypes(
      "usage: UsAge: options: options OPTIONS: default:", 
      {K_USAGE_COLON, K_USAGE_COLON,
       K_OPTIONS_COLON, K_OPTIONS, K_OPTIONS_COLON,
       K_DEFAULT_COLON});
}
