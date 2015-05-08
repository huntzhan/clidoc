
#include "clidoc/generated_scanner_for_argv.h"

#define FlexGeneratedScannerClass FlexGeneratedScannerForArgv
#include "shared/utils-inl.h"

TEST(tokenizer_for_argv, token_type) {
  CheckTokenTypes(
      "--", {
        TerminalType::K_DOUBLE_HYPHEN,
      });
  CheckTokenTypes(
      "--long SOME-VALUE", {
        TerminalType::GNU_OPTION,
        TerminalType::GENERAL_ELEMENT,
      });
  CheckTokenTypes(
      "-c --long -afjb", {
      TerminalType::POSIX_OPTION,
      TerminalType::GNU_OPTION,
      TerminalType::GROUPED_OPTIONS,
    });
  CheckTokenTypes(
      "-f test_file.py FILE abc 42.19243", {
        TerminalType::POSIX_OPTION,
        TerminalType::GENERAL_ELEMENT,
        TerminalType::GENERAL_ELEMENT,
        TerminalType::GENERAL_ELEMENT,
        TerminalType::GENERAL_ELEMENT,
      });
}

TEST(tokenizer_for_argv, raw_token_type) {
  CheckRawTokenTypes(
      "--long = SOME-VALUE", {
        TypeID::GNU_OPTION,
        TypeID::GENERAL_ELEMENT,
      });
}

TEST(tokenizer_for_argv, ambiguous) {
  // such kind of ambiguity should be handle after tokenized.
  CheckTokenTypes(
      "--long = SOME-VALUE", {
        TerminalType::GNU_OPTION,
        TerminalType::GENERAL_ELEMENT,
      });
  CheckTokenTypes(
      "--long= SOME-VALUE", {
        TerminalType::GENERAL_ELEMENT,
        TerminalType::GENERAL_ELEMENT,
      });
  CheckTokenTypes(
      "--long =SOME-VALUE", {
        TerminalType::GNU_OPTION,
        TerminalType::GENERAL_ELEMENT,
      });
  CheckTokenTypes(
      "--long=SOME-VALUE", {
        TerminalType::GENERAL_ELEMENT,
      });
}

TEST(tokenizer_for_argv, token_value) {
  CheckTokenValues(
      "SOME-VALUE",
      {"SOME-VALUE"});
  CheckTokenValues(
      "-f test_file.py FILE abc 42.19243", 
      {"-f", "test_file.py", "FILE", "abc", "42.19243"});
}

TEST(tokenizer_for_argv, raw_token_value) {
  CheckRawTokenValues(
      "-f test_file.py FILE abc 42.19243", 
      {"-f", "test_file.py", "FILE", "abc", "42.19243"});
}
