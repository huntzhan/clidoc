
#include <vector>
#include <vector>
#include "gtest/gtest.h"
#include "utils.h"
#include "tokenizer.h"

using namespace clidoc;
using std::string;
using std::vector;

#define L_PARENTHESIS        Token::TypeID::L_PARENTHESIS
#define R_PARENTHESIS        Token::TypeID::R_PARENTHESIS
#define L_BRACKET            Token::TypeID::L_BRACKET
#define R_BRACKET            Token::TypeID::R_BRACKET
#define EXCLUSIVE_OR         Token::TypeID::EXCLUSIVE_OR
#define EQUAL_SIGN           Token::TypeID::EQUAL_SIGN
#define ELLIPSES             Token::TypeID::ELLIPSES
#define K_USAGE_COLON        Token::TypeID::K_USAGE_COLON
#define K_OPTIONS_COLON      Token::TypeID::K_OPTIONS_COLON
#define K_DEFAULT_COLON      Token::TypeID::K_DEFAULT_COLON
#define K_OPTIONS            Token::TypeID::K_OPTIONS
#define K_UTILITY_DELIMITER  Token::TypeID::K_UTILITY_DELIMITER
#define K_DESC_DELIMITER     Token::TypeID::K_DESC_DELIMITER
#define POSIX_OPTION         Token::TypeID::POSIX_OPTION
#define GROUPED_OPTIONS      Token::TypeID::GROUPED_OPTIONS
#define GNU_OPTION           Token::TypeID::GNU_OPTION
#define OPTION_ARGUEMENT     Token::TypeID::OPTION_ARGUEMENT
#define OPERAND              Token::TypeID::OPERAND
#define OPTION_DEFAULT_VALUE Token::TypeID::OPTION_DEFAULT_VALUE
#define COMMENT              Token::TypeID::COMMENT

vector<Token::Type> ExtractTokenType(const vector<Token> &tokens) {
  vector<Token::Type> type_ids;
  for (const Token &token : tokens) {
    type_ids.push_back(token.type_id());
  }
  return type_ids;
}

void CheckTokenTypes(const string &text,
                     const vector<Token::Type> &type_ids) {
  auto tokens = Tokenizer::FromString(text);
  EXPECT_EQ(type_ids, ExtractTokenType(tokens));
}

TEST(tokenizer, token_type) {
  CheckTokenTypes(
      "( ) [ ] | = ...", 
      {L_PARENTHESIS, R_PARENTHESIS, L_BRACKET, R_BRACKET, EXCLUSIVE_OR,
       EQUAL_SIGN, ELLIPSES});
  CheckTokenTypes(
      "usage: UsAge: options: options OPTIONS: default:", 
      {K_USAGE_COLON, K_USAGE_COLON,
       K_OPTIONS_COLON, K_OPTIONS, K_OPTIONS_COLON,
       K_DEFAULT_COLON});
  CheckTokenTypes(
      "*UTILITY_DELIMITER* *DESC_DELIMITER*", 
      {K_UTILITY_DELIMITER, K_DESC_DELIMITER});
  CheckTokenTypes(
      "-c --long <arg> ARG operand \"1.414 whatever\" #comment ", 
      {POSIX_OPTION, GNU_OPTION,
       OPTION_ARGUEMENT, OPTION_ARGUEMENT,
       OPERAND, OPTION_DEFAULT_VALUE, COMMENT});
}
