
#include "test_utils-inl.h"


TEST(tokenizer_for_doc, token_type) {
  CheckTokenTypes(
      "-c --long -afciFEj <arg> ARG operand", {
        TerminalType::POSIX_OPTION,
        TerminalType::GNU_OPTION,
        TerminalType::GROUPED_OPTIONS,
        TerminalType::OPTION_ARGUEMENT,
        TerminalType::OPTION_ARGUEMENT,
        TerminalType::OPERAND,
      });
  CheckTokenTypes(
      "\"1.414 whatever\" # long long comment", {
        TerminalType::OPTION_DEFAULT_VALUE,
        TerminalType::COMMENT,
      });
}

TEST(tokenizer_for_doc, raw_token_type) {
  CheckRawTokenTypes(
      "( ) [ ] | = ...", {
        TypeID::K_L_PARENTHESIS,
        TypeID::K_R_PARENTHESIS,
        TypeID::K_L_BRACKET,
        TypeID::K_R_BRACKET,
        TypeID::K_EXCLUSIVE_OR,
        TypeID::K_EQUAL_SIGN,
        TypeID::K_ELLIPSES,
      });
  CheckRawTokenTypes(
      "usage: UsAge: options: options OPTIONS: default:", {
        TypeID::K_USAGE_COLON,
        TypeID::K_USAGE_COLON,
        TypeID::K_OPTIONS_COLON,
        TypeID::K_OPTIONS,
        TypeID::K_OPTIONS_COLON,
        TypeID::K_DEFAULT_COLON,
      });
  CheckRawTokenTypes(
      "*UTILITY_DELIMITER* *DESC_DELIMITER*", {
        TypeID::K_UTILITY_DELIMITER,
        TypeID::K_DESC_DELIMITER,
      });
  CheckRawTokenTypes(
      "-c --long <arg> ARG operand", {
        TypeID::POSIX_OPTION,
        TypeID::GNU_OPTION,
        TypeID::OPTION_ARGUEMENT,
        TypeID::OPTION_ARGUEMENT,
        TypeID::OPERAND,
      });
  CheckRawTokenTypes(
      "<arg t> <another arg> <<<<<<<<<<q>", {
        TypeID::OPTION_ARGUEMENT,
        TypeID::OPTION_ARGUEMENT,
        TypeID::OPTION_ARGUEMENT,
      });
  CheckRawTokenTypes(
      "\"1.414 whatever\" # long long comment", {
        TypeID::OPTION_DEFAULT_VALUE,
        TypeID::COMMENT,
      });
}

TEST(tokenizer_for_doc, token_value) {
  CheckTokenValues(
      "-c  - --long ",
      {"-c", "-", "--long"});
  CheckTokenValues(
      "-c FILE",
      {"-c", "FILE"});
}

TEST(tokenizer_for_doc, raw_token_value) {
  CheckRawTokenValues(
      "-c  - --long ",
      {"-c", "-", "--long"});
  CheckRawTokenValues(
      "-c FILE",
      {"-c", "FILE"});
}
