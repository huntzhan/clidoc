
#include "test_utils-inl.h"


TEST(tokenizer_for_doc, token_type) {
  CheckTokenTypes(
      "-c --long -afciFEj <arg> ARG command", {
        TerminalType::POSIX_OPTION,
        TerminalType::GNU_OPTION,
        TerminalType::GROUPED_OPTIONS,
        TerminalType::ARGUMENT,
        TerminalType::ARGUMENT,
        TerminalType::COMMAND,
      });
  CheckTokenTypes(
      "\"1.414 whatever\"", {
        TerminalType::DEFAULT_VALUE,
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
      "-c --long --output-file <arg> ARG OUTPUT-FILE command", {
        TypeID::POSIX_OPTION,
        TypeID::GNU_OPTION,
        TypeID::GNU_OPTION,
        TypeID::ARGUMENT,
        TypeID::ARGUMENT,
        TypeID::ARGUMENT,
        TypeID::COMMAND,
      });
  CheckRawTokenTypes(
      "<arg t> <another arg>", {
        TypeID::ARGUMENT,
        TypeID::ARGUMENT,
      });
  CheckRawTokenTypes(
      "\"1.414 whatever\"", {
        TypeID::DEFAULT_VALUE,
      });
}

TEST(tokenizer_for_doc, token_value) {
  CheckTokenValues(
      "-c  - --long   --output-file",
      {"-c", "-", "--long", "--output-file"});
  CheckTokenValues(
      "-c FILE OUTPUT-FILE",
      {"-c", "FILE", "OUTPUT-FILE"});
}

TEST(tokenizer_for_doc, raw_token_value) {
  CheckRawTokenValues(
      "-c  - --long ",
      {"-c", "-", "--long"});
  CheckRawTokenValues(
      "-c FILE",
      {"-c", "FILE"});
}
