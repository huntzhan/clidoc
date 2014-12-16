
#include "test_utils-inl.h"

#define OPTION_ARGUEMENT     TerminalType::OPTION_ARGUEMENT
#define GNU_OPTION           TerminalType::GNU_OPTION
#define OPERAND              TerminalType::OPERAND
#define COMMENT              TerminalType::COMMENT
#define OPTION_DEFAULT_VALUE TerminalType::OPTION_DEFAULT_VALUE
#define POSIX_OPTION         TerminalType::POSIX_OPTION
#define GROUPED_OPTIONS      TerminalType::GROUPED_OPTIONS

TEST(tokenizer_for_doc, token_type) {
  CheckTokenTypes(
      "-c --long -afciFEj <arg> ARG operand", 
      {POSIX_OPTION, GNU_OPTION, GROUPED_OPTIONS,
       OPTION_ARGUEMENT, OPTION_ARGUEMENT,
       OPERAND});
  CheckTokenTypes(
      "\"1.414 whatever\" # long long comment", 
      {OPTION_DEFAULT_VALUE, COMMENT});
}

/*
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
}

TEST(tokenizer_for_doc, token_value) {
  CheckTokenValues(
      "-c  - --long ",
      {"-c", "-", "--long"});
  CheckTokenValues(
      "-c FILE",
      {"-c", "FILE"});
}
*/
