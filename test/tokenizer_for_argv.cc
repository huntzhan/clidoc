
#include "test_utils-inl.h"

#define K_DOUBLE_HYPHEN TerminalType::K_DOUBLE_HYPHEN
#define POSIX_OPTION    TerminalType::POSIX_OPTION
#define GROUPED_OPTIONS TerminalType::GROUPED_OPTIONS
#define GNU_OPTION      TerminalType::GNU_OPTION
#define ARGUMENT        TerminalType::ARGUMENT

TEST(tokenizer_for_argv, token_type) {
  CheckTokenTypes(
      "--",
      {K_DOUBLE_HYPHEN});
  CheckTokenTypes(
      "--long SOME_VALUE",
      {GNU_OPTION, ARGUMENT});
  CheckTokenTypes(
      "-c --long -afjb",
      {POSIX_OPTION, GNU_OPTION, GROUPED_OPTIONS});
  CheckTokenTypes(
      "-f test_file.py FILE abc 42.19243", 
      {POSIX_OPTION, ARGUMENT, ARGUMENT, ARGUMENT, ARGUMENT});
}

TEST(tokenizer_for_argv, ambiguous) {
  // such kind of ambiguity should be handle after tokenized.
  CheckTokenTypes(
      "--long = SOME_VALUE",
      {GNU_OPTION, ARGUMENT});
  CheckTokenTypes(
      "--long= SOME_VALUE",
      {ARGUMENT, ARGUMENT});
  CheckTokenTypes(
      "--long =SOME_VALUE",
      {GNU_OPTION, ARGUMENT});
  CheckTokenTypes(
      "--long=SOME_VALUE",
      {ARGUMENT});
}

TEST(tokenizer_for_argv, token_value) {
  CheckTokenValues(
      "-f test_file.py FILE abc 42.19243", 
      {"-f", "test_file.py", "FILE", "abc", "42.19243"});
}
