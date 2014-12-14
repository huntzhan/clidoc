
#include "test_utils-inl.h"

#define L_PARENTHESIS        Token::TypeID::L_PARENTHESIS
#define R_PARENTHESIS        Token::TypeID::R_PARENTHESIS
#define L_BRACKET            Token::TypeID::L_BRACKET
#define R_BRACKET            Token::TypeID::R_BRACKET
#define EXCLUSIVE_OR         Token::TypeID::EXCLUSIVE_OR
#define ELLIPSES             Token::TypeID::ELLIPSES
#define K_USAGE_COLON        Token::TypeID::K_USAGE_COLON
#define K_OPTIONS_COLON      Token::TypeID::K_OPTIONS_COLON
#define K_DEFAULT_COLON      Token::TypeID::K_DEFAULT_COLON
#define K_OPTIONS            Token::TypeID::K_OPTIONS
#define K_UTILITY_DELIMITER  Token::TypeID::K_UTILITY_DELIMITER
#define K_DESC_DELIMITER     Token::TypeID::K_DESC_DELIMITER
#define EQUAL_SIGN           Token::TypeID::EQUAL_SIGN
#define POSIX_OPTION         Token::TypeID::POSIX_OPTION
#define GROUPED_OPTIONS      Token::TypeID::GROUPED_OPTIONS
#define GNU_OPTION           Token::TypeID::GNU_OPTION
#define OPTION_ARGUEMENT     Token::TypeID::OPTION_ARGUEMENT
#define OPERAND              Token::TypeID::OPERAND
#define OPTION_DEFAULT_VALUE Token::TypeID::OPTION_DEFAULT_VALUE
#define COMMENT              Token::TypeID::COMMENT


TEST(tokenizer_for_doc, token_type) {
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
      "-c --long <arg> ARG operand", 
      {POSIX_OPTION, GNU_OPTION,
       OPTION_ARGUEMENT, OPTION_ARGUEMENT,
       OPERAND});
  CheckTokenTypes(
      "\"1.414 whatever\" # long long comment", 
      {OPTION_DEFAULT_VALUE, COMMENT});
}

TEST(tokenizer_for_doc, token_value) {
  CheckTokenValues(
      "-c  - --long ",
      {"-c", "-", "--long"});
  CheckTokenValues(
      "-c FILE",
      {"-c", "FILE"});
}
