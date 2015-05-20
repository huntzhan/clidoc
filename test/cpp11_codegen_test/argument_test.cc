#include "gtest/gtest.h"
#include "clidoc/cpp11.h"
#include "cpp11_codegen_test/util-inl.h"

const set<string> kBooleanKeys = {
  "flag-1",
  "flag-2",
  "command1",
  "command2",
};
const set<string> kStringKeys = {
  "ARG1",
  "<arg2>",
};
const set<string> kStringListKeys = {
  "<arg3>",
};
auto KeyChecker = GenerateKeysOfOutcomeChecker(
    kBooleanKeys, kStringKeys, kStringListKeys);

TEST(simple_option, arg1) {
  const char *argv[] = {"utility_name", "flag-1", "value"};
  EXPECT_TRUE(
      clidoc::ParseArguments(3, argv, kTestMode));
  KeyChecker();
  EXPECT_EQ(
      "value",
      clidoc::string_outcome["ARG1"]);
}

TEST(simple_option, arg2) {
  const char *argv[] = {"utility_name", "flag-2", "value"};
  EXPECT_TRUE(
      clidoc::ParseArguments(3, argv, kTestMode));
  KeyChecker();
  EXPECT_EQ(
      "value",
      clidoc::string_outcome["<arg2>"]);
}

TEST(simple_option, arg3) {
  vector<string> expected;

  const char *argv_1[] = {"utility_name", "a", "b", "c"};
  EXPECT_TRUE(
      clidoc::ParseArguments(4, argv_1, kTestMode));
  KeyChecker();
  expected = {"a", "b", "c"};
  EXPECT_EQ(
      expected,
      clidoc::string_list_outcome["<arg3>"]);

  const char *argv_2[] = {"utility_name", "command1", "a"};
  EXPECT_TRUE(
      clidoc::ParseArguments(3, argv_2, kTestMode));
  KeyChecker();
  expected = {"command1", "a"};
  EXPECT_EQ(
      expected,
      clidoc::string_list_outcome["<arg3>"]);

  const char *argv_3[] = {"utility_name", "flag-1", "a", "b"};
  EXPECT_TRUE(
      clidoc::ParseArguments(4, argv_3, kTestMode));
  KeyChecker();
  expected = {"flag-1", "a", "b"};
  EXPECT_EQ(
      expected,
      clidoc::string_list_outcome["<arg3>"]);
}
