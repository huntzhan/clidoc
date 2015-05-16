#include "gtest/gtest.h"
#include "clidoc/cpp11.h"
#include "cpp11_codegen_test/util-inl.h"

const set<string> kBooleanKeys = {
  "flag_a",
  "flag_arg2",
};
const set<string> kStringKeys = {
  "-a",
  "<arg2>",
};
const set<string> kStringListKeys = {};
auto KeyChecker = GenerateKeysOfOutcomeChecker(
    kBooleanKeys, kStringKeys, kStringListKeys);

TEST(default_value, option_a) {
  const char *argv[] = {"utility_name", "flag_a"};
  EXPECT_TRUE(
      clidoc::ParseArguments(2, argv, kTestMode));
  KeyChecker();
  EXPECT_EQ("42", clidoc::string_outcome["-a"]);
}

TEST(default_value, arg2) {
  const char *argv[] = {"utility_name", "flag_arg2"};
  EXPECT_TRUE(
      clidoc::ParseArguments(2, argv, kTestMode));
  KeyChecker();
  EXPECT_EQ("43", clidoc::string_outcome["<arg2>"]);
}
