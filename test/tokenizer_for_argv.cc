
#include "test_utils-inl.h"

#define K_DOUBLE_HYPHEN Token::TypeID::K_DOUBLE_HYPHEN
#define EQUAL_SIGN      Token::TypeID::EQUAL_SIGN
#define POSIX_OPTION    Token::TypeID::POSIX_OPTION
#define GROUPED_OPTIONS Token::TypeID::GROUPED_OPTIONS
#define GNU_OPTION      Token::TypeID::GNU_OPTION
#define ARGUMENT        Token::TypeID::ARGUMENT

TEST(tokenizer_for_argv, token_type) {
  CheckTokenTypes(
      "-- =",
      {K_DOUBLE_HYPHEN, EQUAL_SIGN});
  CheckTokenTypes(
      "-c --long -afjb",
      {POSIX_OPTION, GNU_OPTION, GROUPED_OPTIONS});
  CheckTokenTypes(
      "-f test_file.py FILE abc 42.19243", 
      {POSIX_OPTION, ARGUMENT, ARGUMENT, ARGUMENT, ARGUMENT});
}

TEST(tokenizer_for_argv, token_value) {
  CheckTokenValues(
      "-f test_file.py FILE abc 42.19243", 
      {"-f", "test_file.py", "FILE", "abc", "42.19243"});
}
